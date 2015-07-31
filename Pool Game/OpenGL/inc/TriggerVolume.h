//-------------------------------------
//Author: Tommas Solarino
//Description: This is the trigger volume
//-------------------------------------


#ifndef _TRIGGER_VOLUME_H_
#define	_TRIGGER_VOLUME_H_
#include "PxPhysicsAPI.h"
using namespace physx;

class TriggerVolume : public physx::PxSimulationEventCallback {
public:
	TriggerVolume(physx::PxScene* _scene, physx::PxRigidStatic* _actor, physx::PxShape* _shape);
	~TriggerVolume();

	void Update(float _dt);
	void Draw();

	bool m_triggered;

private:
	physx::PxShape* m_shape;
	physx::PxRigidStatic* m_actor;

	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
	virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) {}
	virtual void onWake(physx::PxActor**, physx::PxU32) {}
	virtual void onSleep(physx::PxActor**, physx::PxU32){}
};

#endif