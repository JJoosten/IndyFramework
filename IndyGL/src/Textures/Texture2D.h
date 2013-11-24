// Juul Joosten 2013

#pragma once

#include "GL/glew.h"

namespace Indy
{
	class Texture2D
	{

	public:
		Texture2D( void);
		Texture2D( const Texture2D& cpyCtor);
		~Texture2D( void);

		const Texture2D& operator=( const Texture2D& rvalue);


		void Create( const char* const textureFile);

		void Create( const unsigned int width, 
					 const unsigned int height, 
				     const unsigned char* const textureData, 
					 const unsigned char numComponents,
				     const unsigned char componentSizeInBytes);
		
		void Destroy( void);

		void GenerateGPUTexture( const bool generateMipMaps = true);
		
		void DestroyGPUTexture( void);

		void DestroyLocalTexture( void);

		
		void Bind( void) const;

		void Unbind( void) const;


		void SetTextureWrapMode( const GLint wrapModeS = GL_REPEAT, 
								 const GLint wrapModeT = GL_REPEAT);

		void SetSamplerFilter( const GLint minSampler = GL_NEAREST_MIPMAP_LINEAR, 
							   const GLint magSampler = GL_LINEAR);

		void SetTextureBorderColor( const GLuint color = 0x0);


		void UpdateSubRegion( const unsigned int xOffset, const unsigned int yOffset, 
							  const unsigned int width, const unsigned int height, 
							  const char* const data);


		/* --- Getters & Setters --- */
		bool IsOnGPU( void) const { return m_isOnGPU; }
		
		bool IsLocalDataAvailable( void) const { return m_isLocalDataAvailable; }

		unsigned int GetTextureID( void) const { return m_textureID; }

		unsigned int GetWidth( void) const { return m_width; }
		
		unsigned int GetHeight( void) const { return m_width; }

		unsigned int GetNumComponents( void) const { return m_numComponents; }

		unsigned char GetComponentSizeInBytes( void) const { return m_componentSizeInBytes; }

		const void* const GetReadOnlyTextureData( void) const { return m_textureData; }


	private:
		void loadTextureData( const char* const textureFile);


	private:
		unsigned char*	m_textureData;
		unsigned int	m_textureID;
		unsigned int	m_width;
		unsigned int	m_height;
		unsigned char	m_numComponents;
		unsigned char	m_componentSizeInBytes;
		bool			m_isOnGPU;
		bool			m_isLocalDataAvailable;
		bool			m_hasMipMaps;
		bool			m_isLoadedFromImage;
	};
}