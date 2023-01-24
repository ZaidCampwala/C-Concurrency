#ifndef SCENE2_H
#define SCENE2_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Component.h"
#include "AssetManager.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Actor;
class CameraActor;
class LightActor;


class Scene2 : public Scene {
private:
	AssetManager* assetManager;
	Ref<Actor> checker_Red[16];
	Ref<Actor> checkeredBoard;
	Ref<Actor> checker_black[16];

	//Actor* checkerPiece;
	Ref<CameraActor> camera;
	Ref<LightActor> light;
public:
	explicit Scene2();
	virtual ~Scene2();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);





};

#endif // SCENE2_H