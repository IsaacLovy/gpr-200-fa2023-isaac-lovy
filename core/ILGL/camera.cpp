#include "camera.h"
#include "../ILGL/transformations.h"

ew::Mat4 ilgl::Camera::ViewMatrix()
{
	return ilgl::LookAt(position, target, ew::Vec3(0, 1, 0));
}

ew::Mat4 ilgl::Camera::ProjectionMatrix()
{
	if (orthographic)
	{
		return ilgl::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
	}

	return ilgl::Perspective(fov, aspectRatio, nearPlane, farPlane);
}
