#include "MaterialComponent.h"
#include <SDL_image.h>


MaterialComponent::MaterialComponent(Component *parent_, const char *filename_) 
	: Component(parent_), filename(filename_), textureID(0) {
}

MaterialComponent::~MaterialComponent() {
	OnDestroy();
}

bool MaterialComponent::OnCreate() {
	if (isCreated) return true; // OR return isCreated if already created
	isCreated = LoadImage(filename); // isCreated = true;
	return isCreated;
}

void MaterialComponent::OnDestroy() {
	glDeleteTextures(1, &textureID);
	isCreated = false;
}

void MaterialComponent::Update(const float deltaTime) {

}

void MaterialComponent::Render()const {

}

bool MaterialComponent::LoadImage(const char *filename) {
	glGenTextures(1, &textureID); // generate texture(s) and give a 'name' -> member variable(textureID) array of 1?
	glBindTexture(GL_TEXTURE_2D, textureID); // bind/connect the texture 2d to the member variable(textureID)
	SDL_Surface *textureSurface = IMG_Load(filename); // create an sdl_surface and call it IMG_Load with filename(.jpg, .png, .gif)

	if (textureSurface == nullptr) {
		return false;
	}

	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB; // '?' if true, ':' else
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);

	SDL_FreeSurface(textureSurface);
	/// Wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); /// Unbind the texture
	return true;
}

