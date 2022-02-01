#include "inc\Texture.h"

Texture2D::Texture2D(const std::string imagePath, const unsigned int textureUnit)
	: m_TextureUnit(textureUnit)
{
	GLCALL(glGenTextures(1, &this->m_ID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, this->m_ID));

	int height, width, channels;
	
	// load in image as bytes
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(
		imagePath.c_str(), 
		&width, 
		&height, 
		&channels, 
		0);


	if (data) {
		std::cout << "[TU:" << this->m_TextureUnit << "] NUMBER OF CHANNELS: " << channels << std::endl;
		unsigned int c = (channels == 3 ? GL_RGB : GL_RGBA);
		GLCALL(glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			(channels == 3 ? GL_RGB : GL_RGBA),
			GL_UNSIGNED_BYTE,
			data
		));
		GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		std::cout << "ERROR: Image did not load successfully" << std::endl;
	}

	stbi_image_free(data);
}

void Texture2D::SetTexWrap(const unsigned int S, const unsigned int T)
{
	Bind();
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T));
}

void Texture2D::SetTexBorderColour(const float R, const float G, const float B, const float A)
{
	Bind();
	float borderColour[] = { R, G, B, A };
	GLCALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour));
}

void Texture2D::SetTexFilter(const unsigned int mag, const unsigned int min)
{
	Bind();
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag));
}

void Texture2D::Bind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, this->m_ID));
}

/* TODO: Make sure to have an automated system to set the texture units */
void Texture2D::BindTextureUnit() const
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + this->m_TextureUnit))
	GLCALL(glBindTexture(GL_TEXTURE_2D, this->m_ID));
}

void Texture2D::Unbind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}
