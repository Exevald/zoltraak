#include "CEventDispatcher.h"

void CEventDispatcher::Subscribe(EventType type, const CEventDispatcher::EventHandler& handler)
{
	m_handlers[type].push_back(handler);
}

void CEventDispatcher::Dispatch(const SEvent& event)
{
	for (auto& handler : m_handlers[event.type])
	{
		handler(event);
	}
}
