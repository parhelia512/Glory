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
		virtual void Initialize(size_t componentManagerIndex) = 0;
		virtual uint32_t ComponentHash() const = 0;
		virtual void* Add(EntityID entity) = 0;
		virtual void* Add(EntityID entity, const void* data) = 0;
		virtual void Remove(EntityID entity) = 0;
		virtual void* GetAddress(EntityID entity) = 0;
		virtual const void* GetAddress(EntityID entity) const = 0;
		virtual size_t NumComponents() = 0;
		virtual EntityID EntityAt(size_t index) = 0;
		virtual void Clear() = 0;
		virtual void Sort(const std::vector<std::vector<EntityID>>& entityTrees) = 0;
		virtual bool IsActive(EntityID entity) = 0;
		virtual size_t ActiveSize() const = 0;
		virtual void Activate(EntityID entity) = 0;
		virtual void Deactivate(EntityID entity) = 0;
		virtual void GetReferences(std::vector<UUID>& references) const = 0;
		virtual void Serialize(BinaryStream& stream) const = 0;
		virtual void Deserialize(BinaryStream& stream) = 0;
		virtual bool Compare(const IComponentManager* other) const = 0;
		virtual size_t IndexOf(EntityID entity) const = 0;
		virtual std::type_index ComponentType() const = 0;
		virtual void SetComponentActive(EntityID entity, bool active) = 0;

		/* Global calls */
		virtual void Dirty() = 0;
		virtual void Validate() = 0;
		virtual void Activate() = 0;
		virtual void Deactivate() = 0;
		virtual void EnableDraw() = 0;
		virtual void DisableDraw() = 0;
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
		virtual void CallStart(EntityID entity) = 0;
		virtual void CallStop(EntityID entity) = 0;
		virtual void CallOnDirty(EntityID entity) = 0;
		virtual void CallPreUpdate(EntityID entity, float dt) = 0;
		virtual void CallUpdate(EntityID entity, float dt) = 0;
		virtual void CallPostUpdate(EntityID entity, float dt) = 0;
	};
}
