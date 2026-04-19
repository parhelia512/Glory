#pragma once
#include "Resource.h"
#include "MaterialPropertyInfo.h"
#include "GraphicsEnums.h"
#include "AssetReference.h"
#include "TextureData.h"

#include <engine_visibility.h>

#include <vector>
#include <unordered_map>
#include <mutex>

namespace Glory
{
    class FileData;
    class PipelineData;
    class PipelineManager;

    class MaterialData : public Resource
    {
    public:
        GLORY_ENGINE_API MaterialData();
        GLORY_ENGINE_API MaterialData(MaterialData&&) noexcept = default;
        GLORY_ENGINE_API MaterialData& operator=(MaterialData&&) noexcept = default;
        GLORY_ENGINE_API virtual ~MaterialData();

        GLORY_ENGINE_API void AddProperty(const std::string& displayName, const std::string& shaderName,
            uint32_t typeHash, size_t size, uint32_t flags = 0);
        GLORY_ENGINE_API void AddResourceProperty(const std::string& displayName, const std::string& shaderName,
            uint32_t typeHash, UUID resourceUUID, TextureType type, uint32_t flags = 0);
        GLORY_ENGINE_API void AddProperty(const MaterialPropertyInfo& other);

        GLORY_ENGINE_API void SetPipeline(PipelineData* pPipeline);
        GLORY_ENGINE_API void SetPipeline(UUID pipelineID);
        GLORY_ENGINE_API virtual PipelineData* GetPipeline(const PipelineManager& pipelineManager) const;
        GLORY_ENGINE_API virtual UUID GetPipelineID() const;

        GLORY_ENGINE_API [[nodiscard]]virtual size_t PropertyInfoCount() const;
        GLORY_ENGINE_API virtual MaterialPropertyInfo* GetPropertyInfoAt(size_t index);
        GLORY_ENGINE_API [[nodiscard]]virtual size_t GetCurrentBufferOffset() const;
        GLORY_ENGINE_API std::vector<char>& GetBufferReference();
        GLORY_ENGINE_API bool GetPropertyInfoIndex(const std::string& name, size_t& index) const;
        GLORY_ENGINE_API bool GetPropertyInfoIndex(TextureType textureType, size_t texIndex, size_t& index) const;
        GLORY_ENGINE_API [[nodiscard]]size_t ResourceCount() const;
        GLORY_ENGINE_API ResourceReference<TextureData>* GetResourceUUIDPointer(size_t index);
        GLORY_ENGINE_API [[nodiscard]]virtual size_t GetResourcePropertyCount() const;
        GLORY_ENGINE_API MaterialPropertyInfo* GetResourcePropertyInfo(size_t index);
        GLORY_ENGINE_API [[nodiscard]]virtual size_t GetPropertyIndexFromResourceIndex(size_t index) const;
        GLORY_ENGINE_API void ClearProperties();

        GLORY_ENGINE_API size_t TextureCount(TextureType textureType) const;

        GLORY_ENGINE_API void Serialize(Utils::BinaryStream& container) const override;
        GLORY_ENGINE_API void Deserialize(Utils::BinaryStream& container) override;

        GLORY_ENGINE_API void References(IEngine* pEngine, std::vector<UUID>& references) const override;

        GLORY_ENGINE_API void CopyProperties(void* dst) const;
        GLORY_ENGINE_API size_t PropertyDataSize() const;

        GLORY_ENGINE_API MaterialData* CreateCopy() const;

        GLORY_ENGINE_API virtual uint32_t TextureSetBits() const;

    public: // Properties
        // Setters
        template<typename T>
        inline void Set(const std::string& name, const T& value)
        {
            size_t index;
            if (!GetPropertyInfoIndex(name, index)) return;
            GetPropertyInfoAt(index)->Write<T>(m_PropertyBuffer, value);
        }

        // Getters
        template<typename T>
        inline bool Get(const std::string& name, T& value)
        {
            size_t index;
            if (!GetPropertyInfoIndex(name, index)) return false;
            return GetPropertyInfoAt(index)->Read<T>(m_PropertyBuffer, value);
        }

        GLORY_ENGINE_API void* Address(size_t index);

        GLORY_ENGINE_API void SetTexture(const std::string& name, TextureData* value);
        GLORY_ENGINE_API void SetTexture(const std::string& name, UUID uuid);
        GLORY_ENGINE_API void SetTexture(TextureType textureType, size_t texIndex, UUID uuid);
        GLORY_ENGINE_API bool GetTexture(const std::string& name, TextureData** value, Resources* pResources);
        GLORY_ENGINE_API bool GetTexture(const std::string& name, UUID* texID);

    protected:
        friend class PipelineData;

        UUID m_Pipeline;
        std::vector<MaterialPropertyInfo> m_PropertyInfos;
        std::vector<char> m_PropertyBuffer;

        std::vector<size_t> m_ResourcePropertyInfoIndices;
        std::vector<ResourceReference<TextureData>> m_Resources;
        uint32_t m_TextureSetBits;
        std::unordered_map<uint32_t, size_t> m_HashToPropertyInfoIndex;

        std::vector<std::vector<size_t>> m_TextureTypeIndices;

        size_t m_CurrentOffset;
    };
}
