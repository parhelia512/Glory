#pragma once
#include "TextureAtlas.h"
#include "GraphicsHandles.h"
#include "GraphicsDevice.h"

#include <engine_visibility.h>

#include <memory>

namespace Glory
{
	class GraphicsDevice;

	/** @brief Texture atlas for Open GL graphics API */
	class GPUTextureAtlas : public TextureAtlas
	{
	public:
		/** @brief Constructor
		 * @param createInfo Creation info for the attached texture
		 * @param pEngine Engine instance
		 */
		GLORY_ENGINE_API GPUTextureAtlas(TextureCreateInfo&& createInfo, IEngine* pEngine, TextureHandle texture=0);
		/** @brief Destructor */
		GLORY_ENGINE_API virtual ~GPUTextureAtlas();

		/** @brief Get the attached texture */
		GLORY_ENGINE_API virtual TextureHandle GetTexture() const override;
		/** @brief Initialize the atlas by creating the render texture */
		GLORY_ENGINE_API virtual void Initialize() override;

	private:
		/** @brief GPU implementation for resizing the atlas */
		virtual void OnResize() override;

	private:
		TextureCreateInfo m_TextureInfo;
		TextureHandle m_Texture;
	};
}
