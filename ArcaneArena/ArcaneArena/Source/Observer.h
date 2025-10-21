#pragma once

#include "Event.h"

class Observer
{
public:
	Observer() = default;

	virtual void OnEvent(Event& event) = 0;
};


class HealthBar : public Observer
{
public:
	void OnEvent(Event& event) override;
};