#pragma once

#include <glad/glad.h>
#include <string>
#include "ErrorHandler.h"
#include "../lib/stb_image/stb_image.h"

class Texture2D
{
private:
	unsigned int m_ID;
	unsigned int m_TextureUnit;

public:
	Texture2D(const std::string imagePath, const unsigned int textureUnit);
	/* TODO: Deal with the deletion of the texture */
	//~Texture2D();

	void SetTexWrap(const unsigned int S, const unsigned int T);
	void SetTexBorderColour(const float R, const float G, const float B, const float A);
	void SetTexFilter(const unsigned int mag, const unsigned int min);

	void Bind() const;
	void BindTextureUnit() const;
	void Unbind() const;

	inline unsigned int GetTextureUnit() { return m_TextureUnit; }
};