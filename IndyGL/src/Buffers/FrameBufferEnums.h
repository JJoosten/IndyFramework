#pragma once

namespace Indy
{
	namespace FrameBufferMipMaps
	{
		enum FrameBufferMipMapping
		{
			DISABLED,
			ENABLED
		};
	}

	namespace FrameBufferDepth
	{
		enum FrameBufferDepthBuffer
		{
			DISABLED,
			ENABLED
		};
	}

	namespace RenderTargets
	{
		enum RenderTarget
		{
			RT0 = 0,
			RT1,
			RT2,
			RT3,
			RT4,
			RT5,
			RT6,
			RT7
		};
	}

	namespace NumRenderTargets
	{
		enum NumRenderTargets
		{
			ONE = 1,
			TWO,
			THREE,
			FOUR,
			FIVE,
			SIX,
			SEVEN,
			EIGHT
		};
	}
}