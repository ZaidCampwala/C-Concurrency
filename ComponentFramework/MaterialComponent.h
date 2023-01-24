#ifndef MATERIALCOMPONENT_H
#define MATERIALCOMPONENT_H
#include "Component.h"
#include <glew.h>


class MaterialComponent : public Component {
	const char *filename;
	GLuint textureID;
public:
	MaterialComponent(Component *parent_, const char *filename_);
	virtual ~MaterialComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	bool LoadImage(const char *filename);
	inline GLuint GetTextureID() const { return textureID; }
};

#endif // !MATERIALCOMPONENT_H