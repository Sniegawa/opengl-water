#pragma once

#include <string>

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

	Texture2D(const std::string& path);

	void Bind() const;
	void Bind(unsigned int slot) const;
	void Unbind() const;

private:
	unsigned int m_ID;
};