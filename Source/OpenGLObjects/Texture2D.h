#pragma once

#include <string>
#include <glad/glad.h>
class Texture2D
{
public:
	Texture2D();
	~Texture2D();

	Texture2D(const std::string& path);

	void Bind() const;
	void Bind(unsigned int slot) const;
	void Unbind() const;

	void SetData(int w, int h, GLenum internalFormat, GLenum format, GLenum type, const void* data);

	//Debug
	unsigned int GetID() { return m_ID; }
private:
	unsigned int m_ID;
};