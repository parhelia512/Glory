#include "MeshRendererEditor.h"
#include "EditorApplication.h"

#include <IEngine.h>
#include <Renderer.h>
#include <Resources.h>
#include <Components.h>

namespace Glory::Editor
{
	MeshRendererEditor::MeshRendererEditor()
	{
	}

	MeshRendererEditor::~MeshRendererEditor()
	{
	}

	void MeshRendererEditor::Initialize()
	{
		EntityComponentEditor::Initialize();
	}

	bool MeshRendererEditor::OnGUI()
	{
		EditorApplication* pApp = EditorApplication::GetInstance();

		Transform& transform = m_pComponentObject->GetRegistry()->GetComponent<Transform>(m_pComponentObject->EntityID());
		MeshRenderer& meshRenderer = GetTargetComponent();
		const UUID meshID = meshRenderer.m_Mesh.GetUUID();
		Resource* pMeshResource = pApp->GetEngine()->GetResources().GetResource(meshID);
		if (pMeshResource)
		{
			MeshData* pMeshData = static_cast<MeshData*>(pMeshResource);
			const BoundingBox& boundingBox = pMeshData->GetBoundingBox();
			const BoundingSphere& boundingSphere = pMeshData->GetBoundingSphere();
			pApp->GetEngine()->ActiveRenderer()
				->DrawLineBox(transform.MatTransform, boundingBox.m_Center, boundingBox.m_HalfExtends, { 1, 1, 0, 1 });
			pApp->GetEngine()->ActiveRenderer()
				->DrawLineSphere(transform.MatTransform, boundingSphere.m_Center, boundingSphere.m_Radius, { 1, 1, 0, 1 });
		}
		return EntityComponentEditor::OnGUI();
	}

	std::string MeshRendererEditor::Name()
	{
		return "Mesh Renderer";
	}
}