#pragma once
#include "GraphicsEnums.h"
#include "GraphicsDevice.h"

#include <engine_visibility.h>

namespace Glory
{
	class GraphicsDevice;

	GLORY_ENGINE_API void CreateBufferDescriptorLayoutAndSet(GraphicsDevice* pDevice, size_t numBuffers,
		const std::vector<uint32_t>& bindingIndices, const std::vector<BufferType>& bufferTypes,
		const std::vector<ShaderTypeFlag>& shaderStages, const std::vector<BufferHandle>& bufferHandles,
		const std::vector<std::pair<uint32_t, uint32_t>>& bufferOffsetsAndSizes, DescriptorSetLayoutHandle& layout,
		DescriptorSetHandle& set, ShaderTypeFlag constantsShaderStage=ShaderTypeFlag(0),
		uint32_t constantsOffset=0, uint32_t constantsSize=0);

	GLORY_ENGINE_API DescriptorSetLayoutHandle CreateBufferDescriptorLayout(GraphicsDevice* pDevice, size_t numBuffers,
		const std::vector<uint32_t>& bindingIndices, const std::vector<BufferType>& bufferTypes,
		const std::vector<ShaderTypeFlag>& shaderStages);

	GLORY_ENGINE_API DescriptorSetHandle CreateBufferDescriptorSet(GraphicsDevice* pDevice, size_t numBuffers,
		const std::vector<BufferHandle>& bufferHandles, const std::vector<std::pair<uint32_t, uint32_t>>& bufferOffsetsAndSizes,
		DescriptorSetLayoutHandle layout, uint32_t constantsOffset=0, uint32_t constantsSize=0);

	GLORY_ENGINE_API void CreateSamplerDescriptorLayoutAndSet(GraphicsDevice* pDevice, size_t numSamplers,
		const std::vector<uint32_t>& bindingIndices, const std::vector<ShaderTypeFlag>& shaderStages,
		std::vector<std::string>&& samplerNames, const std::vector<TextureHandle>& textureHandles,
		DescriptorSetLayoutHandle& layout, DescriptorSetHandle& set);

	GLORY_ENGINE_API DescriptorSetLayoutHandle CreateSamplerDescriptorLayout(GraphicsDevice* pDevice, size_t numSamplers,
		const std::vector<uint32_t>& bindingIndices, const std::vector<ShaderTypeFlag>& shaderStages,
		std::vector<std::string>&& samplerNames);

	GLORY_ENGINE_API DescriptorSetHandle CreateSamplerDescriptorSet(GraphicsDevice* pDevice, size_t numSamplers,
		const std::vector<TextureHandle>& textureHandles, DescriptorSetLayoutHandle layout);
}