#include "PawnRenderEntity.hpp"

// Katarenga
#include <common/common_utils.hpp>

PawnRenderEntity::PawnRenderEntity(const std::shared_ptr<PawnEntity>& pawn_entity) : Entity(),
    m_material(addComponent<PhongMaterial>()),
    m_transform(addComponent<Transform>()),
    _weak_pawn_entity(pawn_entity)
{
    m_transform->setScale(glm::vec3(.1f));

    auto f = std::bind(&PawnRenderEntity::update, this);

    pawn_entity->case_changed.connect(f);
    pawn_entity->actor_changed.connect(f);

    update();
}

PawnEntity::SPtr PawnRenderEntity::pawn_entity() const
{
    return PawnEntity::SPtr(_weak_pawn_entity);
}

void PawnRenderEntity::update()
{
    PawnEntity::SPtr pawn = pawn_entity();

    if(!pawn)
        return;

    // update Transform
    {
        Common::Case c = pawn->c();

        int row = case_row(c);
        int column = case_col(c);

        m_transform->setTranslation(column * 2.f, row * 2.f, 0.f);
    }

    // update PhongMaterial
    {
        glm::vec4 diffuse_color = (pawn->actor() == Common::GameActor::White ? glm::vec4(1,0,0,1) : glm::vec4(0,1,0,1));

        m_material->setDiffuseColor(diffuse_color);
    }
}
