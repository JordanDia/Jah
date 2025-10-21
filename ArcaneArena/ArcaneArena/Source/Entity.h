#pragma once

#include <string>
#include <vector>
#include "Logger.h"

#include "Observer.h"

class Entity
{
public:
	Entity() = default;

	void TakeDamage(int damage)
	{
		m_Health -= damage;
		DamageEvent event(this, damage);
		Notify(event);
	}

	bool IsDead() const { return m_Health <= 0; }
	void AddObserver(std::shared_ptr<Observer> observer) { m_Observers.push_back(observer); }

	const std::string& GetName() const { return m_Name; }

protected:
	void Notify(Event& event)
	{
		for (std::shared_ptr<Observer>& observer : m_Observers)
		{
			observer->OnEvent(event);
		}
	}

protected:
	int m_Health = 100;
	std::string m_Name;

private:
	std::vector<std::shared_ptr<Observer>> m_Observers{};
};

class Enemy : public Entity
{
public:
	Enemy() = default;

private:
	int m_AttackPower = 10;
};

class Player : public Entity
{
public:
	Player(const std::string& name, int health)
		: m_Name(name)
	{
		Logger::Get().Info("Player '" + m_Name + "' created (" + std::to_string(m_Health) + "HP)");
	}

	void TakeAction(std::vector<std::shared_ptr<Enemy>>& enemies)
	{
		for (std::shared_ptr<Enemy>& enemy : enemies)
			enemy->TakeDamage(10);
	}

private:
	std::string m_Name;
	int m_AttackPower = 10;
};

class Goblin : public Enemy
{
public:
	Goblin()
	{
		m_Name = "Goblin" + std::to_string(++m_ID);
		Logger::Get().Info("Goblin created!");
	}

private:
	inline static int m_ID = 0;
};

class Orc : public Enemy
{
public:
	Orc()
	{
		m_Name = "Orc" + std::to_string(++m_ID);
		Logger::Get().Info("Orc created!");
	}

private:
	inline static int m_ID = 0;
};

class Factory
{
public:
	static std::shared_ptr<Enemy> Create(const std::string& type)
	{
		if (type == "Goblin")
			return std::make_shared<Goblin>();
		if (type == "Orc")
			return std::make_shared<Orc>();
	}

private:
	
};
