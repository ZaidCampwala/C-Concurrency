#include <glew.h>
#include <iostream>
#include <memory>
#include <vector>
#include <SDL.h>
#include "Scene2.h"
#include "CameraActor.h"
#include "Debug.h"
#include "LightActor.h"
#include "MaterialComponent.h"
#include "Matrix.h"
#include "MeshComponent.h"
#include "MMath.h"
#include "QMath.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include <thread>

int totalBytes = 0;
void LoadRedChecker(Ref<Actor> checkeredBoard, Ref<Actor> checker_Red[],
								Ref<MeshComponent> m, Ref<ShaderComponent> shader, Ref<MaterialComponent> rText);

void LoadBlackChecker(Ref<Actor> checkeredBoard, Ref<Actor> checker_black[],
								Ref<MeshComponent> m, Ref<ShaderComponent> shader, Ref<MaterialComponent> bText);


Scene2::Scene2() : camera(nullptr), checkeredBoard{ nullptr }, checker_Red{ nullptr }, checker_black{ nullptr }{
	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2::~Scene2() {
	Debug::Info("Deleted Scene2: ", __FILE__, __LINE__);
}

bool Scene2::OnCreate() {
	Debug::Info("Loading assets Scene2: ", __FILE__, __LINE__);	
	assetManager = new AssetManager();
	std::cout <<"this is totalbytes " <<totalBytes << std::endl;
	 
	Ref<MeshComponent> m = assetManager->GetComponent<MeshComponent>("CheckerPiece");
	Ref<MaterialComponent> rText = assetManager->GetComponent<MaterialComponent>("checker_Red");
	Ref<MaterialComponent> bText = assetManager->GetComponent<MaterialComponent>("checker_Black");
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("TextureShader");

	// LIGHTS //
	light = std::make_shared<LightActor>(nullptr, LightStyle::DirectionLight, Vec3(1.0f, 1.0f, 1.0f), Vec4(0.6f, 0.6f, 0.6f, 0.0f));
	light->OnCreate();

	// CAMERA //
	camera = std::make_shared<CameraActor>(nullptr);

	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -10.0f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("Projection Matrix");
	camera->GetViewMatrix().print("View Matrix");

	// ACTOR : CheckerBoard //
	
	checkeredBoard = std::make_shared<Actor>(nullptr);
	checkeredBoard->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -10.0f),  Quaternion(), Vec3(1.0f, 1, 1));
	checkeredBoard->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	checkeredBoard->AddComponent<ShaderComponent>(shader);
	checkeredBoard->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	checkeredBoard->OnCreate();
	
	//Threads//
	std::thread threadLoadRedChecker(LoadRedChecker, checkeredBoard, checker_Red, m, shader, rText);
	std::thread threadLoadBlackChecker(LoadBlackChecker, checkeredBoard, checker_black, m, shader, bText);

	threadLoadRedChecker.join();
	threadLoadBlackChecker.join();

	return true;
}

void Scene2::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	if (light) {
		light->RemoveComponent<LightActor>();
	}
	if (camera) {
		camera->RemoveComponent<CameraActor>();
		//delete camera;
	}
	if (checkeredBoard) {
		checkeredBoard->OnDestroy();
		//delete checkeredBoard;
	}
	if (checker_Red) {
		for (int i = 0; i < 16; i++) {
			checker_Red[i]->OnDestroy();
			//delete checker_Red;
		}
	}
	if (checker_black) {
		for (int i = 0; i < 16; i++) {
			checker_black[i]->OnDestroy();
			//delete checker_Black;
		}
	}
}

void Scene2::HandleEvents(const SDL_Event &sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:

		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			Ref<TransformComponent> tc = camera->GetComponent<TransformComponent>();
			tc->SetTransform(tc->GetPosition() + Vec3(1.0f, 0.0f, 0.0f), tc->GetQuaternion());
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			Ref<TransformComponent> tc = camera->GetComponent<TransformComponent>();
			tc->SetTransform(tc->GetPosition() + Vec3(-1.0f, 0.0f, 0.0f), tc->GetQuaternion());
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			Ref<TransformComponent> tc = camera->GetComponent<TransformComponent>();
			tc->SetTransform(tc->GetPosition() + Vec3(0.0f, 0.0f, -1.0f), tc->GetQuaternion());
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			Ref<TransformComponent> tc = camera->GetComponent<TransformComponent>();
			tc->SetTransform(tc->GetPosition() + Vec3(0.0f, 0.0f, 1.0f), tc->GetQuaternion());
			camera->UpdateViewMatrix();

		}

		break;

	case SDL_KEYUP:
		std::cout << "I like when you press my buttons ;)\n";
		break;

	case SDL_MOUSEBUTTONDOWN:
		std::cout << "Mouse button down!\n";
		break;

	case SDL_MOUSEBUTTONUP:
		std::cout << "Mouse button up!\n";
		break;

	default:
		break;
	}
}


void Scene2::Update(const float deltaTime) {

}

void Scene2::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// checkeredBoard //
	Ref<ShaderComponent> shader = checkeredBoard->GetComponent<ShaderComponent>();
	Ref<MeshComponent> mesh = checkeredBoard->GetComponent<MeshComponent>();
	Ref<TransformComponent> transform = checkeredBoard->GetComponent<TransformComponent>();
	Ref<MaterialComponent> texture = checkeredBoard->GetComponent<MaterialComponent>();

	if (shader == nullptr || mesh == nullptr || transform == nullptr || texture == nullptr) {
		return;
	}
	
	glUseProgram(shader->GetProgram());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checkeredBoard->GetModelMatrix());
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	mesh->Render(GL_TRIANGLES);

	for(int i = 0; i < 16; i++)// checkerPiece //
	{
		shader = checker_Red[i]->GetComponent<ShaderComponent>();
		mesh = checker_Red[i]->GetComponent<MeshComponent>();
		texture = checker_Red[i]->GetComponent<MaterialComponent>();

		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checker_Red[i]->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		mesh->Render(GL_TRIANGLES);
	}
	//checker_black//
	for (int i = 0; i < 16; i++)// checkerPiece //

	{
		shader = checker_black[i]->GetComponent<ShaderComponent>();
		mesh = checker_black[i]->GetComponent<MeshComponent>();
		texture = checker_black[i]->GetComponent<MaterialComponent>();

		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checker_black[i]->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		mesh->Render(GL_TRIANGLES);
	}


	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


// Memory Monitor Code Start//

void* operator new(std::size_t amount) {
	totalBytes = totalBytes + amount;

	//if (LoadBlackChecker != NULL || LoadRedChecker != NULL) {
	std::cout << "allocating " << amount << " bytes of memory\n";
	return malloc(amount);

	//}
} 


void operator delete(void* memoryLocation, std::size_t amount) {
	totalBytes = totalBytes - amount;
	//if (LoadBlackChecker != NULL || LoadRedChecker != NULL) {
		std::cout << "freeing " << amount << " bytes of memory\n";
		free(memoryLocation);
	//}
}

// Memory Monitor Code End//


// Loading the Red Checkers

void LoadRedChecker(Ref<Actor> checkeredBoard, Ref<Actor> checker_Red[], Ref<MeshComponent> m, Ref<ShaderComponent> shader, Ref<MaterialComponent> rText) {
	int current = 0;
	for (int i = -3; i <= -0; i++) {
		for (int j = 0; j < 4; j++) {
			Ref<Actor> temp = std::make_shared<Actor>(checkeredBoard.get());
			if (j % 2 != 0) {
				temp->AddComponent<TransformComponent>(nullptr, Vec3(2.920f + 2.35 * i, 4.2f - 1.15 * j, 1.0f),
					Quaternion(), Vec3(0.165f, 0.165f, 0.165f));

			}
			else {
				temp->AddComponent<TransformComponent>(nullptr, Vec3(4.10f + 2.35 * i, 4.20f - 1.15 * j, 1.0f),
					Quaternion(), Vec3(0.165f, 0.165f, 0.165f));

			}

			temp->AddComponent<MeshComponent>(m);
			temp->AddComponent<ShaderComponent>(shader);
			temp->AddComponent<MaterialComponent>(rText);
			temp->OnCreate();
			checker_Red[current++] = temp;
		}
	}
}

// Loading the Black checkers//

void LoadBlackChecker(Ref<Actor> checkeredBoard, Ref<Actor> checker_black[], Ref<MeshComponent> m, Ref<ShaderComponent> shader, Ref<MaterialComponent> bText) {
	int current = 0;
	for (int i = -3; i <= 0; i++) {
		for (int j = 0; j < 4; j++) {
			Ref<Actor> temp = std::make_shared<Actor>(checkeredBoard.get());
			if (j % 2 == 0) {
				temp->AddComponent<TransformComponent>(nullptr, Vec3(2.920f + 2.35 * i, -4.1f + 1.17 * j, 1.0f),
					Quaternion(), Vec3(0.165f, 0.165f, 0.165f));

			}
			else {
				temp->AddComponent<TransformComponent>(nullptr, Vec3(4.10f + 2.35 * i, -4.1f + 1.17 * j, 1.0f),
					Quaternion(), Vec3(0.165f, 0.165f, 0.165f));

			}

			temp->AddComponent<MeshComponent>(m);
			temp->AddComponent<ShaderComponent>(shader);
			temp->AddComponent<MaterialComponent>(bText);
			temp->OnCreate();
			checker_black[current++] = temp;
		}
	}
}

