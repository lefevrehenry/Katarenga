#include "BoardEntity.hpp"

// Katarenga
#include <client/client.hpp>

// Standard Library
#include <functional>

BoardEntity::BoardEntity() :
    m_client(nullptr),
    m_pawns(),
    _sc1(),
    _sc2()
{
    auto callback = std::bind(&BoardEntity::update_client, this);

    client_changed.connect(callback);
}

Client* BoardEntity::client() const
{
    return m_client;
}

void BoardEntity::set_client(Client* client)
{
    if(m_client == client)
        return;

    m_client = client;

    client_changed(client);
}

const BoardEntity::PawnsEntity BoardEntity::pawns() const
{
    return m_pawns;
}

bool BoardEntity::pawn_exists(const PawnEntity::SPtr& pawn) const
{
    auto it = std::find(std::begin(m_pawns), std::end(m_pawns), pawn);

    return it != std::end(m_pawns);
}

void BoardEntity::add_pawn(const PawnEntity::SPtr& pawn)
{
    if(pawn_exists(pawn))
        return;

    m_pawns.push_back(pawn);

    pawn_added(pawn);
}

void BoardEntity::remove_pawn(const PawnEntity::SPtr& pawn)
{
    auto it = std::find(std::begin(m_pawns), std::end(m_pawns), pawn);

    if(it != std::end(m_pawns))
        return;

    PawnEntity::SPtr old = *it;

    m_pawns.erase(it);

    pawn_removed(old);
}

void BoardEntity::move_pawn(const PawnEntity::SPtr& pawn, Common::Case c)
{
    if(!pawn)
        return;

    pawn->set_case(c);
}

void BoardEntity::update_client()
{
    _sc1.disconnect();
    _sc2.disconnect();

    if(m_client)
    {
        auto init = std::bind(&BoardEntity::init, this, std::placeholders::_1);
        _sc1 = m_client->position_changed.connect(init);

        auto sync = std::bind(&BoardEntity::sync, this, std::placeholders::_1);
        _sc2 = m_client->move_played.connect(sync);

        m_client->request_position();
    }
}

void BoardEntity::init(const Common::Position& position)
{
    while(!m_pawns.empty()) {
        remove_pawn(m_pawns.front());
    }

    m_pawns.clear();

    yield_pawns(position, [&](Common::Case c, Common::GameActor actor) {
        PawnEntity::SPtr pawn = std::make_shared<PawnEntity>(c, actor);

        add_pawn(pawn);
    });
}

void BoardEntity::sync(Common::Move move)
{
    Common::Case from = move_from(move);
    Common::Case to = move_to(move);

    PawnEntity::SPtr pawn_to_move;
    PawnEntity::SPtr pawn_to_remove;

    for (const PawnEntity::SPtr& pawn : m_pawns)
    {
        if(pawn->c() != to)
            continue;

        pawn_to_remove = pawn;
    }

    for (const PawnEntity::SPtr& pawn : m_pawns)
    {
        if(pawn->c() != from)
            continue;

        pawn_to_move = pawn;
    }

    if(pawn_to_move == nullptr)
    {
        // sync error (warning ? throw ?)
        return;
    }

    if(pawn_to_remove)
        remove_pawn(pawn_to_remove);

    move_pawn(pawn_to_move, to);
}
