#include "inc\Buffers.h"
#include "inc\ErrorHandler.h"

VertexBuffer::VertexBuffer(const float* vertices, const unsigned int count, const unsigned int usage)
{
	GLCALL(glGenBuffers(1, &this->m_ID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->m_ID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, usage));
}

void VertexBuffer::Update(const float* vertices, const unsigned int count, const unsigned int usage)
{
	GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, usage));
}

void VertexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->m_ID));
}

void VertexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

ElementBuffer::ElementBuffer(const unsigned int* indices, const unsigned int count, const unsigned int usage)
	: m_count(count)
{
	GLCALL(glGenBuffers(1, &this->m_ID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, usage));
}

void ElementBuffer::Update(const unsigned int* indices, const unsigned int count, const unsigned int usage)
{
	GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), indices, usage));
}

void ElementBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ID));
}

void ElementBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
