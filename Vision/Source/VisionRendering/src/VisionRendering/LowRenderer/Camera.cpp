#include <cmath>

#include "VisionRendering/LowRenderer/Camera.h"
#include "VisionMaths/FMatrix4.h"

VisionRendering::LowRenderer::Camera::Camera() :
    m_projectionMode(Settings::EProjectionMode::PERSPECTIVE),
	m_fov(45.0f),
    m_size(5.0f),
	m_near(0.1f),
	m_far(100.f),
	m_clearColor(0.f, 0.f, 0.f),
	m_frustumGeometryCulling(false),
	m_frustumLightCulling(false)
{
}

void VisionRendering::LowRenderer::Camera::CacheMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const VisionMaths::FVector3& p_position, const VisionMaths::FQuaternion& p_rotation)
{
	CacheProjectionMatrix(p_windowWidth, p_windowHeight);
	CacheViewMatrix(p_position, p_rotation);
	CacheFrustum(m_viewMatrix, m_projectionMatrix);
}

void VisionRendering::LowRenderer::Camera::CacheProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight)
{
	m_projectionMatrix = CalculateProjectionMatrix(p_windowWidth, p_windowHeight);
}

void VisionRendering::LowRenderer::Camera::CacheViewMatrix(const VisionMaths::FVector3& p_position, const VisionMaths::FQuaternion& p_rotation)
{
	m_viewMatrix = CalculateViewMatrix(p_position, p_rotation);
}

void VisionRendering::LowRenderer::Camera::CacheFrustum(const VisionMaths::FMatrix4& p_view, const VisionMaths::FMatrix4& p_projection)
{
	m_frustum.CalculateFrustum(p_projection * p_view);
}

float VisionRendering::LowRenderer::Camera::GetFov() const
{
	return m_fov;
}

float VisionRendering::LowRenderer::Camera::GetSize() const
{
    return m_size;
}

float VisionRendering::LowRenderer::Camera::GetNear() const
{
	return m_near;
}

float VisionRendering::LowRenderer::Camera::GetFar() const
{
	return m_far;
}

const VisionMaths::FVector3 & VisionRendering::LowRenderer::Camera::GetClearColor() const
{
	return m_clearColor;
}

const VisionMaths::FMatrix4& VisionRendering::LowRenderer::Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

const VisionMaths::FMatrix4& VisionRendering::LowRenderer::Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const VisionRendering::Data::Frustum& VisionRendering::LowRenderer::Camera::GetFrustum() const
{
	return m_frustum;
}

bool VisionRendering::LowRenderer::Camera::HasFrustumGeometryCulling() const
{
	return m_frustumGeometryCulling;
}

bool VisionRendering::LowRenderer::Camera::HasFrustumLightCulling() const
{
	return m_frustumLightCulling;
}

VisionRendering::Settings::EProjectionMode VisionRendering::LowRenderer::Camera::GetProjectionMode() const
{
    return m_projectionMode;
}

void VisionRendering::LowRenderer::Camera::SetFov(float p_value)
{
	m_fov = p_value;
}

void VisionRendering::LowRenderer::Camera::SetSize(float p_value)
{
    m_size = p_value;
}

void VisionRendering::LowRenderer::Camera::SetNear(float p_value)
{
	m_near = p_value;
}

void VisionRendering::LowRenderer::Camera::SetFar(float p_value)
{
	m_far = p_value;
}

void VisionRendering::LowRenderer::Camera::SetClearColor(const VisionMaths::FVector3 & p_clearColor)
{
	m_clearColor = p_clearColor;
}

void VisionRendering::LowRenderer::Camera::SetFrustumGeometryCulling(bool p_enable)
{
	m_frustumGeometryCulling = p_enable;
}

void VisionRendering::LowRenderer::Camera::SetFrustumLightCulling(bool p_enable)
{
	m_frustumLightCulling = p_enable;
}

void VisionRendering::LowRenderer::Camera::SetProjectionMode(VisionRendering::Settings::EProjectionMode p_projectionMode)
{
    m_projectionMode = p_projectionMode;
}

VisionMaths::FMatrix4 VisionRendering::LowRenderer::Camera::CalculateProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight) const
{
    using namespace VisionMaths;
    using namespace VisionRendering::Settings;

    const auto ratio = p_windowWidth / static_cast<float>(p_windowHeight);

    switch (m_projectionMode)
    {
    case EProjectionMode::ORTHOGRAPHIC:
        return FMatrix4::CreateOrthographic(m_size, ratio, m_near, m_far);

    case EProjectionMode::PERSPECTIVE: 
        return FMatrix4::CreatePerspective(m_fov, ratio, m_near, m_far);

    default:
        return FMatrix4::Identity;
    }
}

VisionMaths::FMatrix4 VisionRendering::LowRenderer::Camera::CalculateViewMatrix(const VisionMaths::FVector3& p_position, const VisionMaths::FQuaternion& p_rotation) const
{
	const auto& up = p_rotation * VisionMaths::FVector3::Up;
	const auto& forward = p_rotation * VisionMaths::FVector3::Forward;

	return VisionMaths::FMatrix4::CreateView
	(
		p_position.x, p_position.y, p_position.z,												// Position
		p_position.x + forward.x, p_position.y + forward.y, p_position.z + forward.z,			// LookAt (Position + Forward)
		up.x, up.y, up.z																		// Up Vector
	);
}