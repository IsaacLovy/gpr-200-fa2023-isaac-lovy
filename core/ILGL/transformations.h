#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include <math.h>

namespace ilgl
{
	inline ew::Mat4 Identity() 
	{
		return ew::Mat4
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	//scale xyz
	inline ew::Mat4 Scale(ew::Vec3 s)
	{
		return ew::Mat4
		(
			s.x,  0,   0,  0,
			 0,  s.y,  0,  0,
			 0,   0,  s.z, 0,
			 0,   0,   0,  1
		);
	}
	//rotate along X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad)
	{
		return ew::Mat4
		(
			1, 0, 0, 0,
			0, cos(rad), -sin(rad), 0,
			0, sin(rad), cos(rad), 0,
			0, 0, 0, 1
		);
	}
	//rotate along Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad)
	{
		return ew::Mat4
		(
			cos(rad), 0, sin(rad), 0,
			0, 1, 0, 0,
			-sin(rad), 0, cos(rad), 0,
			0, 0, 0, 1
		);
	}
	//rotate along Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad)
	{
		return ew::Mat4
		(
			cos(rad), -sin(rad), 0, 0,
			sin(rad), cos(rad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
	
	//do the othe rotate funtions in ZXY order in radians
	inline ew::Mat4 RotateZXY(ew::Vec3 r)
	{
		return RotateY(r.y) * RotateX(r.x) * RotateZ(r.z);
	}

	//translate xyz
	inline ew::Mat4 Translate(ew::Vec3 t)
	{
		return ew::Mat4
		(
			1, 0, 0, t.x,
			0, 1, 0, t.y,
			0, 0, 1, t.z,
			0, 0, 0,  1
		);
	}

	struct Transform 
	{
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);

		ew::Mat4 getModelMatrix() const
		{
			return Translate(position) * RotateZXY(rotation) * Scale(scale);
		}
	};
}