#include "FontData.h"
#include "Resources.h"
#include "TextureData.h"
#include "IEngine.h"

#include <BinaryStream.h>

namespace Glory
{
	FontData::FontData(): m_FontHeight(0), m_Texture(0ull), m_Material(0ull)
	{
		APPEND_TYPE(FontData);
	}
	
	FontData::FontData(uint32_t height, std::vector<uint64_t>&& characterCodes,
		std::vector<GlyphData>&& chars):
		m_FontHeight(height), m_CharacterCodes(std::move(characterCodes)),
		m_Glyphs(std::move(chars)), m_Texture(0ull), m_Material(0ull)
	{
		APPEND_TYPE(FontData);
	}

	FontData::~FontData()
	{
		m_Glyphs.clear();
	}

	uint32_t FontData::FontHeight() const
	{
		return m_FontHeight;
	}

	size_t FontData::GetGlyphIndex(uint64_t c) const
	{
		auto iter = std::find(m_CharacterCodes.begin(), m_CharacterCodes.end(), c);
		if (iter == m_CharacterCodes.end()) return m_CharacterCodes.size();
		return iter - m_CharacterCodes.begin();
	}

	const GlyphData* FontData::GetGlyph(size_t index) const
	{
		if (index >= m_Glyphs.size()) return nullptr;
		return &m_Glyphs[index];
	}

	TextureData* FontData::GetGlyphTexture(Resources& resources) const
	{
		Resource* pResource = resources.GetResource(m_Texture.GetUUID());
		return pResource ? static_cast<TextureData*>(pResource) : nullptr;
	}

	void FontData::SetTexture(UUID texture)
	{
		m_Texture.SetUUID(texture);
	}

	void FontData::SetMaterial(UUID material)
	{
		m_Material.SetUUID(material);
	}

	UUID FontData::Texture()
	{
		return m_Texture.GetUUID();
	}

	UUID FontData::Material()
	{
		return m_Material.GetUUID();
	}

	void FontData::Serialize(Utils::BinaryStream& container) const
	{
		container.Write(m_FontHeight).Write(m_Glyphs).
			Write(m_Texture.GetUUID()).Write(m_Material.GetUUID());
	}

	void FontData::Deserialize(Utils::BinaryStream& container)
	{
		UUID textureID, materialID;

		container.Read(m_FontHeight).Read(m_Glyphs).
			Read(textureID).Read(materialID);
		m_CharacterCodes.resize(m_Glyphs.size());
		for (size_t i = 0; i < m_CharacterCodes.size(); ++i)
			m_CharacterCodes[i] = m_Glyphs[i].Code;

		m_Texture = textureID;
		m_Material = materialID;
	}

	void FontData::References(IEngine* pEngine, std::vector<UUID>& references) const
	{
		if (m_Texture)
		{
			references.push_back(m_Texture.GetUUID());
			Resource* pTextureResource = pEngine->GetResources().GetResource(m_Texture.GetUUID());
			if (pTextureResource) pTextureResource->References(pEngine, references);
		}
		if (m_Material)
		{
			references.push_back(m_Material.GetUUID());
			Resource* pMaterialResource = pEngine->GetResources().GetResource(m_Material.GetUUID());
			if (pMaterialResource) pMaterialResource->References(pEngine, references);
		}
	}
}
