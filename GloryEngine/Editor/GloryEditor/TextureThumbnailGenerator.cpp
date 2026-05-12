#include "TextureThumbnailGenerator.h"
#include "Resources.h"
#include "EditorApplication.h"

namespace Glory::Editor
{
	TextureThumbnailGenerator::TextureThumbnailGenerator()
	{
	}

	TextureThumbnailGenerator::~TextureThumbnailGenerator()
	{
	}

	TextureData* TextureThumbnailGenerator::GetThumbnail(const ResourceMeta* pResourceType)
	{
		UUID id = pResourceType->ID();
		if (std::find(m_AlreadyRequestedThumbnails.begin(), m_AlreadyRequestedThumbnails.end(), id) != m_AlreadyRequestedThumbnails.end()) return nullptr;

		if (m_pLoadedTextures.find(id) != m_pLoadedTextures.end())
		{
			TextureData* pImage = m_pLoadedTextures.at(id);
			m_pLoadedTextures.erase(id);
			return pImage;
		}

		m_AlreadyRequestedThumbnails.push_back(id);

		Resource* pResource = EditorApplication::GetInstance()->GetEngine()->GetResources().GetResource(id);
		if (pResource)
		{
			UUID uuid = pResource->GetUUID();
			auto it = std::find(m_AlreadyRequestedThumbnails.begin(), m_AlreadyRequestedThumbnails.end(), uuid);
			m_pLoadedTextures[uuid] = (TextureData*)pResource;
			if (it == m_AlreadyRequestedThumbnails.end()) return nullptr;
			m_AlreadyRequestedThumbnails.erase(it);
		}
		return nullptr;
	}
}