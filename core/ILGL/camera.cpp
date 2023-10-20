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

	float radianFOV = fov * ew::DEG2RAD;
	return ilgl::Perspective(radianFOV, aspectRatio, nearPlane, farPlane);
}

ew::Vec3 ilgl::Camera::getCameraForward(float pitch, float yaw)
{
	return ew::Vec3(sin(yaw) * cos(pitch), sin(pitch), -cos(yaw) * cos(pitch));
}

void ilgl::Camera::moveCamera(GLFWwindow* window, float deltaTime)
{
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		//release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls.firstMouse = true;
		return;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (controls.firstMouse)
	{
		controls.firstMouse = false;
		controls.prevMouseX = mouseX;
		controls.prevMouseY = mouseY;
	}

	controls.yaw += (mouseX - controls.prevMouseX) * controls.sensitivity * deltaTime;
	controls.pitch -= (mouseY - controls.prevMouseY) * controls.sensitivity * deltaTime;
	controls.pitch = clamp(controls.pitch, -89, 89);

	controls.prevMouseX = mouseX;
	controls.prevMouseY = mouseY;
	ew::Vec3 forward = getCameraForward(controls.pitch * ew::DEG2RAD, controls.yaw * ew::DEG2RAD);
	target = position + forward;

	ew::Vec3 f = ew::Normalize(forward);
	ew::Vec3 r = ew::Normalize(ew::Cross(f, ew::Vec3(0, 1, 0)));
	ew::Vec3 u = ew::Normalize(ew::Cross(r, f));

	if (glfwGetKey(window, GLFW_KEY_W))
	{
		position += f * controls.moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		position -= f * controls.moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		position += r * controls.moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		position -= r * controls.moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		position += u * controls.moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		position -= u * controls.moveSpeed * deltaTime;
	}

	target = position + forward;
}
