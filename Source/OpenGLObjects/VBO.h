#pragma once

#include <glad/glad.h>

class VBO
{
public:
	VBO(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

	VBO();

	~VBO();

	void SetData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

	VBO(const VBO&) = delete;
	VBO& operator=(const VBO&) = delete;

	void Bind() const;
	void Unbind() const;


private:
	unsigned int m_ID;
};