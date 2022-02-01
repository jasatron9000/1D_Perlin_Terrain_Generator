#pragma once

#include <glad/glad.h>

class VertexBuffer
{
private:
	unsigned int m_ID;

public:
	VertexBuffer(const float* indices, const unsigned int count, const unsigned int usage);
	
	void Update(const float* indices, const unsigned int count, const unsigned int usage);

	void Bind() const;
	void Unbind() const;
};

class ElementBuffer
{
private:
	unsigned int m_ID;
	unsigned int m_count;

public:
	ElementBuffer(const unsigned int *indices, const unsigned int count, const unsigned int usage);
	
	void Update(const unsigned int* indices, const unsigned int count, const unsigned int usage);

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() { return this->m_count; }
};