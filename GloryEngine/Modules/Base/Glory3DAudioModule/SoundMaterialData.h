#pragma once
#include <Resource.h>

#include <glm/glm.hpp>

#include <Reflection.h>

namespace Glory::Utils
{
    class BinaryStream;
}

namespace Glory
{
    /** @brief Sound materia; */
    struct SoundMaterial
    {
        SoundMaterial():
            m_Absorption{ 0.2f, 0.2f, 0.2f }, m_Scattering{ 0.5f }, m_Transmission{0.1f, 0.1f, 0.1f} {}

        REFLECTABLE(SoundMaterial,
            (glm::vec3)(m_Absorption),
            (float)(m_Scattering),
            (glm::vec3)(m_Transmission)
        );
    };

    /** @brief Resource that can store a sound material */
    class SoundMaterialData : public Resource
    {
    public:
        SoundMaterialData();
        SoundMaterialData(SoundMaterial&& material);
        /** @overload */
        SoundMaterialData(SoundMaterialData&&) noexcept = default;
        /** @brief Move assignment operator */
        SoundMaterialData& operator=(SoundMaterialData&&) noexcept = default;
        virtual ~SoundMaterialData();

        /** @brief Get the attached material */
        SoundMaterial& Material();
        /** @overload */
        const SoundMaterial& Material() const;

        void Serialize(Utils::BinaryStream& container) const override;
        void Deserialize(Utils::BinaryStream& container) override;

    private:
        virtual void References(IEngine*, std::vector<UUID>&) const override {}

    private:
        SoundMaterial m_Material;
    };
}
