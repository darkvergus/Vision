#include <functional>

#include "VisionRendering/Core/Renderer.h"

VisionRendering::Core::Renderer::Renderer(Context::Driver& p_driver) : m_driver(p_driver), m_state(0)
{
}

void VisionRendering::Core::Renderer::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha)
{
	glClearColor(p_red, p_green, p_blue, p_alpha);
}

void VisionRendering::Core::Renderer::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
	glClear
	(
		(p_colorBuffer ? GL_COLOR_BUFFER_BIT : 0) |
		(p_depthBuffer ? GL_DEPTH_BUFFER_BIT : 0) |
		(p_stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
	);
}

void VisionRendering::Core::Renderer::Clear(VisionRendering::LowRenderer::Camera & p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
	/* Backup the previous OpenGL clear color */
	GLfloat previousClearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, previousClearColor);

	/* Clear the screen using the camera clear color */
	const VisionMaths::FVector3& cameraClearColor = p_camera.GetClearColor();
	SetClearColor(cameraClearColor.x, cameraClearColor.y, cameraClearColor.z, 1.0f);
	Clear(p_colorBuffer, p_depthBuffer, p_stencilBuffer);

	/* Reset the OpenGL clear color to the previous clear color (Backuped one) */
	SetClearColor(previousClearColor[0], previousClearColor[1], previousClearColor[2], previousClearColor[3]);
}

void VisionRendering::Core::Renderer::SetRasterizationLinesWidth(float p_width)
{
	glLineWidth(p_width);
}

void VisionRendering::Core::Renderer::SetRasterizationMode(Settings::ERasterizationMode p_rasterizationMode)
{
	glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(p_rasterizationMode));
}

void VisionRendering::Core::Renderer::SetCapability(Settings::ERenderingCapability p_capability, bool p_value)
{
	(p_value ? glEnable : glDisable)(static_cast<GLenum>(p_capability));
}

bool VisionRendering::Core::Renderer::GetCapability(Settings::ERenderingCapability p_capability) const
{
	return glIsEnabled(static_cast<GLenum>(p_capability));
}

void VisionRendering::Core::Renderer::SetStencilAlgorithm(Settings::EComparaisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask)
{
	glStencilFunc(static_cast<GLenum>(p_algorithm), p_reference, p_mask);
}

void VisionRendering::Core::Renderer::SetDepthAlgorithm(Settings::EComparaisonAlgorithm p_algorithm)
{
	glDepthFunc(static_cast<GLenum>(p_algorithm));
}

void VisionRendering::Core::Renderer::SetStencilMask(uint32_t p_mask)
{
	glStencilMask(p_mask);
}

void VisionRendering::Core::Renderer::SetStencilOperations(Settings::EOperation p_stencilFail, Settings::EOperation p_depthFail, Settings::EOperation p_bothPass)
{
	glStencilOp(static_cast<GLenum>(p_stencilFail), static_cast<GLenum>(p_depthFail), static_cast<GLenum>(p_bothPass));
}

void VisionRendering::Core::Renderer::SetCullFace(Settings::ECullFace p_cullFace)
{
	glCullFace(static_cast<GLenum>(p_cullFace));
}

void VisionRendering::Core::Renderer::SetDepthWriting(bool p_enable)
{
	glDepthMask(p_enable);
}

void VisionRendering::Core::Renderer::SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha)
{
	glColorMask(p_enableRed, p_enableGreen, p_enableBlue, p_enableAlpha);
}

void VisionRendering::Core::Renderer::SetColorWriting(bool p_enable)
{
	glColorMask(p_enable, p_enable, p_enable, p_enable);
}

void VisionRendering::Core::Renderer::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void VisionRendering::Core::Renderer::ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Settings::EPixelDataFormat format, Settings::EPixelDataType type, void* data)
{
	glReadPixels(x, y, width, height, static_cast<GLenum>(format), static_cast<GLenum>(type), data);
}

bool VisionRendering::Core::Renderer::GetBool(GLenum p_parameter)
{
	GLboolean result;
	glGetBooleanv(p_parameter, &result);
	return static_cast<bool>(result);
}

bool VisionRendering::Core::Renderer::GetBool(GLenum p_parameter, uint32_t p_index)
{
	GLboolean result;
	glGetBooleani_v(p_parameter, p_index, &result);
	return static_cast<bool>(result);
}

int VisionRendering::Core::Renderer::GetInt(GLenum p_parameter)
{
	GLint result;
	glGetIntegerv(p_parameter, &result);
	return static_cast<int>(result);
}

int VisionRendering::Core::Renderer::GetInt(GLenum p_parameter, uint32_t p_index)
{
	GLint result;
	glGetIntegeri_v(p_parameter, p_index, &result);
	return static_cast<int>(result);
}

float VisionRendering::Core::Renderer::GetFloat(GLenum p_parameter)
{
	GLfloat result;
	glGetFloatv(p_parameter, &result);
	return static_cast<float>(result);
}

float VisionRendering::Core::Renderer::GetFloat(GLenum p_parameter, uint32_t p_index)
{
	GLfloat result;
	glGetFloati_v(p_parameter, p_index, &result);
	return static_cast<float>(result);
}

double VisionRendering::Core::Renderer::GetDouble(GLenum p_parameter)
{
	GLdouble result;
	glGetDoublev(p_parameter, &result);
	return static_cast<double>(result);
}

double VisionRendering::Core::Renderer::GetDouble(GLenum p_parameter, uint32_t p_index)
{
	GLdouble result;
	glGetDoublei_v(p_parameter, p_index, &result);
	return static_cast<double>(result);
}

int64_t VisionRendering::Core::Renderer::GetInt64(GLenum p_parameter)
{
	GLint64 result;
	glGetInteger64v(p_parameter, &result);
	return static_cast<int64_t>(result);
}

int64_t VisionRendering::Core::Renderer::GetInt64(GLenum p_parameter, uint32_t p_index)
{
	GLint64 result;
	glGetInteger64i_v(p_parameter, p_index, &result);
	return static_cast<int64_t>(result);
}

std::string VisionRendering::Core::Renderer::GetString(GLenum p_parameter)
{
	const GLubyte* result = glGetString(p_parameter);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

std::string VisionRendering::Core::Renderer::GetString(GLenum p_parameter, uint32_t p_index)
{
	const GLubyte* result = glGetStringi(p_parameter, p_index);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

void VisionRendering::Core::Renderer::ClearFrameInfo()
{
	m_frameInfo.batchCount		= 0;
	m_frameInfo.instanceCount	= 0;
	m_frameInfo.polyCount		= 0;
}

void VisionRendering::Core::Renderer::Draw(Resources::IMesh& p_mesh, Settings::EPrimitiveMode p_primitiveMode, uint32_t p_instances)
{
	if (p_instances > 0)
	{
		++m_frameInfo.batchCount;
		m_frameInfo.instanceCount += p_instances;
		m_frameInfo.polyCount += (p_mesh.GetIndexCount() / 3) * p_instances;

		p_mesh.Bind();
		
		if (p_mesh.GetIndexCount() > 0)
		{
			/* With EBO */
			if (p_instances == 1)
				glDrawElements(static_cast<GLenum>(p_primitiveMode), p_mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
			else
				glDrawElementsInstanced(static_cast<GLenum>(p_primitiveMode), p_mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, p_instances);
		}
		else
		{
			/* Without EBO */
			if (p_instances == 1)
				glDrawArrays(static_cast<GLenum>(p_primitiveMode), 0, p_mesh.GetVertexCount());
			else
				glDrawArraysInstanced(static_cast<GLenum>(p_primitiveMode), 0, p_mesh.GetVertexCount(), p_instances);
		}

		p_mesh.Unbind();
	}
}

std::vector<std::reference_wrapper<VisionRendering::Resources::Mesh>> VisionRendering::Core::Renderer::GetMeshesInFrustum
(
	const VisionRendering::Resources::Model& p_model,
	const VisionRendering::Geometry::BoundingSphere& p_modelBoundingSphere,
	const VisionMaths::FTransform& p_modelTransform,
	const VisionRendering::Data::Frustum& p_frustum,
	VisionRendering::Settings::ECullingOptions p_cullingOptions
)
{
	const bool frustumPerModel = VisionRendering::Settings::IsFlagSet(Settings::ECullingOptions::FRUSTUM_PER_MODEL, p_cullingOptions);

	if (!frustumPerModel || p_frustum.BoundingSphereInFrustum(p_modelBoundingSphere, p_modelTransform))
	{
		std::vector<std::reference_wrapper<VisionRendering::Resources::Mesh>> result;

		const bool frustumPerMesh = VisionRendering::Settings::IsFlagSet(Settings::ECullingOptions::FRUSTUM_PER_MESH, p_cullingOptions);

		const auto& meshes = p_model.GetMeshes();

		for (auto mesh : meshes)
		{
			// Do not check if the mesh is in frustum if the model has only one mesh, because model and mesh bounding sphere are equals
			if (meshes.size() == 1 || !frustumPerMesh || p_frustum.BoundingSphereInFrustum(mesh->GetBoundingSphere(), p_modelTransform))
			{
				result.emplace_back(*mesh);
			}
		}

		return result;
	}

	return {};
}

uint8_t VisionRendering::Core::Renderer::FetchGLState()
{
	using namespace VisionRendering::Settings;

	uint8_t result = 0;

	GLboolean cMask[4];
	glGetBooleanv(GL_COLOR_WRITEMASK, cMask);

	if (GetBool(GL_DEPTH_WRITEMASK))						result |= 0b0000'0001;
	if (cMask[0])											result |= 0b0000'0010;
	if (GetCapability(ERenderingCapability::BLEND))			result |= 0b0000'0100;
	if (GetCapability(ERenderingCapability::CULL_FACE))		result |= 0b0000'1000;
	if (GetCapability(ERenderingCapability::DEPTH_TEST))	result |= 0b0001'0000;

	switch (static_cast<ECullFace>(GetInt(GL_CULL_FACE)))
	{
	case VisionRendering::Settings::ECullFace::BACK:			result |= 0b0010'0000; break;
	case VisionRendering::Settings::ECullFace::FRONT:			result |= 0b0100'0000; break;
	case VisionRendering::Settings::ECullFace::FRONT_AND_BACK:  result |= 0b0110'0000; break;
	}

	return result;
}

void VisionRendering::Core::Renderer::ApplyStateMask(uint8_t p_mask)
{
	if (p_mask != m_state)
	{
		using namespace VisionRendering::Settings;

		if ((p_mask & 0x01) != (m_state & 0x01))	SetDepthWriting(p_mask & 0x01);
		if ((p_mask & 0x02) != (m_state & 0x02))	SetColorWriting(p_mask & 0x02);
		if ((p_mask & 0x04) != (m_state & 0x04))	SetCapability(ERenderingCapability::BLEND, p_mask & 0x04);
		if ((p_mask & 0x08) != (m_state & 0x08))	SetCapability(ERenderingCapability::CULL_FACE, p_mask & 0x8);
		if ((p_mask & 0x10) != (m_state & 0x10))	SetCapability(ERenderingCapability::DEPTH_TEST, p_mask & 0x10);

		if ((p_mask & 0x08) && ((p_mask & 0x20) != (m_state & 0x20) || (p_mask & 0x40) != (m_state & 0x40)))
		{
			int backBit = p_mask & 0x20;
			int frontBit = p_mask & 0x40;
			SetCullFace(backBit && frontBit ? ECullFace::FRONT_AND_BACK : (backBit ? ECullFace::BACK : ECullFace::FRONT));
		}

		m_state = p_mask;
	}
}

void VisionRendering::Core::Renderer::SetState(uint8_t p_state)
{
	m_state = p_state;
}

const VisionRendering::Core::Renderer::FrameInfo& VisionRendering::Core::Renderer::GetFrameInfo() const
{
	return m_frameInfo;
}
