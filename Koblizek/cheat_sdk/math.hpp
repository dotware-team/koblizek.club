#pragma once
#include <cassert>

#include "Vector.h"
#include <Windows.h>

int distance(Vector a, Vector b) {
	double distance;

	distance = sqrt(((int)a.x - (int)b.x) * ((int)a.x - (int)b.x) +
		((int)a.y - (int)b.y) * ((int)a.y - (int)b.y) +
		((int)a.z - (int)b.z) * ((int)a.z - (int)b.z));

	return (int)abs(round(distance));
}

Vector CalcAngle(Vector src, Vector dst)
{
	Vector angles;
	Vector delta = src - dst;

	if (delta[1] == 0.0f && delta[0] == 0.0f)
	{
		angles[0] = (delta[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = atan2(-delta[2], delta.length()) * -180 / M_PI;
		angles[1] = atan2(delta[1], delta[0]) * 180 / M_PI;

		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}

	angles[2] = 0.0f;
	angles.normalize();

	return angles;
}

inline void Clamp(float& value, float min, float max)
{
	if (value < min) value = min;
	if (value > max) value = max;
}

void ClampAngle(Vector& angle)
{
	while (angle.x < -89.0f)
		angle.x = -89.0f;
	while (angle.x > 89.0f)
		angle.x = 89.0f;

	while (angle.y < -180.0f)
		angle.y += 360.0f;
	while (angle.y > 180.0f)
		angle.y -= 360.0f;

	angle.z = 0;
}

template<class T>
void Normalize3(T& vec) {
	for (auto i = 0; i < 2; i++) {
		while (vec[i] < -180.0f) vec[i] += 360.0f;
		while (vec[i] > 180.0f) vec[i] -= 360.0f;
	}
	vec[2] = 0.f;
}

void FixAngles(Vector& angles) {
	Normalize3(angles);
	ClampAngle(angles);
}