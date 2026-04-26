#pragma once

#include <glad/glad.h>

class VAO
{
public:
	VAO();
	~VAO();

	VAO(const VAO&) = delete;
	VAO& operator=(const VAO&) = delete;

	void Bind() const;
	void Unbind() const;

	void linkAttrib(unsigned int layout, int size, GLenum type, GLsizei stride, const void* offset) const;

private:
	unsigned int m_ID;
};