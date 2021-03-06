#pragma once

#include "GL/glew.h"

namespace Indy
{
	namespace TextureWrapModes
	{
		enum TextureWrapMode
		{
			REPEAT               = GL_REPEAT,
			MIRRORED_REPEAT      = GL_MIRRORED_REPEAT,
			CLAMP_TO_EDGE        = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER      = GL_CLAMP_TO_BORDER,
			MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE
		};
	}

	namespace TextureMagFilters
	{
		enum TextureMagFilter
		{
			NEAREST                = GL_NEAREST,
			LINEAR                 = GL_LINEAR

		};
	}

	namespace TextureMinFilters
	{
		enum TextureMinFilter
		{
			NEAREST                = GL_NEAREST,
			LINEAR                 = GL_LINEAR,
			NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
			NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
			LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
		};
	}

	namespace TextureStorageLocations
	{
		enum TextureStorageLocation
		{
			MAIN_MEMORY = 1,
			GPU_MEMORY,
			MAIN_AND_GPU_MEMORY = MAIN_MEMORY | GPU_MEMORY
		};
	}

	namespace TextureCreateMipMaps
	{
		enum TextureCreateMipMap
		{
			NONE,
			ON_GPU
		};
	}

	namespace TextureInternalFormats
	{
		enum TextureInternalFormat
		{
			RGBA                               = GL_RGBA,
			RGB                                = GL_RGB,
			R                                  = GL_R,
			RG                                 = GL_RG,
			DEPTH                              = GL_DEPTH_COMPONENT,
			DEPTH_STENCIL                      = GL_DEPTH_STENCIL,
			R8                                 = GL_R8,
			R8_SNORM                           = GL_R8_SNORM,
			R16                                = GL_R16,
			R16_SNORM                          = GL_R16_SNORM,
			RG8                                = GL_RG8,
			RG8_SNORM                          = GL_RG8_SNORM,
			RG16                               = GL_RG16,
			RG16_SNORM                         = GL_RG16_SNORM,
			R3_G3_B2                           = GL_R3_G3_B2,
			RGB4                               = GL_RGB4,
			RGB5                               = GL_RGB5,
			RGB8                               = GL_RGB8,
			RGB8_SNORM                         = GL_RGB8_SNORM,
			RGB10                              = GL_RGB10,
			RGB12                              = GL_RGB12,
			RGB16                              = GL_RGB16,
			RGB16_SNORM                        = GL_RGB16_SNORM,
			RGBA2                              = GL_RGBA2,
			RGBA4                              = GL_RGBA4,
			RGB5_A1                            = GL_RGB5_A1,
			RGBA8                              = GL_RGBA8,
			RGBA8_SNORM                        = GL_RGBA8_SNORM,
			RGB10_A2                           = GL_RGB10_A2,
			RGB10_A2UI                         = GL_RGB10_A2UI,
			RGBA12                             = GL_RGBA12,
			RGBA16                             = GL_RGBA16,
			sRGB8                              = GL_SRGB8,
			sRGBA8                             = GL_SRGB8_ALPHA8,
			R16F                               = GL_R16F,
			RG16F                              = GL_RG16F,
			RGB16F                             = GL_RGB16F,
			RGBA16F                            = GL_RGBA16F,
			R32F                               = GL_R32F,
			RG32F                              = GL_RG32F,
			RGB32F                             = GL_RGB32F,
			RGBA32F                            = GL_RGBA32F,
			R11F_G11F_B10F                     = GL_R11F_G11F_B10F,
			RGB9_E5		                       = GL_RGB9_E5,
			R8I                                = GL_R8I,
			R8UI                               = GL_R8UI,
			R16I                               = GL_R16I,
			R16UI                              = GL_R16UI,
			R32I                               = GL_R32I,
			R32UI                              = GL_R32UI,
			RG8I                               = GL_RG8I,
			RG8UI                              = GL_RG8UI,
			RG16I                              = GL_RG16I,
			RG16UI                             = GL_RG16UI,
			RG32I                              = GL_RG32I,
			RG32UI                             = GL_RG32UI,
			RGB8I                              = GL_RGB8I,
			RGB8UI                             = GL_RGB8UI,
			RGB16I                             = GL_RGB16I,
			RGB16UI                            = GL_RGB16UI,
			RGB32I                             = GL_RGB32I,
			RGB32UI                            = GL_RGB32UI,
			RGBA8I                             = GL_RGBA8I,
			RGBA8UI                            = GL_RGBA8UI,
			RGBA16I                            = GL_RGBA16I,
			RGBA16UI                           = GL_RGBA16UI,
			RGBA32I                            = GL_RGBA32I,
			RGBA32UI                           = GL_RGBA32UI,
			COMPRESSED_R                       = GL_COMPRESSED_RED,
			COMPRESSED_RG                      = GL_COMPRESSED_RG,
			COMPRESSED_RGB                     = GL_COMPRESSED_RGB,
			COMPRESSED_RGBA                    = GL_COMPRESSED_RGBA,
			COMPRESSED_sRGB                    = GL_COMPRESSED_SRGB,
			COMPRESSED_sRGBA                   = GL_COMPRESSED_SRGB_ALPHA,
			COMPRESSED_R_RGTC1                 = GL_COMPRESSED_RED_RGTC1,
			COMPRESSED_SIGNED_R_RGTC1          = GL_COMPRESSED_SIGNED_RED_RGTC1,
			COMPRESSED_RG_RGTC2                = GL_COMPRESSED_RG_RGTC2,
			COMPRESSED_SIGNED_RG_RGTC2         = GL_COMPRESSED_SIGNED_RG_RGTC2,
			COMPRESSED_RGBA_BPTC_UNORM         = GL_COMPRESSED_RGBA_BPTC_UNORM,
			COMPRESSED_sRGB_ALPHA_BPTC_UNORM   = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
			COMPRESSED_RGB_BPTC_SIGNED_FLOAT   = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
			COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
		};
	}

	namespace TextureFormats
	{
		enum TextureFormat
		{
			R             = GL_RED,
			RG            = GL_RG,
			RGB           = GL_RGB,
			BGR           = GL_BGR,
			RGBA          = GL_RGBA,
			BGRA          = GL_BGRA,
			R_INTEGER     = GL_RED_INTEGER,
			RG_INTEGER    = GL_RG_INTEGER,
			RGB_INTEGER   = GL_RGB_INTEGER,
			BGR_INTEGER   = GL_BGR_INTEGER,
			RGBA_INTEGER  = GL_RGBA_INTEGER,
			BGRA_INTEGER  = GL_BGRA_INTEGER,
			STENCIL       = GL_STENCIL_INDEX,
			DEPTH         = GL_DEPTH_COMPONENT,
			DEPTH_STENCIL = GL_DEPTH_STENCIL
		};
	}

	namespace TextureTypes
	{
		enum TextureType
		{
			UNSIGNED_BYTE              = GL_UNSIGNED_BYTE,
			BYTE                       = GL_BYTE,
			UNSIGNED_SHORT             = GL_UNSIGNED_SHORT,
			SHORT                      = GL_SHORT,
			UNSIGNED_INT               = GL_UNSIGNED_INT,
			INT                        = GL_INT,
			HALF_FLOAT                 = GL_HALF_FLOAT,
			FLOAT                      = GL_FLOAT,
			UNSIGNED_BYTE_3_3_2        = GL_UNSIGNED_BYTE_3_3_2,
			UNSIGNED_BYTE_2_3_3        = GL_UNSIGNED_BYTE_2_3_3_REV,
			UNSIGNED_SHORT_5_6_5       = GL_UNSIGNED_SHORT_5_6_5,
			UNSIGNED_SHORT_4_4_4_4     = GL_UNSIGNED_SHORT_4_4_4_4,
			UNSIGNED_SHORT_5_5_5_1     = GL_UNSIGNED_SHORT_5_5_5_1,
			UNSIGNED_INT_8_8_8_8       = GL_UNSIGNED_INT_8_8_8_8,
			UNSIGNED_INT_10_10_10_2    = GL_UNSIGNED_INT_10_10_10_2,
			UNSIGNED_INT_2_10_10_10    = GL_UNSIGNED_INT_2_10_10_10_REV,
			UNSIGNED_INT_24_8          = GL_UNSIGNED_INT_24_8,
			UNSIGNED_INT_10F_11F_11F   = GL_UNSIGNED_INT_10F_11F_11F_REV,
			UNSIGNED_INT_5_9_9_9       = GL_UNSIGNED_INT_5_9_9_9_REV,
			FLOAT_32_UNSIGNED_INT_24_8 = GL_FLOAT_32_UNSIGNED_INT_24_8_REV
		};
	}
}