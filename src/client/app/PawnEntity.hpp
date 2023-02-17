#ifndef KATARENGA_CLIENT_APP_PAWN_ENTITY_HPP
#define KATARENGA_CLIENT_APP_PAWN_ENTITY_HPP

// Katarenga
#include <common/common_utils.hpp>

// Nod
#include <nod/nod.hpp>

// Standard Library
#include <memory>

/**
 * @brief The PawnEntity class
 */
class PawnEntity
{
    using Case = Common::Case;
    using GameActor = Common::GameActor;

public:
    using SPtr = std::shared_ptr<PawnEntity>;
    using WPtr = std::weak_ptr<PawnEntity>;

public:
    PawnEntity(Case c, GameActor actor);

public:
    Case c() const { return m_c; }
    void set_case(Case c);

    GameActor actor() const { return m_actor; }
    void set_actor(GameActor actor);

public:
    nod::signal<void(Case c)> case_changed;
    nod::signal<void(GameActor actor)> actor_changed;

private:
    Case m_c;
    GameActor m_actor;

};

#endif // KATARENGA_CLIENT_APP_PAWN_ENTITY_HPP
