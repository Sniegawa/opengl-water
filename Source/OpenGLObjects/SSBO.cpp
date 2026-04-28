#include "SSBO.h"

SSBO::SSBO()
{
	glGenBuffers(1, &m_ID);
}

SSBO::~SSBO()
{
	glDeleteBuffers(1, &m_ID);
}

void SSBO::Bind(GLuint bindingPoint) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_ID);
}

void SSBO::Unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}