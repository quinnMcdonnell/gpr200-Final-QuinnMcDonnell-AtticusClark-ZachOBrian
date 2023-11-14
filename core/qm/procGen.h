#pragma once
#include "../ew/ewMath/ewMath.h"
#include "../ew/mesh.h"

namespace qm 
{
	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData data;
		ew::Vertex v;

		//Verices
		float thetaStep = 6.28f / numSegments;
		float phiStep = 3.14f / numSegments;

		for (int row = 0; row <= numSegments; row++)
		{
			float phi = row * phiStep;

			for (int col = 0; col <= numSegments; col++)
			{
				float theta = col * thetaStep;
				v.pos.x = radius * cos(theta) * sin(phi);
				v.pos.y = radius * cos(phi);
				v.pos.z = radius * sin(theta) * sin(phi);

				//UV and Normal here
				v.normal = ew::Vec3(ew::Normalize(v.pos));
				v.uv = ew::Vec2((float)(col) / (float)numSegments, (float)(numSegments - row) / (float)numSegments);

				data.vertices.push_back(v);
			}
		}

		//Indices
		int poleStart = 0;
		int sideStart = numSegments + 1;

		for (int i = 0; i <= numSegments; i++)
		{
			data.indices.push_back(sideStart + i);
			data.indices.push_back(poleStart + i);
			data.indices.push_back(sideStart + i + 1);
		}

		int columns = numSegments + 1;

		for (int row = 0; row <= numSegments - 1; row++)
		{
			for (int col = 0; col <= numSegments; col++)
			{
				int start = row * columns + col;

				data.indices.push_back(start);
				data.indices.push_back(start + 1);
				data.indices.push_back(start + columns);

				data.indices.push_back(start + columns + 1);
				data.indices.push_back(start + columns);
				data.indices.push_back(start + 1);

			}
		}

		return data;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData data;
		ew::Vertex v;

		float topY = height / 2;
		float bottomY = -topY;

		v.pos = ew::Vec3(0, topY, 0);
		v.normal = ew::Vec3(0, 1, 0);
		v.uv = ew::Vec2(0.5f, 0.5f);

		data.vertices.push_back(v);

		float thetaStep = 6.28f / (float)numSegments; //see how to get pi
		
		for (int dupe = 0; dupe < 2; dupe++) {
			for (int i = 0; i <= numSegments; i++)
			{
				float theta = i * thetaStep;
				v.pos.x = cos(theta) * radius;
				v.pos.z = sin(theta) * radius;
				v.pos.y = topY;

				if (dupe == 0)
				{
					v.normal = ew::Vec3(0, 1, 0);
					v.uv = ew::Vec2(cos(theta) / 2 + 0.5f, sin(theta) / 2 + 0.5f);
				}
				else if (dupe == 1)
				{
					v.normal = ew::Vec3(cos(theta),0,sin(theta));
					v.uv = ew::Vec2(((float)i / (float)numSegments), 1);
				}

				data.vertices.push_back(v);
			}
		}
		for (int dupe = 0; dupe < 2; dupe++)
		{
			for (int i = 0; i <= numSegments; i++)
			{
				float theta = i * thetaStep;
				v.pos.x = cos(theta) * radius;
				v.pos.z = sin(theta) * radius;
				v.pos.y = bottomY;

				if (dupe == 0)
				{
					v.normal = ew::Vec3(cos(theta), 0, sin(theta));
					v.uv = ew::Vec2(((float)i / (float)numSegments), 0);
				}
				else if (dupe == 1)
				{
					v.normal = ew::Vec3(0, -1, 0);
					v.uv = ew::Vec2(cos(theta)/2 + 0.5f, sin(theta)/2 + 0.5f);
				}

				data.vertices.push_back(v);
			}
		}
		
		v.pos = ew::Vec3(0, bottomY, 0);
		v.normal = ew::Vec3(0, -1, 0);
		v.uv = ew::Vec2(0.5f, 0.5f);
		data.vertices.push_back(v);


		//Indices are done here
		int start = 1;
		int center = 0;

		for (int i = 0; i < numSegments; i++)
		{
			data.indices.push_back(start + i);
			data.indices.push_back(center);
			data.indices.push_back(start + i + 1);
		}

		start = data.vertices.size()-(numSegments+2);
		center = data.vertices.size()-1;

		for (int i = 0; i < numSegments; i++)
		{
			data.indices.push_back(start + i + 1);
			data.indices.push_back(center);
			data.indices.push_back(start + i);
		}
		
		int sideStart = numSegments + 2;
		int columns = numSegments + 1;

		for (int j = 0; j < numSegments; j++)
		{
			int start = sideStart + j;
			data.indices.push_back(start);
			data.indices.push_back(start + 1);
			data.indices.push_back(start + columns);

			data.indices.push_back(start + columns + 1);
			data.indices.push_back(start + columns);
			data.indices.push_back(start + 1);
		}

		return data;
	}

	ew::MeshData createPlane(float size, int subdivisions)
	{
		ew::MeshData data;
		ew::Vertex v;

		for (float row = 0; row <= subdivisions; row++)
		{
			for (float col = 0; col <= subdivisions; col++)
			{
				v.pos.x = size * (col / subdivisions);
				v.pos.y = 0;
				v.pos.z = -size * (row / subdivisions);

				v.normal = ew::Vec3(0, 1, 0);
				v.uv = ew::Vec2(row/subdivisions, col/subdivisions);
				data.vertices.push_back(v);
			}
		}

		int columns = subdivisions + 1;
		for (int row = 0; row < subdivisions; row++)
		{
			for (int col = 0; col < subdivisions; col++)
			{
				int start = row * columns + col;
				data.indices.push_back(start);
				data.indices.push_back(start + 1);
				data.indices.push_back(start + columns + 1);

				data.indices.push_back(start);
				data.indices.push_back(start + columns + 1);
				data.indices.push_back(start + columns);
			}
		}

		return data;
	}
}
