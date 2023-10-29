#pragma once
#include "../ew/mesh.h"

namespace ilgl {
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	int createCylinderRing(float height, float yPos, float radius, int numSegments, ew::MeshData* mesh, bool oneNormal, ew::Vec3 normalDir = ew::Vec3(0, 1, 0));
	ew::MeshData createPlane(float width, float height, int subdivisions);
}