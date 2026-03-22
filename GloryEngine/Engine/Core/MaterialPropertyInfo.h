#pragma once
#include "Resource.h"
#include "GraphicsEnums.h"

#include <engine_visibility.h>

#include <UUID.h>

#include <string>
#include <vector>

namespace Glory
{
	struct MaterialPropertyInfo
	{
	public:
		GLORY_ENGINE_API MaterialPropertyInfo();
		GLORY_ENGINE_API MaterialPropertyInfo(const MaterialPropertyInfo& other);
		GLORY_ENGINE_API MaterialPropertyInfo(const std::string& displayName, const std::string& shaderName, uint32_t typeHash, size_t size, size_t offset, uint32_t flags);
		GLORY_ENGINE_API MaterialPropertyInfo(const std::string& displayName, const std::string& shaderName, uint32_t typeHash, size_t offset, TextureType textureType, uint32_t flags);

		GLORY_ENGINE_API const std::string& DisplayName() const;
		GLORY_ENGINE_API const std::string& ShaderName() const;
		GLORY_ENGINE_API const uint32_t TypeHash() const;
		GLORY_ENGINE_API const size_t Size() const;
		GLORY_ENGINE_API const size_t Offset() const;
		GLORY_ENGINE_API const size_t EndOffset() const;
		GLORY_ENGINE_API bool IsResource() const;
		GLORY_ENGINE_API TextureType GetTextureType() const;
		GLORY_ENGINE_API uint32_t Flags() const;

		template<typename T>
		inline bool Read(const std::vector<char> buffer, T& data) const
		{
			if (m_TextureType) return false;
			if (sizeof(T) > m_Size) return false;
			if (buffer.size() > m_Offset + m_Size) return false;
			memcpy((void*)&data, &buffer[m_Offset], m_Size);
			return true;
		}

		template<typename T>
		inline bool Write(std::vector<char>& buffer, const T& data)
		{
			if (m_TextureType) return false;
			if (sizeof(T) > m_Size) return false;
			if (buffer.size() <= m_Offset + m_Size) buffer.resize(m_Offset + m_Size);
			memcpy(&buffer[m_Offset], (void*)&data, m_Size);
			return true;
		}

		GLORY_ENGINE_API void* Address(std::vector<char>& buffer);

		GLORY_ENGINE_API bool Write(std::vector<char>& buffer, const void* data);
		GLORY_ENGINE_API void Reserve(std::vector<char>& buffer);

		GLORY_ENGINE_API void SetDefaultValue(void* dst) const;

	private:
		friend class MaterialData;
		friend class PipelineData;
		std::string m_PropertyDisplayName;
		std::string m_PropertyShaderName;
		uint32_t m_TypeHash;
		size_t m_Size;
		size_t m_Offset;
		TextureType m_TextureType;
		uint32_t m_Flags;
	};
}
