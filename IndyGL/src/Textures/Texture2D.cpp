// Juul Joosten 2013

#include "Texture2D.h"

// #include "GL/glew.h" included via Texture2D.h

#include <IndyCore/CoreDefines.h>

#include "stb_image/stb_image.h"

#include <cstdio>

namespace Indy
{
	Texture2D::Texture2D( void)
		:
	m_textureID(0)
	,m_width(0)
	,m_height(0)
	,m_type(TextureTypes::UNSIGNED_BYTE)
	,m_format(TextureFormats::RGB)
	,m_internalFormat(TextureInternalFormats::RGB8)
	,m_isLoadedFromImage(false)
	,m_textureData(NULL)
	,m_storageLocation(TextureStorageLocations::MAIN_AND_GPU_MEMORY)
	,m_textureCreateMipMaps(TextureCreateMipMaps::NONE)
	{

	}

	Texture2D::~Texture2D( void)
	{

	}


	void Texture2D::Create( const char* const textureFile,
							TextureStorageLocations::TextureStorageLocation storageLocation,
							TextureCreateMipMaps::TextureCreateMipMap createMipMaps)
	{
		m_storageLocation = storageLocation;
		m_textureCreateMipMaps = createMipMaps;

		// load texture data
		loadTextureData( textureFile);
		
		m_type = getTextureTypeFromInternalFormat(m_internalFormat);

		if( IsOnGPU())
			generateGPUTexture();

		if( !IsOnMainMemory())
			destroyMainMemoryTexture();
	}

	void Texture2D::Create( const unsigned int width, 
							const unsigned int height, 
							TextureFormats::TextureFormat format,
							TextureInternalFormats::TextureInternalFormat internalFormat,
							TextureStorageLocations::TextureStorageLocation storageLocation,
							TextureCreateMipMaps::TextureCreateMipMap createMipMaps)
	{
		m_storageLocation = storageLocation;
		m_textureCreateMipMaps = createMipMaps;
		m_format = format;
		m_internalFormat = internalFormat;

		m_type = getTextureTypeFromInternalFormat(m_internalFormat);

		const unsigned int texelSizeInBytes = getTexelSizeInBytesFromInternalFormat(internalFormat);
		
		m_width = width;
		m_height = height;
		
		if( IsOnMainMemory())
			m_textureData = new unsigned char[width * height * texelSizeInBytes];

		if( IsOnGPU())
			generateGPUTexture();
	}

	void Texture2D::Create( const unsigned int width, 
							const unsigned int height, 
							const unsigned char* const textureData, 
							TextureFormats::TextureFormat format,
							TextureInternalFormats::TextureInternalFormat internalFormat,
							TextureStorageLocations::TextureStorageLocation storageLocation,
							TextureCreateMipMaps::TextureCreateMipMap createMipMaps)
	{
		if(textureData == NULL)
			BREAKPOINT( textureData == NULL in Texture2D::Create );

		m_storageLocation = storageLocation;
		m_textureCreateMipMaps = createMipMaps;
		
		m_type = getTextureTypeFromInternalFormat(m_internalFormat);

		m_width = width;
		m_height = height;

		
		const unsigned int texelSizeInBytes = getTexelSizeInBytesFromInternalFormat(internalFormat);
		
		m_textureData = new unsigned char[width * height * texelSizeInBytes];
		memcpy( m_textureData, textureData, m_width * m_height * texelSizeInBytes);

		if( IsOnGPU())
			generateGPUTexture();

		if( !IsOnMainMemory())
			destroyMainMemoryTexture();
	}

	void Texture2D::Destroy( void)
	{
		if( IsOnGPU())
			destroyGPUTexture();

		if( IsOnMainMemory())
			destroyMainMemoryTexture();
	}


	Texture2D* Texture2D::MakeCopy( void) const
	{
		Texture2D* texture = new Texture2D();

		if( IsOnGPU() && IsOnMainMemory())
			texture->Create( m_width, m_height, m_textureData, m_format, m_internalFormat, m_storageLocation, m_textureCreateMipMaps);
		else if( IsOnMainMemory())
			texture->Create( m_width, m_height, m_textureData, m_format, m_internalFormat, m_storageLocation, m_textureCreateMipMaps);
		else if( IsOnGPU())
		{
			const unsigned int texelSizeInBytes = getTexelSizeInBytesFromInternalFormat(m_internalFormat);
			unsigned char* texDataFromGPU = new unsigned char[m_width * m_height * texelSizeInBytes];

			glGetTexImage( GL_TEXTURE_2D, 0, m_format, m_type, texDataFromGPU);
			
			texture->Create( m_width, m_height, texDataFromGPU, m_format, m_internalFormat, m_storageLocation, m_textureCreateMipMaps);
			
			delete[] texDataFromGPU;
		}

		return texture;
	}

	void Texture2D::UpdateTexture( const unsigned int xOffset, const unsigned int yOffset, 
								   const unsigned int width, const unsigned int height, 
								   const char* const data)
	{
		if( !IsOnGPU())
			BREAKPOINT(Texture is not on GPU);

		Bind();
		
		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xOffset, (GLint)yOffset, (GLsizei)width, (GLsizei)height, m_format, m_type, (GLvoid*)data );

		UpdateMipMaps();

		Unbind();
	}

	
	void Texture2D::UpdateMipMaps( void)
	{
		Bind();
		if( HasGPUMipMaps())
			glGenerateMipmap(GL_TEXTURE_2D);
		Unbind();
	}

	void Texture2D::Bind( void) const
	{
		glBindTexture( GL_TEXTURE_2D, m_textureID);
	}

	void Texture2D::Unbind( void) const
	{
		glBindTexture( GL_TEXTURE_2D, 0);
	}


	void Texture2D::SetTextureWrapMode( const TextureWrapModes::TextureWrapMode wrapModeS /*= TextureWrapModes::REPEAT*/, 
									    const TextureWrapModes::TextureWrapMode wrapModeT /*= TextureWrapModes::REPEAT*/)
	{
		Bind();
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeS );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeT );
		Unbind();
	}

	void Texture2D::SetSamplerFilter( const TextureMinFilters::TextureMinFilter minSampler /*= TextureMinFilters::NEAREST_MIPMAP_LINEAR*/, 
									  const TextureMagFilters::TextureMagFilter magSampler /*= TextureMagFilters::LINEAR*/)
	{
		/* TODO: let people set a global filter?
		if( GLEW_EXT_texture_filter_anisotropic)
		{
			Float32 maxAnisotropic = 0;
			glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropic);
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropic);
		} */
		Bind();
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minSampler );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magSampler );
		Unbind();
	}

	void Texture2D::SetTextureBorderColor( const GLuint color /* = 0x0 */)
	{
		Bind();
		GLfloat colorf[4];
		colorf[0] = (GLfloat)(color >> 24) / 255.0f;
		colorf[1] = (GLfloat)((color >> 16) & 255) / 255.0f; 
		colorf[2] = (GLfloat)((color >> 8) & 255) / 255.0f;
		colorf[3] = 0.0f;
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colorf);
		Unbind();
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

		m_format = numComponents == 4 ? TextureFormats::RGBA : TextureFormats::RGB;
		m_format = numComponents == 2 ? TextureFormats::RG : m_format;
		m_format = numComponents == 1 ? TextureFormats::R : m_format;

		// we know that stbimage always returns images in 8 byte per channel format
		m_internalFormat = numComponents == 4 ? TextureInternalFormats::RGBA8 : TextureInternalFormats::RGB8;
		m_internalFormat = numComponents == 2 ? TextureInternalFormats::RG8 : m_internalFormat;
		m_internalFormat = numComponents == 1 ? TextureInternalFormats::R8 : m_internalFormat;
	}

	void Texture2D::generateGPUTexture( void)
	{
		// generate texture ID
		glGenTextures( 1, &m_textureID);

		// send texture data to openGL
		Bind();

		glTexImage2D( GL_TEXTURE_2D, 0, m_internalFormat, (GLsizei)m_width, (GLsizei)m_height, 0, m_format, m_type, (GLvoid*)m_textureData);
		
		if( HasGPUMipMaps())
			glGenerateMipmap(GL_TEXTURE_2D);

		Unbind();
	}

	void Texture2D::destroyGPUTexture( void)
	{
		if( m_textureID == 0)
			BREAKPOINT("GPU texture was not initialized or was already deleted!");

		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
	}

	void Texture2D::destroyMainMemoryTexture( void)
	{
		if( m_textureData != NULL)
		{
			if( m_isLoadedFromImage)
				stbi_image_free( m_textureData);
			else
				delete[] m_textureData;

			m_textureData = NULL;
		}
	}

	unsigned int Texture2D::getNumComponentsFromFormat( const TextureFormats::TextureFormat format) const
	{
		switch(format)
		{
			case TextureFormats::R:
			case TextureFormats::STENCIL:
			case TextureFormats::DEPTH:
			case TextureFormats::R_INTEGER:
				return 1;
			break;

			case TextureFormats::RG:
			case TextureFormats::RG_INTEGER:
			case TextureFormats::DEPTH_STENCIL:
				return 2;
			break;

			case TextureFormats::RGB:
			case TextureFormats::BGR:
			case TextureFormats::RGB_INTEGER:
			case TextureFormats::BGR_INTEGER:
				return 3;
			break;

			case TextureFormats::RGBA:
			case TextureFormats::BGRA:
			case TextureFormats::RGBA_INTEGER:
			case TextureFormats::BGRA_INTEGER:
				return 4;
			break;
		}
		BREAKPOINT(Texture format is not recognised in Texture2D::getNumComponentsFromFormat);
		return 0;
	}

	unsigned int Texture2D::getTexelSizeInBytesFromInternalFormat( const TextureInternalFormats::TextureInternalFormat internalFormat) const
	{
		switch(internalFormat)
		{
			case TextureInternalFormats::RGBA                              :
				return 4;
			break;
			case TextureInternalFormats::RGB                               :
				return 3;
			break;
			case TextureInternalFormats::R                                 :
				return 1;
			break;
			case TextureInternalFormats::RG                                :
				return 2;
			break;
			case TextureInternalFormats::DEPTH                             :
				return 4;
			break;
			case TextureInternalFormats::DEPTH_STENCIL                     :
				return 4;
			break;
			case TextureInternalFormats::R8                                :
				return 1;
			break;
			case TextureInternalFormats::R8_SNORM                          :
				return 1;
			break;		
			case TextureInternalFormats::R16                               :
				return 2;
			break;		
			case TextureInternalFormats::R16_SNORM                         :
				return 2;
			break;		
			case TextureInternalFormats::RG8                               :
				return 2;
			break;		
			case TextureInternalFormats::RG8_SNORM                         :
				return 2;
			break;		
			case TextureInternalFormats::RG16                              :
				return 4;
			break;		
			case TextureInternalFormats::RG16_SNORM                        :
				return 4;
			break;		
			case TextureInternalFormats::R3_G3_B2                          :
				return 1;
			break;		
			case TextureInternalFormats::RGB4                              :
				return 2;
			break;		
			case TextureInternalFormats::RGB5                              :
				return 2;
			break;		
			case TextureInternalFormats::RGB8                              :
				return 3;
			break;		
			case TextureInternalFormats::RGB8_SNORM                        :
				return 3;
			break;		
			case TextureInternalFormats::RGB10                             :
				return 4;
			break;		
			case TextureInternalFormats::RGB12                             :
				return 5;
			break;		
			case TextureInternalFormats::RGB16                             :
				return 6;
			break;		
			case TextureInternalFormats::RGB16_SNORM                       :
				return 6;
			break;		
			case TextureInternalFormats::RGBA2                             :
				return 1;
			break;		
			case TextureInternalFormats::RGBA4                             :
				return 2;
			break;		
			case TextureInternalFormats::RGB5_A1                           :
				return 2;
			break;		
			case TextureInternalFormats::RGBA8                             :
				return 4;
			break;		
			case TextureInternalFormats::RGBA8_SNORM                       :
				return 4;
			break;		
			case TextureInternalFormats::RGB10_A2                          :
				return 4;
			break;		
			case TextureInternalFormats::RGB10_A2UI                        :
				return 4;
			break;		
			case TextureInternalFormats::RGBA12                            :
				return 6;
			break;		
			case TextureInternalFormats::RGBA16                            :
				return 8;
			break;		
			case TextureInternalFormats::sRGB8                             :
				return 3;
			break;		
			case TextureInternalFormats::sRGBA8                            :
				return 4;
			break;		
			case TextureInternalFormats::R16F                              :
				return 2;
			break;		
			case TextureInternalFormats::RG16F                             :
				return 4;
			break;		
			case TextureInternalFormats::RGB16F                            :
				return 6;
			break;		
			case TextureInternalFormats::RGBA16F                           :
				return 8;
			break;		
			case TextureInternalFormats::R32F                              :
				return 4;
			break;		
			case TextureInternalFormats::RG32F                             :
				return 8;
			break;		
			case TextureInternalFormats::RGB32F                            :
				return 12;
			break;		
			case TextureInternalFormats::RGBA32F                           :
				return 16;
			break;		
			case TextureInternalFormats::R11F_G11F_B10F                    :
				return 4;
			break;		
			case TextureInternalFormats::RGB9_E5		                   :   
				return 4;
			break;		
			case TextureInternalFormats::R8I                               :
				return 1;
			break;		
			case TextureInternalFormats::R8UI                              :
				return 1;
			break;		
			case TextureInternalFormats::R16I                              :
				return 2;
			break;		
			case TextureInternalFormats::R16UI                             :
				return 2;
			break;		
			case TextureInternalFormats::R32I                              :
				return 4;
			break;		
			case TextureInternalFormats::R32UI                             :
				return 4;
			break;		
			case TextureInternalFormats::RG8I                              :
				return 2;
			break;		
			case TextureInternalFormats::RG8UI                             :
				return 2;
			break;		
			case TextureInternalFormats::RG16I                             :
				return 4;
			break;		
			case TextureInternalFormats::RG16UI                            :
				return 4;
			break;		
			case TextureInternalFormats::RG32I                             :
				return 8;
			break;		
			case TextureInternalFormats::RG32UI                            :
				return 8;
			break;		
			case TextureInternalFormats::RGB8I                             :
				return 3;
			break;		
			case TextureInternalFormats::RGB8UI                            :
				return 3;
			break;		
			case TextureInternalFormats::RGB16I                            :
				return 6;
			break;		
			case TextureInternalFormats::RGB16UI                           :
				return 6;
			break;		
			case TextureInternalFormats::RGB32I                            :
				return 12;
			break;		
			case TextureInternalFormats::RGB32UI                           :
				return 12;
			break;		
			case TextureInternalFormats::RGBA8I                            :
				return 4;
			break;		
			case TextureInternalFormats::RGBA8UI                           :
				return 4;
			break;		
			case TextureInternalFormats::RGBA16I                           :
				return 8;
			break;		
			case TextureInternalFormats::RGBA16UI                          :
				return 8;
			break;		
			case TextureInternalFormats::RGBA32I                           :
				return 16;
			break;		
			case TextureInternalFormats::RGBA32UI                          :
				return 16;
			break;		

			// QQQ NOT SURE ABOUT THE COMPRESSED FORMATS!!
			case TextureInternalFormats::COMPRESSED_R                      :
				return 1;
			break;		
			case TextureInternalFormats::COMPRESSED_RG                     :
				return 2;
			break;		
			case TextureInternalFormats::COMPRESSED_RGB                    :
				return 3;
			break;		
			case TextureInternalFormats::COMPRESSED_RGBA                   :
				return 4;
			break;		
			case TextureInternalFormats::COMPRESSED_sRGB                   :
				return 3;
			break;		
			case TextureInternalFormats::COMPRESSED_sRGBA                  :
				return 4;
			break;		
			case TextureInternalFormats::COMPRESSED_R_RGTC1                :
				return 1;
			break;		
			case TextureInternalFormats::COMPRESSED_SIGNED_R_RGTC1         :
				return 1;
			break;		
			case TextureInternalFormats::COMPRESSED_RG_RGTC2               :
				return 2;
			break;		
			case TextureInternalFormats::COMPRESSED_SIGNED_RG_RGTC2        :
				return 2;
			break;		
			case TextureInternalFormats::COMPRESSED_RGBA_BPTC_UNORM        :
				return 4;
			break;		
			case TextureInternalFormats::COMPRESSED_sRGB_ALPHA_BPTC_UNORM  :
				return 3;
			break;		
			case TextureInternalFormats::COMPRESSED_RGB_BPTC_SIGNED_FLOAT  :
				return 12;
			break;		
			case TextureInternalFormats::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
				return 12;
			break;		
			
		}
		
		BREAKPOINT(Texture internalFormat is not recognised in Texture2D::getTexelSizeInBytesFromInternalFormat);
		return 0;
	}

	TextureTypes::TextureType Texture2D::getTextureTypeFromInternalFormat( const TextureInternalFormats::TextureInternalFormat internalFormat) const
	{
		switch(internalFormat)
		{
			case TextureInternalFormats::RGBA                              :
				return TextureTypes::UNSIGNED_BYTE;
			break;
			case TextureInternalFormats::RGB                               :
				return TextureTypes::UNSIGNED_BYTE;
			break;
			case TextureInternalFormats::R                                 :
				return TextureTypes::UNSIGNED_BYTE;
			break;
			case TextureInternalFormats::RG                                :
				return TextureTypes::UNSIGNED_BYTE;
			break;
			case TextureInternalFormats::DEPTH                             :
				return TextureTypes::UNSIGNED_INT;
			break;
			case TextureInternalFormats::DEPTH_STENCIL                     :
				return TextureTypes::FLOAT_32_UNSIGNED_INT_24_8;
			break;
			case TextureInternalFormats::R8                                :
				return TextureTypes::UNSIGNED_BYTE;
			break;
			case TextureInternalFormats::R8_SNORM                          :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::R16                               :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::R16_SNORM                         :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RG8                               :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RG8_SNORM                         :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RG16                              :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RG16_SNORM                        :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::R3_G3_B2                          :
				return TextureTypes::UNSIGNED_BYTE_3_3_2;
			break;		
			case TextureInternalFormats::RGB4                              :
				return TextureTypes::UNSIGNED_SHORT_4_4_4_4;
			break;		
			case TextureInternalFormats::RGB5                              :
				return TextureTypes::UNSIGNED_SHORT_5_5_5_1;
			break;		
			case TextureInternalFormats::RGB8                              :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGB8_SNORM                        :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGB10                             :
				return TextureTypes::UNSIGNED_INT_10_10_10_2;
			break;		
			case TextureInternalFormats::RGB12                             :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RGB16                             :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RGB16_SNORM                       :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RGBA2                             :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGBA4                             :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGB5_A1                           :
				return TextureTypes::UNSIGNED_SHORT_5_5_5_1;
			break;		
			case TextureInternalFormats::RGBA8                             :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGBA8_SNORM                       :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGB10_A2                          :
				return TextureTypes::UNSIGNED_INT_10_10_10_2;
			break;		
			case TextureInternalFormats::RGB10_A2UI                        :
				return TextureTypes::UNSIGNED_INT_10_10_10_2;
			break;		
			case TextureInternalFormats::RGBA12                            :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RGBA16                            :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::sRGB8                             :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::sRGBA8                            :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::R16F                              :
				return TextureTypes::HALF_FLOAT;
			break;		
			case TextureInternalFormats::RG16F                             :
				return TextureTypes::HALF_FLOAT;
			break;		
			case TextureInternalFormats::RGB16F                            :
				return TextureTypes::HALF_FLOAT;
			break;		
			case TextureInternalFormats::RGBA16F                           :
				return TextureTypes::HALF_FLOAT;
			break;		
			case TextureInternalFormats::R32F                              :
				return TextureTypes::FLOAT;
			break;		
			case TextureInternalFormats::RG32F                             :
				return TextureTypes::FLOAT;
			break;		
			case TextureInternalFormats::RGB32F                            :
				return TextureTypes::FLOAT;
			break;		
			case TextureInternalFormats::RGBA32F                           :
				return TextureTypes::FLOAT;
			break;		
			case TextureInternalFormats::R11F_G11F_B10F                    :
				return TextureTypes::UNSIGNED_INT_10F_11F_11F;
			break;		
			case TextureInternalFormats::RGB9_E5		                   :   
				return TextureTypes::UNSIGNED_INT_5_9_9_9;
			break;		
			case TextureInternalFormats::R8I                               :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::R8UI                              :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::R16I                              :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::R16UI                             :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::R32I                              :
				return TextureTypes::UNSIGNED_INT;
			break;		
			case TextureInternalFormats::R32UI                             :
				return TextureTypes::UNSIGNED_INT;
			break;		
			case TextureInternalFormats::RG8I                              :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RG8UI                             :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RG16I                             :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RG16UI                            :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RG32I                             :
				return TextureTypes::UNSIGNED_INT;
			break;		
			case TextureInternalFormats::RG32UI                            :
				return TextureTypes::UNSIGNED_INT;
			break;		
			case TextureInternalFormats::RGB8I                             :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGB8UI                            :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGB16I                            :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RGB16UI                           :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RGB32I                            :
				return TextureTypes::UNSIGNED_INT;
			break;		
			case TextureInternalFormats::RGB32UI                           :
				return TextureTypes::UNSIGNED_INT;
			break;		
			case TextureInternalFormats::RGBA8I                            :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGBA8UI                           :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::RGBA16I                           :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RGBA16UI                          :
				return TextureTypes::SHORT;
			break;		
			case TextureInternalFormats::RGBA32I                           :
				return TextureTypes::UNSIGNED_INT;
			break;		
			case TextureInternalFormats::RGBA32UI                          :
				return TextureTypes::UNSIGNED_INT;
			break;		

			// QQQ NOT SURE ABOUT THE COMPRESSED FORMATS!!
			case TextureInternalFormats::COMPRESSED_R                      :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_RG                     :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_RGB                    :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_RGBA                   :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_sRGB                   :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_sRGBA                  :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_R_RGTC1                :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_SIGNED_R_RGTC1         :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_RG_RGTC2               :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_SIGNED_RG_RGTC2        :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_RGBA_BPTC_UNORM        :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_sRGB_ALPHA_BPTC_UNORM  :
				return TextureTypes::UNSIGNED_BYTE;
			break;		
			case TextureInternalFormats::COMPRESSED_RGB_BPTC_SIGNED_FLOAT  :
				return TextureTypes::FLOAT;
			break;		
			case TextureInternalFormats::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
				return TextureTypes::FLOAT;
			break;		
			
		}
		
		BREAKPOINT(Texture internalFormat is not recognised in Texture2D::getTexelSizeInBytesFromInternalFormat);
		return TextureTypes::UNSIGNED_BYTE;
	}
}