#pragma once

#include <glad/glad.h>

class EBO
{
public:
	EBO();
	~EBO();

	EBO(const EBO&) = delete;
	EBO& operator=(const EBO&) = delete;

	void SetData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);
	
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_ID;
};