#include "VAO.h"
#include "VBO.h"

VAO::VAO()
{
	glCreateVertexArrays(1, &m_ID);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VAO::Bind() const
{
	glBindVertexArray(m_ID);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}

void VAO::linkAttrib(unsigned int layout, int size, GLenum type, GLsizei stride, const void* offset) const
{
	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
}
