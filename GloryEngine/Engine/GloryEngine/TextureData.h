#pragma once
#include "ImageData.h"
#include "AssetReference.h"

#include <engine_visibility.h>

namespace Glory
{
    class AssetManager;
    class IEngine;

	class TextureData : public Resource
	{
    public:
        GLORY_ENGINE_API TextureData();
        GLORY_ENGINE_API TextureData(ImageData* pImageData);
        GLORY_ENGINE_API TextureData(TextureData&&) noexcept = default;
        GLORY_ENGINE_API TextureData& operator=(TextureData&&) noexcept = default;
        GLORY_ENGINE_API virtual ~TextureData();

        GLORY_ENGINE_API virtual ImageData* GetImageData(AssetManager* pAssetManager);
        GLORY_ENGINE_API AssetReference<ImageData>& Image();
        GLORY_ENGINE_API SamplerSettings& GetSamplerSettings();

        GLORY_ENGINE_API void Serialize(Utils::BinaryStream& container) const override;
        GLORY_ENGINE_API void Deserialize(Utils::BinaryStream& container) override;

        GLORY_ENGINE_API void References(IEngine* pEngine, std::vector<UUID>& references) const override;

    private:
        friend class TextureDataLoaderModule;
        friend class ImageLoaderModule;
        AssetReference<ImageData> m_Image;
        SamplerSettings m_SamplerSettings;
	};
}
