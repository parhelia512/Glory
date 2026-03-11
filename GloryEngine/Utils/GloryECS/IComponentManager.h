#pragma once
#include "EntityID.h"

#include <UUID.h>

#include <vector>
#include <typeindex>

namespace Glory::Utils
{
	class BinaryStream;
}

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
		virtual void* Add(EntityID entity, const void* data) = 0;
		virtual void Remove(EntityID entity) = 0;
		virtual void* GetAddress(EntityID entity) = 0;
		virtual const void* GetAddress(EntityID entity) const = 0;
		virtual void Clear() = 0;
		virtual void Sort(const std::vector<std::vector<EntityID>>& entityTrees) = 0;
		virtual bool IsActive(EntityID entity) = 0;
		virtual void Activate(EntityID entity) = 0;
		virtual void Deactivate(EntityID entity) = 0;
		virtual void GetReferences(std::vector<UUID>& references) const = 0;
		virtual void Serialize(BinaryStream& stream) const = 0;
		virtual void Deserialize(BinaryStream& stream) = 0;
		virtual bool Compare(const IComponentManager* other) const = 0;
		virtual size_t IndexOf(EntityID entity) const = 0;
		virtual std::type_index ComponentType() const = 0;
		virtual void ActivateComponent(EntityID entity) = 0;
		virtual void DeactivateComponent(EntityID entity) = 0;

		/* Global calls */
		virtual void Validate() {};
		virtual void Activate() {};
		virtual void Deactivate() {};
		virtual void EnableDraw() {};
		virtual void DisableDraw() {};
		virtual void Start() {};
		virtual void Stop() {};
		virtual void PreUpdate(float dt) {};
		virtual void Update(float dt) {};
		virtual void PostUpdate(float dt) {};
		virtual void PreDraw() {};
		virtual void Draw() {};
		virtual void PostDraw() {};

		/* Manual calls */
		virtual void CallValidate(EntityID entity) {};
		virtual void CallOnAdd(EntityID entity) {};
		virtual void CallOnRemove(EntityID entity) {};
		virtual void CallOnActivate(EntityID entity) {};
		virtual void CallOnDeactivate(EntityID entity) {};
		virtual void CallOnEnableDraw(EntityID entity) {};
		virtual void CallOnDisableDraw(EntityID entity) {};
		virtual void CallStart(EntityID entity) {};
		virtual void CallStop(EntityID entity) {};
		virtual void CallOnDirty(EntityID entity) {};
	};
}
