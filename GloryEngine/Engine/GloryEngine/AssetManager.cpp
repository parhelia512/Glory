#include "AssetManager.h"
#include "AssetDatabase.h"
#include "IEngine.h"
#include "Debug.h"
#include "AssetArchive.h"

#include <BinaryStream.h>
#include <Hash.h>

namespace Glory
{
	AssetManager::AssetManager(IEngine* pEngine) : m_pEngine(pEngine) {}

	AssetManager::~AssetManager()
	{
		m_pEngine = nullptr;
	}
}
