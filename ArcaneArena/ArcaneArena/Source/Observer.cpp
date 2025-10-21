#include "Observer.h"
#include "Entity.h"
#include "Logger.h"


void HealthBar::OnEvent(Event& event)
{

	if (event.GetType() == EventType::Damage)
	{
		DamageEvent damageEvent = dynamic_cast<DamageEvent&>(event);
		Entity* target = damageEvent.GetTarget();

		int damage = damageEvent.GetDamage();
		Logger::Get().Info(target->GetName() + " took " + std::to_string(damage) + " damage!");
	}
}