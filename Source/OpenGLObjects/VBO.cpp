#include "VBO.h"

VBO::VBO(const void* data, GLsizeiptr size, GLenum usage)
{
	glGenBuffers(1, &m_ID);
	Bind();
	SetData(data, size, usage);
}

VBO::VBO()
{
	glGenBuffers(1, &m_ID);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &m_ID);
}

void VBO::SetData(const void* data, GLsizeiptr size, GLenum usage)
{
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VBO::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}