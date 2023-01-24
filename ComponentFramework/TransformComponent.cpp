#include <iostream>
#include "TransformComponent.h"
#include "QMath.h"
#include "MMath.h"
using namespace MATH;

TransformComponent::TransformComponent(Component *parent_) : Component(parent_) {
	position = Vec3(0.0f, 0.0f, 0.0f);
	orientation = Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));
	scale = Vec3(1.0f, 1.0f, 1.0f);
}

TransformComponent::TransformComponent(Component *parent_, Vec3 position_, Quaternion orientation_, Vec3 scale_) :
	Component(parent_), position(position_), orientation(orientation_), scale(scale_) {}

TransformComponent::~TransformComponent() {
	OnDestroy();
}

bool TransformComponent::OnCreate() {
	if (isCreated) return true; // OR return isCreated if already created
	isCreated = true;
	return isCreated;
}

void TransformComponent::OnDestroy() {
	isCreated = false;
}

void TransformComponent::Update(const float deltaTime) {}

void TransformComponent::Render()const {}

Matrix4 TransformComponent::GetTransformMatrix() const {
	// Matrix4 m;
	// m = MMath::translate(this->pos) * MMath::scale(this->scale) * QMath::toMatrix4(this->orientation);
	// return m;

	// return the Matrix on the stack
	return MMath::translate(this->position) *		// return as a Matrix4 instead of "Matrix4 = m"
		   MMath::scale(this->scale) * 
		   QMath::toMatrix4(this->orientation); // "this->" mentioning it's an own member variable
}
