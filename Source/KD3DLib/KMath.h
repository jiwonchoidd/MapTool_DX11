#pragma once
#include <d3d11_1.h> 
//#include <d3dx11.h> 
#if !defined(__d3d11_h__) && !defined(__d3d11_x_h__) && !defined(__d3d12_h__) && !defined(__d3d12_x_h__)
#error include d3d11.h or d3d12.h before including TMath.h
#endif

#if !defined(_XBOX_ONE) || !defined(_TITLE)
//#include <dxgi1_2.h>
#endif

#include <functional>
#include <assert.h>
#include <memory.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

#ifndef XM_CONSTEXPR
#define XM_CONSTEXPR
#endif

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace TBASIS_EX
{
	struct KVector2;
	struct KVector3;
	struct KVector4;
	struct KMatrix;
	struct KQuaternion;
	struct KPlane;

	//------------------------------------------------------------------------------
	// 2D rectangle
	struct KRectangle
	{
		long x;
		long y;
		long width;
		long height;

		// Creators
		KRectangle() noexcept : x(0), y(0), width(0), height(0) {}
		XM_CONSTEXPR KRectangle(long ix, long iy, long iw, long ih) : x(ix), y(iy), width(iw), height(ih) {}
		explicit KRectangle(const RECT& rct) : x(rct.left), y(rct.top), width(rct.right - rct.left), height(rct.bottom - rct.top) {}

		KRectangle(const KRectangle&) = default;
		KRectangle& operator=(const KRectangle&) = default;

		KRectangle(KRectangle&&) = default;
		KRectangle& operator=(KRectangle&&) = default;

		operator RECT() { RECT rct; rct.left = x; rct.top = y; rct.right = (x + width); rct.bottom = (y + height); return rct; }
#ifdef __cplusplus_winrt
		operator Windows::Foundation::Rect() { return Windows::Foundation::Rect(float(x), float(y), float(width), float(height)); }
#endif

		// Comparison operators
		bool operator == (const KRectangle& r) const { return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height); }
		bool operator == (const RECT& rct) const { return (x == rct.left) && (y == rct.top) && (width == (rct.right - rct.left)) && (height == (rct.bottom - rct.top)); }

		bool operator != (const KRectangle& r) const { return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height); }
		bool operator != (const RECT& rct) const { return (x != rct.left) || (y != rct.top) || (width != (rct.right - rct.left)) || (height != (rct.bottom - rct.top)); }

		// Assignment operators
		KRectangle& operator=(_In_ const RECT& rct) { x = rct.left; y = rct.top; width = (rct.right - rct.left); height = (rct.bottom - rct.top); return *this; }

		// KRectangle operations
		KVector2 Location() const;
		KVector2 Center() const;

		bool IsEmpty() const { return (width == 0 && height == 0 && x == 0 && y == 0); }

		bool Contains(long ix, long iy) const { return (x <= ix) && (ix < (x + width)) && (y <= iy) && (iy < (y + height)); }
		bool Contains(const KVector2& point) const;
		bool Contains(const KRectangle& r) const { return (x <= r.x) && ((r.x + r.width) <= (x + width)) && (y <= r.y) && ((r.y + r.height) <= (y + height)); }
		bool Contains(const RECT& rct) const { return (x <= rct.left) && (rct.right <= (x + width)) && (y <= rct.top) && (rct.bottom <= (y + height)); }

		void Inflate(long horizAmount, long vertAmount);

		bool Intersects(const KRectangle& r) const { return (r.x < (x + width)) && (x < (r.x + r.width)) && (r.y < (y + height)) && (y < (r.y + r.height)); }
		bool Intersects(const RECT& rct) const { return (rct.left < (x + width)) && (x < rct.right) && (rct.top < (y + height)) && (y < rct.bottom); }

		void Offset(long ox, long oy) { x += ox; y += oy; }

		// Static functions
		static KRectangle Intersect(const KRectangle& ra, const KRectangle& rb);
		static RECT Intersect(const RECT& rcta, const RECT& rctb);

		static KRectangle Union(const KRectangle& ra, const KRectangle& rb);
		static RECT Union(const RECT& rcta, const RECT& rctb);
	};

	//------------------------------------------------------------------------------
	// 2D vector
	struct KVector2 : DirectX::XMFLOAT2
	{
		KVector2() noexcept : XMFLOAT2(0.f, 0.f) {}
		XM_CONSTEXPR explicit KVector2(float x) : XMFLOAT2(x, x) {}
		XM_CONSTEXPR KVector2(float _x, float _y) : XMFLOAT2(_x, _y) {}
		explicit KVector2(_In_reads_(2) const float* pArray) : XMFLOAT2(pArray) {}
		KVector2(XMFLOAT2 V) { x = V.x; y = V.y; }
		KVector2(const XMFLOAT2& V) { this->x = V.x; this->y = V.y; }
		explicit KVector2(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; }

		KVector2(const KVector2&) = default;
		KVector2& operator=(const KVector2&) = default;

		KVector2(KVector2&&) = default;
		KVector2& operator=(KVector2&&) = default;

		operator DirectX::XMVECTOR() const { return XMLoadFloat2(this); }

		// Comparison operators
		bool operator == (const KVector2& V) const;
		bool operator != (const KVector2& V) const;

		// Assignment operators
		KVector2& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; return *this; }
		KVector2& operator+= (const KVector2& V);
		KVector2& operator-= (const KVector2& V);
		KVector2& operator*= (const KVector2& V);
		KVector2& operator*= (float S);
		KVector2& operator/= (float S);

		// Unary operators
		KVector2 operator+ () const { return *this; }
		KVector2 operator- () const { return KVector2(-x, -y); }

		// Vector operations
		bool InBounds(const KVector2& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const KVector2& V) const;
		void Cross(const KVector2& V, KVector2& result) const;
		KVector2 Cross(const KVector2& V) const;

		void Normalize();
		void Normalize(KVector2& result) const;

		void Clamp(const KVector2& vmin, const KVector2& vmax);
		void Clamp(const KVector2& vmin, const KVector2& vmax, KVector2& result) const;

		// Static functions
		static float Distance(const KVector2& v1, const KVector2& v2);
		static float DistanceSquared(const KVector2& v1, const KVector2& v2);

		static void Min(const KVector2& v1, const KVector2& v2, KVector2& result);
		static KVector2 Min(const KVector2& v1, const KVector2& v2);

		static void Max(const KVector2& v1, const KVector2& v2, KVector2& result);
		static KVector2 Max(const KVector2& v1, const KVector2& v2);

		static void Lerp(const KVector2& v1, const KVector2& v2, float t, KVector2& result);
		static KVector2 Lerp(const KVector2& v1, const KVector2& v2, float t);

		static void SmoothStep(const KVector2& v1, const KVector2& v2, float t, KVector2& result);
		static KVector2 SmoothStep(const KVector2& v1, const KVector2& v2, float t);

		static void Barycentric(const KVector2& v1, const KVector2& v2, const KVector2& v3, float f, float g, KVector2& result);
		static KVector2 Barycentric(const KVector2& v1, const KVector2& v2, const KVector2& v3, float f, float g);

		static void CatmullRom(const KVector2& v1, const KVector2& v2, const KVector2& v3, const KVector2& v4, float t, KVector2& result);
		static KVector2 CatmullRom(const KVector2& v1, const KVector2& v2, const KVector2& v3, const KVector2& v4, float t);

		static void Hermite(const KVector2& v1, const KVector2& t1, const KVector2& v2, const KVector2& t2, float t, KVector2& result);
		static KVector2 Hermite(const KVector2& v1, const KVector2& t1, const KVector2& v2, const KVector2& t2, float t);

		static void Reflect(const KVector2& ivec, const KVector2& nvec, KVector2& result);
		static KVector2 Reflect(const KVector2& ivec, const KVector2& nvec);

		static void Refract(const KVector2& ivec, const KVector2& nvec, float refractionIndex, KVector2& result);
		static KVector2 Refract(const KVector2& ivec, const KVector2& nvec, float refractionIndex);

		static void Transform(const KVector2& v, const KQuaternion& quat, KVector2& result);
		static KVector2 Transform(const KVector2& v, const KQuaternion& quat);

		static void Transform(const KVector2& v, const KMatrix& m, KVector2& result);
		static KVector2 Transform(const KVector2& v, const KMatrix& m);
		static void Transform(_In_reads_(count) const KVector2* varray, size_t count, const KMatrix& m, _Out_writes_(count) KVector2* resultArray);

		static void Transform(const KVector2& v, const KMatrix& m, KVector4& result);
		static void Transform(_In_reads_(count) const KVector2* varray, size_t count, const KMatrix& m, _Out_writes_(count) KVector4* resultArray);

		static void TransformNormal(const KVector2& v, const KMatrix& m, KVector2& result);
		static KVector2 TransformNormal(const KVector2& v, const KMatrix& m);
		static void TransformNormal(_In_reads_(count) const KVector2* varray, size_t count, const KMatrix& m, _Out_writes_(count) KVector2* resultArray);

		// Constants
		static const KVector2 Zero;
		static const KVector2 One;
		static const KVector2 UnitX;
		static const KVector2 UnitY;
	};

	// Binary operators
	KVector2 operator+ (const KVector2& V1, const KVector2& V2);
	KVector2 operator- (const KVector2& V1, const KVector2& V2);
	KVector2 operator* (const KVector2& V1, const KVector2& V2);
	KVector2 operator* (const KVector2& V, float S);
	KVector2 operator/ (const KVector2& V1, const KVector2& V2);
	KVector2 operator* (float S, const KVector2& V);

	//------------------------------------------------------------------------------
	// 3D vector
	struct KVector3 : DirectX::XMFLOAT3
	{
		KVector3() noexcept : DirectX::XMFLOAT3(0.f, 0.f, 0.f) {}
		XM_CONSTEXPR explicit KVector3(float x) : DirectX::XMFLOAT3(x, x, x) {}
		XM_CONSTEXPR KVector3(float _x, float _y, float _z) : DirectX::XMFLOAT3(_x, _y, _z) {}
		KVector3(const float* pArray) : DirectX::XMFLOAT3(pArray) {}
		KVector3(DirectX::XMFLOAT3 V) { x = V.x; y = V.y; z = V.z; }
		KVector3(const DirectX::XMFLOAT3& V) { this->x = V.x; this->y = V.y; this->z = V.z; }
		explicit KVector3(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

		KVector3(const KVector3&) = default;
		KVector3& operator=(const KVector3&) = default;

		KVector3(KVector3&&) = default;
		KVector3& operator=(KVector3&&) = default;

		operator DirectX::XMVECTOR() const { return XMLoadFloat3(this); }

		float operator [](int i)
		{
			if (i == 0) return x;
			if (i == 1) return y;
			if (i == 2) return z;
			return 0.0f;
		}

		// Comparison operators
		bool operator == (const KVector3& V) const;
		bool operator != (const KVector3& V) const;

		// Assignment operators
		KVector3& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
		KVector3& operator+= (const KVector3& V);
		KVector3& operator-= (const KVector3& V);
		KVector3& operator*= (const KVector3& V);
		KVector3& operator*= (float S);
		KVector3& operator/= (float S);

		// Unary operators
		KVector3 operator+ () const { return *this; }
		KVector3 operator- () const;

		// Vector operations
		bool InBounds(const KVector3& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const KVector3& V) const;
		void Cross(const KVector3& V, KVector3& result) const;
		KVector3 Cross(const KVector3& V) const;

		void Normalize();
		void Normalize(KVector3& result) const;

		void Clamp(const KVector3& vmin, const KVector3& vmax);
		void Clamp(const KVector3& vmin, const KVector3& vmax, KVector3& result) const;

		// Static functions
		static float Distance(const KVector3& v1, const KVector3& v2);
		static float DistanceSquared(const KVector3& v1, const KVector3& v2);

		static void Min(const KVector3& v1, const KVector3& v2, KVector3& result);
		static KVector3 Min(const KVector3& v1, const KVector3& v2);

		static void Max(const KVector3& v1, const KVector3& v2, KVector3& result);
		static KVector3 Max(const KVector3& v1, const KVector3& v2);

		static void Lerp(const KVector3& v1, const KVector3& v2, float t, KVector3& result);
		static KVector3 Lerp(const KVector3& v1, const KVector3& v2, float t);

		static void SmoothStep(const KVector3& v1, const KVector3& v2, float t, KVector3& result);
		static KVector3 SmoothStep(const KVector3& v1, const KVector3& v2, float t);

		static void Barycentric(const KVector3& v1, const KVector3& v2, const KVector3& v3, float f, float g, KVector3& result);
		static KVector3 Barycentric(const KVector3& v1, const KVector3& v2, const KVector3& v3, float f, float g);

		static void CatmullRom(const KVector3& v1, const KVector3& v2, const KVector3& v3, const KVector3& v4, float t, KVector3& result);
		static KVector3 CatmullRom(const KVector3& v1, const KVector3& v2, const KVector3& v3, const KVector3& v4, float t);

		static void Hermite(const KVector3& v1, const KVector3& t1, const KVector3& v2, const KVector3& t2, float t, KVector3& result);
		static KVector3 Hermite(const KVector3& v1, const KVector3& t1, const KVector3& v2, const KVector3& t2, float t);

		static void Reflect(const KVector3& ivec, const KVector3& nvec, KVector3& result);
		static KVector3 Reflect(const KVector3& ivec, const KVector3& nvec);

		static void Refract(const KVector3& ivec, const KVector3& nvec, float refractionIndex, KVector3& result);
		static KVector3 Refract(const KVector3& ivec, const KVector3& nvec, float refractionIndex);

		static void Transform(const KVector3& v, const KQuaternion& quat, KVector3& result);
		static KVector3 Transform(const KVector3& v, const KQuaternion& quat);

		static void Transform(const KVector3& v, const KMatrix& m, KVector3& result);
		static KVector3 Transform(const KVector3& v, const KMatrix& m);
		static void Transform(_In_reads_(count) const KVector3* varray, size_t count, const KMatrix& m, _Out_writes_(count) KVector3* resultArray);

		static void Transform(const KVector3& v, const KMatrix& m, KVector4& result);
		static void Transform(_In_reads_(count) const KVector3* varray, size_t count, const KMatrix& m, _Out_writes_(count) KVector4* resultArray);

		static void TransformNormal(const KVector3& v, const KMatrix& m, KVector3& result);
		static KVector3 TransformNormal(const KVector3& v, const KMatrix& m);
		static void TransformNormal(_In_reads_(count) const KVector3* varray, size_t count, const KMatrix& m, _Out_writes_(count) KVector3* resultArray);

		// Constants
		static const KVector3 Zero;
		static const KVector3 One;
		static const KVector3 UnitX;
		static const KVector3 UnitY;
		static const KVector3 UnitZ;
		static const KVector3 Up;
		static const KVector3 Down;
		static const KVector3 Right;
		static const KVector3 Left;
		static const KVector3 Forward;
		static const KVector3 Backward;
	};

	// Binary operators
	KVector3 operator+ (const KVector3& V1, const KVector3& V2);
	KVector3 operator- (const KVector3& V1, const KVector3& V2);
	KVector3 operator* (const KVector3& V1, const KVector3& V2);
	KVector3 operator* (const KVector3& V, float S);
	KVector3 operator/ (const KVector3& V1, const KVector3& V2);
	KVector3 operator* (float S, const KVector3& V);

	//------------------------------------------------------------------------------
	// 4D vector
	struct KVector4 : public XMFLOAT4
	{
		KVector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
		XM_CONSTEXPR explicit KVector4(float x) : XMFLOAT4(x, x, x, x) {}
		XM_CONSTEXPR KVector4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		explicit KVector4(_In_reads_(4) const float* pArray) : XMFLOAT4(pArray) {}
		KVector4(XMFLOAT4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
		KVector4(const XMFLOAT4& V) { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
		explicit KVector4(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		KVector4(const KVector4&) = default;
		KVector4& operator=(const KVector4&) = default;

		KVector4(KVector4&&) = default;
		KVector4& operator=(KVector4&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparison operators
		bool operator == (const KVector4& V) const;
		bool operator != (const KVector4& V) const;

		// Assignment operators
		KVector4& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
		KVector4& operator+= (const KVector4& V);
		KVector4& operator-= (const KVector4& V);
		KVector4& operator*= (const KVector4& V);
		KVector4& operator*= (float S);
		KVector4& operator/= (float S);

		// Unary operators
		KVector4 operator+ () const { return *this; }
		KVector4 operator- () const;

		// Vector operations
		bool InBounds(const KVector4& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const KVector4& V) const;
		void Cross(const KVector4& v1, const KVector4& v2, KVector4& result) const;
		KVector4 Cross(const KVector4& v1, const KVector4& v2) const;

		void Normalize();
		void Normalize(KVector4& result) const;

		void Clamp(const KVector4& vmin, const KVector4& vmax);
		void Clamp(const KVector4& vmin, const KVector4& vmax, KVector4& result) const;

		// Static functions
		static float Distance(const KVector4& v1, const KVector4& v2);
		static float DistanceSquared(const KVector4& v1, const KVector4& v2);

		static void Min(const KVector4& v1, const KVector4& v2, KVector4& result);
		static KVector4 Min(const KVector4& v1, const KVector4& v2);

		static void Max(const KVector4& v1, const KVector4& v2, KVector4& result);
		static KVector4 Max(const KVector4& v1, const KVector4& v2);

		static void Lerp(const KVector4& v1, const KVector4& v2, float t, KVector4& result);
		static KVector4 Lerp(const KVector4& v1, const KVector4& v2, float t);

		static void SmoothStep(const KVector4& v1, const KVector4& v2, float t, KVector4& result);
		static KVector4 SmoothStep(const KVector4& v1, const KVector4& v2, float t);

		static void Barycentric(const KVector4& v1, const KVector4& v2, const KVector4& v3, float f, float g, KVector4& result);
		static KVector4 Barycentric(const KVector4& v1, const KVector4& v2, const KVector4& v3, float f, float g);

		static void CatmullRom(const KVector4& v1, const KVector4& v2, const KVector4& v3, const KVector4& v4, float t, KVector4& result);
		static KVector4 CatmullRom(const KVector4& v1, const KVector4& v2, const KVector4& v3, const KVector4& v4, float t);

		static void Hermite(const KVector4& v1, const KVector4& t1, const KVector4& v2, const KVector4& t2, float t, KVector4& result);
		static KVector4 Hermite(const KVector4& v1, const KVector4& t1, const KVector4& v2, const KVector4& t2, float t);

		static void Reflect(const KVector4& ivec, const KVector4& nvec, KVector4& result);
		static KVector4 Reflect(const KVector4& ivec, const KVector4& nvec);

		static void Refract(const KVector4& ivec, const KVector4& nvec, float refractionIndex, KVector4& result);
		static KVector4 Refract(const KVector4& ivec, const KVector4& nvec, float refractionIndex);

		static void Transform(const KVector2& v, const KQuaternion& quat, KVector4& result);
		static KVector4 Transform(const KVector2& v, const KQuaternion& quat);

		static void Transform(const KVector3& v, const KQuaternion& quat, KVector4& result);
		static KVector4 Transform(const KVector3& v, const KQuaternion& quat);

		static void Transform(const KVector4& v, const KQuaternion& quat, KVector4& result);
		static KVector4 Transform(const KVector4& v, const KQuaternion& quat);

		static void Transform(const KVector4& v, const KMatrix& m, KVector4& result);
		static KVector4 Transform(const KVector4& v, const KMatrix& m);
		static void Transform(_In_reads_(count) const KVector4* varray, size_t count, const KMatrix& m, _Out_writes_(count) KVector4* resultArray);

		// Constants
		static const KVector4 Zero;
		static const KVector4 One;
		static const KVector4 UnitX;
		static const KVector4 UnitY;
		static const KVector4 UnitZ;
		static const KVector4 UnitW;
	};

	// Binary operators
	KVector4 operator+ (const KVector4& V1, const KVector4& V2);
	KVector4 operator- (const KVector4& V1, const KVector4& V2);
	KVector4 operator* (const KVector4& V1, const KVector4& V2);
	KVector4 operator* (const KVector4& V, float S);
	KVector4 operator/ (const KVector4& V1, const KVector4& V2);
	KVector4 operator* (float S, const KVector4& V);

	//------------------------------------------------------------------------------
	// 4x4 KMatrix (assumes right-handed cooordinates)
	struct KMatrix : public XMFLOAT4X4
	{
		KMatrix() noexcept
			: XMFLOAT4X4(1.f, 0, 0, 0,
				0, 1.f, 0, 0,
				0, 0, 1.f, 0,
				0, 0, 0, 1.f) {}
		XM_CONSTEXPR KMatrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: XMFLOAT4X4(m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33) {}
		explicit KMatrix(const KVector3& r0, const KVector3& r1, const KVector3& r2)
			: XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
				r1.x, r1.y, r1.z, 0,
				r2.x, r2.y, r2.z, 0,
				0, 0, 0, 1.f) {}
		explicit KMatrix(const KVector4& r0, const KVector4& r1, const KVector4& r2, const KVector4& r3)
			: XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
				r1.x, r1.y, r1.z, r1.w,
				r2.x, r2.y, r2.z, r2.w,
				r3.x, r3.y, r3.z, r3.w) {}
		KMatrix(const XMFLOAT4X4& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); }
		KMatrix(const XMFLOAT3X3& M);
		KMatrix(const XMFLOAT4X3& M);

		explicit KMatrix(_In_reads_(16) const float* pArray) : XMFLOAT4X4(pArray) {}
		KMatrix(CXMMATRIX M) { XMStoreFloat4x4(this, M); }

		KMatrix(const KMatrix&) = default;
		KMatrix& operator=(const KMatrix&) = default;

		KMatrix(KMatrix&&) = default;
		KMatrix& operator=(KMatrix&&) = default;

		operator XMFLOAT4X4() const { return *this; }

		// Comparison operators
		bool operator == (const KMatrix& M) const;
		bool operator != (const KMatrix& M) const;

		// Assignment operators
		KMatrix& operator= (const XMFLOAT3X3& M);
		KMatrix& operator= (const XMFLOAT4X3& M);
		KMatrix& operator+= (const KMatrix& M);
		KMatrix& operator-= (const KMatrix& M);
		KMatrix& operator*= (const KMatrix& M);
		KMatrix& operator*= (float S);
		KMatrix& operator/= (float S);

		KMatrix& operator/= (const KMatrix& M);
		// Element-wise divide

		// Unary operators
		KMatrix operator+ () const { return *this; }
		KMatrix operator- () const;

		// Properties
		KVector3 Up() const { return KVector3(_21, _22, _23); }
		void Up(const KVector3& v) { _21 = v.x; _22 = v.y; _23 = v.z; }

		KVector3 Down() const { return KVector3(-_21, -_22, -_23); }
		void Down(const KVector3& v) { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

		KVector3 Right() const { return KVector3(_11, _12, _13); }
		void Right(const KVector3& v) { _11 = v.x; _12 = v.y; _13 = v.z; }

		KVector3 Left() const { return KVector3(-_11, -_12, -_13); }
		void Left(const KVector3& v) { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

		KVector3 Forward() const { return KVector3(-_31, -_32, -_33); }
		void Forward(const KVector3& v) { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

		KVector3 Backward() const { return KVector3(_31, _32, _33); }
		void Backward(const KVector3& v) { _31 = v.x; _32 = v.y; _33 = v.z; }

		KVector3 Translation() const { return KVector3(_41, _42, _43); }
		void Translation(const KVector3& v) { _41 = v.x; _42 = v.y; _43 = v.z; }

		// KMatrix operations
		bool Decompose(KVector3& scale, KQuaternion& rotation, KVector3& translation);
		
		KMatrix Transpose() const;
		void Transpose(KMatrix& result) const;

		KMatrix Invert() const;
		void Invert(KMatrix& result) const;

		float Determinant() const;

		// Static functions
		static KMatrix PerspectiveFovLH(
		float fNearPlane, float fFarPlane,
		float fovy, float Aspect){
		float    h, w, Q;
		// 1/tans(x) = cot(x)
		h = 1 / tan(fovy * 0.5f); 
		w = h / Aspect;
		Q = fFarPlane / (fFarPlane - fNearPlane);
		KMatrix ret;
		ZeroMemory(&ret, sizeof(KMatrix));
		ret._11 = w;
		ret._22 = h;
		ret._33 = Q;
		ret._43 = -Q * fNearPlane;
		ret._34 = 1;
		return ret;
	}
		static KMatrix CreateBillboard(const KVector3& object, const KVector3& cameraPosition, const KVector3& cameraUp, _In_opt_ const KVector3* cameraForward = nullptr);

		static KMatrix CreateConstrainedBillboard(const KVector3& object, const KVector3& cameraPosition, const KVector3& rotateAxis,
			_In_opt_ const KVector3* cameraForward = nullptr, _In_opt_ const KVector3* objectForward = nullptr);

		static KMatrix CreateTranslation(const KVector3& position);
		static KMatrix CreateTranslation(float x, float y, float z);

		static KMatrix CreateScale(const KVector3& scales);
		static KMatrix CreateScale(float xs, float ys, float zs);
		static KMatrix CreateScale(float scale);

		static KMatrix CreateRotationX(float radians);
		static KMatrix CreateRotationY(float radians);
		static KMatrix CreateRotationZ(float radians);

		static KMatrix CreateFromAxisAngle(const KVector3& axis, float angle);

		static KMatrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane);
		static KMatrix CreatePerspective(float width, float height, float nearPlane, float farPlane);
		static KMatrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane);
		static KMatrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane);
		static KMatrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane);

		static KMatrix CreateLookAt(const KVector3& position, const KVector3& target, const KVector3& up);
		static KMatrix CreateWorld(const KVector3& position, const KVector3& forward, const KVector3& up);

		static KMatrix CreateFromQuaternion(const KQuaternion& quat);

		static KMatrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);

		static KMatrix CreateShadow(const KVector3& lightDir, const KPlane& plane);

		static KMatrix CreateReflection(const KPlane& plane);

		static void Lerp(const KMatrix& M1, const KMatrix& M2, float t, KMatrix& result);
		static KMatrix Lerp(const KMatrix& M1, const KMatrix& M2, float t);

		static void Transform(const KMatrix& M, const KQuaternion& rotation, KMatrix& result);
		static KMatrix Transform(const KMatrix& M, const KQuaternion& rotation);

		// Constants
		static const KMatrix Identity;
	};

	// Binary operators
	KMatrix operator+ (const KMatrix& M1, const KMatrix& M2);
	KMatrix operator- (const KMatrix& M1, const KMatrix& M2);
	KMatrix operator* (const KMatrix& M1, const KMatrix& M2);
	KMatrix operator* (const KMatrix& M, float S);
	KMatrix operator/ (const KMatrix& M, float S);
	KMatrix operator/ (const KMatrix& M1, const KMatrix& M2);
	// Element-wise divide
	KMatrix operator* (float S, const KMatrix& M);


	//-----------------------------------------------------------------------------
	// KPlane
	struct KPlane : public XMFLOAT4
	{
		KPlane() noexcept : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
		XM_CONSTEXPR KPlane(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		KPlane(const KVector3& normal, float d) : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
		KPlane(const KVector3& point1, const KVector3& point2, const KVector3& point3);
		KPlane(const KVector3& point, const KVector3& normal);
		explicit KPlane(const KVector4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
		explicit KPlane(_In_reads_(4) const float* pArray) : XMFLOAT4(pArray) {}
		KPlane(XMFLOAT4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
		KPlane(const XMFLOAT4& p) { this->x = p.x; this->y = p.y; this->z = p.z; this->w = p.w; }
		explicit KPlane(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		KPlane(const KPlane&) = default;
		KPlane& operator=(const KPlane&) = default;

		KPlane(KPlane&&) = default;
		KPlane& operator=(KPlane&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparison operators
		bool operator == (const KPlane& p) const;
		bool operator != (const KPlane& p) const;

		// Assignment operators
		KPlane& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }

		// Properties
		KVector3 Normal() const { return KVector3(x, y, z); }
		void Normal(const KVector3& normal) { x = normal.x; y = normal.y; z = normal.z; }

		float D() const { return w; }
		void D(float d) { w = d; }

		// KPlane operations
		void Normalize();
		void Normalize(KPlane& result) const;

		float Dot(const KVector4& v) const;
		float DotCoordinate(const KVector3& position) const;
		float DotNormal(const KVector3& normal) const;

		// Static functions
		static void Transform(const KPlane& plane, const KMatrix& M, KPlane& result);
		static KPlane Transform(const KPlane& plane, const KMatrix& M);

		static void Transform(const KPlane& plane, const KQuaternion& rotation, KPlane& result);
		static KPlane Transform(const KPlane& plane, const KQuaternion& rotation);
		// Input quaternion must be the inverse transpose of the transformation
	};

	//------------------------------------------------------------------------------
	// KQuaternion
	struct KQuaternion : public XMFLOAT4
	{
		KQuaternion() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
		XM_CONSTEXPR KQuaternion(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		KQuaternion(const KVector3& v, float scalar) : XMFLOAT4(v.x, v.y, v.z, scalar) {}
		explicit KQuaternion(const KVector4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
		explicit KQuaternion(_In_reads_(4) const float* pArray) : XMFLOAT4(pArray) {}
		KQuaternion(XMFLOAT4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
		KQuaternion(const XMFLOAT4& q) { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }
		explicit KQuaternion(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		KQuaternion(const KQuaternion&) = default;
		KQuaternion& operator=(const KQuaternion&) = default;

		KQuaternion(KQuaternion&&) = default;
		KQuaternion& operator=(KQuaternion&&) = default;

		//operator int () const { return 0; }
		//SampleClass f;
		//int i = f; //  f.operator int () 를 호출하고 초기화 및 반한됨.
		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparison operators
		bool operator == (const KQuaternion& q) const;
		bool operator != (const KQuaternion& q) const;

		// Assignment operators
		KQuaternion& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
		KQuaternion& operator+= (const KQuaternion& q);
		KQuaternion& operator-= (const KQuaternion& q);
		KQuaternion& operator*= (const KQuaternion& q);
		KQuaternion& operator*= (float S);
		KQuaternion& operator/= (const KQuaternion& q);

		// Unary operators
		KQuaternion operator+ () const { return *this; }
		KQuaternion operator- () const;

		// KQuaternion operations
		float Length() const;
		float LengthSquared() const;

		void Normalize();
		void Normalize(KQuaternion& result) const;

		void Conjugate();
		void Conjugate(KQuaternion& result) const;

		void Inverse(KQuaternion& result) const;

		float Dot(const KQuaternion& Q) const;

		// Static functions
		static KQuaternion CreateFromAxisAngle(const KVector3& axis, float angle);
		static KQuaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll);
		static KQuaternion CreateFromRotationMatrix(const KMatrix& M);

		static void Lerp(const KQuaternion& q1, const KQuaternion& q2, float t, KQuaternion& result);
		static KQuaternion Lerp(const KQuaternion& q1, const KQuaternion& q2, float t);

		static void Slerp(const KQuaternion& q1, const KQuaternion& q2, float t, KQuaternion& result);
		static KQuaternion Slerp(const KQuaternion& q1, const KQuaternion& q2, float t);

		static void Concatenate(const KQuaternion& q1, const KQuaternion& q2, KQuaternion& result);
		static KQuaternion Concatenate(const KQuaternion& q1, const KQuaternion& q2);

		// Constants
		static const KQuaternion Identity;
	};

	// Binary operators
	KQuaternion operator+ (const KQuaternion& Q1, const KQuaternion& Q2);
	KQuaternion operator- (const KQuaternion& Q1, const KQuaternion& Q2);
	KQuaternion operator* (const KQuaternion& Q1, const KQuaternion& Q2);
	KQuaternion operator* (const KQuaternion& Q, float S);
	KQuaternion operator/ (const KQuaternion& Q1, const KQuaternion& Q2);
	KQuaternion operator* (float S, const KQuaternion& Q);

	//------------------------------------------------------------------------------
	// KColor
	struct KColor : public XMFLOAT4
	{
		KColor() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
		XM_CONSTEXPR KColor(float _r, float _g, float _b) : XMFLOAT4(_r, _g, _b, 1.f) {}
		XM_CONSTEXPR KColor(float _r, float _g, float _b, float _a) : XMFLOAT4(_r, _g, _b, _a) {}
		explicit KColor(const KVector3& clr) : XMFLOAT4(clr.x, clr.y, clr.z, 1.f) {}
		explicit KColor(const KVector4& clr) : XMFLOAT4(clr.x, clr.y, clr.z, clr.w) {}
		explicit KColor(_In_reads_(4) const float* pArray) : XMFLOAT4(pArray) {}
		KColor(XMFLOAT3 V) { x = V.x; y = V.y; z = V.z; }
		KColor(const XMFLOAT4& c) { this->x = c.x; this->y = c.y; this->z = c.z; this->w = c.w; }
		explicit KColor(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		explicit KColor(const DirectX::PackedVector::XMCOLOR& Packed);
		// BGRA Direct3D 9 D3DCOLOR packed color

		explicit KColor(const DirectX::PackedVector::XMUBYTEN4& Packed);
		// RGBA XNA Game Studio packed color

		KColor(const KColor&) = default;
		KColor& operator=(const KColor&) = default;

		KColor(KColor&&) = default;
		KColor& operator=(KColor&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }
		operator const float* () const { return reinterpret_cast<const float*>(this); }

		// Comparison operators
		bool operator == (const KColor& c) const;
		bool operator != (const KColor& c) const;

		// Assignment operators
		KColor& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
		KColor& operator= (const DirectX::PackedVector::XMCOLOR& Packed);
		KColor& operator= (const DirectX::PackedVector::XMUBYTEN4& Packed);
		KColor& operator+= (const KColor& c);
		KColor& operator-= (const KColor& c);
		KColor& operator*= (const KColor& c);
		KColor& operator*= (float S);
		KColor& operator/= (const KColor& c);

		// Unary operators
		KColor operator+ () const { return *this; }
		KColor operator- () const;

		// Properties
		float R() const { return x; }
		void R(float r) { x = r; }

		float G() const { return y; }
		void G(float g) { y = g; }

		float B() const { return z; }
		void B(float b) { z = b; }

		float A() const { return w; }
		void A(float a) { w = a; }

		// KColor operations
		DirectX::PackedVector::XMCOLOR BGRA() const;
		DirectX::PackedVector::XMUBYTEN4 RGBA() const;

		KVector3 ToVector3() const;
		KVector4 ToVector4() const;

		void Negate();
		void Negate(KColor& result) const;

		void Saturate();
		void Saturate(KColor& result) const;

		void Premultiply();
		void Premultiply(KColor& result) const;

		void AdjustSaturation(float sat);
		void AdjustSaturation(float sat, KColor& result) const;

		void AdjustContrast(float contrast);
		void AdjustContrast(float contrast, KColor& result) const;

		// Static functions
		static void Modulate(const KColor& c1, const KColor& c2, KColor& result);
		static KColor Modulate(const KColor& c1, const KColor& c2);

		static void Lerp(const KColor& c1, const KColor& c2, float t, KColor& result);
		static KColor Lerp(const KColor& c1, const KColor& c2, float t);
	};

	// Binary operators
	KColor operator+ (const KColor& C1, const KColor& C2);
	KColor operator- (const KColor& C1, const KColor& C2);
	KColor operator* (const KColor& C1, const KColor& C2);
	KColor operator* (const KColor& C, float S);
	KColor operator/ (const KColor& C1, const KColor& C2);
	KColor operator* (float S, const KColor& C);

	//------------------------------------------------------------------------------
	// TRay
	class TRay
	{
	public:
		KVector3 position;
		KVector3 direction;

		TRay() noexcept : position(0, 0, 0), direction(0, 0, 1) {}
		TRay(const KVector3& pos, const KVector3& dir) : position(pos), direction(dir) {}

		TRay(const TRay&) = default;
		TRay& operator=(const TRay&) = default;

		TRay(TRay&&) = default;
		TRay& operator=(TRay&&) = default;

		// Comparison operators
		bool operator == (const TRay& r) const;
		bool operator != (const TRay& r) const;

		// TRay operations
		bool Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const;
		bool Intersects(const BoundingBox& box, _Out_ float& Dist) const;
		bool Intersects(const KVector3& tri0, const KVector3& tri1, const KVector3& tri2, _Out_ float& Dist) const;
		bool Intersects(const KPlane& plane, _Out_ float& Dist) const;
	};

	//------------------------------------------------------------------------------
	// TViewport
	class TViewport
	{
	public:
		float x;
		float y;
		float width;
		float height;
		float minDepth;
		float maxDepth;

		TViewport() noexcept :
			x(0.f), y(0.f), width(0.f), height(0.f), minDepth(0.f), maxDepth(1.f) {}
		XM_CONSTEXPR TViewport(float ix, float iy, float iw, float ih, float iminz = 0.f, float imaxz = 1.f) :
			x(ix), y(iy), width(iw), height(ih), minDepth(iminz), maxDepth(imaxz) {}
		explicit TViewport(const RECT& rct) :
			x(float(rct.left)), y(float(rct.top)),
			width(float(rct.right - rct.left)),
			height(float(rct.bottom - rct.top)),
			minDepth(0.f), maxDepth(1.f) {}

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
		// Direct3D 11 interop
		explicit TViewport(const D3D11_VIEWPORT& vp) :
			x(vp.TopLeftX), y(vp.TopLeftY),
			width(vp.Width), height(vp.Height),
			minDepth(vp.MinDepth), maxDepth(vp.MaxDepth) {}

		operator D3D11_VIEWPORT() { return *reinterpret_cast<const D3D11_VIEWPORT*>(this); }
		const D3D11_VIEWPORT* Get11() const { return reinterpret_cast<const D3D11_VIEWPORT*>(this); }
		TViewport& operator= (const D3D11_VIEWPORT& vp);
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__)
		// Direct3D 12 interop
		explicit TViewport(const D3D12_VIEWPORT& vp) :
			x(vp.TopLeftX), y(vp.TopLeftY),
			width(vp.Width), height(vp.Height),
			minDepth(vp.MinDepth), maxDepth(vp.MaxDepth) {}

		operator D3D12_VIEWPORT() { return *reinterpret_cast<const D3D12_VIEWPORT*>(this); }
		const D3D12_VIEWPORT* Get12() const { return reinterpret_cast<const D3D12_VIEWPORT*>(this); }
		TViewport& operator= (const D3D12_VIEWPORT& vp);
#endif

		TViewport(const TViewport&) = default;
		TViewport& operator=(const TViewport&) = default;

		TViewport(TViewport&&) = default;
		TViewport& operator=(TViewport&&) = default;

		// Comparison operators
		bool operator == (const TViewport& vp) const;
		bool operator != (const TViewport& vp) const;

		// Assignment operators
		TViewport& operator= (const RECT& rct);

		// TViewport operations
		float AspectRatio() const;

		KVector3 Project(const KVector3& p, const KMatrix& proj, const KMatrix& view, const KMatrix& world) const;
		void Project(const KVector3& p, const KMatrix& proj, const KMatrix& view, const KMatrix& world, KVector3& result) const;

		KVector3 Unproject(const KVector3& p, const KMatrix& proj, const KMatrix& view, const KMatrix& world) const;
		void Unproject(const KVector3& p, const KMatrix& proj, const KMatrix& view, const KMatrix& world, KVector3& result) const;

		// Static methods
		static RECT __cdecl ComputeDisplayArea(DXGI_SCALING scaling, UINT backBufferWidth, UINT backBufferHeight, int outputWidth, int outputHeight);
		static RECT __cdecl ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight);
	};


	///////////////////////////////////////// static ///////////////////////////////////////////
	//https://docs.microsoft.com/en-us/windows/win32/dxmath/pg-xnamath-migration-d3dx
	//--------------------------
	// 2D Vector
	//--------------------------

	static float D3DXVec2Length(CONST KVector2* pV)
	{
		return pV->Length();
	}

	static float D3DXVec2LengthSq(CONST KVector2* pV)
	{
		return 0.0f;
	}

	static float D3DXVec2Dot(CONST KVector2* pV1, CONST KVector2* pV2)
	{
		return 0.0f;
	}

	// Z component of ((x1,y1,0) cross (x2,y2,0))
	static float D3DXVec2CCW(CONST KVector2* pV1, CONST KVector2* pV2)
	{
		return 0.0f;
	}

	static KVector2* D3DXVec2Add(KVector2* pOut, CONST KVector2* pV1, CONST KVector2* pV2)
	{
		return pOut;
	}

	static KVector2* D3DXVec2Subtract(KVector2* pOut, CONST KVector2* pV1, CONST KVector2* pV2)
	{
		return pOut;
	}

	// Minimize each component.  x = min(x1, x2), y = min(y1, y2)
	static KVector2* D3DXVec2Minimize(KVector2* pOut, CONST KVector2* pV1, CONST KVector2* pV2)
	{
		return pOut;
	}

	// Maximize each component.  x = max(x1, x2), y = max(y1, y2)
	static KVector2* D3DXVec2Maximize(KVector2* pOut, CONST KVector2* pV1, CONST KVector2* pV2)
	{
		return pOut;
	}

	static KVector2* D3DXVec2Scale(KVector2* pOut, CONST KVector2* pV, float s)
	{
		return pOut;
	}

	// Linear interpolation. V1 + s(V2-V1)
	static KVector2* D3DXVec2Lerp(KVector2* pOut, CONST KVector2* pV1, CONST KVector2* pV2,
		float s)
	{
		*pOut = KVector2::Lerp(*pV1, *pV2, s);
		return pOut;
	}
	static KVector2* D3DXVec2Normalize(KVector2* pOut, CONST KVector2* pV)
	{
		return pOut;
	}

	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	static KVector2* D3DXVec2Hermite(KVector2* pOut, CONST KVector2* pV1, CONST KVector2* pT1,
		CONST KVector2* pV2, CONST KVector2* pT2, float s)
	{
		return pOut;
	}

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	static KVector2* D3DXVec2CatmullRom(KVector2* pOut, CONST KVector2* pV0, CONST KVector2* pV1,
		CONST KVector2* pV2, CONST KVector2* pV3, float s)
	{
		return pOut;
	}

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	static KVector2* D3DXVec2BaryCentric(KVector2* pOut, CONST KVector2* pV1, CONST KVector2* pV2,
		CONST KVector2* pV3, float f, float g)
	{
		return pOut;
	}

	// Transform (x, y, 0, 1) by matrix.
	static KVector4* D3DXVec2Transform(KVector4* pOut, CONST KVector2* pV, CONST KMatrix* pM)
	{
		return pOut;
	}

	// Transform (x, y, 0, 1) by matrix, project result back into w=1.
	static KVector2* D3DXVec2TransformCoord(KVector2* pOut, CONST KVector2* pV, CONST KMatrix* pM)
	{
		return pOut;
	}

	// Transform (x, y, 0, 0) by matrix.
	static KVector2* D3DXVec2TransformNormal(KVector2* pOut, CONST KVector2* pV, CONST KMatrix* pM)
	{
		return pOut;
	}

	// Transform Array (x, y, 0, 1) by matrix.
	static KVector4* D3DXVec2TransformArray(KVector4* pOut, UINT OutStride, CONST KVector2* pV, UINT VStride, CONST KMatrix* pM, UINT n)
	{
		return pOut;
	}

	// Transform Array (x, y, 0, 1) by matrix, project result back into w=1.
	static KVector2* D3DXVec2TransformCoordArray(KVector2* pOut, UINT OutStride, CONST KVector2* pV, UINT VStride, CONST KMatrix* pM, UINT n)
	{
		return pOut;
	}

	// Transform Array (x, y, 0, 0) by matrix.
	static KVector2* D3DXVec2TransformNormalArray(KVector2* pOut, UINT OutStride, CONST KVector2* pV, UINT VStride, CONST KMatrix* pM, UINT n)
	{
		return pOut;
	}
	//--------------------------
	// 3D Vector
	//--------------------------

	static  float D3DXVec3Dot(CONST KVector3* pV1, CONST KVector3* pV2)
	{
		return pV1->Dot(*pV2);
	}
	static KVector3* D3DXVec3Cross(KVector3* pOut, CONST KVector3* pV1, CONST KVector3* pV2)
	{
		*pOut = pV1->Cross(*pV2);
		return pOut;
	}
	static KVector3* D3DXVec3Normalize(KVector3* pOut, CONST KVector3* pV)
	{
		pV->Normalize(*pOut);
		return pOut;
	}
	static KVector3* D3DXVec3TransformCoord(KVector3* pOut, CONST KVector3* pV, CONST KMatrix* pM)
	{
		*pOut = KVector3::Transform(*pV, *pM);
		return pOut;
	}
	static float D3DXVec3Length(CONST KVector3* pV)
	{
		return pV->Length();
	}



	static float D3DXVec3LengthSq(CONST KVector3* pV)
	{
		return pV->LengthSquared();
	}


	static KVector3* D3DXVec3Add(KVector3* pOut, CONST KVector3* pV1, CONST KVector3* pV2)
	{
		*pOut = *pV1 + *pV2;
		return pOut;
	}

	static KVector3* D3DXVec3Subtract(KVector3* pOut, CONST KVector3* pV1, CONST KVector3* pV2)
	{
		*pOut = *pV1 - *pV2;
		return pOut;
	}

	// Minimize each component.  x = min(x1, x2), y = min(y1, y2), ...
	static KVector3* D3DXVec3Minimize(KVector3* pOut, CONST KVector3* pV1, CONST KVector3* pV2) {
	}

	// Maximize each component.  x = max(x1, x2), y = max(y1, y2), ...
	static KVector3* D3DXVec3Maximize(KVector3* pOut, CONST KVector3* pV1, CONST KVector3* pV2)
	{
		return pOut;
	}

	static KVector3* D3DXVec3Scale(KVector3* pOut, CONST KVector3* pV, float s)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(pV);
		XMVECTOR X = XMVectorScale(v1, s);
		KVector3 R;
		XMStoreFloat3(&R, X);
		*pOut = R;
		return pOut;
	}

	// Linear interpolation. V1 + s(V2-V1)
	static KVector3* D3DXVec3Lerp(KVector3* pOut, CONST KVector3* pV1, CONST KVector3* pV2, float s)
	{
		*pOut = KVector3::Lerp(*pV1, *pV2, s);
		return pOut;
	}


	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	static KVector3* D3DXVec3Hermite(KVector3* pOut, CONST KVector3* pV1, CONST KVector3* pT1,
		CONST KVector3* pV2, CONST KVector3* pT2, float s)
	{
		*pOut = KVector3::Hermite(*pV1, *pT1, *pV2, *pT2, s);
		return pOut;
	}

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	static KVector3* D3DXVec3CatmullRom(KVector3* pOut, CONST KVector3* pV0, CONST KVector3* pV1,
		CONST KVector3* pV2, CONST KVector3* pV3, float s)
	{
		*pOut = KVector3::CatmullRom(*pV0, *pV1, *pV2, *pV3, s);
		return pOut;
	}

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	static KVector3* D3DXVec3BaryCentric(KVector3* pOut, CONST KVector3* pV1, CONST KVector3* pV2,
		CONST KVector3* pV3, float f, float g)
	{
		*pOut = KVector3::Barycentric(*pV1, *pV2, *pV3, f, g);
		return pOut;
	}

	// Transform (x, y, z, 1) by matrix.
	static KVector4* D3DXVec3Transform(KVector4* pOut, CONST KVector3* pV, CONST KMatrix* pM)
	{
		return pOut;
	}

	// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	// non-affine matrix, the matrix you pass to this function should be the 
	// transpose of the inverse of the matrix you would use to transform a coord.
	static KVector3* D3DXVec3TransformNormal(KVector3* pOut, CONST KVector3* pV, CONST KMatrix* pM)
	{
		*pOut = KVector3::TransformNormal(*pV, *pM);
		return pOut;
	}


	// Transform Array (x, y, z, 1) by matrix. 
	static KVector4* D3DXVec3TransformArray(KVector4* pOut, UINT OutStride, CONST KVector3* pV, UINT VStride, CONST KMatrix* pM, UINT n)
	{
		return pOut;
	}

	// Transform Array (x, y, z, 1) by matrix, project result back into w=1.
	static KVector3* D3DXVec3TransformCoordArray(KVector3* pOut, UINT OutStride, CONST KVector3* pV, UINT VStride, CONST KMatrix* pM, UINT n)
	{
		return pOut;
	}

	// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	// non-affine matrix, the matrix you pass to this function should be the 
	// transpose of the inverse of the matrix you would use to transform a coord.
	static KVector3* D3DXVec3TransformNormalArray(KVector3* pOut, UINT OutStride, CONST KVector3* pV, UINT VStride, CONST KMatrix* pM, UINT n)
	{
		return pOut;
	}

	// Project vector from object space into screen space
	static KVector3* D3DXVec3Project(KVector3* pOut, CONST KVector3* pV, CONST D3D10_VIEWPORT* pViewport,
		CONST KMatrix* pProjection, CONST KMatrix* pView, CONST KMatrix* pWorld)
	{
		//*pOut = TViewport::Project();
		return pOut;
	}

	// Project vector from screen space into object space
	static KVector3* D3DXVec3Unproject(KVector3* pOut, CONST KVector3* pV, CONST D3D10_VIEWPORT* pViewport,
		CONST KMatrix* pProjection, CONST KMatrix* pView, CONST KMatrix* pWorld)
	{
		//*pOut = TViewport::Unproject();
		return pOut;
	}

	// Project vector Array from object space into screen space
	static KVector3* D3DXVec3ProjectArray(KVector3* pOut, UINT OutStride, CONST KVector3* pV, UINT VStride, CONST D3D10_VIEWPORT* pViewport,
		CONST KMatrix* pProjection, CONST KMatrix* pView, CONST KMatrix* pWorld, UINT n)
	{
		return pOut;
	}

	// Project vector Array from screen space into object space
	static KVector3* D3DXVec3UnprojectArray(KVector3* pOut, UINT OutStride, CONST KVector3* pV, UINT VStride, CONST D3D10_VIEWPORT* pViewport,
		CONST KMatrix* pProjection, CONST KMatrix* pView, CONST KMatrix* pWorld, UINT n)
	{
		return pOut;
	}

	//--------------------------
	// 4D Vector
	//--------------------------

	static float D3DXVec4Length(CONST KVector4* pV)
	{
		return pV->Length();
	}


	static float D3DXVec4LengthSq(CONST KVector4* pV)
	{
		return 0.0f;
	}

	static float D3DXVec4Dot(CONST KVector4* pV1, CONST KVector4* pV2)
	{
		return 0.0f;
	}
	static KVector4* D3DXVec4Add(KVector4* pOut, CONST KVector4* pV1, CONST KVector4* pV2)
	{
		return pOut;
	}

	static KVector4* D3DXVec4Subtract(KVector4* pOut, CONST KVector4* pV1, CONST KVector4* pV2)
	{
		return pOut;
	}

	// Minimize each component.  x = min(x1, x2), y = min(y1, y2), ...
	static KVector4* D3DXVec4Minimize(KVector4* pOut, CONST KVector4* pV1, CONST KVector4* pV2)
	{
		return pOut;
	}

	// Maximize each component.  x = max(x1, x2), y = max(y1, y2), ...
	static KVector4* D3DXVec4Maximize(KVector4* pOut, CONST KVector4* pV1, CONST KVector4* pV2)
	{
		return pOut;
	}
	static KVector4* D3DXVec4Scale(KVector4* pOut, CONST KVector4* pV, float s) {
		return pOut;
	}

	// Linear interpolation. V1 + s(V2-V1)
	static KVector4* D3DXVec4Lerp(KVector4* pOut, CONST KVector4* pV1, CONST KVector4* pV2, float s)
	{
		*pOut = KVector4::Lerp(*pV1, *pV2, s);
		return pOut;
	}

	// Cross-product in 4 dimensions.
	static KVector4* D3DXVec4Cross(KVector4* pOut, CONST KVector4* pV1, CONST KVector4* pV2,
		CONST KVector4* pV3)
	{
		return pOut;
	}

	static KVector4* D3DXVec4Normalize(KVector4* pOut, CONST KVector4* pV)
	{
		return pOut;
	}

	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	static KVector4* D3DXVec4Hermite(KVector4* pOut, CONST KVector4* pV1, CONST KVector4* pT1,
		CONST KVector4* pV2, CONST KVector4* pT2, float s)
	{
		return pOut;
	}

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	static KVector4* D3DXVec4CatmullRom(KVector4* pOut, CONST KVector4* pV0, CONST KVector4* pV1,
		CONST KVector4* pV2, CONST KVector4* pV3, float s)
	{
		return pOut;
	}

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	static KVector4* D3DXVec4BaryCentric(KVector4* pOut, CONST KVector4* pV1, CONST KVector4* pV2,
		CONST KVector4* pV3, float f, float g)
	{
		return pOut;
	}

	// Transform vector by matrix.
	static KVector4* D3DXVec4Transform(KVector4* pOut, CONST KVector4* pV, CONST KMatrix* pM)
	{
		return pOut;
	}

	// Transform vector array by matrix.
	static KVector4* D3DXVec4TransformArray(KVector4* pOut, UINT OutStride, CONST KVector4* pV, UINT VStride, CONST KMatrix* pM, UINT n)
	{
		return pOut;
	}


	static KMatrix* D3DKMatrixIdentity(KMatrix* pOut)
	{
		XMStoreFloat4x4(pOut, XMMatrixIdentity());
		return pOut;
	}
	static KMatrix* D3DKMatrixRotationQuaternion(KMatrix* pOut, CONST KQuaternion* pQ)
	{
		*pOut = KMatrix::CreateFromQuaternion(*pQ);
		return pOut;
	}
	static KQuaternion* D3DXQuaternionRotationMatrix(KQuaternion* pOut, CONST KMatrix* pM)
	{
		*pOut = KQuaternion::CreateFromRotationMatrix(*pM);
		return pOut;
	}

	static float D3DKMatrixDeterminant(CONST KMatrix* pM) {
		return pM->Determinant();
	}

	static HRESULT D3DKMatrixDecompose(KVector3* pOutScale, KQuaternion* pOutRotation,
		KVector3* pOutTranslation, KMatrix* pM)
	{
		if (pM->Decompose(*pOutScale, *pOutRotation, *pOutTranslation) == false)
		{
			return S_FALSE;
		}
		return S_OK;
	}

	static KMatrix* D3DKMatrixTranspose(KMatrix* pOut, CONST KMatrix* pM) {

		*pOut = pM->Transpose();
		return pOut;
	}

	// Matrix multiplication.  The result represents the transformation M2
	// followed by the transformation M1.  (Out = M1 * M2)
	static KMatrix* D3DKMatrixMultiply(KMatrix* pOut, CONST KMatrix* pM1, CONST KMatrix* pM2) {
		*pOut = (*pM1) * (*pM2);
		return pOut;
	}

	// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
	static KMatrix* D3DKMatrixMultiplyTranspose(KMatrix* pOut, CONST KMatrix* pM1, CONST KMatrix* pM2) {

		*pOut = (*pM1) * (*pM2);
		pOut->Transpose();
		return pOut;
	}

	// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
	// be returned.  The determinant of pM is also returned it pfDeterminant
	// is non-NULL.
	static KMatrix* D3DKMatrixInverse(KMatrix* pOut, float* pDeterminant, CONST KMatrix* pM) {
		pM->Invert(*pOut);
		if (pDeterminant != nullptr)
		{
			*pDeterminant = pM->Determinant();
		}
		return pOut;
	}

	// Build a matrix which scales by (sx, sy, sz)
	static KMatrix* D3DKMatrixScaling(KMatrix* pOut, float sx, float sy, float sz) {
		*pOut = KMatrix::CreateScale(sx, sy, sz);
		return pOut;
	}

	// Build a matrix which translates by (x, y, z)
	static KMatrix* D3DKMatrixTranslation(KMatrix* pOut, float x, float y, float z) {
		*pOut = KMatrix::CreateTranslation(x, y, z);
		return pOut;
	}

	// Build a matrix which rotates around the X axis
	static KMatrix* D3DKMatrixRotationX(KMatrix* pOut, float Angle) {
		*pOut = KMatrix::CreateRotationX(Angle);
		return pOut;
	}

	// Build a matrix which rotates around the Y axis
	static KMatrix* D3DKMatrixRotationY(KMatrix* pOut, float Angle) {
		*pOut = KMatrix::CreateRotationY(Angle);
		return pOut;
	}

	// Build a matrix which rotates around the Z axis
	static KMatrix* D3DKMatrixRotationZ(KMatrix* pOut, float Angle) {
		*pOut = KMatrix::CreateRotationZ(Angle);
		return pOut;
	}

	// Build a matrix which rotates around an arbitrary axis
	static KMatrix* D3DKMatrixRotationAxis(KMatrix* pOut, CONST KVector3* pV, float Angle) {
		*pOut = KMatrix::CreateFromAxisAngle(*pV, Angle);
		return pOut;
	}

	// Yaw around the Y axis, a pitch around the X axis,
	// and a roll around the Z axis.
	static KMatrix* D3DKMatrixRotationYawPitchRoll(KMatrix* pOut, float Yaw, float Pitch, float Roll) {
		*pOut = KMatrix::CreateFromYawPitchRoll(Yaw, Pitch, Roll);
		return pOut;
	}

	// Build transformation matrix.  NULL arguments are treated as identity.
	// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
	static KMatrix* D3DKMatrixTransformation(KMatrix* pOut, CONST KVector3* pScalingCenter,
		CONST KQuaternion* pScalingRotation, CONST KVector3* pScaling,
		CONST KVector3* pRotationCenter, CONST KQuaternion* pRotation,
		CONST KVector3* pTranslation)
	{
		return pOut;
	}

	// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
	// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
	static KMatrix* D3DKMatrixTransformation2D(KMatrix* pOut, CONST KVector2* pScalingCenter,
		float ScalingRotation, CONST KVector2* pScaling,
		CONST KVector2* pRotationCenter, float Rotation,
		CONST KVector2* pTranslation) {
		return pOut;
	}

	// Build affine transformation matrix.  NULL arguments are treated as identity.
	// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
	static KMatrix* D3DKMatrixAffineTransformation(KMatrix* pOut, float Scaling, CONST KVector3* pRotationCenter,
		CONST KQuaternion* pRotation, CONST KVector3* pTranslation)
	{
		XMVECTOR S = XMVectorReplicate(Scaling);//XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR O = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		if (pRotationCenter != NULL)
		{
			O = DirectX::XMLoadFloat3(pRotationCenter);
		}
		XMVECTOR P = DirectX::XMLoadFloat3(pTranslation);
		XMVECTOR Q = DirectX::XMLoadFloat4(pRotation);
		*pOut = DirectX::XMMatrixAffineTransformation(S, O, Q, P);
		return pOut;
	}

	// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
	// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
	static KMatrix* D3DKMatrixAffineTransformation2D(KMatrix* pOut, float Scaling, CONST KVector2* pRotationCenter,
		float Rotation, CONST KVector2* pTranslation) {
		return pOut;
	}

	// Build a lookat matrix. (right-handed)
	static KMatrix* D3DKMatrixLookAtRH(KMatrix* pOut, CONST KVector3* pEye, CONST KVector3* pAt, CONST KVector3* pUp) {
		return pOut;
	}

	// Build a lookat matrix. (left-handed)
	static KMatrix* D3DKMatrixLookAtLH(KMatrix* pOut, CONST KVector3* pEye, CONST KVector3* pAt, CONST KVector3* pUp) {
		*pOut = KMatrix::CreateLookAt(*pEye, *pAt, *pUp);
		return pOut;
	}

	// Build a perspective projection matrix. (right-handed)
	static KMatrix* D3DKMatrixPerspectiveRH(KMatrix* pOut, float w, float h, float zn, float zf) {
		return pOut;
	}

	// Build a perspective projection matrix. (left-handed)
	static KMatrix* D3DKMatrixPerspectiveLH(KMatrix* pOut, float w, float h, float zn, float zf) {
		*pOut = KMatrix::CreatePerspective(w, h, zn, zf);
		return pOut;
	}

	// Build a perspective projection matrix. (right-handed)
	static KMatrix* D3DKMatrixPerspectiveFovRH(KMatrix* pOut, float fovy, float Aspect, float zn, float zf)
	{
		using namespace DirectX;
		XMStoreFloat4x4(pOut, XMMatrixPerspectiveFovRH(fovy, Aspect, zn, zf));
		return pOut;
	}

	// Build a perspective projection matrix. (left-handed)
	static KMatrix* D3DKMatrixPerspectiveFovLH(KMatrix* pOut, float fovy, float Aspect, float zn, float zf)
	{
		using namespace DirectX;
		*pOut = KMatrix::CreatePerspectiveFieldOfView(fovy, Aspect, zn, zf);
		return pOut;
	}

	// Build a perspective projection matrix. (right-handed)
	static KMatrix* D3DKMatrixPerspectiveOffCenterRH(KMatrix* pOut, float l, float r, float b, float t, float zn, float zf) {
		return pOut;
	}

	// Build a perspective projection matrix. (left-handed)
	static KMatrix* D3DKMatrixPerspectiveOffCenterLH(KMatrix* pOut, float l, float r, float b, float t, float zn, float zf) {
		*pOut = KMatrix::CreatePerspectiveOffCenter(l, r, b, t, zn, zf);
		return pOut;
	}

	// Build an ortho projection matrix. (right-handed)
	static KMatrix* D3DKMatrixOrthoRH(KMatrix* pOut, float w, float h, float zn, float zf) {
		return pOut;
	}

	// Build an ortho projection matrix. (left-handed)
	static KMatrix* D3DKMatrixOrthoLH(KMatrix* pOut, float w, float h, float zn, float zf) {
		*pOut = KMatrix::CreateOrthographic(w, h, zn, zf);
		return pOut;
	}

	// Build an ortho projection matrix. (right-handed)
	static KMatrix* D3DKMatrixOrthoOffCenterRH(KMatrix* pOut, float l, float r, float b, float t, float zn, float zf) {
		return pOut;
	}

	// Build an ortho projection matrix. (left-handed)
	static KMatrix* D3DKMatrixOrthoOffCenterLH(KMatrix* pOut, float l, float r, float b, float t, float zn, float zf) {
		*pOut = KMatrix::CreateOrthographicOffCenter(l, r, b, t, zn, zf);
		return pOut;
	}

	// Build a matrix which flattens geometry into a plane, as if casting
	// a shadow from a light.
	static KMatrix* D3DKMatrixShadow(KMatrix* pOut, CONST KVector4* pLight, CONST KPlane* pPlane)
	{
		CONST KVector3 pLightLight = KVector3(pLight->x, pLight->y, pLight->z);
		*pOut = KMatrix::CreateShadow(pLightLight, *pPlane);
		return pOut;
	}

	// Build a matrix which reflects the coordinate system about a plane
	static KMatrix* D3DKMatrixReflect(KMatrix* pOut, CONST KPlane* pPlane) {
		*pOut = KMatrix::CreateReflection(*pPlane);
		return pOut;
	}

	//--------------------------
	// Quaternion
	//--------------------------

	// inline

	static float D3DXQuaternionLength(CONST KQuaternion* pQ)
	{
		return pQ->Length();
	};

	// Length squared, or "norm"
	static float D3DXQuaternionLengthSq(CONST KQuaternion* pQ)
	{
		return pQ->LengthSquared();
	};

	static float D3DXQuaternionDot(CONST KQuaternion* pQ1, CONST KQuaternion* pQ2)
	{
		return pQ1->Dot(*pQ2);;
	};

	// (0, 0, 0, 1)
	static KQuaternion* D3DXQuaternionIdentity(KQuaternion* pOut) {
		//*pOut = KQuaternion::Identity;
		XMStoreFloat4(pOut, XMQuaternionIdentity());
		return pOut;
	};

	static BOOL D3DXQuaternionIsIdentity(CONST KQuaternion* pQ) {
		return TRUE;
	};

	// (-x, -y, -z, w)
	static KQuaternion* D3DXQuaternionConjugate(KQuaternion* pOut, CONST KQuaternion* pQ) {
		return pOut;
	};


	// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
	static void D3DXQuaternionToAxisAngle(CONST KQuaternion* pQ, KVector3* pAxis, float* pAngle)
	{

	};

	static KQuaternion* D3DXQuaternionRotationAxis(KQuaternion* pOut, CONST KVector3* pV, FLOAT Angle)
	{
		*pOut = KQuaternion::CreateFromAxisAngle(*pV, Angle);
		return pOut;
	}

	// Yaw around the Y axis, a pitch around the X axis,
	// and a roll around the Z axis.
	static KQuaternion* D3DXQuaternionRotationYawPitchRoll(KQuaternion* pOut, float Yaw, float Pitch, float Roll)
	{
		*pOut = KQuaternion::CreateFromYawPitchRoll(Yaw, Pitch, Roll);
		return pOut;
	};

	// Quaternion multiplication.  The result represents the rotation Q2
	// followed by the rotation Q1.  (Out = Q2 * Q1)
	static KQuaternion* D3DXQuaternionMultiply(KQuaternion* pOut, CONST KQuaternion* pQ1,
		CONST KQuaternion* pQ2)
	{
		*pOut = *pQ1 * *pQ2;
		return pOut;
	};

	static KQuaternion* D3DXQuaternionNormalize(KQuaternion* pOut, CONST KQuaternion* pQ)
	{
		*pOut = *pQ;
		pOut->Normalize();
		return pOut;
	};

	// Conjugate and re-norm
	static KQuaternion* D3DXQuaternionInverse(KQuaternion* pOut, CONST KQuaternion* pQ)
	{
		KQuaternion qRet = *pQ;
		pQ->Inverse(qRet);
		*pOut = qRet;
		return pOut;
	};

	// Expects unit quaternions.
	// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
	static KQuaternion* D3DXQuaternionLn(KQuaternion* pOut, CONST KQuaternion* pQ) {
		return pOut;
	};

	// Expects pure quaternions. (w == 0)  w is ignored in calculation.
	// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
	static KQuaternion* D3DXQuaternionExp(KQuaternion* pOut, CONST KQuaternion* pQ) {
		return pOut;
	};

	// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
	// Expects unit quaternions.
	static KQuaternion* D3DXQuaternionSlerp(KQuaternion* pOut, CONST KQuaternion* pQ1, CONST KQuaternion* pQ2, float t)
	{
		*pOut = KQuaternion::Slerp(*pQ1, *pQ2, t);
		return pOut;
	};

	// Spherical quadrangle interpolation.
	// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
	static KQuaternion* D3DXQuaternionSquad(KQuaternion* pOut, CONST KQuaternion* pQ1,
		CONST KQuaternion* pA, CONST KQuaternion* pB,
		CONST KQuaternion* pC, float t) {
		return pOut;
	};

	// Setup control points for spherical quadrangle interpolation
	// from Q1 to Q2.  The control points are chosen in such a way 
	// to ensure the continuity of tangents with adjacent segments.
	static void D3DXQuaternionSquadSetup(KQuaternion* pAOut, KQuaternion* pBOut, KQuaternion* pCOut,
		CONST KQuaternion* pQ0, CONST KQuaternion* pQ1,
		CONST KQuaternion* pQ2, CONST KQuaternion* pQ3) {
	};

	// Barycentric interpolation.
	// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
	static KQuaternion* D3DXQuaternionBaryCentric(KQuaternion* pOut, CONST KQuaternion* pQ1,
		CONST KQuaternion* pQ2, CONST KQuaternion* pQ3,
		float f, float g) {
		return pOut;
	};


	//--------------------------
	// Plane
	//--------------------------

	// inline

	// ax + by + cz + dw
	static float D3DXPlaneDot(CONST KPlane* pP, CONST KVector4* pV) {
		return 0.0f;
	};
	// ax + by + cz + d
	static float D3DXPlaneDotCoord(CONST KPlane* pP, CONST KVector3* pV) {
		return 0.0f;
	};
	// ax + by + cz
	static float D3DXPlaneDotNormal(CONST KPlane* pP, CONST KVector3* pV) {
		return 0.0f;
	};
	static KPlane* D3DXPlaneScale(KPlane* pOut, CONST KPlane* pP, float s) {
		return pOut;
	};


	// Normalize plane (so that |a,b,c| == 1)
	static KPlane* D3DXPlaneNormalize(KPlane* pOut, CONST KPlane* pP) {
		return pOut;
	};

	// Find the intersection between a plane and a line.  If the line is
	// parallel to the plane, NULL is returned.
	static KVector3* D3DXPlaneIntersectLine(KVector3* pOut, CONST KPlane* pP, CONST KVector3* pV1,
		CONST KVector3* pV2) {
		return pOut;
	};

	// Construct a plane from a point and a normal
	static KPlane* D3DXPlaneFromPointNormal(KPlane* pOut, CONST KVector3* pPoint, CONST KVector3* pNormal) {
		return pOut;
	};

	// Construct a plane from 3 points
	static KPlane* D3DXPlaneFromPoints(KPlane* pOut, CONST KVector3* pV1, CONST KVector3* pV2,
		CONST KVector3* pV3) {
		return pOut;
	};

	// Transform a plane by a matrix.  The vector (a,b,c) must be normal.
	// M should be the inverse transpose of the transformation desired.
	static KPlane* D3DXPlaneTransform(KPlane* pOut, CONST KPlane* pP, CONST KMatrix* pM) {
		return pOut;
	};
	// Transform an array of planes by a matrix.  The vectors (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
	static KPlane* D3DXPlaneTransformArray(KPlane* pOut, UINT OutStride, CONST KPlane* pP, UINT PStride, CONST KMatrix* pM, UINT n) {
		return pOut;
	};



	//--------------------------
	// Color
	//--------------------------

	// inline

	// (1-r, 1-g, 1-b, a)
	static KColor* D3DXColorNegative(KColor* pOut, CONST KColor* pC) {
		return pOut;
	};

	static KColor* D3DXColorAdd(KColor* pOut, CONST KColor* pC1, CONST KColor* pC2) {
		return pOut;
	};

	static KColor* D3DXColorSubtract(KColor* pOut, CONST KColor* pC1, CONST KColor* pC2) {
		return pOut;
	};

	static KColor* D3DXColorScale(KColor* pOut, CONST KColor* pC, float s) {
		return pOut;
	};

	// (r1*r2, g1*g2, b1*b2, a1*a2)
	static KColor* D3DXColorModulate(KColor* pOut, CONST KColor* pC1, CONST KColor* pC2) {
		return pOut;
	};

	// Linear interpolation of r,g,b, and a. C1 + s(C2-C1)
	static KColor* D3DXColorLerp(KColor* pOut, CONST KColor* pC1, CONST KColor* pC2, float s) {
		return pOut;
	};


	// Interpolate r,g,b between desaturated color and color.
	// DesaturatedColor + s(Color - DesaturatedColor)
	static KColor* D3DXColorAdjustSaturation(KColor* pOut, CONST KColor* pC, float s) {
		return pOut;
	};
	// Interpolate r,g,b between 50% grey and color.  Grey + s(Color - Grey)
	static KColor* D3DXColorAdjustContrast(KColor* pOut, CONST KColor* pC, float c) {
		return pOut;
	};





	//--------------------------
	// Misc
	//--------------------------
		// Calculate Fresnel term given the cosine of theta (likely obtained by
		// taking the dot of two normals), and the refraction index of the material.
	static float D3DXFresnelTerm(float CosTheta, float RefractionIndex)
	{
		return 0.0f;
	};
}

//  ------------------------------------------------------------------------------
// Support for TMath and Standard C++ Library containers
namespace std
{
	template<> struct less<TBASIS_EX::KRectangle>
	{
		bool operator()(const TBASIS_EX::KRectangle& r1, const TBASIS_EX::KRectangle& r2) const
		{
			return ((r1.x < r2.x)
				|| ((r1.x == r2.x) && (r1.y < r2.y))
				|| ((r1.x == r2.x) && (r1.y == r2.y) && (r1.width < r2.width))
				|| ((r1.x == r2.x) && (r1.y == r2.y) && (r1.width == r2.width) && (r1.height < r2.height)));
		}
	};

	template<> struct less<TBASIS_EX::KVector2>
	{
		bool operator()(const TBASIS_EX::KVector2& V1, const TBASIS_EX::KVector2& V2) const
		{
			return ((V1.x < V2.x) || ((V1.x == V2.x) && (V1.y < V2.y)));
		}
	};

	template<> struct less<TBASIS_EX::KVector3>
	{
		bool operator()(const TBASIS_EX::KVector3& V1, const TBASIS_EX::KVector3& V2) const
		{
			return ((V1.x < V2.x)
				|| ((V1.x == V2.x) && (V1.y < V2.y))
				|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z)));
		}
	};

	template<> struct less<TBASIS_EX::KVector4>
	{
		bool operator()(const TBASIS_EX::KVector4& V1, const TBASIS_EX::KVector4& V2) const
		{
			return ((V1.x < V2.x)
				|| ((V1.x == V2.x) && (V1.y < V2.y))
				|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z))
				|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z == V2.z) && (V1.w < V2.w)));
		}
	};

	template<> struct less<TBASIS_EX::KMatrix>
	{
		bool operator()(const TBASIS_EX::KMatrix& M1, const TBASIS_EX::KMatrix& M2) const
		{
			if (M1._11 != M2._11) return M1._11 < M2._11;
			if (M1._12 != M2._12) return M1._12 < M2._12;
			if (M1._13 != M2._13) return M1._13 < M2._13;
			if (M1._14 != M2._14) return M1._14 < M2._14;
			if (M1._21 != M2._21) return M1._21 < M2._21;
			if (M1._22 != M2._22) return M1._22 < M2._22;
			if (M1._23 != M2._23) return M1._23 < M2._23;
			if (M1._24 != M2._24) return M1._24 < M2._24;
			if (M1._31 != M2._31) return M1._31 < M2._31;
			if (M1._32 != M2._32) return M1._32 < M2._32;
			if (M1._33 != M2._33) return M1._33 < M2._33;
			if (M1._34 != M2._34) return M1._34 < M2._34;
			if (M1._41 != M2._41) return M1._41 < M2._41;
			if (M1._42 != M2._42) return M1._42 < M2._42;
			if (M1._43 != M2._43) return M1._43 < M2._43;
			if (M1._44 != M2._44) return M1._44 < M2._44;

			return false;
		}
	};

	template<> struct less<TBASIS_EX::KPlane>
	{
		bool operator()(const TBASIS_EX::KPlane& P1, const TBASIS_EX::KPlane& P2) const
		{
			return ((P1.x < P2.x)
				|| ((P1.x == P2.x) && (P1.y < P2.y))
				|| ((P1.x == P2.x) && (P1.y == P2.y) && (P1.z < P2.z))
				|| ((P1.x == P2.x) && (P1.y == P2.y) && (P1.z == P2.z) && (P1.w < P2.w)));
		}
	};

	template<> struct less<TBASIS_EX::KQuaternion>
	{
		bool operator()(const TBASIS_EX::KQuaternion& Q1, const TBASIS_EX::KQuaternion& Q2) const
		{
			return ((Q1.x < Q2.x)
				|| ((Q1.x == Q2.x) && (Q1.y < Q2.y))
				|| ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z < Q2.z))
				|| ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z == Q2.z) && (Q1.w < Q2.w)));
		}
	};

	template<> struct less<TBASIS_EX::KColor>
	{
		bool operator()(const TBASIS_EX::KColor& C1, const TBASIS_EX::KColor& C2) const
		{
			return ((C1.x < C2.x)
				|| ((C1.x == C2.x) && (C1.y < C2.y))
				|| ((C1.x == C2.x) && (C1.y == C2.y) && (C1.z < C2.z))
				|| ((C1.x == C2.x) && (C1.y == C2.y) && (C1.z == C2.z) && (C1.w < C2.w)));
		}
	};

	template<> struct less<TBASIS_EX::TRay>
	{
		bool operator()(const TBASIS_EX::TRay& R1, const TBASIS_EX::TRay& R2) const
		{
			if (R1.position.x != R2.position.x) return R1.position.x < R2.position.x;
			if (R1.position.y != R2.position.y) return R1.position.y < R2.position.y;
			if (R1.position.z != R2.position.z) return R1.position.z < R2.position.z;

			if (R1.direction.x != R2.direction.x) return R1.direction.x < R2.direction.x;
			if (R1.direction.y != R2.direction.y) return R1.direction.y < R2.direction.y;
			if (R1.direction.z != R2.direction.z) return R1.direction.z < R2.direction.z;

			return false;
		}
	};

	template<> struct less<TBASIS_EX::TViewport>
	{
		bool operator()(const TBASIS_EX::TViewport& vp1, const TBASIS_EX::TViewport& vp2) const
		{
			if (vp1.x != vp2.x) return (vp1.x < vp2.x);
			if (vp1.y != vp2.y) return (vp1.y < vp2.y);

			if (vp1.width != vp2.width) return (vp1.width < vp2.width);
			if (vp1.height != vp2.height) return (vp1.height < vp2.height);

			if (vp1.minDepth != vp2.minDepth) return (vp1.minDepth < vp2.minDepth);
			if (vp1.maxDepth != vp2.maxDepth) return (vp1.maxDepth < vp2.maxDepth);

			return false;
		}
	};


};
using namespace TBASIS_EX;
