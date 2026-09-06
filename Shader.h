#ifndef SHADER_H
#define SHADER_H

#include "Camera.h"

class Shader
{
public:
	// Porgram ID
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);

	// Functions

	// Use/Activate Program
	void use() const;
	// Uniform tool functions
	// Always *.use() before Call set-fuction.
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
};

#endif // !SHADER_H

