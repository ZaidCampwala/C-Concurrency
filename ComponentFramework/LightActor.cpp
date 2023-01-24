#include "LightActor.h"
#include "UBO_Padding.h"


LightActor::LightActor(Component *parent_, LightStyle lightStyle_, Vec3 location_, Vec4 colour_,
	float intensity, Vec3 falloff): Actor(parent_) {
	lightStyle = lightStyle_;
	position = location_;
	colour = colour_;
}

LightActor::~LightActor() {
	OnDestroy();
}

bool LightActor::OnCreate() {
	if (isCreated) return true; // OR return isCreated if already created
	size_t buffer_size = UBO_PADDING::VEC3 + UBO_PADDING::VEC4;

	glGenBuffers(1, &uboLightDataID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightDataID);
	glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

	size_t offset = 0;

	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec3), position);
	offset = UBO_PADDING::VEC3;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4), colour);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboLightDataID);

	isCreated = true;
	return isCreated;
}

void LightActor::OnDestroy() {
	glDeleteBuffers(1, &uboLightDataID);
	isCreated = false;
}