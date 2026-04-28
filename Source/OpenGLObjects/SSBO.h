#pragma once

#include <glad/glad.h>
#include <vector>


class SSBO
{
public:
	SSBO();
	~SSBO();

	template<typename T>
	void Upload(const T& data, GLenum usage = GL_DYNAMIC_DRAW)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T), &data, usage);
	};

	template<typename T>
	void Upload(const std::vector<T>& data, GLenum usage = GL_DYNAMIC_DRAW)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * data.size(), data.data(), usage);
	};

	template<typename T>
	void Update(const T& data, GLintptr offset = 0)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T), &data);
	};

	template<typename T>
	void Update(const std::vector<T>& data, GLintptr offset = 0)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T) * data.size(), data.data());
	};


	void Bind(GLuint bindingPoint) const;

	void Unbind() const;

private:
	unsigned int m_ID;
};