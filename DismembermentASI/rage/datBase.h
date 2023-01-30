#pragma once

namespace rage
{
	class datBase //0x0-0x8
	{
	public:
		virtual ~datBase() {}
	}; static_assert(sizeof(datBase) == 0x8, "datBase is of wrong size");

	typedef __m128 CVector; //CVector for SIMD vector

	struct Matrix34
	{
		CVector right;
		CVector forward;
		CVector up;
	};

	struct Matrix44
	{
		CVector right;
		CVector forward;
		CVector up;
		CVector pos;
	};

	// normal vector
	template<typename T>
	union vector3
	{
		T data[3];
		struct { T x, y, z; };
	};
	typedef vector3<float> fvector3;

	template<typename T>
	union vector4
	{
		T data[4];
		struct { T x, y, z, w; };
	};
	typedef vector4<float> fvector4;

	template<typename T>
	union matrix44
	{
		T data[4][4];
		struct { struct { T x, y, z, w; } rows[4]; };
	};
	typedef matrix44<float> fmatrix44;
}
