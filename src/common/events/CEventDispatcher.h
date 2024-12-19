#pragma once

#include "Event.h"
#include <functional>
#include <optional>
#include <unordered_map>

class CEventDispatcher
{
public:
	using EventHandler = std::function<void(const SEvent&)>;

	static CEventDispatcher& GetInstance()
	{
		static CEventDispatcher Instance;
		return Instance;
	}

	void Subscribe(EventType type, const EventHandler& handler)
	{
		m_handlers[type].push_back(handler);
	}

	void Dispatch(const SEvent& event)
	{
		m_lastEvents[event.type] = event;

		if (m_handlers.find(event.type) != m_handlers.end())
		{
			for (const auto& handler : m_handlers[event.type])
			{
				handler(event);
			}
		}
	}

	std::optional<SEvent> GetLastEvent(const EventType& type) const
	{
		auto it = m_lastEvents.find(type);
		if (it != m_lastEvents.end())
		{
			return it->second;
		}
		return std::nullopt;
	}

	CEventDispatcher(const CEventDispatcher&) = delete;
	CEventDispatcher& operator=(const CEventDispatcher&) = delete;

private:
	CEventDispatcher() = default;
	std::unordered_map<EventType, std::vector<EventHandler>> m_handlers;
	std::unordered_map<EventType, SEvent> m_lastEvents;
};
