#ifndef KATARENGA_CLIENT_APP_PAWN_RENDER_ENTITY_HPP
#define KATARENGA_CLIENT_APP_PAWN_RENDER_ENTITY_HPP

// Katarenga
#include <PawnEntity.hpp>

// RenderEngine
#include <RenderEngine/Entity.h>
#include <RenderEngine/PhongMaterial.h>
#include <RenderEngine/Transform.h>

// Standard Library
#include <memory>

/**
 * @brief The PawnRenderEntity class
 */
class PawnRenderEntity : public Entity
{
public:
    using SPtr = std::shared_ptr<PawnRenderEntity>;

public:
    PawnRenderEntity(const std::shared_ptr<PawnEntity>& pawn_entity);

public:
    PawnEntity::SPtr pawn_entity() const;

private:
    void update();

private:
    PhongMaterial::SPtr m_material;
    Transform::SPtr     m_transform;

    PawnEntity::WPtr    _weak_pawn_entity;

};

#endif // KATARENGA_CLIENT_APP_PAWN_RENDER_ENTITY_HPP
