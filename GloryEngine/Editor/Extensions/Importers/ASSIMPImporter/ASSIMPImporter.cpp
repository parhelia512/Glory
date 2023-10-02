#include "ASSIMPImporter.h"
#include "VertexDefinitions.h"

#include <Debug.h>
#include <sstream>
#include <ModelData.h>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

constexpr size_t NumSupportedExtensions = 2;
constexpr std::string_view SupportedExtensions[NumSupportedExtensions] = {
    ".fbx",
    ".obj",
};

namespace Glory::Editor
{
    ASSIMPImporter::ASSIMPImporter()
	{
	}

    ASSIMPImporter::~ASSIMPImporter()
	{
	}

    bool ASSIMPImporter::SupportsExtension(const std::filesystem::path& extension) const
    {
        for (size_t i = 0; i < NumSupportedExtensions; i++)
        {
            const std::string_view ext = SupportedExtensions[i];
            if (extension.compare(ext) != 0) continue;
            return true;
        }
        return false;
    }

    ModelData* ASSIMPImporter::LoadResource(const std::filesystem::path& path) const
    {
        Assimp::Importer importer;

        const std::string pathStr = path.string();
        const aiScene* pScene = importer.ReadFile(pathStr, aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

        //const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        //const aiScene* pScene = importer.ReadFile(path, 0);

        if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
        {
            Debug::LogError("ASSIMP: Could not import file: " + pathStr + " Error: " + importer.GetErrorString());
            return nullptr;
        }
        //directory = file.substr(0, file.find_last_of('/'));

        ModelData* pModel = new ModelData();
        ProcessNode(pScene->mRootNode, pScene, pModel);

        importer.FreeScene();

        return pModel;
    }

    void ASSIMPImporter::SaveResource(const std::filesystem::path& path, ModelData* pResource) const
    {
        Debug::LogError("Exporting model files is not supported by the ASSIMPImporter extension");
    }

    void ASSIMPImporter::ProcessNode(aiNode* node, const aiScene* scene, ModelData* pModel) const
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            MeshData* pMeshData = ProcessMesh(mesh);
            pModel->AddSubresource(pMeshData, pMeshData->Name());
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene, pModel);
        }
    }

    MeshData* ASSIMPImporter::ProcessMesh(aiMesh* mesh) const
    {
        float* vertices = nullptr;
        std::vector<AttributeType> attributes;
        std::vector<uint32_t> indices;
        uint32_t vertexSize = 0;
        size_t arraySize = 0;
        //vector<Texture> textures;

        if (mesh->mNormals == nullptr)
        {
            Debug::LogError("Mesh has no normals");
            return nullptr;
        }

        attributes.push_back(AttributeType::Float3);
        attributes.push_back(AttributeType::Float3);
        attributes.push_back(AttributeType::Float3);
        attributes.push_back(AttributeType::Float3);
        attributes.push_back(AttributeType::Float2);
        attributes.push_back(AttributeType::Float4);
        vertexSize = sizeof(float) * 18;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            std::vector<float> vertexData;
            // process vertex positions, normals and texture coordinates
            vertexData.push_back(mesh->mVertices[i].x);
            vertexData.push_back(mesh->mVertices[i].y);
            vertexData.push_back(mesh->mVertices[i].z);
            vertexData.push_back(mesh->mNormals[i].x);
            vertexData.push_back(mesh->mNormals[i].y);
            vertexData.push_back(mesh->mNormals[i].z);

            // Tangents and Bitangents
            vertexData.push_back(mesh->mTangents[i].x);
            vertexData.push_back(mesh->mTangents[i].y);
            vertexData.push_back(mesh->mTangents[i].z);
            vertexData.push_back(mesh->mBitangents[i].x);
            vertexData.push_back(mesh->mBitangents[i].y);
            vertexData.push_back(mesh->mBitangents[i].z);

            if (mesh->mTextureCoords[0])
            {
                vertexData.push_back(mesh->mTextureCoords[0][i].x);
                vertexData.push_back(1.0f - mesh->mTextureCoords[0][i].y);

                //if (i == 0)
                //{
                //    attributes.push_back(AttributeType::Float2);
                //    vertexSize += sizeof(float) * 2;
                //}
            }
            else
            {
                vertexData.push_back(0.0f);
                vertexData.push_back(0.0f);
            }

            if (mesh->mColors[0])
            {
                vertexData.push_back(mesh->mColors[0][i].r);
                vertexData.push_back(mesh->mColors[0][i].g);
                vertexData.push_back(mesh->mColors[0][i].b);
                vertexData.push_back(mesh->mColors[0][i].a);
            }
            else
            {
                vertexData.push_back(1.0f);
                vertexData.push_back(1.0f);
                vertexData.push_back(1.0f);
                vertexData.push_back(1.0f);
            }
            if (i == 0)
            {
                arraySize = mesh->mNumVertices * (vertexSize / sizeof(float));
                vertices = new float[arraySize];
            }

            //vertices[i] = RawVertex(vertexData);
            memcpy(&vertices[i * (vertexSize / sizeof(float))], &vertexData[0], vertexSize);
        }

        // process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        std::vector<float> verticesVector;
        verticesVector.assign(&vertices[0], &vertices[arraySize]);
        std::stringstream stream;
        stream << mesh->mName.C_Str() << " Material " << mesh->mMaterialIndex;
        MeshData* pMesh = new MeshData(mesh->mNumVertices, vertexSize, verticesVector, (uint32_t)indices.size(), indices, attributes);
        pMesh->SetName(stream.str());
        delete[] vertices;
        return pMesh;
    }
}
