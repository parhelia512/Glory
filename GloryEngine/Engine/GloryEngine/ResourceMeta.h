#pragma once
#include "ResourceLoaderModule.h"
#include "ModelLoaderModule.h"

#include <engine_visibility.h>

#include <string>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Glory
{
	class ResourceMeta
	{
	public:
		GLORY_ENGINE_API ResourceMeta();
		GLORY_ENGINE_API ResourceMeta(const std::string& extension, const std::string& name, UUID uuid, uint32_t hash, size_t serializedVersion = 0);
		GLORY_ENGINE_API virtual ~ResourceMeta();

		GLORY_ENGINE_API void Read(const YAML::Node& node);
		GLORY_ENGINE_API const std::string& Extension() const;
		GLORY_ENGINE_API std::string& Name();
		GLORY_ENGINE_API const std::string& Name() const;
		GLORY_ENGINE_API UUID ID() const;
		GLORY_ENGINE_API UUID& ID();
		GLORY_ENGINE_API uint32_t Hash() const;
		GLORY_ENGINE_API uint32_t& Hash();
		GLORY_ENGINE_API size_t SerializedVersion() const;
		GLORY_ENGINE_API void IncrementSerializedVersion();

	private:
		uint64_t ReadUUID() const;
		std::string ReadName() const;
		uint32_t ReadHash() const;
		size_t ReadSerializedVersion() const;

	private:
		friend class AssetDatabase;
		YAML::Node m_Node;
		std::string m_Extension;
		std::string m_Name;
		UUID m_UUID;
		uint32_t m_TypeHash;
		size_t m_SerializedVersion;
	};
}

namespace std
{
	//template<>
	//struct hash<Glory::ResourceMeta>
	//{
	//	std::size_t operator()(const Glory::ResourceMeta& meta) const
	//	{
	//		std::ifstream inStream(meta.Path());
	//		std::stringstream buffer;
	//		buffer << inStream.rdbuf();
	//		inStream.close();
	//		std::string str = buffer.str();
	//		return std::hash<std::string>()(str);
	//	}
	//};
}

#include <yaml-cpp/yaml.h>
namespace YAML
{
	GLORY_ENGINE_API Emitter& operator<<(Emitter& out, const Glory::ResourceMeta& meta);

	template<>
	struct convert<Glory::ResourceMeta>
	{
		static Node encode(const Glory::ResourceMeta& meta)
		{
			Node node;
			node = YAML::Node(YAML::NodeType::Map);
			node["Extension"] = meta.Extension();
			node["Name"] = meta.Name();
			node["UUID"] = (uint64_t)meta.ID();
			node["Hash"] = (uint32_t)meta.Hash();
			node["SerializedVersion"] = (uint64_t)meta.SerializedVersion();
			return node;
		}

		static bool decode(const Node& node, Glory::ResourceMeta& meta)
		{
			if (!node.IsMap())
				return false;

			meta.Read(node);
			return true;
		}
	};
}
