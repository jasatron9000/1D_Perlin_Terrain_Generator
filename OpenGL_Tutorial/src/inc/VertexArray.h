#pragma once
#include "Buffers.h"
#include "ErrorHandler.h"
#include <vector>

/* TODO: Create a class for the Vertex Array */
struct VertexAttributeData
{
	unsigned int type;
	unsigned int count;
	unsigned int normalised;

	static unsigned int GetTypeSize(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT			: return sizeof(GLfloat);
			case GL_UNSIGNED_INT	: return sizeof(GLuint);
			case GL_UNSIGNED_BYTE	: return sizeof(GLbyte);
		}

		ASSERT(false);
		return 0;
	}
};


class VertexAttribute
{
private:
	std::vector<VertexAttributeData> m_Attributes;
	unsigned int m_Stride;

public:
	VertexAttribute() : m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count, const unsigned int norm);

	template<>
	void Push<float>(unsigned int count, const unsigned int norm);

	template<>
	void Push<unsigned int>(unsigned int count, const unsigned int norm);

	template<>
	void Push<char>(unsigned int count, const unsigned int norm);

	inline std::vector<VertexAttributeData> GetAttributes() const { return this->m_Attributes; }
	inline unsigned int GetStride() const { return this->m_Stride; }
};

class VertexArray
{
private:
	unsigned int m_ID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& buffer, const VertexAttribute& attrib);

	void Bind() const;
	void Unbind() const;

};