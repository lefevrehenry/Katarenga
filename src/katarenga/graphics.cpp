#include "graphics.hpp"
#include "player.hpp"
#include "utils.hpp"
#include <common/board/board_utils.hpp>

// GLTK
#include <GLTK/Camera.h>
#include <GLTK/FileRepository.h>
#include <GLTK/GLFWApplication.h>
#include <GLTK/GLFWCameraController.h>
#include <GLTK/Light.h>
#include <GLTK/Node.h>
#include <GLTK/Rect.h>
#include <GLTK/SceneGraph.h>
#include <GLTK/SceneView.h>
#include <GLTK/ShaderProgram.h>
#include <GLTK/VisualModel.h>

#include <GLTK/PhongShaderProgram.h>
#include <GLTK/HighLightShaderProgram.h>
#include <GLTK/NormalShaderProgram.h>
#include <GLTK/MatCapShaderProgram.h>
#include <GLTK/Texture2D.h>
#include <GLTK/Mesh.h>

// Glfw
#include <GLFW/glfw3.h>

// Glm
#include <glm/glm.hpp>

#include <iostream>
#include <unistd.h>
#include <memory>

using namespace gl;
using namespace gl::helper;
using MessageType = MessageWrapper::MessageType;

static std::unique_ptr<GLFWApplication> app;
static std::shared_ptr<SceneGraph> sceneGraph;

static constexpr int ScreenWidth = 1024;
static constexpr int ScreenHeight = 768;

std::string format_board(const std::string& board_configuration)
{
    std::string s = "";

    s += "\n";
    s += "    1  2  3  4  5  6  7  8   ";
    s += "\n";
    s += "             White           ";
    s += "\n";
    s += "  X                         X";
    s += "\n";

    std::string board = board_configuration.substr(0,128);

    for (int j = 0; j < 8; ++j) {
        s += std::to_string(j+1) + "  ";
        for (int i = 0; i < 8; ++i) {
            size_t index = (j * 8) + i;
            std::string c = board.substr(2*index,2);
            s += " " + c;
        }
        s += "\n";
    }

    s += "  X                         X";
    s += "\n";
    s += "             Black           ";

    return s;
}

void print_help()
{
    render_msg("h,help for help");
    render_msg("c,click for click in a case");
    render_msg("p,print to print the board");
    render_msg("s,stop for quit");
}

void Graphics::process_main_answer_game_status(zmqpp::message& message)
{
    AnswerGameStatus m = ConstructObject<AnswerGameStatus>(message);
    std::string board_configuration = m.getConfiguration();

    render_msg("AnswerBoardConfiguration received");
    std::string s = format_board(board_configuration);
    render_msg(s);
}

void Graphics::process_main_answer_move_message(zmqpp::message& message)
{
    MoveMessage m = ConstructObject<MoveMessage>(message);

    //BoardPlayer move_player = m.getPlayer();
    MoveType type = m.getType();

    if (type == MoveType::MovePlayed)
    {
        render_msg("MovePlayed");
    }
    else if (type == MoveType::InvalidMove)
    {
        render_msg("InvalidMove");
    }
    else if (type == MoveType::PlayThisMove)
    {
        render_msg("PlayThisMove");
    }
}

Graphics::Graphics(zmqpp::context& zmq_context, const std::string& main_thread_binding_point) :
    m_poller(),
    m_main_thread_socket(zmq_context, zmqpp::socket_type::pair),
    m_main_thread_reactor(),
    m_end_game(false)
{
    m_main_thread_socket.connect(main_thread_binding_point);

    // Listen to the main thread and std::cin
    m_poller.add(m_main_thread_socket, zmqpp::poller::poll_in);
    m_poller.add(STDIN_FILENO, zmqpp::poller::poll_in);

    using MessageType = MessageWrapper::MessageType;
    using Callback = MessageReactor::Callback;

    // Add callback functions to react to messages received from the main thread
    Callback process_main_answer_game_status = std::bind(&Graphics::process_main_answer_game_status, this, std::placeholders::_1);
    Callback process_main_answer_move_message = std::bind(&Graphics::process_main_answer_move_message, this, std::placeholders::_1);

    m_main_thread_reactor.add(MessageType::AnswerGameStatus, process_main_answer_game_status);
    m_main_thread_reactor.add(MessageType::MoveMessage, process_main_answer_move_message);

}

Graphics::~Graphics()
{
    m_main_thread_socket.close();
}

void errorCallback(int, const char* description)
{
    throw std::runtime_error(description);
}

void Graphics::loop()
{
    render_msg("render thread ready to play!");

    /* Set an error callback */
    glfwSetErrorCallback(errorCallback);

    /* Create a window and its OpenGL context */
    app.reset(GLFWApplication::CreateWindow(ScreenWidth,ScreenHeight));

    try
    {
        // Initialise GLTK
        initGLTK();

        // Throws the main loop
        app->loop();
    }
    // Catch exception if any
    catch (const std::exception& error)
    {
        render_msg(error.what());
    }

    GLFWApplication::Terminate();


    zmqpp::message message = ConstructMessage<StopGame>("human decide to stop", 0);

    // envoie le message (non bloquant)
    bool ret = m_main_thread_socket.send(message, true);

    if(!ret)
        std::cout << "(stop) error, message not sent" << std::endl;
    else
        m_end_game = true;

//    print_help();

//    while(!m_end_game)
//    {
//        render_msg("Enter a command: ");

//        if(m_poller.poll(zmqpp::poller::wait_forever))
//        {
//            if(m_poller.has_input(m_main_thread_socket))
//            {
//                zmqpp::message input_message;

//                // receive the message
//                m_main_thread_socket.receive(input_message);

//                // Will call the callback corresponding to the message type
//                bool processed = m_main_thread_reactor.process_message(input_message);

//                if(!processed)
//                    render_msg("message received from the main thread but no callback were defined for its type");
//            }
//            if(m_poller.has_input(STDIN_FILENO))
//            {
//                std::string command;
//                std::getline(std::cin, command);

//                if(command == "h" || command == "help")
//                {
//                    print_help();
//                }
//                else if(command == "c" || command == "click")
//                {
//                    std::cout << "Enter your string as the index of the cell '<src_cell_index>' ";

//                    std::string move_str;
//                    std::cin >> move_str;

//                    zmqpp::message message = ConstructMessage<CaseClicked>(move_str);

//                    // envoie le message (non bloquant)
//                    bool ret = m_main_thread_socket.send(message, true);

//                    if(!ret)
//                        std::cout << "(click) error, message not sent" << std::endl;
//                }
//                else if(command == "p" || command == "print")
//                {
//                    zmqpp::message message = ConstructMessage<AskBoardConfiguration>();

//                    // envoie le coup (non bloquant)
//                    bool ret = m_main_thread_socket.send(message, true);

//                    if(!ret)
//                        std::cout << "(print) error, message not sent" << std::endl;
//                }
//                else if(command == "s" || command == "stop")
//                {
//                    zmqpp::message message = ConstructMessage<StopGame>("human decide to stop", 0);

//                    // envoie le message (non bloquant)
//                    bool ret = m_main_thread_socket.send(message, true);

//                    if(!ret)
//                        std::cout << "(stop) error, message not sent" << std::endl;
//                    else
//                        m_end_game = true;
//                }
//                else
//                {
//                    std::cout << "unknow command '" << command << "'" << std::endl;
//                }
//            }
//        }
//    }

    render_msg("Terminating");
}

void Graphics::initGLTK()
{
    std::shared_ptr<SceneView> sceneView(new SceneView());

    std::shared_ptr<Controller> controller(new GLFWCameraController(sceneView));
    sceneView->setInterface(controller);

    Rect rect(0,0,ScreenWidth,ScreenHeight);
    sceneView->setRect(rect);

    Light light;
    light.setColor(glm::vec3(1,0,0));
    light.setDirection(glm::vec3(-1,-1,-1));
    sceneView->setLight(light);

    sceneGraph.reset(new SceneGraph());
    sceneView->setScene(sceneGraph);

    app->addSceneView(sceneView);

    DataRepository.addFirstPath("/home/henry/dev/QtProject/GLTK/share/");
    DataRepository.addFirstPath("/home/henry/dev/QtProject/GLTK/share/shaders/");

    Node* root = sceneGraph->root();
    createScene(root);

    fitCamera(sceneView->camera());
}

void addPiece(Node* node, Mesh::SPtr mesh, Material material, int i, int j)
{
    VisualModel::SPtr visualModel(new VisualModel(mesh, material));
    visualModel->transform().translate(-12.5f - (3 * 25) + (i * 25), -12.5f - (3 * 25) + (j * 25), 0);
    node->addVisual(visualModel);
}

void addPhongShadingShader(Node* node) {
    ShaderProgram::SPtr sp(new PhongShaderProgram());
    node->setShaderProgram(sp);
}

//void addMatCapShader(Node* node, const std::string& filename) {
//    MatCapShaderProgram::SPtr sp(new MatCapShaderProgram());
////    sp->setMatCapTexture(filename);

//    node->setShaderProgram(sp);
//}

void createChessboard(Node* node)
{
    std::string folder = "low_res";
    std::string extension = ".obj";

    Mesh::SPtr   pawn(new Mesh("mesh/" + folder + "/pion" + extension));

    // White Set

    Node* nodeWhite = node->addChild();
    addPhongShadingShader(nodeWhite);
//    addMatCapShader(nodeWhite, "textures/m8.jpg");

    Material materialWhite = Material::Brass();

    addPiece(nodeWhite, pawn,   materialWhite, 0, 0);
    addPiece(nodeWhite, pawn,   materialWhite, 1, 0);
    addPiece(nodeWhite, pawn,   materialWhite, 2, 0);
    addPiece(nodeWhite, pawn,   materialWhite, 3, 0);
    addPiece(nodeWhite, pawn,   materialWhite, 4, 0);
    addPiece(nodeWhite, pawn,   materialWhite, 5, 0);
    addPiece(nodeWhite, pawn,   materialWhite, 6, 0);
    addPiece(nodeWhite, pawn,   materialWhite, 7, 0);

    // Black Set

    Node* nodeBlack = node->addChild();
    addPhongShadingShader(nodeBlack);
//    addMatCapShader(nodeBlack, "textures/metal.jpg");

    Material materialBlack = Material::Silver();

    addPiece(nodeBlack, pawn,   materialBlack, 0, 7);
    addPiece(nodeBlack, pawn,   materialBlack, 1, 7);
    addPiece(nodeBlack, pawn,   materialBlack, 2, 7);
    addPiece(nodeBlack, pawn,   materialBlack, 3, 7);
    addPiece(nodeBlack, pawn,   materialBlack, 4, 7);
    addPiece(nodeBlack, pawn,   materialBlack, 5, 7);
    addPiece(nodeBlack, pawn,   materialBlack, 6, 7);
    addPiece(nodeBlack, pawn,   materialBlack, 7, 7);
}

void createBoard(Node* node)
{
    float s = 118.5;

//    Texture::SPtr boardColorTex(new Texture2D());
//    boardColorTex->load("textures/chessboard2.jpg");

//    ShaderProgram::SPtr shaderProgram( ShaderProgram::Create(ShaderProgramType::BasicTexturing) );
//    shaderProgram->addData("colorMap", boardColorTex.get());

    PhongShaderProgram::SPtr shaderProgram( new PhongShaderProgram() );

    VisualModel::SPtr board(new VisualModel("mesh/flatQuad.obj"));
    board->transform().scale(s,s,s);

    node->setShaderProgram(shaderProgram);
    node->addVisual(board);
}

void Graphics::createScene(Node* rootNode)
{
//    VisualModel::SPtr visualModel;
//    ShaderProgram::SPtr shaderProgram;

//    // rootNode
//    visualModel.reset(new VisualModel("mesh/dragon_low.obj", Material::Emerald()));
//    rootNode->addVisual(visualModel);

//    shaderProgram.reset( new PhongShaderProgram() );
//    rootNode->setShaderProgram(shaderProgram);

//    // node 1
//    Node* node1 = rootNode->addChild();
//    node1->addVisual(visualModel);

//    shaderProgram.reset( new HighLightShaderProgram() );
//    node1->setShaderProgram(shaderProgram);

//    // node 2
//    Node* node2 = rootNode->addChild();
//    node2->addVisual(visualModel);

//    NormalShaderProgram* nsp = new NormalShaderProgram();
//    nsp->setNormalScale(.5f);
//    nsp->setNormalColor(glm::vec3(1,1,1));

//    shaderProgram.reset( nsp );
//    node2->setShaderProgram(shaderProgram);

    createBoard(rootNode);
    createChessboard(rootNode);
}

void Graphics::fitCamera(Camera* camera)
{
    SceneGraph* scene = sceneGraph.get();

    if(!scene || !camera)
        return;

    glm::vec3 min;
    glm::vec3 max;
    scene->getBB(min, max);

    float diagonal = glm::length(max - min);
    glm::vec3 direction(0,-1,0);

    // view
    glm::vec3 target = (min + max) / 2.0f;
    glm::vec3 eye = target - (glm::normalize(direction) * diagonal);
    glm::vec3 up(0,0,1);
    camera->lookAt(eye, target, up);

    // projection
    float fovy = 45.0f;
    float aspect = 4.0f / 3.0f;
    float zNear = 0.02f * diagonal;
    float zFar = 2.0f * diagonal;
    camera->perspective(fovy, aspect, zNear, zFar);
}

void graphics_function(zmqpp::context& zmq_context, const std::string& main_thread_binding_point)
{
    Graphics graphic(zmq_context, main_thread_binding_point);

    graphic.loop();
}
