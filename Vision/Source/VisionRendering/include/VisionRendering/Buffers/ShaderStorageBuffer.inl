#pragma once

#include "VisionRendering/Buffers/ShaderStorageBuffer.h"
#include "UniformBuffer.h"

namespace VisionRendering::Buffers
{
	template<typename T>
	inline void ShaderStorageBuffer::SendBlocks(T* p_data, size_t p_size)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, p_size, p_data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}