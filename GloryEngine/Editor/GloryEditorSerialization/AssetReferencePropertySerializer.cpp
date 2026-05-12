#include "AssetReferencePropertySerializer.h"

#include <AssetReference.h>

namespace Glory
{
	AssetReferencePropertySerializer::AssetReferencePropertySerializer(Serializers* pSerializers):
		PropertySerializer(pSerializers, SerializedType::ST_Asset)
	{
	}

	AssetReferencePropertySerializer::~AssetReferencePropertySerializer()
	{
	}

	void AssetReferencePropertySerializer::Serialize(void* data, uint32_t typeHash, Utils::NodeValueRef node)
	{
		const ResourceReferenceBase* pReferenceMember = (ResourceReferenceBase*)data;
		const UUID uuid = pReferenceMember->GetUUID();
		node.Set((uint64_t)uuid);
	}

	void AssetReferencePropertySerializer::Deserialize(void* data, uint32_t typeHash, Utils::NodeValueRef node)
	{
		if (!node.Exists() || !node.IsScalar()) return;
		ResourceReferenceBase* pReferenceMember = (ResourceReferenceBase*)data;
		const UUID uuid = node.As<uint64_t>();
		pReferenceMember->SetUUID(uuid);
	}
}
