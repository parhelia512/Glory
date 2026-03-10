#include "CameraRef.h"
#include "CameraManager.h"
#include "Camera.h"

namespace Glory
{
	CameraRef::CameraRef() : m_pManager(nullptr), m_CameraID(0)
	{
	}

	CameraRef::CameraRef(std::nullptr_t) : m_pManager(nullptr), m_CameraID(0)
	{
	}

	bool CameraRef::operator==(const CameraRef& other) const
	{
		return m_CameraID == other.m_CameraID;
	}

	CameraRef::CameraRef(CameraManager* pManager, UUID uuid) : m_pManager(pManager), m_CameraID(uuid)
	{
	}

	void CameraRef::SetBaseResolution(uint32_t width, uint32_t height)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetBaseResolution(width, height);
	}

	const glm::uvec2& CameraRef::GetBaseResolution() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetBaseResolution();
	}

	void CameraRef::SetResolutionScale(float width, float height)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetResolutionScale(width, height);
	}

	void CameraRef::SetPerspectiveProjection(float halfFOV, float near, float far)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetPerspectiveProjection(halfFOV, near, far);
	}

	void CameraRef::SetOutput(bool output, int x, int y)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetOutput(output, x, y);
	}

	bool CameraRef::IsOutput() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->IsOutput();
	}

	void CameraRef::SetOrthographicProjection(float near, float far)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetOrthographicProjection(near, far);
	}

	void CameraRef::SetView(const glm::mat4& view)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetView(view);
	}

	void CameraRef::SetPriority(int priority)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetPriority(priority);
	}

	void CameraRef::SetLayerMask(const LayerMask& layerMask)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetLayerMask(layerMask);
	}

	void CameraRef::SetClearColor(const glm::vec4& clearColor)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetClearColor(clearColor);
	}

	void CameraRef::SetUserData(const std::string& name, void* data)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		pCamera->SetUserData(name, data);
	}

	bool CameraRef::IsResolutionDirty()
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->IsResolutionDirty();
	}

	bool CameraRef::IsPerspectiveDirty()
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->IsPerspectiveDirty();
	}

	void CameraRef::SetResolutionDirty(bool dirty)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->SetResolutionDirty(dirty);
	}

	void CameraRef::SetPerspectiveDirty(bool dirty)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->SetPerspectiveDirty(dirty);
	}

	void CameraRef::Focus(const BoundingSphere& boundingSphere)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->Focus(boundingSphere);
	}

	const glm::uvec2& CameraRef::GetResolution() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetResolution();
	}

	const glm::mat4& CameraRef::GetView() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetView();
	}

	glm::mat4 CameraRef::GetViewInverse() const
	{
		return glm::inverse(GetView());
	}

	const glm::mat4& CameraRef::GetProjection() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetProjection();
	}

	glm::mat4 CameraRef::GetProjectionInverse() const
	{
		return glm::inverse(GetProjection());
	}

	glm::mat4 CameraRef::GetFinalView() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetFinalView();
	}

	float* CameraRef::GetViewPointer()
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetViewPointer();
	}

	float* CameraRef::GetProjectionPointer()
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetProjectionPointer();
	}

	int CameraRef::GetPriority() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetPriority();
	}

	const glm::vec4& CameraRef::GetClearColor() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetClearColor();
	}

	const LayerMask& CameraRef::GetLayerMask() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetLayerMask();
	}

	void CameraRef::Free()
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		m_pManager->SetUnused(pCamera);
	}

	uint64_t& CameraRef::GetUserHandle(const std::string& name)
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		return pCamera->GetUserHandle(name);
	}

	float CameraRef::GetNear() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetNear();
	}

	float CameraRef::GetFar() const
	{
		Camera* pCamera = m_pManager->GetCamera(m_CameraID);
		assert(pCamera);
		return pCamera->GetFar();
	}

	UUID CameraRef::GetUUID() const
	{
		return m_CameraID;
	}
}
