#pragma once

#include "VisionRendering/Core/Renderer.h"
#include "VisionRendering/Resources/Mesh.h"

namespace VisionRendering::Core
{
	/**
	* The ShapeDrawer handles the drawing of basic shapes
	*/
	class ShapeDrawer
	{
	public:
		/**
		* Creates the ShapeDrawer instance. It needs a renderer
		* @param p_renderer
		*/
		ShapeDrawer(VisionRendering::Core::Renderer& p_renderer);

		/**
		* Destructor
		*/
		~ShapeDrawer();

		/**
		* Defines the view projection to use when drawing
		* @param p_viewProjection
		*/
		void SetViewProjection(const VisionMaths::FMatrix4& p_viewProjection);

		/**
		* Draw a line in world space
		* @param p_start
		* @param p_end
		* @param p_color
		* @param p_lineWidth
		*/
		void DrawLine(const VisionMaths::FVector3& p_start, const VisionMaths::FVector3& p_end, const VisionMaths::FVector3& p_color, float p_lineWidth = 1.0f);

		/**
		* Draw a grid in world space
		* @param p_viewPos
		* @param p_color
		* @param p_gridSize
		* @param p_linear
		* @param p_quadratic
		* @param p_fadeThreshold
		* @param p_lineWidth
		*/
		void DrawGrid(const VisionMaths::FVector3& p_viewPos, const VisionMaths::FVector3& p_color, int32_t p_gridSize = 50, float p_linear = 0.0f, float p_quadratic = 0.0f, float p_fadeThreshold = 0.0f, float p_lineWidth = 1.0f);

	private:
		VisionRendering::Resources::Shader* m_lineShader = nullptr;
		VisionRendering::Resources::Shader* m_gridShader = nullptr;
		VisionRendering::Resources::Mesh* m_lineMesh = nullptr;
		VisionRendering::Core::Renderer& m_renderer;
	};
}