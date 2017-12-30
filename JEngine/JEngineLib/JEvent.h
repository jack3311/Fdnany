#pragma once

#include <functional>
#include <vector>

namespace JEngine
{
	template <typename... Args>
	class JEvent
	{
	protected:
		std::vector<std::function<void(Args...)>> eventListeners;

	public:
		JEvent();
		JEvent(const JEvent &) = delete;
		~JEvent();

		JEvent<Args...> & operator+=(const std::function<void(Args...)> & _func);

		void triggerEvent(Args... _args);
	};

	template <typename... Args>
	class JEventBlockable : protected JEvent<bool &, Args...>
	{
	public:
		JEventBlockable<Args...> & operator+=(const std::function<void(bool &, Args...)> & _func);

		bool triggerEvent(Args... _args);
	};


	template <typename... Args>
	inline JEvent<Args...>::JEvent()
	{
	}

	template <typename... Args>
	inline JEvent<Args...>::~JEvent()
	{
	}

	template <typename... Args>
	inline JEvent<Args...> & JEvent<Args...>::operator+=(const std::function<void(Args...)> & _func)
	{
		eventListeners.push_back(_func);
		return *this;
	}

	template <typename... Args>
	inline void JEvent<Args...>::triggerEvent(Args... _args)
	{
		for (auto & func : eventListeners)
		{
			func(_args...);
		}
	}




	template<typename... Args>
	inline JEventBlockable<Args...> & JEventBlockable<Args...>::operator+=(const std::function<void(bool &, Args...)> & _func)
	{
		eventListeners.push_back(_func);
		return *this;
	}

	template<typename... Args>
	inline bool JEventBlockable<Args...>::triggerEvent(Args... _args)
	{
		bool res = false;

		for (auto itr = eventListeners.begin(); itr != eventListeners.end(); ++itr)
		{
			(*itr)(res, _args...);
			if (res) return true;
		}

		return false;
	}
}