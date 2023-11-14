#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"

namespace qm
{
	inline ew::Mat4 Identity()
	{
		return ew::Mat4(
			1, 0, 0, 0, //column
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	
	inline ew::Mat4 Scale(ew::Vec3 s) 
	{ 
		ew::Mat4 n = Identity();
		n[0][0] = s.x;
		n[1][1] = s.y;
		n[2][2] = s.z;

		return n;

	};

	inline ew::Mat4 RotateX(float rad) 
	{ 
		ew::Mat4 pitch = ew::Mat4(
			1, 0, 0, 0, //Column
			0, cos(rad), -sin(rad), 0,
			0, sin(rad), cos(rad), 0,
			0, 0, 0, 1
		);

		return pitch;
	};

	inline ew::Mat4 RotateY(float rad) 
	{ 
		ew::Mat4 yaw = ew::Mat4(
			cos(rad), 0, sin(rad), 0,
			0, 1, 0, 0,
			-sin(rad), 0, cos(rad), 0,
			0, 0, 0, 1
		);

		return yaw;
	};

	inline ew::Mat4 RotateZ(float rad) 
	{ 
		ew::Mat4 roll = ew::Mat4(
			cos(rad), -sin(rad), 0, 0,
			sin(rad), cos(rad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		return roll;
	};

	inline ew::Mat4 Translate(ew::Vec3 t) 
	{ 
		ew::Mat4 n = Identity();
		n[3][0] = t.x;
		n[3][1] = t.y;
		n[3][2] = t.z;

		return n;
	};


	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up) 
	{
		ew::Vec3 f = ew::Normalize(eye - target);
		ew::Vec3 r = ew::Normalize(ew::Cross(up,f));
		ew::Vec3 u = ew::Normalize(ew::Cross(f,r));

		return ew::Mat4(
			r.x, r.y, r.z, -(ew::Dot(r, eye)),
			u.x, u.y, u.z, -(ew::Dot(u,eye)),
			f.x, f.y, f.z, -(ew::Dot(f,eye)),
			0, 0, 0, 1
		);
	};

	//Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) 
	{
		float width = height * aspect;
		float r = width / 2;
		float t = height / 2;
		float l = -r;
		float b = -t;

		return ew::Mat4(
			2 / (r - l), 0, 0, -((r + l) / (r - l)),
			0, 2 / (t - b), 0, -((t + b) / (t - b)),
			0, 0, -(2 / (far - near)), -((far + near) / (far - near)),
			0, 0, 0, 1
		);
	};

	//Perspective projection
	//fov = vertical aspect ratio (radians)
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) 
	{
		fov = ew::Radians(fov);
		return ew::Mat4(
			1/(tan(fov*0.5f) * aspect), 0, 0, 0,
			0, 1/(tan(fov*0.5f)), 0, 0,
			0, 0, (near + far)/(near - far), (2 * near * far) / (near - far),
			0, 0, -1, 0
		);
	};

	//Identity Matrix

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); //Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		ew::Mat4 getModelMatrix() const
		{
			ew::Mat4 modelMatrix = qm::Translate(position)* RotateX(rotation.x)* RotateY(rotation.y)* RotateZ(rotation.z)* qm::Scale(scale);
			return modelMatrix;
		}
	};
}