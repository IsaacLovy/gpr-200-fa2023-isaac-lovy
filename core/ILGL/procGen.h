#pragma once
#include "mesh.h"

namespace ilgl {
	ilgl::MeshData createSphere(float radius, int numSegments);
	ilgl::MeshData createCylinder(float height, float radius, int numSegments);
	int createCylinderRing(float height, float yPos, float radius, int numSegments, ilgl::MeshData* mesh, bool oneNormal, ew::Vec3 normalDir = ew::Vec3(0, 1, 0));
	ilgl::MeshData createPlane(float width, float height, int subdivisions);

	ilgl::MeshData createTorus(int rings, int segments, float innerRadius, float outterRadius);
}