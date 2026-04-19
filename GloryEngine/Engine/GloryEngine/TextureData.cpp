#include "TextureData.h"

#include <BinaryStream.h>

namespace Glory
{
	TextureData::TextureData() : m_Image(UUID(0)), m_SamplerSettings()
	{
		APPEND_TYPE(TextureData);
	}

	TextureData::TextureData(ImageData* pImageData) : m_Image(pImageData), m_SamplerSettings()
	{
		APPEND_TYPE(TextureData);
	}

	TextureData::~TextureData()
	{
	}

	ImageData* TextureData::GetImageData(Resources* pResources)
	{
		return Image() ? Image().Get(pResources) : nullptr;
	}

	ResourceReference<ImageData>& TextureData::Image()
	{
		return m_Image;
	}

	SamplerSettings& TextureData::GetSamplerSettings()
	{
		return m_SamplerSettings;
	}

	void TextureData::Serialize(Utils::BinaryStream& container) const
	{
		/* Write image ID */
		container.Write(m_Image.GetUUID()).Write(m_SamplerSettings);
	}

	void TextureData::Deserialize(Utils::BinaryStream& container)
	{
		UUID imageID;
		container.Read(imageID).Read(m_SamplerSettings);
		m_Image.SetUUID(imageID);
	}

	void TextureData::References(IEngine*, std::vector<UUID>& references) const
	{
		if (m_Image.GetUUID()) references.push_back(m_Image.GetUUID());
	}
}
