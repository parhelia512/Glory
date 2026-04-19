#pragma once
#include <ResourceLoader.h>

namespace Glory
{
	class RuntimeResourceLoader : public ResourceLoader
	{
    public:
        RuntimeResourceLoader();
        virtual ~RuntimeResourceLoader();

    private:
        /** @brief Loading implementation */
        virtual void QueueLoad(UUID id) override;
        /** @brief Unloading implementation */
        virtual void QueueUnload(UUID id) override;

        /** @brief Custom processing update implementation */
        virtual void OnUpdate() override;
	};
}
