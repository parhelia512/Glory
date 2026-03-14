#include "TextManager.h"

#include "GScene.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "IEngine.h"
#include "VertexHelpers.h"
#include "RenderData.h"
#include "AssetManager.h"

#include <EntityRegistry.h>

namespace Glory
{
	TextManager::TextManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity):
		ComponentManager(pRegistry, capacity), m_pSceneManager(nullptr),
		m_pAssetManager(nullptr), m_pLayerManager(nullptr)
	{
	}

	TextManager::~TextManager()
	{
	}

	void TextManager::OnDrawImpl(Utils::ECS::EntityID entity, TextComponent& pComponent)
	{
		if (!pComponent.m_Font || pComponent.m_Text.empty()) return;

		GScene* pScene = m_pRegistry->GetUserData<GScene>();
		Renderer* pRenderer = m_pSceneManager->GetRenderer();
		if (!pRenderer) return;

		FontData* pFont = pComponent.m_Font.Get(m_pAssetManager);
		if (!pFont) return;

		Transform& transform = m_pRegistry->GetComponent<Transform>(entity);

		LayerMask mask;
		if (m_pRegistry->HasComponent<LayerComponent>(entity))
		{
			LayerComponent& layer = m_pRegistry->GetComponent<LayerComponent>(entity);
			mask = layer.m_Layer.Layer(m_pLayerManager) != nullptr ? layer.m_Layer.Layer(m_pLayerManager)->m_Mask : LayerMask(0ull);
		}

		const UUID fontID = pComponent.m_Font.AssetUUID();

		TextData textData;
		textData.m_Text = pComponent.m_Text;
		textData.m_TextDirty = pComponent.m_Dirty;
		textData.m_Scale = pComponent.m_Scale;
		textData.m_Alignment = pComponent.m_Alignment;
		textData.m_TextWrap = pComponent.m_WrapWidth;
		textData.m_Color = pComponent.m_Color;
		pComponent.m_Dirty = false;

		RenderData renderData;
		renderData.m_World = transform.MatTransform;
		renderData.m_LayerMask = mask;
		renderData.m_ObjectID = pScene->GetEntityUUID(entity);
		renderData.m_SceneID = pScene->GetUUID();
		renderData.m_MeshID = renderData.m_ObjectID;
		renderData.m_MaterialID = pFont->Material();

		Resource* pMeshResource = m_pAssetManager->FindResource(renderData.m_MeshID);
		if (!pMeshResource)
		{
			pMeshResource = new MeshData(textData.m_Text.size()*4, sizeof(VertexPosColorTex),
				{ AttributeType::Float2, AttributeType::Float3, AttributeType::Float2 });
			pMeshResource->SetResourceUUID(renderData.m_MeshID);
			m_pAssetManager->AddLoadedResource(pMeshResource);
			textData.m_TextDirty = true;
		}

		if (textData.m_TextDirty)
		{
			MeshData* pMeshData = static_cast<MeshData*>(pMeshResource);
			const float textWrap = textData.m_TextWrap*textData.m_Scale*pFont->FontHeight();
			Utils::GenerateTextMesh(pMeshData, pFont, textData, textWrap);
		}

		pRenderer->SubmitDynamic(std::move(renderData));
	}

	void TextManager::GetReferencesImpl(std::vector<UUID>& references) const
	{
		for (size_t i = 0; i < Size(); ++i)
		{
			const TextComponent& text = GetAt(i);
			const UUID font = text.m_Font.AssetUUID();
			if (font) references.push_back(font);
		}
	}

	void TextManager::OnInitialize()
	{
		Bind(DoDraw, &TextManager::OnDrawImpl);
		Bind(DoGetReferences, &TextManager::GetReferencesImpl);
	}
}
