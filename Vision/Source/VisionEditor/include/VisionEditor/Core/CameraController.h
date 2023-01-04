#pragma once

#include <queue>

#include <VisionWindowing/Inputs/InputManager.h>
#include <VisionWindowing/Window.h>
#include <VisionRendering/LowRenderer/Camera.h>

#include "VisionEditor/Panels/Hierarchy.h"
#include "VisionEditor/Panels/AView.h"

namespace VisionEditor::Core
{
	/**
	* A simple camera controller used to navigate into views
	*/
	class CameraController
	{
	public:
		/**
		* Constructor
		* @param p_view
		* @param p_camera
		* @param p_position
		* @param p_rotation
		* @param p_enableFocusInputs
		*/
		CameraController
		(
			VisionEditor::Panels::AView& p_view,
			VisionRendering::LowRenderer::Camera& p_camera,
			VisionMaths::FVector3& p_position,
			VisionMaths::FQuaternion& p_rotation,
			bool p_enableFocusInputs = false
		);

		/**
		* Handle mouse and keyboard inputs
		* @parma p_deltaTime
		*/
		void HandleInputs(float p_deltaTime);

		/**
		* Asks the camera to move to the target actor
		* @param p_target
		*/
		void MoveToTarget(VisionCore::ECS::Actor& p_target);

		/**
		* Defines the speed of the camera
		* @param p_speed
		*/
		void SetSpeed(float p_speed);

		/**
		* Returns the camera speed
		*/
		float GetSpeed() const;

		/**
		* Defines the position of the camera
		* @param p_position
		*/
		void SetPosition(const VisionMaths::FVector3& p_position);

		/**
		* Defines the rotation of the camera
		* @param p_rotation
		*/
		void SetRotation(const VisionMaths::FQuaternion& p_rotation);

		/**
		* Returns the position of the camera
		*/
		const VisionMaths::FVector3& GetPosition() const;

		/**
		* Returns the position of the camera
		*/
		const VisionMaths::FQuaternion& GetRotation() const;

		/**
		* Returns true if the right mouse click is being pressed
		*/
		bool IsRightMousePressed() const;

	private:
		void HandleCameraPanning(const VisionMaths::FVector2& p_mouseOffset, bool p_firstMouse);
		void HandleCameraOrbit(const VisionMaths::FVector2& p_mouseOffset, bool p_firstMouse);
		void HandleCameraFPSMouse(const VisionMaths::FVector2& p_mouseOffset, bool p_firstMouse);

		void HandleCameraZoom();
		void HandleCameraFPSKeyboard(float p_deltaTime);
		void UpdateMouseState();

	private:
		VisionWindowing::Inputs::InputManager& m_inputManager;
		VisionWindowing::Window& m_window;
		VisionEditor::Panels::AView& m_view;
		VisionRendering::LowRenderer::Camera& m_camera;
		VisionMaths::FVector3& m_cameraPosition;
		VisionMaths::FQuaternion& m_cameraRotation;

		std::queue<std::tuple<VisionMaths::FVector3, VisionMaths::FQuaternion>> m_cameraDestinations;

		bool m_enableFocusInputs;

		bool m_leftMousePressed = false;
		bool m_middleMousePressed = false;
		bool m_rightMousePressed = false;

		VisionMaths::FVector3 m_targetSpeed;
		VisionMaths::FVector3 m_currentMovementSpeed;

		VisionMaths::FTransform* m_orbitTarget = nullptr;
		VisionMaths::FVector3 m_orbitStartOffset;
		bool m_firstMouse = true;
		double m_lastMousePosX = 0.0;
		double m_lastMousePosY = 0.0;
		VisionMaths::FVector3 m_ypr;
		float m_mouseSensitivity = 0.12f;
		float m_cameraDragSpeed = 0.03f;
		float m_cameraOrbitSpeed = 0.5f;
		float m_cameraMoveSpeed = 15.0f;
		float m_focusDistance = 15.0f;
		float m_focusLerpCoefficient = 8.0f;
	};
}