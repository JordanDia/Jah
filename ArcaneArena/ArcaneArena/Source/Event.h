#pragma once

enum class EventType
{
	Action,
	Damage,
	Death,
};

class Entity;

class Event
{
public:
	virtual ~Event() = default;

	virtual EventType GetType() const = 0;
};

class ActionEvent : public Event
{
public:
	EventType GetType() const override { return EventType::Action; }
};

class DamageEvent : public Event
{
public:
	DamageEvent(Entity* target, int damage)
		: m_Target(target), m_Damage(damage)
	{
	}

	Entity* GetTarget() const { return m_Target; }
	int GetDamage() const { return m_Damage; }

	EventType GetType() const override { return EventType::Damage; }

private:
	Entity* m_Target = nullptr;
	int m_Damage = 0;
};

class DeathEvent : public Event
{
public:
	EventType GetType() const override { return EventType::Death; }
};