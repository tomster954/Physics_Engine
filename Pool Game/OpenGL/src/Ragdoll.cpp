#include "Ragdoll.h"
#include "glm/glm.hpp"
#include "Gizmos.h"
Ragdoll::Ragdoll(PxPhysics* _physics, PxMaterial* _material, PxScene* _scene)
{
	//complex humanoid ragdoll example
	RagdollNode* ragdollData[] =
	{
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NO_PARENT, 1, 3, 1, 1, "lower spine"),
		new RagdollNode(PxQuat(PxPi, Z_AXIS), LOWER_SPINE, 1, 1, -1, 1, "left pelvis"),
		new RagdollNode(PxQuat(0, Z_AXIS), LOWER_SPINE, 1, 1, -1, 1, "right pelvis"),
		new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_PELVIS, 5, 2, -1, 1, "L upper leg"),
		new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_PELVIS, 5, 2, -1, 1, "R upper leg"),
		new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_UPPER_LEG, 5, 1.75, -1, 1, "L lower leg"),
		new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_UPPER_LEG, 5, 1.75, -1, 1, "R lowerleg"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), LOWER_SPINE, 1, 3, 1, -1, "upper spine"),
		new RagdollNode(PxQuat(PxPi, Z_AXIS), UPPER_SPINE, 1, 1.5, 1, 1, "left clavicle"),
		new RagdollNode(PxQuat(0, Z_AXIS), UPPER_SPINE, 1, 1.5, 1, 1, "right clavicle"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), UPPER_SPINE, 1, 1, 1, -1, "neck"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NECK, 1, 3, 1, -1, "HEAD"),
		new RagdollNode(PxQuat(PxPi - .3, Z_AXIS), LEFT_CLAVICLE, 3, 1.5, -1, 1, "left upper arm"),
		new RagdollNode(PxQuat(0.3, Z_AXIS), RIGHT_CLAVICLE, 3, 1.5, -1, 1, "right upper arm"),
		new RagdollNode(PxQuat(PxPi - .3, Z_AXIS), LEFT_UPPER_ARM, 3, 1, -1, 1, "left lower arm"),
		new RagdollNode(PxQuat(0.3, Z_AXIS), RIGHT_UPPER_ARM, 3, 1, -1, 1, "right lower arm"),
		NULL
	};

	m_body = Ragdoll::MakeRagdoll(_physics, ragdollData, PxTransform(PxVec3(0, 20, -50)), .1f, _material);
	_scene->addArticulation(*m_body);
}

Ragdoll::~Ragdoll()
{

}

void Ragdoll::Update(float _dt)
{
	
}

void Ragdoll::Draw()
{
	PxU32 nLinks = m_body->getNbLinks();
	PxArticulationLink** links = new PxArticulationLink*[nLinks];
	m_body->getLinks(links, nLinks);
	while (nLinks--)
	{
		PxArticulationLink* link = links[nLinks];
		PxU32 nShapes = link->getNbShapes();
		PxShape** shapes = new PxShape*[nShapes];
		link->getShapes(shapes, nShapes);
		while (nShapes--)
		{
			PxSphereGeometry geometry;
			shapes[nShapes]->getSphereGeometry(geometry);
			float radius = geometry.radius + 1;
			glm::vec3 center;
			center.x = link->getWorldBounds().getCenter().x;
			center.y = link->getWorldBounds().getCenter().y;
			center.z = link->getWorldBounds().getCenter().z;
			//glm::mat4 newRot = Utils::TransformToMat4(_actor->getGlobalPose());
			Gizmos::addSphere(center, radius, 5, 5, glm::vec4(1, 0, 0, 1));
		}
	}
	delete[] links;
}

PxArticulation* Ragdoll::MakeRagdoll(PxPhysics* g_Physics, RagdollNode** nodeArray,	PxTransform worldPos, float scaleFactor, PxMaterial* ragdollMaterial)
{
	//create the articulation for our ragdoll
	PxArticulation *articulation = g_Physics->createArticulation();
	RagdollNode** currentNode = nodeArray;

	//while there are more nodes to process...
	while (*currentNode != NULL)
	{
		//get a pointer to the current node
		RagdollNode* currentNodePtr = *currentNode;
		currentNode++;
		//get a pointer to the current node
		
		//create a pointer ready to hold the parent node pointer if there is one
		RagdollNode* parentNode = nullptr;
		//get scaled values for capsule
		float radius = currentNodePtr->radius*scaleFactor;
		float halfLength = currentNodePtr->halfLength*scaleFactor;
		float childHalfLength = radius + halfLength;
		float parentHalfLength = 0; //will be set later if there is a parent
		//get a pointer to the parent
		PxArticulationLink* parentLinkPtr = NULL;
		currentNodePtr->scaledGobalPos = worldPos.p;

		if (currentNodePtr->parentNodeIdx != NO_PARENT)
		{
			//if there is a parent then we need to work out our local position for the link
			//get a pointer to the parent node
			parentNode = *(nodeArray + currentNodePtr->parentNodeIdx);
			//get a pointer to the link for the parent
			parentLinkPtr = parentNode->linkPtr;
			parentHalfLength = (parentNode->radius + parentNode->halfLength) *scaleFactor;
			//work out the local position of the node
			PxVec3 currentRelative = currentNodePtr->childLinkPos * currentNodePtr ->globalRotation.rotate(PxVec3(childHalfLength, 0, 0));
			PxVec3 parentRelative = -currentNodePtr->parentLinkPos * parentNode ->globalRotation.rotate(PxVec3(parentHalfLength, 0, 0));
			currentNodePtr->scaledGobalPos = parentNode->scaledGobalPos - (parentRelative +	currentRelative);
		}

		//build the transform for the link
		PxTransform linkTransform = PxTransform(currentNodePtr->scaledGobalPos, currentNodePtr ->globalRotation);
		//create the link in the articulation
		PxArticulationLink* link = articulation->createLink(parentLinkPtr, linkTransform);
		//add the pointer to this link into the ragdoll data so we have it for later when we want to link to it
		currentNodePtr->linkPtr = link;
		
		float jointSpace = .01f; //gap between joints
		float capsuleHalfLength = (halfLength > jointSpace ? halfLength - jointSpace : 0) + .01f;
		PxCapsuleGeometry capsule(radius, capsuleHalfLength);
		link->createShape(capsule, *ragdollMaterial); //adds a capsule collider to the link
		PxRigidBodyExt::updateMassAndInertia(*link, 50.0f); //adds some mass, mass should really be part of the data!

		if (currentNodePtr->parentNodeIdx != -1)
		{
			//get the pointer to the joint from the link
			PxArticulationJoint *joint = link->getInboundJoint();
			//get the relative rotation of this link
			PxQuat frameRotation = parentNode->globalRotation.getConjugate() * currentNodePtr->globalRotation;
			//set the parent contraint frame
			PxTransform parentConstraintFrame = PxTransform(PxVec3(currentNodePtr->parentLinkPos * parentHalfLength, 0, 0), frameRotation);
			//set the child constraint frame (this the constraint frame of the newly added link)
			PxTransform thisConstraintFrame = PxTransform(PxVec3(currentNodePtr->childLinkPos * childHalfLength, 0, 0));
			
			//set up the poses for the joint so it is in the correct place
			joint->setParentPose(parentConstraintFrame);
			joint->setChildPose(thisConstraintFrame);
			
			//set up some constraints to stop it flopping around
			joint->setStiffness(20);
			joint->setDamping(20);
			joint->setSwingLimit(0.4f, 0.4f);
			joint->setSwingLimitEnabled(true);
			joint->setTwistLimit(-0.1f, 0.1f);
			joint->setTwistLimitEnabled(true);
		}
	}
	return articulation;
}

void Ragdoll::AddWidget(PxShape* shape, PxRigidActor* actor)
{
	PxGeometryType::Enum type = shape->getGeometryType();
	switch (type)
	{
	case PxGeometryType::eSPHERE:
		AddSphere(shape, actor);
		break;
	case PxGeometryType::eCAPSULE:
		AddCapsule(shape, actor);
		break;
	}
}

void Ragdoll::AddSphere(PxShape* pShape, PxRigidActor* actor)
{
	//bool status = pShape->getSphereGeometry(geometry);
	//if (status)
	//{
	//	radius = geometry.radius;
	//}
}

void Ragdoll::AddCapsule(PxShape* pShape, PxRigidActor* actor)
{
	//bool status = pShape->getCapsuleGeometry(capsuleGeometry);
	//if (status)
	//{
	//	radius = capsuleGeometry.radius; //copy out capsule radius
	//	halfHeight = capsuleGeometry.halfHeight; //copy out capsule half length
	//}
	//PxTransform transform = PxShapeExt::getGlobalPose(*pShape, *actor);
}