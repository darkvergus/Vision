#include <algorithm>

#include "VisionRendering/Resources/Mesh.h"

VisionRendering::Resources::Mesh::Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex) :
	m_vertexCount(static_cast<uint32_t>(p_vertices.size())),
	m_indicesCount(static_cast<uint32_t>(p_indices.size())),
	m_materialIndex(p_materialIndex)
{
	CreateBuffers(p_vertices, p_indices);
	ComputeBoundingSphere(p_vertices);
}

void VisionRendering::Resources::Mesh::Bind()
{
	m_vertexArray.Bind();
}

void VisionRendering::Resources::Mesh::Unbind()
{
	m_vertexArray.Unbind();
}

uint32_t VisionRendering::Resources::Mesh::GetVertexCount()
{
	return m_vertexCount;
}

uint32_t VisionRendering::Resources::Mesh::GetIndexCount()
{
	return m_indicesCount;
}

uint32_t VisionRendering::Resources::Mesh::GetMaterialIndex() const
{
	return m_materialIndex;
}

const VisionRendering::Geometry::BoundingSphere& VisionRendering::Resources::Mesh::GetBoundingSphere() const
{
	return m_boundingSphere;
}

void VisionRendering::Resources::Mesh::CreateBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices)
{
	std::vector<float> vertexData;

	std::vector<unsigned int> rawIndices;

	for (const auto& vertex : p_vertices)
	{
		vertexData.push_back(vertex.position[0]);
		vertexData.push_back(vertex.position[1]);
		vertexData.push_back(vertex.position[2]);

		vertexData.push_back(vertex.texCoords[0]);
		vertexData.push_back(vertex.texCoords[1]);

		vertexData.push_back(vertex.normals[0]);
		vertexData.push_back(vertex.normals[1]);
		vertexData.push_back(vertex.normals[2]);

		vertexData.push_back(vertex.tangent[0]);
		vertexData.push_back(vertex.tangent[1]);
		vertexData.push_back(vertex.tangent[2]);

		vertexData.push_back(vertex.bitangent[0]);
		vertexData.push_back(vertex.bitangent[1]);
		vertexData.push_back(vertex.bitangent[2]);
	}

	m_vertexBuffer	= std::make_unique<Buffers::VertexBuffer<float>>(vertexData);
	m_indexBuffer	= std::make_unique<Buffers::IndexBuffer>(const_cast<uint32_t*>(p_indices.data()), p_indices.size());

	uint64_t vertexSize = sizeof(Geometry::Vertex);

	m_vertexArray.BindAttribute(0, *m_vertexBuffer, Buffers::EType::FLOAT, 3, vertexSize, 0);
	m_vertexArray.BindAttribute(1, *m_vertexBuffer,	Buffers::EType::FLOAT, 2, vertexSize, sizeof(float) * 3);
	m_vertexArray.BindAttribute(2, *m_vertexBuffer,	Buffers::EType::FLOAT, 3, vertexSize, sizeof(float) * 5);
	m_vertexArray.BindAttribute(3, *m_vertexBuffer,	Buffers::EType::FLOAT, 3, vertexSize, sizeof(float) * 8);
	m_vertexArray.BindAttribute(4, *m_vertexBuffer,	Buffers::EType::FLOAT, 3, vertexSize, sizeof(float) * 11);
}

void VisionRendering::Resources::Mesh::ComputeBoundingSphere(const std::vector<Geometry::Vertex>& p_vertices)
{
	m_boundingSphere.position = VisionMaths::FVector3::Zero;
	m_boundingSphere.radius = 0.0f;

	if (!p_vertices.empty())
	{
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();

		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		float maxZ = std::numeric_limits<float>::min();

		for (const auto& vertex : p_vertices)
		{
			minX = std::min(minX, vertex.position[0]);
			minY = std::min(minY, vertex.position[1]);
			minZ = std::min(minZ, vertex.position[2]);

			maxX = std::max(maxX, vertex.position[0]);
			maxY = std::max(maxY, vertex.position[1]);
			maxZ = std::max(maxZ, vertex.position[2]);
		}

		m_boundingSphere.position = VisionMaths::FVector3{ minX + maxX, minY + maxY, minZ + maxZ } / 2.0f;

		for (const auto& vertex : p_vertices)
		{
			const auto& position = reinterpret_cast<const VisionMaths::FVector3&>(vertex.position);
			m_boundingSphere.radius = std::max(m_boundingSphere.radius, VisionMaths::FVector3::Distance(m_boundingSphere.position, position));
		}
	}
}
