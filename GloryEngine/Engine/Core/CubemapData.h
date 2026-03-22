#pragma once
#include "TextureData.h"

#include <engine_visibility.h>

namespace Glory
{
	class CubemapData : public TextureData
	{
    public:
        GLORY_ENGINE_API CubemapData();
        GLORY_ENGINE_API CubemapData(UUID right, UUID left, UUID down, UUID up, UUID front, UUID back);
        GLORY_ENGINE_API CubemapData(ImageData* pRight, ImageData* pLeft, ImageData* pDown, ImageData* pUp, ImageData* pFront, ImageData* pBack);
        GLORY_ENGINE_API virtual ~CubemapData();

        GLORY_ENGINE_API virtual ImageData* GetImageData(AssetManager* pAssetManager, size_t face);
        GLORY_ENGINE_API AssetReference<ImageData>& Image(size_t face);
        GLORY_ENGINE_API SamplerSettings& GetSamplerSettings();

        GLORY_ENGINE_API void Serialize(Utils::BinaryStream& container) const override;
        GLORY_ENGINE_API void Deserialize(Utils::BinaryStream& container) override;

        GLORY_ENGINE_API void References(IEngine* pEngine, std::vector<UUID>& references) const override;

    private:
        friend class TextureDataLoaderModule;
        friend class ImageLoaderModule;
        AssetReference<ImageData> m_Faces[6];
        SamplerSettings m_SamplerSettings;
	};
}
