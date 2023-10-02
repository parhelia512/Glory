#include "Importer.h"

#include <Resource.h>

namespace Glory::Editor
{
	std::vector<Importer*> Importer::m_pImporters;

	Resource* Importer::Import(const std::filesystem::path& path, void* pImportSettings)
	{
		path.extension();
		auto itor = std::find_if(m_pImporters.begin(), m_pImporters.end(), [&](const Importer* pImporter) {
			return pImporter->SupportsExtension(path.extension());
		});

		return (*itor)->Load(path);
	}

	void Importer::Register(Importer* pImporter)
	{
		m_pImporters.push_back(pImporter);
		pImporter->Initialize();
	}
}
