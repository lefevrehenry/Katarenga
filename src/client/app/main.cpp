
// Katarenga
#include <BoardEntity.hpp>
#include <BoardRenderEntity.hpp>

#include <client/client.hpp>
#include <client/client_utils.hpp>

#include <common/common_utils.hpp>

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// RenderEngine
#include <RenderEngine/Camera.h>
#include <RenderEngine/Command.h>
#include <RenderEngine/ConeMesh.h>
#include <RenderEngine/CubeMesh.h>
#include <RenderEngine/CylinderMesh.h>
#include <RenderEngine/Entity.h>
#include <RenderEngine/ImageTextureData.h>
#include <RenderEngine/MatcapMaterial.h>
#include <RenderEngine/Mesh.h>
#include <RenderEngine/PhongMaterial.h>
#include <RenderEngine/PlaneMesh.h>
#include <RenderEngine/RenderEngine.h>
#include <RenderEngine/ResourcesManager.h>
#include <RenderEngine/Scene3D.h>
#include <RenderEngine/SphereMesh.h>
#include <RenderEngine/TextureMaterial.h>
#include <RenderEngine/TorusMesh.h>
#include <RenderEngine/Transform.h>

// GLFW3
#include <GLFW/glfw3.h>

// Glm
#include <glm/gtc/constants.hpp>

// Standard Library
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>

static int last_x_position = -1;
static int last_y_position = -1;
static GLFWwindow* window = nullptr;
static Scene3D* scene3D = nullptr;
static bool mouse_pressed = false;

static const std::string BLACK         = std::string("\033[0;30m");
static const std::string RED           = std::string("\033[0;31m");
static const std::string GREEN         = std::string("\033[0;32m");
static const std::string YELLOW        = std::string("\033[0;33m");
static const std::string BLUE          = std::string("\033[0;34m");
static const std::string PURPLE        = std::string("\033[0;35m");
static const std::string CYAN          = std::string("\033[0;36m");
static const std::string WHITE         = std::string("\033[0;37m");
static const std::string DEFAULT_COLOR = std::string("\033[0m");

static zmqpp::context context;

template< typename T >
static std::string Name();

template<> std::string Name<Geometry>() { return "Geometry"; }
template<> std::string Name<Material>() { return "Material"; }
template<> std::string Name<Transform>() { return "Transform"; }

template< typename T >
struct Trait
{
    static std::string name() { return Name<T>(); }

    std::ostream& read(std::ostream& o, T& self);
};

static void mouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    mouse_pressed = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);

    if (mouse_pressed)
    {
        double xpos = -1;
        double ypos = -1;
        glfwGetCursorPos(window, &xpos, &ypos);

        last_x_position = xpos;
        last_y_position = ypos;
    }
}

static void cursorPosCallback(GLFWwindow*, double xpos, double ypos)
{
    Camera* camera = scene3D->camera();

    if (mouse_pressed && camera)
    {
        double dx = xpos - last_x_position;
        double dy = ypos - last_y_position;

        int width = 0;
        int height = 0;
        glfwGetWindowSize(window, &width, &height);

        double pi = glm::pi<double>();
        double rx = ( dx / width) * (2.0f * pi);
        double ry = (-dy / height) * pi;
        camera->rotate(float(rx), float(ry));

//        glm::vec3 dir = glm::inverse(camera->normal()) * glm::vec3(-dx/width,dy/height,0);
//        camera->translate(glm::vec3(dir));
    }

    last_x_position = xpos;
    last_y_position = ypos;
}

static void scrollCallback(GLFWwindow*, double /*xoffset*/, double yoffset)
{
    Camera* camera = scene3D->camera();

    float signed_ratio = yoffset * 0.1f;
    glm::vec3 t = signed_ratio * (camera->target() - camera->eye());

    camera->setEye(camera->eye() + t);
}

static void do_print(Entity::SPtr entity, int level)
{
    std::string prefix = "";
    for (int i = 0; i < level; ++i) { prefix += "   |"; }

    std::cout << RED << prefix << "-- Entity " << DEFAULT_COLOR << (bool(entity) ? entity->name() : "0x0") << DEFAULT_COLOR << std::endl;

    if(!entity)
        return;

    const Entity::Entities& children = entity->childEntities();

    prefix = "";
    for (int i = 0; i < level+1; ++i) { prefix += "   |"; }

    const Components& components = entity->components();
    for (const Component::SPtr& component : components)
    {
        Transform* tf = dynamic_cast<Transform*>(component.get());
        if(tf) std::cout << RED << prefix << GREEN << "   -> Transform" << DEFAULT_COLOR << std::endl;

        Geometry* geom = dynamic_cast<Geometry*>(component.get());
        if(geom) std::cout << RED << prefix << GREEN << "   -> Geometry" << DEFAULT_COLOR << std::endl;

        Material* mat = dynamic_cast<Material*>(component.get());
        if(mat) std::cout << RED << prefix << GREEN << "   -> Material" << DEFAULT_COLOR << std::endl;
    }

    for (const Entity::SPtr child : children)
    {
        do_print(child, level+1);
    }
}

static void print_scene_3d()
{
    Entity::SPtr root = scene3D->rootEntity();

    do_print(root, 0);
}

static Scene3D::SPtr CreateScene()
{
    Scene3D::SPtr scene3D(new Scene3D());
    Entity::SPtr rootEntity = scene3D->rootEntity();
    rootEntity->setName("root");

    Camera* camera = scene3D->camera();
    camera->lookAt(glm::vec3(7,-2,2), glm::vec3(7,7,0), glm::vec3(0,0,1));
//    camera->lookAt(glm::vec3(2,2,2), glm::vec3(), glm::vec3(0,0,1));

    // Frame Entity
    if(true)
    {
        CylinderMesh::SPtr cyl;
        ConeMesh::SPtr con;
        PhongMaterial::SPtr mat;
        Transform::SPtr tr;

        Entity::SPtr frameEntity = rootEntity->addEntity();
        frameEntity->setName("frame");

        tr = frameEntity->addComponent<Transform>();

        // z Axis
        {
            Entity::SPtr zAxisEntity = frameEntity->addEntity();
            cyl = zAxisEntity->addComponent<CylinderMesh>();
            mat = zAxisEntity->addComponent<PhongMaterial>();
            mat->setAmbientColor(glm::vec4(0.f,0.f,.3f,0.f));
            mat->setDiffuseColor(glm::vec4(0.f,0.f,1.f,0.f));
            tr = zAxisEntity->addComponent<Transform>();
            tr->setTranslation(0,0,.5);
            tr->setScale(glm::vec3(.5f,.5f,1.f));

            Entity::SPtr zConeEntity = zAxisEntity->addEntity();
            con = zConeEntity->addComponent<ConeMesh>();
            tr = zConeEntity->addComponent<Transform>();
            tr->setTranslation(0,0,(cyl->length()/2.f) + (con->length()/2.f));
        }

        // y Axis
        {
            Entity::SPtr yAxisEntity = frameEntity->addEntity();
            cyl = yAxisEntity->addComponent<CylinderMesh>();
            mat = yAxisEntity->addComponent<PhongMaterial>();
            mat->setAmbientColor(glm::vec4(0.f,.3f,.0f,0.f));
            mat->setDiffuseColor(glm::vec4(0.f,1.f,.0f,0.f));
            tr = yAxisEntity->addComponent<Transform>();
            tr->setTranslation(0,.5,0);
            tr->setRotation(glm::radians(-90.f), glm::vec3(1,0,0));
            tr->setScale(glm::vec3(.5f,.5f,1.f));

            Entity::SPtr yConeEntity = yAxisEntity->addEntity();
            con = yConeEntity->addComponent<ConeMesh>();
            tr = yConeEntity->addComponent<Transform>();
            tr->setTranslation(0,(cyl->length()/2.f) + (con->length()/2.f),0);
        }

        // x Axis
        {
            Entity::SPtr xAxisEntity = frameEntity->addEntity();
            cyl = xAxisEntity->addComponent<CylinderMesh>();
            mat = xAxisEntity->addComponent<PhongMaterial>();
            mat->setAmbientColor(glm::vec4(.3f,0.f,.0f,0.f));
            mat->setDiffuseColor(glm::vec4(1.f,0.f,.0f,0.f));
            tr = xAxisEntity->addComponent<Transform>();
            tr->setTranslation(.5,0,0);
            tr->setRotation(glm::radians(90.f), glm::vec3(0,1,0));
            tr->setScale(glm::vec3(.5f,.5f,1.f));

            Entity::SPtr xConeEntity = xAxisEntity->addEntity();
            con = xConeEntity->addComponent<ConeMesh>();
            tr = xConeEntity->addComponent<Transform>();
            tr->setTranslation((cyl->length()/2.f) + (con->length()/2.f),0,0);
        }
    }

//    Texture::SPtr texture(new Texture());
//    std::unique_ptr<ImageTextureData> imageData(new ImageTextureData("share/textures/board.png"));
//    texture->setTextureData(std::move(imageData));
//    texture->setTextureUnit(1);

//    Texture::SPtr matcap_texture(new Texture());
//    std::unique_ptr<ImageTextureData> matcap_imageData(new ImageTextureData("share/textures/hot.jpg"));
//    matcap_texture->setTextureData(std::move(matcap_imageData));
//    matcap_texture->setTextureUnit(1);

//    Entity::SPtr boardEntity = entity->addEntity();
//    SphereMesh::SPtr mesh = boardEntity->addComponent<SphereMesh>();
//    PhongMaterial::SPtr mat = boardEntity->addComponent<PhongMaterial>();
//    Transform::SPtr tr = boardEntity->addComponent<Transform>();

//    MatcapMaterial::SPtr mat = cubeEntity->addComponent<MatcapMaterial>();
//    mat->setTexture(texture);

//    Entity::SPtr cubeEntity = rootEntity->addEntity();
//    cubeEntity->addComponent<SphereMesh>();
//    cubeEntity->addComponent<CubeMesh>();
//    cubeEntity->addComponent<TorusMesh>();
//    cubeEntity->addComponent<PlaneMesh>();
//    cubeEntity->addComponent<Mesh>("share/mesh/tour.stl");
//    cubeEntity->addComponent<Mesh>("share/mesh/cavalier.stl");
//    cubeEntity->addComponent<Mesh>("share/mesh/pion.stl");
//    mesh->setFilename("share/mesh/cube.obj");
//    PhongMaterial::SPtr mat = cubeEntity->addComponent<PhongMaterial>();
//    mat->setAmbientColor(glm::vec4(glm::vec3(.25f),1.f));
//    mat->setDiffuseColor(glm::vec4(glm::vec3(.6f),1.f));
//    TextureMaterial::SPtr mat = cubeEntity->addComponent<TextureMaterial>();
//    mat->setTexture(texture);
//    MatcapMaterial::SPtr mat = cubeEntity->addComponent<MatcapMaterial>();
//    mat->setTexture(matcap_texture);
//    Transform::SPtr tr = cubeEntity->addComponent<Transform>();

    // Board Entity
    {
        Entity::SPtr boardEntity = rootEntity->addEntity();
        boardEntity->setName("BoardEntity");

        // mesh
        boardEntity->addComponent<PlaneMesh>();

        // material
        Texture::SPtr texture(new Texture());
        std::unique_ptr<ImageTextureData> imageData(new ImageTextureData("share/textures/board.png"));
        texture->setTextureData(std::move(imageData));
//        texture->setTextureUnit(1);

        TextureMaterial::SPtr mat = boardEntity->addComponent<TextureMaterial>();
        mat->setTexture(texture);

        // transform
        Transform::SPtr tr = boardEntity->addComponent<Transform>();
        tr->setScale(glm::vec3(16.f));
        tr->setTranslation(glm::vec3(7.f,7.f,0.f));
    }

    // Pawns

    if(false)
    {
        Entity::SPtr pawnEntities = rootEntity->addEntity();

        Mesh::SPtr pawnMesh = std::make_shared<Mesh>("share/mesh/pion.stl");

        // White
        {
            Entity::SPtr whitePawnEntities = pawnEntities->addEntity();

            PhongMaterial::SPtr pawnMaterial = std::make_shared<PhongMaterial>();
            pawnMaterial->setAmbientColor(glm::vec4(glm::vec3(.25f),1.f));
            pawnMaterial->setDiffuseColor(glm::vec4(.6f,.1f,.1f,1.f));

            for (int i = 0; i < 8; i++) {
                Entity::SPtr pawnEntity = whitePawnEntities->addEntity();

                // mesh
                pawnEntity->addComponent(pawnMesh);

                // material
                pawnEntity->addComponent(pawnMaterial);

                // transform
                Transform::SPtr tr = pawnEntity->addComponent<Transform>();
                tr->setScale(glm::vec3(.1f));
                tr->setTranslation(glm::vec3(2.f * i,0.f,0.f));
            }
        }

        // Black
        {
            Entity::SPtr blackPawnEntities = pawnEntities->addEntity();

            PhongMaterial::SPtr pawnMaterial = std::make_shared<PhongMaterial>();
            pawnMaterial->setAmbientColor(glm::vec4(glm::vec3(.25f),1.f));
            pawnMaterial->setDiffuseColor(glm::vec4(.1f,.1f,.6f,1.f));

            for (int i = 0; i < 8; i++) {
                Entity::SPtr pawnEntity = blackPawnEntities->addEntity();

                // mesh
                pawnEntity->addComponent(pawnMesh);

                // material
                pawnEntity->addComponent(pawnMaterial);

                // transform
                Transform::SPtr tr = pawnEntity->addComponent<Transform>();
                tr->setScale(glm::vec3(.1f));
                tr->setTranslation(glm::vec3(2.f * i,14.f,0.f));
            }
        }
    }

    return scene3D;
}

//static void foo()
//{
//    zmqpp::context context;

//    {
//        Initialize();
//    }

//    ServerInfo server_info = ReadConfigFile();
//    server_info.context = &context;

//    AppInfo app_info;
//    app_info.context = &context;
//    app_info.thread_endpoint = "inproc://katarenga_client";

//    Client client(server_info, app_info);
//}

int main()
{
    /* Initialize the library (main thread only) */
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context (main thread only) */
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Setting callbacks (main thread only) */
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    /* Wait N refresh on screen before swapping buffers */
    glfwSwapInterval(1);

    /* Make OpenGL context current */
    glfwMakeContextCurrent(window);

    /* Create the 3D scene */
    std::shared_ptr<Scene3D> _scene3D = CreateScene();
    scene3D = _scene3D.get();

    /* Create the render engine */
    std::unique_ptr<RenderEngine> renderEngine = std::make_unique<RenderEngine>();

    {
        Initialize();

        std::filesystem::path executable_path = std::filesystem::canonical("/proc/self/exe");
        ResourcesManager.addPath(executable_path.parent_path().parent_path());
    }

    ServerInfo server_info = ReadConfigFile();
    server_info.context = &context;

    AppInfo app_info;
    app_info.context = &context;
//    app_info.thread_endpoint = "inproc://katarenga_client";

    /* Create the Katarenga Client */
    Client client(server_info, app_info);

    /* Create the BoardEntity */
    BoardEntity boardEntity;
    boardEntity.set_client(&client);

    /* Create the BoardRenderEntity */
    BoardRenderEntity::SPtr boardRenderEntiy = std::make_shared<BoardRenderEntity>();
    boardRenderEntiy->set_board_entity(&boardEntity);
    boardRenderEntiy->setName("BoardRenderEntiy");

    {
        Entity::SPtr entity = scene3D->rootEntity();

        entity->addEntity(boardRenderEntiy);
    }

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window))
    {
        /* Poll Katarenga client */
        client.poll();

        /* Update the SceneGraph (generate a bunch of fresh render commands) */
        scene3D->refresh();

        /* Synchronization here (take all render commands from the scene3D) */
        renderEngine->synchronize(scene3D);

        /* Start rendering (translate render commands into OpenGL calls) */
        renderEngine->render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll and process system events (main thread only) */
        glfwPollEvents();
    }

    print_scene_3d();

    /* Clear the RenderEngine before the OpenGL context is destroyed */
    renderEngine.reset();

    /* Release resources (main thread only) */
    glfwTerminate();

    return 0;
}
