#pragma once
#include "TextureData.h"

#include <engine_visibility.h>

namespace Glory
{
	class ImageData;

	class InternalTexture : public TextureData
	{
	public:
		GLORY_ENGINE_API InternalTexture();
		GLORY_ENGINE_API InternalTexture(ImageData* pImage);
		GLORY_ENGINE_API virtual ~InternalTexture();

		GLORY_ENGINE_API virtual ImageData* GetImageData(Resources* pAssetManager);

		GLORY_ENGINE_API void Serialize(Utils::BinaryStream& container) const override;
		GLORY_ENGINE_API void Deserialize(Utils::BinaryStream& container) override;

	private:
		ImageData* m_pImage;
	};
}
