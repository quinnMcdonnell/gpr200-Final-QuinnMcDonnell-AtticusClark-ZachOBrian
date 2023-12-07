// orbit functions by Atticus Clark
// referenced: https://www.mbsoftworks.sk/tutorials/opengl4/026-camera-pt3-orbit-camera/

#include <ew/camera.h>

namespace akc {
	struct Orbit {
		float yaw = 0.0f, pitch = 0.0f;
		float addYaw = 0.0f, addPitch = 0.0f;
		float pitchDir = 1.0f;
		float radius = 5.0f;

		void cameraOrbit(ew::Camera& camera, const float deltaTime) {

			yaw += addYaw * deltaTime;
			if(yaw >= 360.0f) { yaw -= 360.0f; }
			else if(yaw < 0.0f) { yaw += 360.0f; }

			// + or - addPitch to create bobbing effect
			if(pitch <= -89.0f) { pitchDir = -1.0f; }
			else if(pitch >= 89.0f) { pitchDir = 1.0f; }

			pitch -= addPitch * deltaTime * pitchDir;
			pitch = ew::Clamp(pitch, -89.0f, 89.0f);

			float yawRad = ew::Radians(yaw);
			float pitchRad = ew::Radians(pitch);

			camera.position.x = camera.target.x - (radius * cosf(pitchRad) * sinf(yawRad));
			camera.position.y = camera.target.y - (radius * sinf(pitchRad));
			camera.position.z = camera.target.z - (radius * cosf(pitchRad) * -cos(yawRad));
		}

		void resetSettings(float radiusReset) {
			yaw = 0.0f;
			pitch = 0.0f;
			addYaw = 0.0f;
			addPitch = 0.0f;
			pitchDir = 1.0f;
			radius = radiusReset;
		}
	};
}