//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: This is a ragdoll class
//-----------------------------------------------------------------------------

#ifndef RAGDOLL
#define RAGDOLL

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <vector>
using namespace physx;

//Parts which make up our ragdoll
enum RagDollParts
{
	NO_PARENT = -1,
	LOWER_SPINE,
	LEFT_PELVIS,
	RIGHT_PELVIS,
	LEFT_UPPER_LEG,
	RIGHT_UPPER_LEG,
	LEFT_LOWER_LEG,
	RIGHT_LOWER_LEG,
	UPPER_SPINE,
	LEFT_CLAVICLE,
	RIGHT_CLAVICLE,
	NECK,
	HEAD,
	LEFT_UPPER_ARM,
	RIGHT_UPPER_ARM,
	LEFT_LOWER_ARM,
	RIGHT_LOWER_ARM,
};

struct RagdollNode
{
	PxQuat globalRotation; //rotation of this link in model space - we could have done this relative to the parent node but it's harder to visualize when setting up the data by hand
	PxVec3 scaledGobalPos; //Position of the link centre in world space which is calculated when we process the node.It's easiest if we store it here so we have it when we transform the child
	
	int parentNodeIdx; //Index of the parent node
	float halfLength; //half length of the capsule for this node
	float radius; //radius of capsule for thisndoe
	float parentLinkPos; //relative position of link centre in parent to this node. 0 is the centre of hte node, -1 is left end of capsule and 1 is right end of capsule relative to x
	float childLinkPos; //relative position of link centre in child
	char* name; //name of link
	
	PxArticulationLink* linkPtr;
	
	//constructor
	RagdollNode(PxQuat _globalRotation, int _parentNodeIdx, float _halfLength, float _radius, float _parentLinkPos, float _childLinkPos, char* _name)
	{
		globalRotation = _globalRotation, parentNodeIdx = _parentNodeIdx; halfLength = _halfLength; radius = _radius;
		parentLinkPos = _parentLinkPos; childLinkPos = _childLinkPos; name = _name;
		linkPtr = nullptr;
	};
};

class Ragdoll
{
public:
	Ragdoll(PxPhysics* _physics, PxMaterial* _material, PxScene* _scene);
	~Ragdoll();

	void Update(float _dt);
	void Draw();

	PxArticulation* MakeRagdoll(PxPhysics* g_Physics, RagdollNode** nodeArray, PxTransform worldPos, float scaleFactor, PxMaterial* ragdollMaterial);

private:
	const PxVec3 X_AXIS = PxVec3(1, 0, 0);
	const PxVec3 Y_AXIS = PxVec3(0, 1, 0);
	const PxVec3 Z_AXIS = PxVec3(0, 0, 1);
};
#endif