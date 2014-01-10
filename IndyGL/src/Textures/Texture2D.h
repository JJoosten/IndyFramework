// Juul Joosten 2013

#pragma once

//#include "GL/glew.h" included in TextureEnums.h
#include "TextureEnums.h"

#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	class Texture2D : public NonCopyable
	{

	public:
		Texture2D( void);
		~Texture2D( void);


		void Create( const char* const textureFile,
					 TextureStorageLocations::TextureStorageLocation storageLocation,
					 TextureCreateMipMaps::TextureCreateMipMap createMipMaps);

		void Create( const unsigned int width, 
					 const unsigned int height, 
					 TextureFormats::TextureFormat format,
					 TextureInternalFormats::TextureInternalFormat internalFormat,
					 TextureStorageLocations::TextureStorageLocation storageLocation,
					 TextureCreateMipMaps::TextureCreateMipMap createMipMaps);		     

		void Create( const unsigned int width, 
					 const unsigned int height, 
				     const unsigned char* const textureData, 
					 TextureFormats::TextureFormat format,
					 TextureInternalFormats::TextureInternalFormat internalFormat,
					 TextureStorageLocations::TextureStorageLocation storageLocation,
					 TextureCreateMipMaps::TextureCreateMipMap createMipMaps);
		
		void Destroy( void);

		Texture2D* MakeCopy( void) const;


		void UpdateTexture( const unsigned int xOffset, const unsigned int yOffset, 
							const unsigned int width, const unsigned int height, 
							const char* const data);

		void UpdateMipMaps( void);

		void Bind( void) const;

		void Unbind( void) const;


		void SetTextureWrapMode( const TextureWrapModes::TextureWrapMode wrapModeS = TextureWrapModes::REPEAT, 
								 const TextureWrapModes::TextureWrapMode wrapModeT = TextureWrapModes::REPEAT);

		void SetSamplerFilter( const TextureMinFilters::TextureMinFilter minSampler = TextureMinFilters::NEAREST_MIPMAP_LINEAR, 
							   const TextureMagFilters::TextureMagFilter magSampler = TextureMagFilters::LINEAR);

		void SetTextureBorderColor( const GLuint color = 0x0);


		/* --- Getters & Setters --- */
		bool IsOnGPU( void) const { return ((m_storageLocation & TextureStorageLocations::GPU_MEMORY) == TextureStorageLocations::GPU_MEMORY); }
		
		bool IsOnMainMemory( void) const { return ((m_storageLocation & TextureStorageLocations::MAIN_MEMORY) == TextureStorageLocations::MAIN_MEMORY); }

		bool HasGPUMipMaps( void) const { return (m_textureCreateMipMaps & TextureCreateMipMaps::ON_GPU) == TextureCreateMipMaps::ON_GPU; }

		unsigned int GetTextureID( void) const { return m_textureID; }

		unsigned int GetWidth( void) const { return m_width; }
		
		unsigned int GetHeight( void) const { return m_width; }

		unsigned int GetTexelSizeInBytes( void) const { return getTexelSizeInBytesFromInternalFormat(m_internalFormat); }

		unsigned int GetNumChannels( void) const { return getNumComponentsFromFormat(m_format); }

		const void* const GetReadOnlyTextureData( void) const { return m_textureData; }
		
		TextureStorageLocations::TextureStorageLocation GetStorageLocation( void) const { return m_storageLocation; }

		TextureCreateMipMaps::TextureCreateMipMap HasMipMaps( void) const { return m_textureCreateMipMaps; }


	private:
		void loadTextureData( const char* const textureFile);

		void generateGPUTexture( void);
		
		void destroyGPUTexture( void);

		void destroyMainMemoryTexture( void);

		unsigned int getNumComponentsFromFormat( const TextureFormats::TextureFormat format) const;

		unsigned int getTexelSizeInBytesFromInternalFormat( const TextureInternalFormats::TextureInternalFormat internalFormat) const;

		TextureTypes::TextureType getTextureTypeFromInternalFormat( const TextureInternalFormats::TextureInternalFormat internalFormat) const;


	private:
		unsigned char*									m_textureData;
		unsigned int									m_textureID;
		unsigned int									m_width;
		unsigned int									m_height;
		TextureTypes::TextureType						m_type;
		TextureFormats::TextureFormat					m_format;
		TextureInternalFormats::TextureInternalFormat	m_internalFormat;
		TextureStorageLocations::TextureStorageLocation m_storageLocation;
		TextureCreateMipMaps::TextureCreateMipMap		m_textureCreateMipMaps;
		bool											m_isLoadedFromImage;
	};
}