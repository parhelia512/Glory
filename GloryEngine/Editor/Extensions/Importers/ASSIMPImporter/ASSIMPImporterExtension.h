#pragma once
#include <BaseEditorExtension.h>

#ifdef GLORY_ASSIMP_EXTENSION_EXPORTS
// BUILD LIB
#define GLORY_ASSIMP_EXTENSION_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_ASSIMP_EXTENSION_API __declspec(dllimport)
#endif

EXTENSION_H(GLORY_ASSIMP_EXTENSION_API)

namespace Glory::Editor
{
	class ASSIMPImporterExtension : public BaseEditorExtension
	{
	public:
		ASSIMPImporterExtension();
		virtual ~ASSIMPImporterExtension();

	private:
		void Initialize() override;
	};
}
