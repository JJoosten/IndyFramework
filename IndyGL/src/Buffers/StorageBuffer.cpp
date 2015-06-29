#include "StorageBuffer.h"

#include <IndyCore\CoreDefines.h>

namespace Indy
{
	StorageBuffer::StorageBuffer(void)
		:
	m_bufferID(0)
	, m_data(NULL)
	, m_itemSizeInBytes(0)
	, m_totalBufferSizeInBytes(0)
	, m_numItems(0)
	{

	}

	StorageBuffer::~StorageBuffer(void)
	{
		if (m_data != 0)
			BREAKPOINT(VertexAttributeBuffer was not yet destroyed);

		if (m_bufferID != 0)
			BREAKPOINT(VertexAttributeBuffer was not yet destroyed);
	}

	void StorageBuffer::Create(const GLvoid* const data,
		const GLuint itemSizeInBytes,
		const GLuint numItems,
		const BufferUsage::BufferUse usage /*= BufferUsage::DYNAMIC_COPY*/)
	{
		if (m_bufferID != 0)
			BREAKPOINT(This AttributeBuffer is already in use!);

		if (numItems == 0)
			BREAKPOINT(numItems == 0);

		m_numItems = numItems;
		m_itemSizeInBytes = itemSizeInBytes;
		m_totalBufferSizeInBytes = m_itemSizeInBytes * m_numItems;
		m_data = new char[m_totalBufferSizeInBytes];
		memcpy(m_data, data, m_totalBufferSizeInBytes);

		if (m_data == NULL)
			BREAKPOINT(data is NULL);

		glGenBuffers(1, &m_bufferID);

		Bind();

		// create memory on the gpu to store 
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_totalBufferSizeInBytes, 0, usage);

		GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		memcpy(p, m_data, m_totalBufferSizeInBytes);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		Unbind();
	}

	void StorageBuffer::Destroy(void)
	{
		if (m_data == 0)
			BREAKPOINT(VertexAttributeBuffer was already destroyed or not initialized);

		if (m_bufferID == 0)
			BREAKPOINT(VertexAttributeBuffer was already destroyed or not initialized);

		delete m_data;
		m_data = NULL;

		glDeleteBuffers(1, &m_bufferID);
		m_bufferID = 0;
	}

	void StorageBuffer::ReSend(const GLuint numElementsToSend,
		const GLuint numElementsOffset /*= 0*/)
	{
		if (numElementsToSend == 0)
			BREAKPOINT(numElementsToSend is bigger then original data or 0);

		Bind();

		GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		memcpy(p, (char*)m_data + m_itemSizeInBytes * numElementsOffset, m_itemSizeInBytes * numElementsToSend);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		Unbind();
	}

	void StorageBuffer::PullFromGPU()
	{
		Bind();

		GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		memcpy(m_data, p, m_totalBufferSizeInBytes);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		Unbind();
	}

	void StorageBuffer::SetShaderLayoutIndex(unsigned int index)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_bufferID);
	}

	void StorageBuffer::Bind(void) const
	{
		if (m_bufferID == 0)
			BREAKPOINT(VertexAttributeBuffer is not yet a VBO);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
	}

	void StorageBuffer::Unbind(void) const
	{
		if (m_bufferID == 0)
			BREAKPOINT(VertexAttributeBuffer is not yet a VBO);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}