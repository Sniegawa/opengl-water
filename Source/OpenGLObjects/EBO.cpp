#include "EBO.h"

EBO::EBO()
{
	glGenBuffers(1, &m_ID);
}

EBO::~EBO()
{
	glDeleteBuffers(1, &m_ID);
}


void EBO::SetData(const void* data, GLsizeiptr size, GLenum usage)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

void EBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void EBO::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}