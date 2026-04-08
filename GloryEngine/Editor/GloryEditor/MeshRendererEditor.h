#pragma once
#include "EntityComponentEditor.h"
#include "Gizmos.h"

#include <Components.h>

namespace Glory::Editor
{
	class MeshRendererEditor : public EntityComponentEditor<MeshRendererEditor, MeshRenderer>
	{
    public:
        MeshRendererEditor();
        virtual ~MeshRendererEditor();

        virtual void Initialize() override;

    private:
        virtual bool OnGUI() override;
        virtual std::string Name() override;
	};
}