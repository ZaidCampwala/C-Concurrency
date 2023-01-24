#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "Component.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Euler.h"

using namespace MATH;


class TransformComponent : public Component {
private:
	Vec3 position;
	Vec3 scale;
	Quaternion orientation;
	std::shared_ptr<Ref<TransformComponent> > transform;

public:
	TransformComponent(Component *parent_);
	TransformComponent(Component *parent_, Vec3 position_, Quaternion orientation_, Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f));
	~TransformComponent();

	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;

	Vec3 GetPosition() { return position; }
	Vec3 GetScale() { return scale; }
	Quaternion GetQuaternion() { return orientation; }
	Matrix4 GetTransformMatrix() const;
	
	void SetTransform(Vec3 position_, Quaternion orientation_, Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f)) {
		position = position_;
		orientation = orientation_;
		scale = scale_;
	}
};

#endif // TRANSFORMCOMPONENT_H