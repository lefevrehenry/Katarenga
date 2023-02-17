#ifndef KATARENGA_CLIENT_APP_BOARD_ENTITY_HPP
#define KATARENGA_CLIENT_APP_BOARD_ENTITY_HPP

// Katarenga
#include <common/common_utils.hpp>

#include <PawnEntity.hpp>

// Nod
#include <nod/nod.hpp>

// Standard Library
#include <memory>
#include <vector>

class Client;

/**
 * @brief The BoardEntity class
 */
class BoardEntity
{
    using PawnsEntity = std::vector<PawnEntity::SPtr>;

public:
    BoardEntity();

public:
    Client* client() const;
    void set_client(Client* client);

public:
    const PawnsEntity pawns() const;
    bool pawn_exists(const PawnEntity::SPtr& pawn) const;

private:
    void add_pawn(const PawnEntity::SPtr& pawn);
    void remove_pawn(const PawnEntity::SPtr& pawn);
    void move_pawn(const PawnEntity::SPtr& pawn, Common::Case c);

public:
    nod::signal<void(PawnEntity::SPtr)> pawn_added;
    nod::signal<void(PawnEntity::SPtr)> pawn_removed;

private:
    nod::signal<void(Client*)> client_changed;

private:
    void update_client();
    void init(const Common::Position& position);
    void sync(Common::Move move);

private:
    Client* m_client;

    PawnsEntity m_pawns;

    nod::scoped_connection _sc1;
    nod::scoped_connection _sc2;

};

#endif // KATARENGA_CLIENT_APP_BOARD_ENTITY_HPP
