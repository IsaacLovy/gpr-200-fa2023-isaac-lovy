#pragma once
#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/mat4.h"
#include <GLFW/glfw3.h>

namespace ilgl {
	struct CameraControls
	{
		double prevMouseX, prevMouseY;
		float yaw = 0, pitch = 0;
		bool firstMouse = true;
		float moveSpeed = 2;
		float sensitivity = 4;
	};

	struct Camera
	{
		ew::Vec3 position;
		ew::Vec3 target;
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;
		bool orthographic;
		float orthoSize;
		CameraControls controls;

		ew::Mat4 ViewMatrix();
		ew::Mat4 ProjectionMatrix();
		ew::Vec3 getCameraForward(float pitch, float yaw);
		void moveCamera(GLFWwindow* window, float deltaTime);
	};

}
