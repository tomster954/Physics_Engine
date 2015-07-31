//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: This is a class the will use gizmos to draw actors and stuff :P
//-----------------------------------------------------------------------------

#ifndef ADD_WIDGET
#define ADD_WIDGET

#include <PxPhysicsAPI.h>
using namespace physx;

class AddWidget
{
public:
	AddWidget();
	~AddWidget();

	void Update(float _dt);
	void Draw();

	void AddWidgetFunc(PxShape* shape, PxRigidActor* actor);

	void AddBox(PxShape* pShape, PxRigidActor* actor);
	void AddSphere(PxShape* pShape, PxRigidActor* actor);
	void AddPane(PxShape* pShape, PxRigidActor* actor);
	void AddCapsule(PxShape* pShape, PxRigidActor* actor);

private:

};
#endif