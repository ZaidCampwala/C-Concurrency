#include <SDL.h>
#include "SceneManager.h"
#include "Timer.h"
#include "Window.h"
#include "Scene2.h"


SceneManager::SceneManager(): // ":" is preinitializing variables before the "Constructor"
	currentScene(nullptr), window(nullptr), timer(nullptr),
	fps(60), isRunning(false), fullScreen(false) {
	Debug::Info("Starting the SceneManager", __FILE__, __LINE__);
}

SceneManager::~SceneManager() {
	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
	
	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (window) {
		delete window;
		window = nullptr;
	}
	Debug::Info("Deleting the SceneManager", __FILE__, __LINE__);
}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	window = new Window();
	if (!window->OnCreate(name_, width_, height_)) {
		Debug::FatalError("Failed to initialize Window object", __FILE__, __LINE__);
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}

	/********************************   Default first scene   ***********************/
	//BuildNewScene(SCENE_NUMBER::SCENE0);
	BuildNewScene(SCENE_NUMBER::SCENE2);
	
	return true;
}

/// This is the whole game
void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		HandleEvents();
		SDL_GL_SwapWindow(window->getWindow());
		SDL_Delay(timer->GetSleepTime(fps));
	}
}

void SceneManager::HandleEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
				isRunning = false;
				return;
				[[fallthrough]]; /// C17 Prevents switch/case fallthrough warnings
			default:
				break;
			}
		}
		if (currentScene == nullptr) {
			Debug::FatalError("Failed to initialize Scene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		currentScene->HandleEvents(sdlEvent);
	}
}

void SceneManager::BuildNewScene(SCENE_NUMBER scene) {
	bool status; 

	if (currentScene != nullptr) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}


	switch (scene) {
	case SCENE_NUMBER::SCENE2:
		currentScene = new Scene2;
		status = currentScene->OnCreate();
		break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		break;
	}
}


