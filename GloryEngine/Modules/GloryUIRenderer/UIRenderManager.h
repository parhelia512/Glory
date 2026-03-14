#pragma once
#include "UIComponents.h"

#include <ComponentManager.h>

namespace Glory
{
	class UIRendererModule;
	class AssetManager;
	class Debug;
	class InputModule;
	class LayerManager;

	class UIRenderManager : public Utils::ECS::ComponentManager<UIRenderer>
	{
	public:
		UIRenderManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=10);
		virtual ~UIRenderManager();

	public:
		void OnStartImpl(Utils::ECS::EntityID entity, UIRenderer& pComponent);
		void OnValidateImpl(Utils::ECS::EntityID entity, UIRenderer& pComponent);
		void OnDrawImpl(Utils::ECS::EntityID entity, UIRenderer& pComponent);
		void GetReferencesImpl(std::vector<UUID>& references) const;

	private:
		void OnInitialize() override;

	private:
		friend class UIRendererModule;
		UIRendererModule* m_pModule;
		AssetManager* m_pAssetManager;
		Debug* m_pDebug;
		InputModule* m_pInput;
		LayerManager* m_pLayers;
	};
}
