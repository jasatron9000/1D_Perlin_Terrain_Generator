#include "inc\VertexArray.h"

/*========================================================
* 
*	VERTEX ATTRIBUTE CLASS
* 
* ========================================================
*/
template<typename T>
void VertexAttribute::Push(unsigned int count, const unsigned int norm)
{
	static_assert(false);
}

template<>
void VertexAttribute::Push<float>(unsigned int count, const unsigned int norm)
{
	this->m_Attributes.push_back({ GL_FLOAT, count, norm });
	this->m_Stride += count * VertexAttributeData::GetTypeSize(GL_FLOAT);
}

template<>
void VertexAttribute::Push<unsigned int>(unsigned int count, const unsigned int norm)
{
	this->m_Attributes.push_back({ GL_UNSIGNED_INT, count, norm });
	this->m_Stride += count * VertexAttributeData::GetTypeSize(GL_UNSIGNED_INT);
}

template<>
void VertexAttribute::Push<char>(unsigned int count, const unsigned int norm)
{
	this->m_Attributes.push_back({ GL_UNSIGNED_BYTE, count, norm });
	this->m_Stride += count * VertexAttributeData::GetTypeSize(GL_UNSIGNED_BYTE);
}

/*========================================================
*
*	VERTEX ARRAY CLASS
*
* ========================================================
*/

VertexArray::VertexArray()
{
	GLCALL(glGenVertexArrays(1, &this->m_ID));
	GLCALL(glBindVertexArray(this->m_ID));
}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &this->m_ID));
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexAttribute& attrib)
{
	Bind();
	buffer.Bind();

	const auto& attributes = attrib.GetAttributes();
	unsigned int offset = 0;
	
	for (unsigned int i = 0; i < attributes.size(); i++)
	{
		const auto& attribute = attributes[i];
		
		GLCALL(glVertexAttribPointer(
			i,
			attribute.count,
			attribute.type,
			attribute.normalised,
			attrib.GetStride(),
			(void*)offset
		));
		GLCALL(glEnableVertexAttribArray(i));

		offset += attribute.count * VertexAttributeData::GetTypeSize(attribute.type);
	}
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(this->m_ID));
}

void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}
