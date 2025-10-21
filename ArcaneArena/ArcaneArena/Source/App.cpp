#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "Entity.h"
#include "Event.h"
#include "Logger.h"
#include "Observer.h"

enum Difficulty
{
	Easy = 1, Medium, Hard
};


bool AllEnemiesDead(const std::vector<std::shared_ptr<Enemy>>& enemies)
{
	for (const std::shared_ptr<Enemy>& enemy : enemies)
	{
		if (!enemy->IsDead())
			return false;
	}

	return true;
}


int main()
{
	std::string name;
	int difficulty = 0;
	
	std::cout << "Welcome, Champion! Prepare for battle." << std::endl;

	std::cout << "Enter your player name: ";
	std::cin >> name;

	std::cout << "Choose your difficulty (1 - Easy, 2 - Medium, 3 - Hard): ";
	std::cin >> difficulty;

	Player player(name, 100);
	std::shared_ptr<HealthBar> playerHealthBar = std::make_shared<HealthBar>();
	player.AddObserver(playerHealthBar);

	std::vector<std::shared_ptr<Enemy>> enemies;
	auto goblin = Factory::Create("Goblin");
	auto orc = Factory::Create("Orc");

	goblin->AddObserver(std::make_shared<HealthBar>());
	orc->AddObserver(std::make_shared<HealthBar>());

	enemies.push_back(goblin);
	enemies.push_back(orc);

	Logger::Get().Warn("Enemies were spawned for this battle!");

	while (!player.IsDead() && !AllEnemiesDead(enemies))
	{
		std::cout << "In loop!" << std::endl;
		player.TakeAction(enemies);
	}

}