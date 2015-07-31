#include "AddWidget.h"
#include "Gizmos.h"
#include <glm/ext.hpp>

AddWidget::AddWidget()
{

}

AddWidget::~AddWidget()
{

}

void AddWidget::Update(float _dt)
{

}

void AddWidget::Draw()
{

}

void AddWidget::AddWidgetFunc(PxShape* shape, PxRigidActor* actor)
{
	PxGeometryType::Enum type = shape->getGeometryType();
	switch (type)
	{
	case PxGeometryType::eBOX:
		AddBox(shape, actor);
		break;
	case PxGeometryType::ePLANE:
		AddPane(shape, actor);
		break;
	case PxGeometryType::eSPHERE:
		AddSphere(shape, actor);
		break;
	case PxGeometryType::eCAPSULE:
		AddCapsule(shape, actor);
		break;
	}
}

void AddWidget::AddBox(PxShape* pShape, PxRigidActor* actor)
{
	//get the geometry for this PhysX collision volume
	PxBoxGeometry geometry;
	float width = 1, height = 1, length = 1;
	bool status = pShape->getBoxGeometry(geometry);
	if (status)
	{
		width = geometry.halfExtents.x;
		height = geometry.halfExtents.y;
		length = geometry.halfExtents.z;
	}
	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec3 extents = glm::vec3(width, height, length);
	glm::vec4 colour = glm::vec4(1, 0, 0, 1);

	if (actor->getName() != NULL && strcmp(actor->getName(), "Pickup1")) //seriously horrid hack so I can show pickups a different colour
		colour = glm::vec4(0, 1, 0, 1);

	//create our box gizmo
	Gizmos::addAABBFilled(position, extents, colour, &M);
}

void AddWidget::AddSphere(PxShape* pShape, PxRigidActor* actor)
{
	PxSphereGeometry geometry;
	glm::vec3 SpherePosition;
	glm::vec4 SphereColour = glm::vec4(1, 0, 0, 1);
	float SphereRadius;

	bool status = pShape->getSphereGeometry(geometry);
	if (status)
		SphereRadius = geometry.radius;

	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	//get the position out of the transform
	SpherePosition.x = m.getPosition().x;
	SpherePosition.y = m.getPosition().y;
	SpherePosition.z = m.getPosition().z;

	if (actor->getName() != NULL && strcmp(actor->getName(), "Pickup1")) //seriously horrid hack so I can show pickups a different colour
		SphereColour = glm::vec4(0, 1, 0, 1);

	//create our sphere gizmo
	Gizmos::addSphere(SpherePosition, SphereRadius, 10, 10, SphereColour, &M);
}

void AddWidget::AddPane(PxShape* pShape, PxRigidActor* actor)
{
	//get the geometry for this PhysX collision volume
	PxBoxGeometry geometry;
	float width = 1000, height = 0, length = 1000;

	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));

	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec3 extents = glm::vec3(width, height, length);
	glm::vec4 colour = glm::vec4(1, 0.8f, 0.5f, 1);

	//create our box gizmo
	Gizmos::addAABBFilled(position, extents, colour);
}

void AddWidget::AddCapsule(PxShape* pShape, PxRigidActor* actor)
{
	PxCapsuleGeometry capsuleGeometry;

	glm::vec3 cylinderPosition;
	glm::vec4 colour = glm::vec4(1, 1, 0, 1);

	float radius;
	float halfHeight;

	bool status = pShape->getCapsuleGeometry(capsuleGeometry);
	if (status)
	{
		radius = capsuleGeometry.radius; //copy out capsule radius
		halfHeight = capsuleGeometry.halfHeight; //copy out capsule half length
	}

	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	//get the position out of the transform
	cylinderPosition.x = m.getPosition().x;
	cylinderPosition.y = m.getPosition().y;
	cylinderPosition.z = m.getPosition().z;

	if (actor->getName() != NULL && strcmp(actor->getName(), "Pickup1")) //seriously horrid hack so I can show pickups a different colour
		colour = glm::vec4(0, 1, 0, 1);

	glm::vec4 axis(halfHeight, 0, 0, 0); //axis for the capsule
	axis = M * axis; //rotate axis to correct orientation
	//add our 2 end cap spheres...
	Gizmos::addSphere(cylinderPosition + axis.xyz(), radius, 10, 10, colour);
	Gizmos::addSphere(cylinderPosition - axis.xyz(), radius, 10, 10, colour);
	//Fix the gizmo rotation
	glm::mat4 m2 = glm::rotate(M, 11 / 7.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	Gizmos::addCylinderFilled(cylinderPosition, radius, halfHeight, 10, colour, &m2);
}