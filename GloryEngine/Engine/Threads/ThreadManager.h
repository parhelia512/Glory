#pragma once
#include "Thread.h"

#include <vector>
#include <functional>
#include <queue>
#include <mutex>

namespace Glory
{
	/** @brief Thread manager */
	class ThreadManager
	{
	public:
		/** @brief Constructor */
		ThreadManager();
		/** @brief Destructor */
		virtual ~ThreadManager();

		/** @brief Create a new thread and run a method on it
		 * @param func The method to run
		 */
		Thread* Run(std::function<void()> func);
		/** @brief Get the number of hardware threads available */
		static size_t NumHardwareThread();

		/** @brief Kill all running threads and wait for exit */
		void Kill();

		inline const std::thread::id& DefaultThreadID() const { return m_DefaultThreadID; }

	private:
		Thread* CreateThread(std::function<void()> func);

	private:
		bool NewThread();
		Thread* PopIdleThread();

		void OnThreadIdle(Thread* pThread);

	private:
		friend class IEngine;
		static size_t m_HardwareThreads;
		std::mutex m_IdleQueueMutex;
		std::queue<size_t> m_IdleThreads;
		std::vector<Thread*> m_pThreads;
		std::thread::id m_DefaultThreadID;
	};
}
