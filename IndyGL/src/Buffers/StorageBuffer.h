// Juul Joosten 2013

#pragma once

#include "DrawModeEnums.h"

#include "GL/glew.h"
#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	class StorageBuffer : public NonCopyable
	{

	public:
		StorageBuffer(void);

		~StorageBuffer(void);

		// ownership of attribute data is transferred
		void Create( const GLvoid* const data,
					 const GLuint itemSizeInBytes,
					 const GLuint numItems,
					 const BufferUsage::BufferUse usage = BufferUsage::DYNAMIC_COPY);

		void Destroy(void);

		void ReSend( const GLuint numElementsToSend,
					 const GLuint numElementsOffset = 0);


		void Bind(void) const;
		void Unbind(void) const;

		void SetShaderLayoutIndex(unsigned int index);

		/* --- Getters & Setters --- */
		GLuint GetID(void) const { return m_bufferID; }

		GLuint GetItemSizeInBytes(void) const { return m_itemSizeInBytes; }

		GLuint GetTotalBufferSizeInBytes(void) const { return m_totalBufferSizeInBytes; }

		GLuint GetArraySize(void) const { return m_numItems; }

		const GLvoid* const GetReadOnlyData(void) const { return m_data; }

		GLvoid* const GetDataUnsafe(void) const { return m_data; }

	private:
		GLvoid*	m_data;
		GLuint  m_bufferID;
		GLuint  m_numItems;
		GLuint  m_totalBufferSizeInBytes;
		GLubyte m_itemSizeInBytes;
	};
}