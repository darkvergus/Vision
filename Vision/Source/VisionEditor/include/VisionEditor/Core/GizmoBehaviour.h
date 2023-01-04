#pragma once

#include <VisionMaths/FTransform.h>
#include <VisionMaths/FVector2.h>

#include <VisionCore/ECS/Actor.h>

namespace VisionEditor::Core
{
	enum class EGizmoOperation
	{
		TRANSLATE,
		ROTATE,
		SCALE
	};

	/* Handle gizmo behaviours */
	class GizmoBehaviour
	{
	public:
		enum class EDirection
		{
			X,
			Y,
			Z
		};
		
		/**
		* Returns true if the snapping behaviour is enabled
		*/
		bool IsSnappedBehaviourEnabled() const;

		/**
		* Starts the gizmo picking behaviour for the given target in the given direction
		* @param p_actor
		* @param p_cameraPosition
		* @param p_operation
		* @param p_direction
		*/
		void StartPicking(VisionCore::ECS::Actor& p_target, const VisionMaths::FVector3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction);

		/**
		* Stops the gizmo picking behaviour
		*/
		void StopPicking();

		/**
		* Handle the current behaviour
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		void ApplyOperation(const VisionMaths::FMatrix4& p_viewMatrix, const VisionMaths::FMatrix4& p_projectionMatrix, const VisionMaths::FVector2& p_viewSize);

		/**
		* Set the given mouse position as the current mouse position and update the previous mouse position
		* @param p_mousePosition
		*/
		void SetCurrentMouse(const VisionMaths::FVector2& p_mousePosition);

		/**
		* Returns true if the gizmo is currently picked
		*/
		bool IsPicking() const;

		/**
		* Returns the current operation direction
		*/
		EDirection GetDirection() const;

	private:
		/**
		* Returns the global direction matching with the current m_direction
		*/
		VisionMaths::FVector3 GetFakeDirection() const;

		/**
		* Returns the actual direction of the target matching with the current m_direction
		* @param p_relative (If true, the direction depends on hierarchy)
		*/
		VisionMaths::FVector3 GetRealDirection(bool p_relative = false) const;

		/**
		* Returns the 3D vector of the arrow projected to the screen
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		VisionMaths::FVector2 GetScreenDirection(const VisionMaths::FMatrix4& p_viewMatrix, const VisionMaths::FMatrix4& p_projectionMatrix, const VisionMaths::FVector2& p_viewSize) const;

		/**
		* Handle the translation behaviour
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		void ApplyTranslation(const VisionMaths::FMatrix4& p_viewMatrix, const VisionMaths::FMatrix4& p_projectionMatrix, const VisionMaths::FVector2& p_viewSize) const;

		/**
		* Handle the rotation behaviour
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		void ApplyRotation(const VisionMaths::FMatrix4& p_viewMatrix, const VisionMaths::FMatrix4& p_projectionMatrix, const VisionMaths::FVector2& p_viewSize) const;

		/**
		* Handle the scale behaviour
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		void ApplyScale(const VisionMaths::FMatrix4& p_viewMatrix, const VisionMaths::FMatrix4& p_projectionMatrix, const VisionMaths::FVector2& p_viewSize) const;

	private:
		bool m_firstMouse = true;
		float m_distanceToActor = 0.0f;
		VisionCore::ECS::Actor* m_target = nullptr;
		EGizmoOperation m_currentOperation;
		EDirection m_direction;
		VisionMaths::FTransform m_originalTransform;
		VisionMaths::FVector2 m_originMouse;
		VisionMaths::FVector2 m_currentMouse;
		VisionMaths::FVector2 m_screenDirection;
	};
}
