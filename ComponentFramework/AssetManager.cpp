#include <vector> 
#include "Component.h"
#include "TransformComponent.h" 
#include "MeshComponent.h" 
#include "MaterialComponent.h" 
#include "ShaderComponent.h" 
#include "AssetManager.h" 

AssetManager::AssetManager() {

	AddComponent<MeshComponent>("CheckerPiece", nullptr, "meshes/CheckerPiece.obj");
	AddComponent<ShaderComponent>("TextureShader" ,nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	AddComponent<MaterialComponent>("checker_Red" ,nullptr, "textures/redCheckerPiece.png");
	AddComponent<MaterialComponent>("checker_Black", nullptr, "textures/blackCheckerPiece.png");
	OnCreate();
}

AssetManager::~AssetManager() {

}

bool AssetManager::OnCreate() {

	for (std::pair<const char*, Ref<Component>> c : componentCatalog) {
		if (c.second->OnCreate() == false) {
			return false;
		}
	}
	return true;
}

void AssetManager::RemoveAllComponents() {

	componentCatalog.clear();
}