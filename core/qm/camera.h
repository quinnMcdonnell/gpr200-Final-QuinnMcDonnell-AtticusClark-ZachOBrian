#pragma once
#include "transformations.h"
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h" 

namespace qm {
	struct Camera {
		ew::Vec3 position; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum
		inline ew::Mat4 ViewMatrix() //World->View
		{
			return LookAt(position, target, ew::Vec3(0, 1, 0));
		}

		inline ew::Mat4 ProjectionMatrix() //View->Clip
		{
			if (orthographic)
			{
				return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else
			{
				return Perspective(fov, aspectRatio, nearPlane, farPlane);
			}
		}
	};

	struct CameraControls {
		double prevMouseX, prevMouseY; //Mouse position from previous frame
		float yaw = 0, pitch = 0; //Degrees
		float mouseSensitivity = 0.1f; //How fast to turn with mouse
		bool firstMouse = true; //Flag to store initial mouse position
		float moveSpeed = 5.0f; //How fast to move with arrow keys (M/S)
	};

}