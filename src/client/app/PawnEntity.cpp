#include "PawnEntity.hpp"

PawnEntity::PawnEntity(Common::Case c, Common::GameActor actor) :
    m_c(c),
    m_actor(actor)
{
}

void PawnEntity::set_case(Common::Case c)
{
    if(m_c != c)
        return;

    m_c = c;

    case_changed(c);
}

void PawnEntity::set_actor(GameActor actor)
{
    if(m_actor == actor)
        return;

    m_actor = actor;

    actor_changed(actor);
}
