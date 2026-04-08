#pragma once
#include "SoundMaterialData.h"

#include <UUID.h>
#include <AssetReference.h>
#include <BitSet.h>
#include <Resource.h>

#include <vector>

namespace Glory::Utils
{
	class BinaryStream;
}

namespace Glory
{
	class MeshData;
	struct SoundMaterial;

	/**
	 * @brief Meta data of an audio mesh
	 *
	 * The mesh can either be a generated mesh in the @ref AudioScene
	 * or a mesh asset from the project.
	 */
	struct AudioMesh
	{
		AudioMesh(uint64_t idOrIndex, size_t materialIndex):
			m_Mesh(idOrIndex), m_Material(materialIndex) {}
		AudioMesh() = default;

		union MeshUnion
		{
			UUID m_AssetID;
			size_t m_Index;
		} m_Mesh{0};

		size_t m_Material;
	};

	/**
	 * @brief Audio occlusion scene
	 *
	 * This scene contains a collection of meshes that can
	 * be used to do audio occlusion checks against.
	 */
	class AudioScene
	{
	public:
		/** @brief Constructor */
		AudioScene();
		/**
		 * @overload
		 * @param sceneID ID of the @ref GScene this audio scene is created for
		 */
		AudioScene(UUID sceneID);
		/** @overload */
		AudioScene(AudioScene&& other) noexcept;
		/** @brief Destructor */
		~AudioScene() = default;

		void operator=(AudioScene&& other) noexcept;

		/**
		 * @brief Add a mesh asset reference to the scene
		 * @param meshID ID of the mesh asset to add
		 * @param material Material of the mesh
		 */
		void AddMesh(UUID meshID, SoundMaterial&& material);
		/**
		 * @brief Add a mesh to the scene
		 * @param meshData Mesh to add
		 * @param material Material of the mesh
		 */
		void AddMesh(MeshData&& meshData, SoundMaterial&& material);

		/** @brief Serialize this audio scene into a stream */
		void Serialize(Utils::BinaryStream& stream) const;
		/** @brief Deserialize a stream into this audio scene */
		void Deserialize(Utils::BinaryStream& stream);

		/** @brief ID of the scene @ref GScene this audio scene was created for */
		UUID SceneID() const;

		/** @brief Number of meshes in this audio scene */
		size_t MeshCount() const;
		/**
		 * @brief Get a mesh in this audio scene
		 * @param index Index of the mesh
		 */
		const MeshData& Mesh(size_t index) const;
		/** @overload */
		MeshData& Mesh(size_t index);
		/**
		 * @brief Get the material for a mesh in this audio scene
		 * @param index Index of the mesh
		 */
		const SoundMaterial& Material(size_t index) const;
		/** @overload */
		SoundMaterial& Material(size_t index);

	private:
		UUID m_SceneID;
		std::vector<MeshData> m_MeshDatas;
		std::vector<AudioMesh> m_Meshes;
		std::vector<SoundMaterial> m_Materials;
		Utils::BitSet m_IsAssetBits;
	};

	/** @brief Resource wrapper for AudioScene to be used for asset serialization */
	class AudioSceneData : public Resource
	{
	public:
		/** @brief Constructor */
		AudioSceneData();
		/** @overload */
		AudioSceneData(AudioScene&& audioScene);
		/** @brief Destructor */
		virtual ~AudioSceneData() {}

		void Serialize(Utils::BinaryStream& container) const override;
		void Deserialize(Utils::BinaryStream& container) override;

		AudioScene m_AudioScene;

	private:
		virtual void References(IEngine*, std::vector<UUID>&) const override {}
	};
}
