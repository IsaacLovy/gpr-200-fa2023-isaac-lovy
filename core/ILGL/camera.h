#pragma once
#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/mat4.h"

namespace ilgl {
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
		ew::Mat4 ViewMatrix();
		ew::Mat4 ProjectionMatrix();
	};
}