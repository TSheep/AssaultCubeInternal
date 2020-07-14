#pragma once

#include "vec3.hpp"
namespace ac
{
	struct CMatrix
	{
		union
		{
			struct
			{
				float m11, m12, m13, m14; //00, 01, 02, 03
				float m21, m22, m23, m24; //04, 05, 06, 07
				float m31, m32, m33, m34; //08, 09, 10, 11
				float m41, m42, m43, m44; //12, 13, 14, 15
			};
			float matrix[4][4];
		};
	};


	bool world_to_screen(Vector3 world_pos, Vector3& screen_pos);
}