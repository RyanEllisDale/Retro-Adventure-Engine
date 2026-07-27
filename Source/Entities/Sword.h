// small pickup class that specifically wants to be picked up by the player,
// to give them a sword so they can start shooting projectiles,
// destroys itself upon pickup

#pragma once

// Include:
#include "Base Entities/PhysicsEntity.h" 

class Sword : public PhysicsEntity
{
private:
	Animation m_idle; // sword is a static image
	virtual bool collisionScript(void); // custom script to give player sword

public:
	Sword();
	void update(const double& a_deltaTime, const float& a_simulationTimeScalar) override;
};

