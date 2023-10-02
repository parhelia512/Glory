#pragma once
#include "MeshData.h"

#include <BaseEditorExtension.h>
#include <ImporterTemplate.h>

namespace Glory
{
    class ModelData;
}

struct aiNode;
struct aiMesh;
struct aiScene;

namespace Glory::Editor
{
    class ASSIMPImporter : public ImporterTemplate<ModelData>
    {
    public:
        ASSIMPImporter();
        virtual ~ASSIMPImporter();

    private:
        virtual bool SupportsExtension(const std::filesystem::path& extension) const override;
        virtual ModelData* LoadResource(const std::filesystem::path& path) const override;
        virtual void SaveResource(const std::filesystem::path& path, ModelData* pResource) const override;

        virtual void Initialize() override {};
        virtual void Cleanup() override {};

    private:
        void ProcessNode(aiNode* node, const aiScene* scene, ModelData* pModel) const;
        MeshData* ProcessMesh(aiMesh* mesh) const;
    };
}
