#pragma once
#include "FileData.h"
#include "FontDataStructs.h"
#include "AssetReference.h"

#include "TextureData.h"
#include "MaterialData.h"

#include <engine_visibility.h>

namespace Glory
{
    class Resources;

    class FontData : public Resource
    {
    public:
        GLORY_ENGINE_API FontData();
        GLORY_ENGINE_API FontData(uint32_t height, std::vector<uint64_t>&& characterCodes,
            std::vector<GlyphData>&& chars);
        GLORY_ENGINE_API FontData(FontData&&) noexcept = default;
        GLORY_ENGINE_API FontData& operator=(FontData&&) noexcept = default;
        GLORY_ENGINE_API virtual ~FontData();

        GLORY_ENGINE_API uint32_t FontHeight() const;
        GLORY_ENGINE_API size_t GetGlyphIndex(uint64_t c) const;
        GLORY_ENGINE_API const GlyphData* GetGlyph(size_t index) const;
        GLORY_ENGINE_API TextureData* GetGlyphTexture(Resources& assets) const;
        GLORY_ENGINE_API void SetTexture(UUID texture);
        GLORY_ENGINE_API void SetMaterial(UUID material);
        GLORY_ENGINE_API UUID Texture();
        GLORY_ENGINE_API UUID Material();

        GLORY_ENGINE_API void Serialize(Utils::BinaryStream& container) const override;
        GLORY_ENGINE_API void Deserialize(Utils::BinaryStream& container) override;

    private:
        void References(IEngine* pEngine, std::vector<UUID>& references) const override;

    private:
        uint32_t m_FontHeight;
        std::vector<uint64_t> m_CharacterCodes;
        std::vector<GlyphData> m_Glyphs;
        ResourceReference<TextureData> m_Texture;
        ResourceReference<MaterialData> m_Material;
    };
}
