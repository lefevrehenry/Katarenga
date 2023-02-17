#include "BoardRenderEntity.hpp"

// Katarenga
#include <BoardEntity.hpp>

// Standard Library
#include <functional>

static std::string pawn_mesh_filename = "share/mesh/pion.stl";

BoardRenderEntity::BoardRenderEntity() : Entity(),
    m_board_entity(nullptr),
    m_render_pawns(),
    m_mesh(new Mesh(pawn_mesh_filename)),
    _sc1(),
    _sc2()
{
    auto f = std::bind(&BoardRenderEntity::update_board_entity, this);

    board_entity_changed.connect(f);
}

BoardEntity* BoardRenderEntity::board_entity() const
{
    return m_board_entity;
}

void BoardRenderEntity::set_board_entity(BoardEntity* board_entity)
{
    if(m_board_entity == board_entity)
        return;

    m_board_entity = board_entity;

    board_entity_changed(board_entity);
}

bool BoardRenderEntity::render_pawn_exists(const PawnRenderEntity::SPtr& render_pawn) const
{
    auto it = std::find(std::begin(m_render_pawns), std::end(m_render_pawns), render_pawn);

    return it != std::end(m_render_pawns);
}

void BoardRenderEntity::add_render_pawn(PawnRenderEntity::SPtr render_pawn)
{
    if(render_pawn_exists(render_pawn))
        return;

    m_render_pawns.push_back(render_pawn);

    // add child
    {
        Entity::addEntity(render_pawn);
        std::cout << "add Entity (" << render_pawn.get() << ")" << std::endl;

        render_pawn->addComponent(m_mesh);
    }

    render_pawn_added(render_pawn);
}

void BoardRenderEntity::remove_render_pawn(PawnRenderEntity::SPtr render_pawn)
{
    if(!render_pawn_exists(render_pawn))
        return;

    m_render_pawns.erase(std::find(std::begin(m_render_pawns), std::end(m_render_pawns), render_pawn));

    // remove child
    {
        Entity::removeEntity(render_pawn);

        std::cout << "remove Entity (" << render_pawn.get() << ")" << std::endl;
    }

    render_pawn_removed(render_pawn);
}

PawnRenderEntity::SPtr BoardRenderEntity::render_pawn(const PawnEntity::SPtr& pawn_entity)
{
    for (const PawnRenderEntity::SPtr& render_pawn : m_render_pawns) {
        if(render_pawn->pawn_entity() == pawn_entity)
            return render_pawn;
    }

    return PawnRenderEntity::SPtr();
}

void BoardRenderEntity::update_board_entity()
{
    if(m_board_entity)
    {
        auto c1 = std::bind(&BoardRenderEntity::pawn_entity_added, this, std::placeholders::_1);
        _sc1 = m_board_entity->pawn_added.connect(c1);

        auto c2 = std::bind(&BoardRenderEntity::pawn_entity_removed, this, std::placeholders::_1);
        _sc2 = m_board_entity->pawn_removed.connect(c2);

        init(m_board_entity->pawns());
    }
    else
    {
        _sc1.disconnect();
        _sc1.disconnect();
    }
}

void BoardRenderEntity::init(const PawnsEntity& pawns_entity)
{
    while(!m_render_pawns.empty()) {
        remove_render_pawn(m_render_pawns.front());
    }

    m_render_pawns.clear();

    for (const PawnEntity::SPtr& pawn_entity : pawns_entity) {
        pawn_entity_added(pawn_entity);
    }
}

void BoardRenderEntity::pawn_entity_added(const PawnEntity::SPtr& pawn_entity)
{
    PawnRenderEntity::SPtr render_pawn = std::make_shared<PawnRenderEntity>(pawn_entity);

    add_render_pawn(render_pawn);
}

void BoardRenderEntity::pawn_entity_removed(const PawnEntity::SPtr& pawn_entity)
{
    PawnRenderEntity::SPtr render_pawn_to_remove = render_pawn(pawn_entity);

    if(render_pawn_to_remove)
        remove_render_pawn(render_pawn_to_remove);
}
