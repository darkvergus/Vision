#include "VisionRendering/Buffers/VertexArray.h"

VisionRendering::Buffers::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_bufferID);
	glBindVertexArray(m_bufferID);
}

VisionRendering::Buffers::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_bufferID);
}

void VisionRendering::Buffers::VertexArray::Bind()
{
	glBindVertexArray(m_bufferID);
}

void VisionRendering::Buffers::VertexArray::Unbind()
{
	glBindVertexArray(0);
}

GLint VisionRendering::Buffers::VertexArray::GetID()
{
	return m_bufferID;
}
