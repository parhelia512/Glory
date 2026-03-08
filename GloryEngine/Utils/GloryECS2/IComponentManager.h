#pragma once
#include "EntityID.h"

#include <vector>

namespace Glory::Utils::ECS
{
	class IComponentManager
	{
	public:
		virtual ~IComponentManager() = default;

	public:
		virtual void Initialize() = 0;
		virtual uint32_t ComponentHash() const = 0;
		virtual void* Add(EntityID entity) = 0;
		virtual void Remove(EntityID entity) = 0;
		virtual void Sort(const std::vector<std::vector<EntityID>>& entityTrees) = 0;
		virtual bool IsActive(EntityID entity) = 0;
		virtual void Activate(EntityID entity) = 0;
		virtual void Deactivate(EntityID entity) = 0;

		/* Global calls */
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void PreUpdate(float dt) = 0;
		virtual void Update(float dt) = 0;
		virtual void PostUpdate(float dt) = 0;
		virtual void PreDraw() = 0;
		virtual void Draw() = 0;
		virtual void PostDraw() = 0;

		/* Manual calls */
		virtual void CallValidate(EntityID entity) = 0;
		virtual void CallOnAdd(EntityID entity) = 0;
		virtual void CallOnRemove(EntityID entity) = 0;
		virtual void CallOnActivate(EntityID entity) = 0;
		virtual void CallOnDeactivate(EntityID entity) = 0;
		virtual void CallOnEnableDraw(EntityID entity) = 0;
		virtual void CallOnDisableDraw(EntityID entity) = 0;
	};
}
