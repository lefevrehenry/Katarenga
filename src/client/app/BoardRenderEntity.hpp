#ifndef KATARENGA_CLIENT_APP_BOARD_RENDER_ENTITY_HPP
#define KATARENGA_CLIENT_APP_BOARD_RENDER_ENTITY_HPP

// Katarenga
#include <common/common_utils.hpp>

#include <PawnEntity.hpp>
#include <PawnRenderEntity.hpp>

// RenderEngine
#include <RenderEngine/Entity.h>
#include <RenderEngine/Mesh.h>

// Nod
#include <nod/nod.hpp>

// Standard Library
#include <memory>
#include <vector>

class BoardEntity;

/**
 * @brief The BoardRenderEntity class
 */
class BoardRenderEntity : public Entity
{
    using PawnsEntity = std::vector<PawnEntity::SPtr>;
    using RenderPawnsEntity = std::vector<PawnRenderEntity::SPtr>;

public:
    using SPtr = std::shared_ptr<BoardRenderEntity>;

public:
    BoardRenderEntity();

public:
    BoardEntity* board_entity() const;
    void set_board_entity(BoardEntity* board_entity);

public:
    bool render_pawn_exists(const PawnRenderEntity::SPtr& render_pawn) const;

private:
    void add_render_pawn(PawnRenderEntity::SPtr render_pawn);
    void remove_render_pawn(PawnRenderEntity::SPtr render_pawn);

public:
    nod::signal<void(PawnRenderEntity::SPtr)> render_pawn_added;
    nod::signal<void(PawnRenderEntity::SPtr)> render_pawn_removed;

private:
    nod::signal<void(BoardEntity*)> board_entity_changed;

private:
    PawnRenderEntity::SPtr render_pawn(const PawnEntity::SPtr& pawn_entity);

private:
    void update_board_entity();
    void init(const PawnsEntity& pawns_entity);

    void pawn_entity_added(const PawnEntity::SPtr& pawn_entity);
    void pawn_entity_removed(const PawnEntity::SPtr& pawn_entity);

private:
    BoardEntity* m_board_entity;

    RenderPawnsEntity m_render_pawns;

    Mesh::SPtr m_mesh;

    nod::scoped_connection _sc1;
    nod::scoped_connection _sc2;

};

#endif // KATARENGA_CLIENT_APP_BOARD_RENDER_ENTITY_HPP
