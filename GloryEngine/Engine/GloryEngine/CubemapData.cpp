#include "CubemapData.h"
#include "ImageData.h"

#include <BinaryStream.h>

namespace Glory
{
	CubemapData::CubemapData():
		m_SamplerSettings{ Filter::F_Linear, Filter::F_Linear, SamplerAddressMode::SAM_Repeat,
			SamplerAddressMode::SAM_Repeat, SamplerAddressMode::SAM_Repeat, 16.f, false, false,
			CompareOp::OP_Never, Filter::F_None, 0.0f, 0.0f, 1000.0f }
	{
		APPEND_TYPE(CubemapData);
	}

	CubemapData::CubemapData(UUID right, UUID left, UUID down, UUID up, UUID front, UUID back):
		m_Faces{ right, left, down, up, front, back },
		m_SamplerSettings{ Filter::F_Linear, Filter::F_Linear, SamplerAddressMode::SAM_Repeat,
			SamplerAddressMode::SAM_Repeat, SamplerAddressMode::SAM_Repeat, 16.f, false, false,
			CompareOp::OP_Never, Filter::F_None, 0.0f, 0.0f, 1000.0f }
	{
		APPEND_TYPE(CubemapData);
	}

	CubemapData::CubemapData(ImageData* pRight, ImageData* pLeft, ImageData* pDown, ImageData* pUp, ImageData* pFront, ImageData* pBack):
		m_Faces{ pRight->GetUUID(), pLeft->GetUUID(), pDown->GetUUID(), pUp->GetUUID(), pFront->GetUUID(), pBack->GetUUID() },
		m_SamplerSettings{ Filter::F_Linear, Filter::F_Linear, SamplerAddressMode::SAM_Repeat,
			SamplerAddressMode::SAM_Repeat, SamplerAddressMode::SAM_Repeat, 16.f, false, false,
			CompareOp::OP_Never, Filter::F_None, 0.0f, 0.0f, 1000.0f }
	{
		APPEND_TYPE(CubemapData);
	}

	CubemapData::~CubemapData()
	{
	}

	ImageData* CubemapData::GetImageData(Resources* pResources, size_t face)
	{
		auto& faceImage = Image(face);
		return faceImage ? faceImage.Get(pResources) : nullptr;
	}

	ResourceReference<ImageData>& CubemapData::Image(size_t face)
	{
		return m_Faces[face];
	}

	SamplerSettings& CubemapData::GetSamplerSettings()
	{
		return m_SamplerSettings;
	}

	void CubemapData::Serialize(Utils::BinaryStream& container) const
	{
		for (size_t i = 0; i < 6; ++i)
		{
			const UUID faceID = m_Faces[i].GetUUID();
			container.Write(faceID);
		}
		container.Write(m_SamplerSettings);
	}

	void CubemapData::Deserialize(Utils::BinaryStream& container)
	{
		for (size_t i = 0; i < 6; ++i)
		{
			UUID faceID;
			container.Read(faceID);
			m_Faces[i].SetUUID(faceID);
		}
		container.Read(m_SamplerSettings);
	}

	void CubemapData::References(IEngine* pEngine, std::vector<UUID>& references) const
	{
		for (size_t i = 0; i < 6; ++i)
		{
			const UUID faceID = m_Faces[i].GetUUID();
			if (faceID) references.push_back(faceID);
		}
	}
}
