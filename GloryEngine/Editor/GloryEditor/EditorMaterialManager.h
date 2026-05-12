#pragma once
#include "GloryEditor.h"

#include <UUID.h>
#include <MaterialManager.h>

#include <unordered_map>

#include <NodeRef.h>

namespace Glory
{
	class MaterialData;
	class IEngine;
	class PipelineData;

namespace Utils
{
	struct YAMLFileRef;
}

namespace Editor
{
	struct AssetCallbackData;
	class EditorApplication;

	/** @brief Material manager for the editor */
	class EditorMaterialManager : public MaterialManager
	{
	public:
		/** @brief Constructor */
		EditorMaterialManager(EditorApplication* pApp);
		/** @brief Destructor */
		virtual ~EditorMaterialManager();

		/** @brief Initialize the manager by subscribing to asset events */
		void Initialize();
		/** @brief Unsubscribe asset events */
		void Cleanup();

		/** @brief Set a materials pipeline and update its properties
		 * @param pMaterial Material resource to update, set nullptr if not loaded
		 * @param materialID ID of the material
		 * @param pipelineID ID of the pipeline
		 */
		GLORY_EDITOR_API void SetMaterialPipeline(MaterialData* pMaterial, UUID materialID, UUID pipelineID);

		/** @brief Get a material or material instance by ID */
		GLORY_EDITOR_API virtual MaterialData* GetMaterial(UUID materialID) const override;

		GLORY_EDITOR_API virtual MaterialData* CreateRuntimeMaterial(UUID baseMaterial) override;

		GLORY_EDITOR_API virtual void DestroyRuntimeMaterials() override;

		/** @brief Read properties into a material
		 * @param properties Properties YAML data
		 * @param pMaterial Material to read the properties to
		 *
		 * @note Only updates properties that actually exist on the material
		 */
		GLORY_EDITOR_API void ReadPropertiesInto(Utils::NodeValueRef properties, MaterialData* pMaterial) const;
		/** @brief Write properties to YAML
		 * @param properties Properties YAML destination
		 * @param pMaterial Material to read the properties from
		 */
		GLORY_EDITOR_API void WritePropertiesTo(Utils::NodeValueRef properties, MaterialData* pMaterial) const;

	private:
		/** @brief Handler for @ref AssetCallbackType::CT_AssetRegistered events */
		void AssetAddedCallback(const AssetCallbackData& callback);

		/** @brief Callback for when a pipeline changes */
		void PipelineUpdateCallback(PipelineData* pPipeline);

		/** @brief Read properties into a temporary material
		 * @param properties Properties YAML data
		 * @param pMaterial Material to write the properties to
		 * @param resources Where to store resource IDs
		 */
		void LoadTemporary(Utils::NodeValueRef properties, MaterialData* pMaterial, std::vector<UUID>& resources) const;

	private:
		std::unordered_map<UUID, std::vector<UUID>> m_MaterialsPerPipeline;
		std::vector<UUID> m_SubResourceMaterials;
		std::vector<UUID> m_RuntimeMaterials;

		EditorApplication* m_pApplication;
		UUID m_AssetRegisteredCallback;
		UUID m_PipelineUpdatedCallback;
	};
}
}