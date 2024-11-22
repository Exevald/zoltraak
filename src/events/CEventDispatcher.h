#pragma once

#include "Event.h"
#include <functional>

class CEventDispatcher
{
public:
	using EventHandler = std::function<void(const SEvent&)>;

	static CEventDispatcher& GetInstance()
	{
		static CEventDispatcher Instance;
		return Instance;
	}

	void Subscribe(EventType type, const EventHandler& handler);

	void Dispatch(const SEvent& event);

	CEventDispatcher(const CEventDispatcher&) = delete;
	CEventDispatcher& operator=(const CEventDispatcher&) = delete;

private:
	CEventDispatcher() = default;
	std::unordered_map<EventType, std::vector<EventHandler>> m_handlers;
};