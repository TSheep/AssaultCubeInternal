#include "assault_cube.hpp"
#include "globals.hpp"
#include "MemoryPatch.hpp"

namespace ac
{
	CMatrix* view_matrix = (CMatrix*)(0x00501AE8);


	bool world_to_screen(Vector3 world_pos, Vector3& screen_pos)
	{
		float screenX = (view_matrix->m11 * world_pos.x) + (view_matrix->m21 * world_pos.y) + (view_matrix->m31 * world_pos.z) + view_matrix->m41;
		float screenY = (view_matrix->m12 * world_pos.x) + (view_matrix->m22 * world_pos.y) + (view_matrix->m32 * world_pos.z) + view_matrix->m42;
		float screenW = (view_matrix->m14 * world_pos.x) + (view_matrix->m24 * world_pos.y) + (view_matrix->m34 * world_pos.z) + view_matrix->m44;

		float camX = g_globals.window_width / 2.0f;
		float camY = g_globals.window_height / 2.0f;

		//convert to homogeneous position
		screen_pos.x = camX + (camX * screenX / screenW);
		screen_pos.y = camY - (camY * screenY / screenW);

		//check if object is behind camera / off screen (not visible)
		//w = z where z is relative to the camera 
		return (screenW > 0.001f);
	}
}