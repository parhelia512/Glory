#pragma once
#include <engine_visibility.h>

#include <atomic>

#include <UUID.h>

namespace Glory
{
	class Resources;

	class ResourceLoader
	{
	public:
		GLORY_ENGINE_API ResourceLoader();
		GLORY_ENGINE_API virtual ~ResourceLoader();

		GLORY_ENGINE_API void Update();
		GLORY_ENGINE_API void SetResources(Resources* pResources);

	protected:
		virtual void QueueLoad(UUID id) = 0;
		virtual void QueueUnload(UUID id) = 0;

	protected:
		virtual void OnUpdate() = 0;

	protected:
		Resources* m_pResources = nullptr;
	};
}
