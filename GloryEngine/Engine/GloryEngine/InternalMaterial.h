#pragma once
#include "MaterialData.h"

#include <engine_visibility.h>

namespace Glory
{
    class PipelineData;

    /** @brief Internal material data */
    class InternalMaterial : public MaterialData
    {
    public:
        /** @brief Constructor */
        GLORY_ENGINE_API InternalMaterial(PipelineData* pPipeline);

        GLORY_ENGINE_API virtual PipelineData* GetPipeline(const PipelineManager& pipelineManager) const override;
        GLORY_ENGINE_API virtual UUID GetPipelineID() const override;

    private:
        PipelineData* m_pPipeline;
    };
}
