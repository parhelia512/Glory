#pragma once
#include "GloryEditor.h"

#include <filesystem>
#include <vector>

namespace Glory
{
	class Resource;
}

namespace Glory::Editor
{
	class Importer
	{
	public:
		GLORY_EDITOR_API static Resource* Import(const std::filesystem::path& path, void* pImportSettings);
		GLORY_EDITOR_API static void Register(Importer* pImporter);

		template<typename T>
		static void Register()
		{
			Register(new T());
		}

	protected:
		Importer(const std::string_view extensions) {};
		virtual ~Importer() {};

		virtual Resource* Load(const std::filesystem::path& path) const = 0;
		virtual void Save(const std::filesystem::path& path, Resource* pResource) const = 0;
		virtual uint32_t ResourceTypeHash() const = 0;
		virtual bool SupportsExtension(const std::filesystem::path& extension) const = 0;

	protected:
		virtual void Initialize() = 0;
		virtual void Cleanup() = 0;

	private:
		static std::vector<Importer*> m_pImporters;
	};
}
