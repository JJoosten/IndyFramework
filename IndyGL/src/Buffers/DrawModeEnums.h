#pragma once

#include "GL/glew.h"

namespace Indy
{
	namespace DrawModes
	{
		enum DrawMode
		{
			POINTS                   = GL_POINTS,
			LINES                    = GL_LINES,
			LINE_STRIP               = GL_LINE_STRIP,
			LINE_LOOP                = GL_LINE_LOOP,
			LINES_ADJACENCY          = GL_LINES_ADJACENCY,
			LINE_STRIP_ADJACENCY     = GL_LINE_STRIP_ADJACENCY,
			TRIANGLES                = GL_TRIANGLES,
			TRIANGLE_STRIP           = GL_TRIANGLE_STRIP,
			TRIANGLE_FAN             = GL_TRIANGLE_FAN,
			TRIANGLES_ADJACENCY      = GL_TRIANGLES_ADJACENCY,
			TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
			PATCHES                  = GL_PATCHES
		};
	}

	namespace BufferUsage
	{
		enum BufferUse
		{
			STATIC_DRAW  = GL_STATIC_DRAW,
			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
			DYNAMIC_COPY = GL_DYNAMIC_COPY
		};
	}
}