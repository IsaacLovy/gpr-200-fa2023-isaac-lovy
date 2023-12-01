#include "procGen.h"
#include <list>

ilgl::MeshData ilgl::createPlane(float width, float height, int subdivisions)
{
	ilgl::MeshData mesh;

	float xStep = width / (float)subdivisions;
	float zStep = height / (float)subdivisions;

	int columns = subdivisions + 1;

	//ew::Vec3 pos1 = (-width, height, 0.0f);
	//ew::Vec3 pos2 = (-width, -height, 0.0f);
	//ew::Vec3 pos3 = (width, -height, 0.0f);

	//ew::Vec3 edge1 = pos2 - pos1;
	//ew::Vec3 edge2 = pos3 - pos1;

	//ew::Vec2 uv1(0.0f, 1.0f);
	//ew::Vec2 uv2(0.0f, 0.0f);
	//ew::Vec2 uv3(1.0f, 0.0f);

	//ew::Vec2 deltaUV1 = uv2 - uv1;
	//ew::Vec2 deltaUV2 = uv3 - uv1;

	for (int row = 0; row <= subdivisions; row++)
	{
		for (int col = 0; col <= subdivisions; col ++)
		{
			ilgl::Vertex v;
			
			//position
			v.pos.x = col * xStep;
			v.pos.y = 0;
			v.pos.z = -row * zStep;

			//UVs
			v.uv.x = (float)col / (float)subdivisions;
			v.uv.y = (float)row / (float)subdivisions;

			//Normals
			v.normal = ew::Vec3(0, 1, 0);

			ew::Vec3 tangent = ew::Vec3();
			ew::Vec3 bitangent = ew::Vec3();

			bitangent = ew::Normalize(ew::Cross(v.normal, ew::Vec3(1, 0, 0)));
			tangent = ew::Normalize(ew::Cross(bitangent, v.normal));

			v.tangent = tangent;
			v.bitangent = bitangent;

			mesh.vertices.push_back(v);

			//float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			//
			//ew::Vec3 tangent1 = ew::Vec3();
			//ew::Vec3 bitangent1 = ew::Vec3();

			//tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			//tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			//tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			//bitangent1.x = f * (-deltaUV2.y * edge1.x + deltaUV1.y * edge2.x);
			//bitangent1.y = f * (-deltaUV2.y * edge1.y + deltaUV1.y * edge2.y);
			//bitangent1.z = f * (-deltaUV2.y * edge1.z + deltaUV1.y * edge2.z);

			//Indices
			if (row != subdivisions && col != subdivisions) 
			{
				int start = row * columns + col;
				//Bottom right
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);
				
				//Top left
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
				mesh.indices.push_back(start);
			}
		}
	}

	return mesh;
}

ilgl::MeshData ilgl::createTorus(int st, int sl, float innerRadius, float outerRadius)
{
	ilgl::MeshData mesh;

	float phi = 0.0;
	float phiStep = (2 * ew::PI) / (float)sl;

	float theta = 0.0;
	float thetaStep = (2 * ew::PI) / (float)st;

	//Vertices
	for (int stack = 0; stack <= st; stack++)
	{
		theta = thetaStep * stack;

		for (int slice = 0; slice <= sl; slice++)
		{
			phi = phiStep * slice;

			ilgl::Vertex v;
			
			v.pos.x = cos(theta) * (outerRadius + cos(phi) * innerRadius);
			v.pos.y = sin(theta) * (outerRadius + cos(phi) * innerRadius);
			v.pos.z = sin(phi) * innerRadius;

			mesh.vertices.push_back(v);
		}
	}
	
	//Indices
	for (int stack = 0; stack < st; stack++)
	{
		for (int slice = 0; slice <= sl; slice++)
		{
			//int i1 = stack + (slice * sl);
			//int i2 = (stack + 1) + (slice * sl);
			//int i3 = stack + ((slice + 1) * sl);
			//int i4 = (stack + 1) + ((slice + 1) * sl);

			int i1 = stack + (slice * sl);
			int i2 = i1 + (slice * (sl + 1));
			int i3 = 0;
			int i4 = 0;

			mesh.indices.push_back(i1);
			mesh.indices.push_back(i3);
			mesh.indices.push_back(i4);

			mesh.indices.push_back(i1);
			mesh.indices.push_back(i4);
			mesh.indices.push_back(i2);
		}
	}

	return mesh;
}

ilgl::MeshData ilgl::createSphere(float radius, int numSegments)
{
	ilgl::MeshData mesh;

	float thetaStep = 2 * ew::PI / numSegments;
	float phiStep = ew::PI / numSegments;

	for (int row = 0; row <= numSegments; row++)
	{
		float phi = row * phiStep;

		for (int col = 0; col <= numSegments; col++)
		{

			ilgl::Vertex v;

			float theta = col * thetaStep;

			v.pos.x = radius * sin(phi) * cos(theta);
			v.pos.y = radius * cos(phi);
			v.pos.z = radius * sin(phi) * sin(theta);

			v.normal = ew::Normalize(v.pos - ew::Vec3(0, 0, 0));

			v.uv.x = theta / (2 * ew::PI);
			v.uv.y = 1 - (phi / ew::PI);

			mesh.vertices.push_back(v);
		}
	}
	
	//Top Cap
	float poleStart = 0;
	int sidestart = numSegments + 1;
	for (int i = 0; i < numSegments; i++)
	{
		mesh.indices.push_back(sidestart + i);
		mesh.indices.push_back(poleStart + i);
		mesh.indices.push_back(sidestart + i + 1);
	}

	//sides
	int columns = numSegments + 1;
	for (int row = 1; row < numSegments - 1; row++)
	{
		for (int col = 0; col < numSegments; col++)
		{
			int start = row * columns + col;
			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns);

			mesh.indices.push_back(start + columns);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns + 1);

		}
	}

	//bottom cap
	poleStart = mesh.vertices.size() - numSegments - 1;
	sidestart = poleStart - numSegments - 1;
	for (int i = 0; i < numSegments; i++)
	{
		mesh.indices.push_back(sidestart + i);
		mesh.indices.push_back(sidestart + i + 1);
		mesh.indices.push_back(poleStart + i);
	}

	return mesh;
}

ilgl::MeshData ilgl::createCylinder(float height, float radius, int numSegments)
{
	ilgl::MeshData mesh;
	float topY = height / 2;
	float bottomY = -topY;

	ilgl::Vertex topVertex;
	topVertex.pos = ew::Vec3(0, topY, 0);
	topVertex.uv = ew::Vec2(0.5f, 0.5f);
	topVertex.normal = ew::Vec3(0, 1, 0);

	mesh.vertices.push_back(topVertex);

	int topUpStart = createCylinderRing(height, topY, radius, numSegments, &mesh, true, ew::Vec3(0, 1, 0));
	int topSideStart = createCylinderRing(height, topY, radius, numSegments, &mesh, false);
	int bottomSideStart = createCylinderRing(height, bottomY, radius, numSegments, &mesh, false);
	int bottomDownStart = createCylinderRing(height, bottomY, radius, numSegments, &mesh, true, ew::Vec3(0, -1, 0));

	ilgl::Vertex bottomVertex;
	bottomVertex.pos = ew::Vec3(0, bottomY, 0);
	bottomVertex.uv = ew::Vec2(0.5f, 0.5f);
	bottomVertex.normal = ew::Vec3(0, -1, 0);

	mesh.vertices.push_back(bottomVertex);
	
	int columns = numSegments + 1;
	for (int i = 0; i < columns; i++)
	{
		int start = topSideStart + i;
		mesh.indices.push_back(start);
		mesh.indices.push_back(start + 1);
		mesh.indices.push_back(start + columns);

		mesh.indices.push_back(start + columns);
		mesh.indices.push_back(start + 1);
		mesh.indices.push_back(start + columns+  1);
	}

	//For the top cap
	for (int i = 0; i <= numSegments; i++)
	{
		mesh.indices.push_back(topUpStart + i);
		mesh.indices.push_back(0);
		mesh.indices.push_back(topUpStart + i + 1);
	}

	//For the bottom cap (needs to spin the other way)
	for (int i = 0; i <= numSegments; i++)
	{
		mesh.indices.push_back(bottomDownStart + i);
		mesh.indices.push_back(bottomDownStart + i + 1);
		mesh.indices.push_back(mesh.vertices.size() - 1);
	}

	return mesh;
}

int ilgl::createCylinderRing(float height, float yPos, float radius, int numSegments, ilgl::MeshData* mesh, bool oneNormal, ew::Vec3 normalDir)
{
	int startedAt = mesh->vertices.size();
	float thetaStep = 2 * ew::PI / numSegments;
	for (int i = 0; i <= numSegments; i++)
	{
		ilgl::Vertex v;

		float theta = i * thetaStep;
		v.pos.x = cos(theta) * radius;
		v.pos.y = yPos;
		v.pos.z = sin(theta) * radius;

		if (oneNormal)
		{
			v.normal = normalDir;
			v.uv.x = cos(theta) * 0.5 + 0.5;
			v.uv.y = sin(theta) * 0.5 + 0.5;
		}
		else
		{
			v.normal = ew::Normalize(v.pos - ew::Vec3(0, yPos, 0));
			v.uv.x = (float)i / (float)numSegments;
			v.uv.y = (yPos) / height;
		}

		mesh->vertices.push_back(v);
	}

	return startedAt;
}