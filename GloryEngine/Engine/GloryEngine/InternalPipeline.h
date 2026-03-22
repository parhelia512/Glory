#pragma once
#include "PipelineData.h"
#include "GraphicsEnums.h"

#include <engine_visibility.h>

namespace Glory
{
    class PipelineManager;

    /** @brief Internal pipeline data */
    class InternalPipeline : public PipelineData
    {
    public:
        /** @brief Constructor */
        GLORY_ENGINE_API InternalPipeline();
        GLORY_ENGINE_API InternalPipeline(std::vector<FileData*>&& compiledShaders, std::vector<ShaderType>&& shaderTypes);
        GLORY_ENGINE_API virtual ~InternalPipeline();

        GLORY_ENGINE_API void SetShaders(std::vector<FileData*>&& compiledShaders, std::vector<ShaderType>&& shaderTypes);

        GLORY_ENGINE_API virtual size_t ShaderCount() const override;
        GLORY_ENGINE_API virtual UUID ShaderID(size_t index) const override;
        GLORY_ENGINE_API virtual FileData* Shader(const PipelineManager& manager, size_t index) const override;
        GLORY_ENGINE_API virtual ShaderType GetShaderType(const PipelineManager& manager, size_t index) const override;

    private:
        std::vector<FileData*> m_pCompiledShaders;
        std::vector<ShaderType> m_ShaderTypes;
    };
}
