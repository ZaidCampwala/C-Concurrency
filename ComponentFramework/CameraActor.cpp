#include <glew.h>
#include "CameraActor.h"
#include "Debug.h"
#include "MMath.h"
#include "QMath.h"
#include "TransformComponent.h"
#include "UBO_Padding.h"
using namespace MATH;


CameraActor::CameraActor(Component* parent_):Actor(parent_) {}

CameraActor::~CameraActor() {
    OnDestroy();
}

bool CameraActor::OnCreate() {
    if (isCreated) return true; // OR return isCreated if already created
    Debug::Info("Creating values for CameraActor: ", __FILE__, __LINE__);
    size_t buffer_size = 2 * UBO_PADDING::MAT4;
    glGenBuffers(1, &uboMatriciesID);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatriciesID);
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboMatriciesID);

    UpdateProjectionMatrix(45.0f, 16.0f / 9.0f, 0.5f, 100.0f); // default projection
    UpdateViewMatrix();
    isCreated = true;
    return isCreated;
}

void CameraActor::OnDestroy() {
    glDeleteBuffers(1, &uboMatriciesID);
    isCreated = false;
}

void CameraActor::UpdateViewMatrix() {
    Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>();
    if (transformComponent == nullptr) { // no such component, use default view
        viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    }
    else {
        Quaternion orientation = transformComponent->GetQuaternion();
        Vec3 position = transformComponent->GetPosition();
        viewMatrix = MMath::translate(position) * QMath::toMatrix4(orientation);
    }
    size_t offset = sizeof(Matrix4);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatriciesID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Matrix4), viewMatrix);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CameraActor::UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far) {
    size_t offset = 0;
    projectionMatrix = MMath::perspective(fovy, aspectRatio, near, far);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatriciesID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Matrix4), projectionMatrix);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
