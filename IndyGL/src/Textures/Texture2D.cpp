// Juul Joosten 2013

#include "Texture2D.h"

#include <IndyCore/CoreDefines.h>

#include "GL/glew.h"

#include "stb_image/stb_image.h"

#include <iostream>

namespace Indy
{
	Texture2D::Texture2D( const char* const textureFile)
		:
	m_textureData(NULL),
	m_textureID(0),
	m_width(0),
	m_height(0),
	m_numComponents(0),
	m_componentSizeInBytes(0),
	m_isOnGPU(false),
	m_isLocalDataAvailable(false),
	m_hasMipMaps(false),
	m_isLoadedFromImage(true)
	{
		// load texture data
		loadTextureData( textureFile);
	}

	Texture2D::Texture2D( const unsigned int width, const unsigned int height, 
						  const unsigned char* textureData, const unsigned char numComponents, 
						  const unsigned char componentSizeInBytes)
	  :
	m_textureData( new unsigned char[width * height * numComponents * componentSizeInBytes]),
	m_textureID(0),
	m_width(width),
	m_height(height),
	m_numComponents(numComponents),
	m_componentSizeInBytes(componentSizeInBytes),
	m_isOnGPU(false),
	m_isLocalDataAvailable(true),
	m_hasMipMaps(false),
	m_isLoadedFromImage(false)
	{
		if(textureData != NULL)
			memcpy(m_textureData, textureData, m_width * m_height * m_componentSizeInBytes * m_numComponents);
	}

	Texture2D::~Texture2D( void)
	{
		DeleteGPUData();

		DeleteLocalData();
	}


	void Texture2D::CreateGPUTexture( const bool generateMipMaps /*= true*/)
	{
		if( !m_isLocalDataAvailable)
			BREAKPOINT(Local texture data is not available!);

		// generate texture ID
		glGenTextures( 1, &m_textureID);

		// send texture data to openGL
		Bind();

		m_hasMipMaps = generateMipMaps;

		// set texture filters to bilinear
		if(generateMipMaps)
			glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		// TODO: make the next block better adjustable by external parameters to override auto select settings

		// select internal format
		GLenum type = GL_UNSIGNED_BYTE;
		GLint internalFormat = 0;
		switch(m_componentSizeInBytes)
		{
		case 1:
			// unsigned char buffer
			internalFormat = m_numComponents == 4 ? GL_RGBA8 : GL_RGB8;
			internalFormat = m_numComponents == 1 ? GL_R8 : internalFormat;
			type = GL_UNSIGNED_BYTE;
		break;
		case 2:
			// half float point buffer
			internalFormat = m_numComponents == 4 ? GL_RGBA16F : GL_RGB16F;
			internalFormat = m_numComponents == 1 ? GL_R16F : internalFormat;
			type = GL_FLOAT;
		break;
		case 4:
			// floating point buffer
			internalFormat = m_numComponents == 4 ? GL_RGBA32F : GL_RGB32F;
			internalFormat = m_numComponents == 1 ? GL_R32F : internalFormat;
			type = GL_FLOAT;
		break;
		}

		// select format from RGBA, RGB or RED
		GLenum format = m_numComponents == 4 ? GL_RGBA : GL_RGB;
		format = m_numComponents == 1 ? GL_RED : format;


		glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, (GLsizei)m_width, (GLsizei)m_height, 0, format, type, (GLvoid*)m_textureData);

		UnBind();

		m_isOnGPU = true;
	}


	void Texture2D::DeleteGPUData( void)
	{
		glDeleteTextures(1, &m_textureID);

		m_isOnGPU = false;
	}

	void Texture2D::DeleteLocalData( void)
	{
		if( m_textureData != NULL)
		{
			if(m_isLocalDataAvailable)
			{
				if( m_isLoadedFromImage)
					stbi_image_free( m_textureData);
				else
					delete[] m_textureData;
			}

			m_textureData = NULL;
		}

		m_isLocalDataAvailable = false;
	}


	void Texture2D::Bind( void)
	{
		glBindTexture( GL_TEXTURE_2D, m_textureID);
	}

	void Texture2D::UnBind( void)
	{
		glBindTexture( GL_TEXTURE_2D, 0);
	}


	void Texture2D::SetTextureWrapMode( const GLint wrapModeS /* = GL_REPEAT */,
										const GLint wrapModeT /* = GL_REPEAT */)
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeS );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeT );
	}

	void Texture2D::SetSamplerFilter( const GLint minSampler /* = GL_NEAREST_MIPMAP_LINEAR */, 
									  const GLint magSampler /* = GL_LINEAR */)
	{
		/* TODO: let people set a global filter?
		if( GLEW_EXT_texture_filter_anisotropic)
		{
			Float32 maxAnisotropic = 0;
			glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropic);
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropic);
		}
		*/

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minSampler );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magSampler );
	}

	void Texture2D::SetTextureBorderColor( const GLuint color /* = 0x0 */)
	{
		GLfloat colorf[4];
		colorf[0] = (GLfloat)(color >> 24) / 255.0f;
		colorf[1] = (GLfloat)((color >> 16) & 255) / 255.0f; 
		colorf[2] = (GLfloat)((color >> 8) & 255) / 255.0f;
		colorf[3] = 0.0f;
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colorf);
	}


	void Texture2D::UpdateSubRegion( const unsigned int xOffset, const unsigned int yOffset, 
									 const unsigned int width, const unsigned int height, 
									 const char* const data)
	{
#ifdef DEBUG
		if( m_hasMipMaps )
			printf("Update can be done but mipmaps stay uneffected\n");
#endif

		if( !m_isOnGPU)
			BREAKPOINT(Texture is not on GPU);

				// select internal format
		GLenum type = GL_UNSIGNED_BYTE;
		GLint internalFormat = 0;
		switch(m_componentSizeInBytes)
		{
		case 1:
			// unsigned char buffer
			internalFormat = m_numComponents == 4 ? GL_RGBA8 : GL_RGB8;
			internalFormat = m_numComponents == 1 ? GL_R8 : internalFormat;
			type = GL_UNSIGNED_BYTE;
		break;
		case 2:
			// half float point buffer
			internalFormat = m_numComponents == 4 ? GL_RGBA16F : GL_RGB16F;
			internalFormat = m_numComponents == 1 ? GL_R16F : internalFormat;
			type = GL_FLOAT;
		break;
		case 4:
			// floating point buffer
			internalFormat = m_numComponents == 4 ? GL_RGBA32F : GL_RGB32F;
			internalFormat = m_numComponents == 1 ? GL_R32F : internalFormat;
			type = GL_FLOAT;
		break;
		}

		// select format from RGBA, RGB or RED
		GLenum format = m_numComponents == 4 ? GL_RGBA : GL_RGB;
		format = m_numComponents == 1 ? GL_RED : m_numComponents;

		Bind();
		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xOffset, (GLint)yOffset, (GLsizei)width, (GLsizei)height, format, type, (GLvoid*)data );
		UnBind();
	}



	/* --- private --- */
	void Texture2D::loadTextureData(const char* textureFile)
	{
		FILE* file = NULL;
		file = fopen( textureFile, "rb");

		if(file == NULL)
		{
			BREAKPOINT(Texture could not be loaded!);
			return;
		}

		int x; 
		int y; 
		int numComponents;

		m_textureData = stbi_load_from_file( file, &x, &y, &numComponents, 0);

		fclose(file);

		m_width = x;
		m_height = y;
		m_numComponents = numComponents;
		m_componentSizeInBytes = 1;
		m_isLocalDataAvailable = true;
	}

}