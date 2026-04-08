#pragma once
#include "sdl_input_visibility.h"

#include <Version.h>
#include <InputModule.h>

namespace Glory
{
	class SDLInputModule : public InputModule
	{
	public:
		GLORY_SDL_INPUT_API SDLInputModule();
		GLORY_SDL_INPUT_API virtual ~SDLInputModule();

		GLORY_MODULE_VERSION_H(0, 2, 0);

	private:
		virtual void OnInitialize() override;
		virtual void OnCleanup() override;
		virtual void OnUpdate() override;

	private:
		std::hash<std::string> m_Hasher;
	};
}
