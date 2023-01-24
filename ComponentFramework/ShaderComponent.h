#pragma once
#include "Component.h"
#include "glew.h"
#include <unordered_map>

class ShaderComponent: public Component {
	ShaderComponent(const ShaderComponent&) = delete;
	ShaderComponent(ShaderComponent&&) = delete;
	ShaderComponent& operator = (const ShaderComponent&) = delete;
	ShaderComponent& operator = (ShaderComponent&&) = delete;

private:
	const char* vsFilename;
	const char* fsFilename;
	unsigned int shaderID;
	unsigned int vertShaderID;
	unsigned int fragShaderID;
	std::unordered_map<std::string, GLuint> uniformMap;

	/// Private helper methods
	char* ReadTextFile(const char *fileName);	
	bool CompileAttach();
	bool Link();
	void SetUniformLocations();
	

public:
	ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_);
	~ShaderComponent();
	inline GLuint GetProgram() const { return shaderID;}
	GLuint GetUniformID(std::string name);
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime ) override;
	void Render() const override;
};


