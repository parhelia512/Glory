#pragma once
#include "ImageData.h"
#include "AssetReference.h"

#include <engine_visibility.h>

namespace Glory
{
    class Resources;
    class IEngine;

	class TextureData : public Resource
	{
    public:
        GLORY_ENGINE_API TextureData();
        GLORY_ENGINE_API TextureData(ImageData* pImageData);
        GLORY_ENGINE_API TextureData(TextureData&&) noexcept = default;
        GLORY_ENGINE_API TextureData& operator=(TextureData&&) noexcept = default;
        GLORY_ENGINE_API virtual ~TextureData();

        GLORY_ENGINE_API virtual ImageData* GetImageData(Resources* pResources);
        GLORY_ENGINE_API ResourceReference<ImageData>& Image();
        GLORY_ENGINE_API SamplerSettings& GetSamplerSettings();

        GLORY_ENGINE_API void Serialize(Utils::BinaryStream& container) const override;
        GLORY_ENGINE_API void Deserialize(Utils::BinaryStream& container) override;

        GLORY_ENGINE_API void References(IEngine* pEngine, std::vector<UUID>& references) const override;

    private:
        friend class TextureDataLoaderModule;
        friend class ImageLoaderModule;
        ResourceReference<ImageData> m_Image;
        SamplerSettings m_SamplerSettings;
	};
}
