#pragma once

#include <functional>
#include <vector>

template <typename... Args>
class JEventParameterised
{
private:
	std::vector<const std::function<void(Args...)>> eventListeners;

public:
	JEventParameterised();
	JEventParameterised(const JEventParameterised &) = delete;
	~JEventParameterised();

	JEventParameterised & operator+=(const std::function<void(Args...)> _func);

	void triggerEvent(Args... _args);
};

template<typename ...Args>
inline JEventParameterised<Args...>::JEventParameterised()
{
}

template<typename ...Args>
inline JEventParameterised<Args...>::~JEventParameterised()
{
}

template<typename ...Args>
inline JEventParameterised<Args...> & JEventParameterised<Args...>::operator+=(const std::function<void(Args...)> _func)
{
	eventListeners.push_back(_func);
}

template<typename ...Args>
inline void JEventParameterised<Args...>::triggerEvent(Args ..._args)
{
	for (auto & func : eventListeners)
	{
		func(_args...);
	}
}
