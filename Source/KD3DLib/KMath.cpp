#include "KMath.h"
//https://docs.microsoft.com/en-us/windows/win32/dxmath/pg-xnamath-migration-d3dx
namespace TBASIS_EX
{
	const KVector2 KVector2::Zero = { 0.f, 0.f };
	const KVector2 KVector2::One = { 1.f, 1.f };
	const KVector2 KVector2::UnitX = { 1.f, 0.f };
	const KVector2 KVector2::UnitY = { 0.f, 1.f };

	const KVector3 KVector3::Zero = { 0.f, 0.f, 0.f };
	const KVector3 KVector3::One = { 1.f, 1.f, 1.f };
	const KVector3 KVector3::UnitX = { 1.f, 0.f, 0.f };
	const KVector3 KVector3::UnitY = { 0.f, 1.f, 0.f };
	const KVector3 KVector3::UnitZ = { 0.f, 0.f, 1.f };
	const KVector3 KVector3::Up = { 0.f, 1.f, 0.f };
	const KVector3 KVector3::Down = { 0.f, -1.f, 0.f };
	const KVector3 KVector3::Right = { 1.f, 0.f, 0.f };
	const KVector3 KVector3::Left = { -1.f, 0.f, 0.f };
	const KVector3 KVector3::Forward = { 0.f, 0.f, -1.f };
	const KVector3 KVector3::Backward = { 0.f, 0.f, 1.f };

	const KVector4 KVector4::Zero = { 0.f, 0.f, 0.f, 0.f };
	const KVector4 KVector4::One = { 1.f, 1.f, 1.f, 1.f };
	const KVector4 KVector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
	const KVector4 KVector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
	const KVector4 KVector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
	const KVector4 KVector4::UnitW = { 0.f, 0.f, 0.f, 1.f };

	const KMatrix KMatrix::Identity = { 1.f, 0.f, 0.f, 0.f,
									  0.f, 1.f, 0.f, 0.f,
									  0.f, 0.f, 1.f, 0.f,
									  0.f, 0.f, 0.f, 1.f };

	const KQuaternion KQuaternion::Identity = { 0.f, 0.f, 0.f, 1.f };


	KVector2 KRectangle::Location() const
	{
		return KVector2(float(x), float(y));
	}

	KVector2 KRectangle::Center() const
	{
		return KVector2(float(x) + float(width / 2.f), float(y) + float(height / 2.f));
	}

	bool KRectangle::Contains(const KVector2& point) const
	{
		return (float(x) <= point.x) && (point.x < float(x + width)) && (float(y) <= point.y) && (point.y < float(y + height));
	}

	void KRectangle::Inflate(long horizAmount, long vertAmount)
	{
		x -= horizAmount;
		y -= vertAmount;
		width += horizAmount;
		height += vertAmount;
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	KRectangle KRectangle::Intersect(const KRectangle& ra, const KRectangle& rb)
	{
		long righta = ra.x + ra.width;
		long rightb = rb.x + rb.width;

		long bottoma = ra.y + ra.height;
		long bottomb = rb.y + rb.height;

		long maxX = ra.x > rb.x ? ra.x : rb.x;
		long maxY = ra.y > rb.y ? ra.y : rb.y;

		long minRight = righta < rightb ? righta : rightb;
		long minBottom = bottoma < bottomb ? bottoma : bottomb;

		KRectangle result;

		if ((minRight > maxX) && (minBottom > maxY))
		{
			result.x = maxX;
			result.y = maxY;
			result.width = minRight - maxX;
			result.height = minBottom - maxY;
		}
		else
		{
			result.x = 0;
			result.y = 0;
			result.width = 0;
			result.height = 0;
		}

		return result;
	}

	RECT KRectangle::Intersect(const RECT& rcta, const RECT& rctb)
	{
		long maxX = rcta.left > rctb.left ? rcta.left : rctb.left;
		long maxY = rcta.top > rctb.top ? rcta.top : rctb.top;

		long minRight = rcta.right < rctb.right ? rcta.right : rctb.right;
		long minBottom = rcta.bottom < rctb.bottom ? rcta.bottom : rctb.bottom;

		RECT result;

		if ((minRight > maxX) && (minBottom > maxY))
		{
			result.left = maxX;
			result.top = maxY;
			result.right = minRight;
			result.bottom = minBottom;
		}
		else
		{
			result.left = 0;
			result.top = 0;
			result.right = 0;
			result.bottom = 0;
		}

		return result;
	}

	KRectangle KRectangle::Union(const KRectangle& ra, const KRectangle& rb)
	{
		long righta = ra.x + ra.width;
		long rightb = rb.x + rb.width;

		long bottoma = ra.y + ra.height;
		long bottomb = rb.y + rb.height;

		int minX = ra.x < rb.x ? ra.x : rb.x;
		int minY = ra.y < rb.y ? ra.y : rb.y;

		int maxRight = righta > rightb ? righta : rightb;
		int maxBottom = bottoma > bottomb ? bottoma : bottomb;

		KRectangle result;
		result.x = minX;
		result.y = minY;
		result.width = maxRight - minX;
		result.height = maxBottom - minY;
		return result;
	}

	RECT KRectangle::Union(const RECT& rcta, const RECT& rctb)
	{
		RECT result;
		result.left = rcta.left < rctb.left ? rcta.left : rctb.left;
		result.top = rcta.top < rctb.top ? rcta.top : rctb.top;
		result.right = rcta.right > rctb.right ? rcta.right : rctb.right;
		result.bottom = rcta.bottom > rctb.bottom ? rcta.bottom : rctb.bottom;
		return result;
	}


	/****************************************************************************
	*
	* KVector2
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool KVector2::operator == (const KVector2& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		return XMVector2Equal(v1, v2);
	}

	bool KVector2::operator != (const KVector2& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		return XMVector2NotEqual(v1, v2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	KVector2& KVector2::operator+= (const KVector2& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorAdd(v1, v2);
		XMStoreFloat2(this, X);
		return *this;
	}

	KVector2& KVector2::operator-= (const KVector2& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		XMStoreFloat2(this, X);
		return *this;
	}

	KVector2& KVector2::operator*= (const KVector2& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		XMStoreFloat2(this, X);
		return *this;
	}

	KVector2& KVector2::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVectorScale(v1, S);
		XMStoreFloat2(this, X);
		return *this;
	}

	KVector2& KVector2::operator/= (float S)
	{
		using namespace DirectX;
		assert(S != 0.0f);
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVectorScale(v1, 1.f / S);
		XMStoreFloat2(this, X);
		return *this;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	KVector2 operator+ (const KVector2& V1, const KVector2& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V1);
		XMVECTOR v2 = XMLoadFloat2(&V2);
		XMVECTOR X = XMVectorAdd(v1, v2);
		KVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	KVector2 operator- (const KVector2& V1, const KVector2& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V1);
		XMVECTOR v2 = XMLoadFloat2(&V2);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		KVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	KVector2 operator* (const KVector2& V1, const KVector2& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V1);
		XMVECTOR v2 = XMLoadFloat2(&V2);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		KVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	KVector2 operator* (const KVector2& V, float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		KVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	KVector2 operator/ (const KVector2& V1, const KVector2& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V1);
		XMVECTOR v2 = XMLoadFloat2(&V2);
		XMVECTOR X = XMVectorDivide(v1, v2);
		KVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	KVector2 operator* (float S, const KVector2& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		KVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Vector operations
	//------------------------------------------------------------------------------

	bool KVector2::InBounds(const KVector2& Bounds) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&Bounds);
		return XMVector2InBounds(v1, v2);
	}

	float KVector2::Length() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVector2Length(v1);
		return XMVectorGetX(X);
	}

	float KVector2::LengthSquared() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVector2LengthSq(v1);
		return XMVectorGetX(X);
	}

	float KVector2::Dot(const KVector2& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR X = XMVector2Dot(v1, v2);
		return XMVectorGetX(X);
	}

	void KVector2::Cross(const KVector2& V, KVector2& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR R = XMVector2Cross(v1, v2);
		XMStoreFloat2(&result, R);
	}

	KVector2 KVector2::Cross(const KVector2& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR R = XMVector2Cross(v1, v2);

		KVector2 result;
		XMStoreFloat2(&result, R);
		return result;
	}

	void KVector2::Normalize()
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVector2Normalize(v1);
		XMStoreFloat2(this, X);
	}

	void KVector2::Normalize(KVector2& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVector2Normalize(v1);
		XMStoreFloat2(&result, X);
	}

	void KVector2::Clamp(const KVector2& vmin, const KVector2& vmax)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&vmin);
		XMVECTOR v3 = XMLoadFloat2(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat2(this, X);
	}

	void KVector2::Clamp(const KVector2& vmin, const KVector2& vmax, KVector2& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&vmin);
		XMVECTOR v3 = XMLoadFloat2(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat2(&result, X);
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	float KVector2::Distance(const KVector2& v1, const KVector2& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector2Length(V);
		return XMVectorGetX(X);
	}

	float KVector2::DistanceSquared(const KVector2& v1, const KVector2& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector2LengthSq(V);
		return XMVectorGetX(X);
	}

	void KVector2::Min(const KVector2& v1, const KVector2& v2, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::Min(const KVector2& v1, const KVector2& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::Max(const KVector2& v1, const KVector2& v2, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::Max(const KVector2& v1, const KVector2& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::Lerp(const KVector2& v1, const KVector2& v2, float t, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::Lerp(const KVector2& v1, const KVector2& v2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::SmoothStep(const KVector2& v1, const KVector2& v2, float t, KVector2& result)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t * (3.f - 2.f * t);
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::SmoothStep(const KVector2& v1, const KVector2& v2, float t)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t * (3.f - 2.f * t);
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::Barycentric(const KVector2& v1, const KVector2& v2, const KVector2& v3, float f, float g, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR x3 = XMLoadFloat2(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::Barycentric(const KVector2& v1, const KVector2& v2, const KVector2& v3, float f, float g)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR x3 = XMLoadFloat2(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::CatmullRom(const KVector2& v1, const KVector2& v2, const KVector2& v3, const KVector2& v4, float t, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR x3 = XMLoadFloat2(&v3);
		XMVECTOR x4 = XMLoadFloat2(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::CatmullRom(const KVector2& v1, const KVector2& v2, const KVector2& v3, const KVector2& v4, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR x3 = XMLoadFloat2(&v3);
		XMVECTOR x4 = XMLoadFloat2(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::Hermite(const KVector2& v1, const KVector2& t1, const KVector2& v2, const KVector2& t2, float t, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&t1);
		XMVECTOR x3 = XMLoadFloat2(&v2);
		XMVECTOR x4 = XMLoadFloat2(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::Hermite(const KVector2& v1, const KVector2& t1, const KVector2& v2, const KVector2& t2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&t1);
		XMVECTOR x3 = XMLoadFloat2(&v2);
		XMVECTOR x4 = XMLoadFloat2(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::Reflect(const KVector2& ivec, const KVector2& nvec, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat2(&ivec);
		XMVECTOR n = XMLoadFloat2(&nvec);
		XMVECTOR X = XMVector2Reflect(i, n);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::Reflect(const KVector2& ivec, const KVector2& nvec)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat2(&ivec);
		XMVECTOR n = XMLoadFloat2(&nvec);
		XMVECTOR X = XMVector2Reflect(i, n);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::Refract(const KVector2& ivec, const KVector2& nvec, float refractionIndex, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat2(&ivec);
		XMVECTOR n = XMLoadFloat2(&nvec);
		XMVECTOR X = XMVector2Refract(i, n, refractionIndex);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::Refract(const KVector2& ivec, const KVector2& nvec, float refractionIndex)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat2(&ivec);
		XMVECTOR n = XMLoadFloat2(&nvec);
		XMVECTOR X = XMVector2Refract(i, n, refractionIndex);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::Transform(const KVector2& v, const KQuaternion& quat, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMVECTOR q = XMLoadFloat4((DirectX::XMFLOAT4*)&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::Transform(const KVector2& v, const KQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMVECTOR q = XMLoadFloat4((DirectX::XMFLOAT4*)&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void KVector2::Transform(const KVector2& v, const KMatrix& m, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector2TransformCoord(v1, M);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::Transform(const KVector2& v, const KMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector2TransformCoord(v1, M);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void KVector2::Transform(const KVector2* varray, size_t count, const KMatrix& m, KVector2* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVector2TransformCoordStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
	}

	void KVector2::Transform(const KVector2& v, const KMatrix& m, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector2Transform(v1, M);
		XMStoreFloat4((DirectX::XMFLOAT4*)&result, X);
	}

	_Use_decl_annotations_
		void KVector2::Transform(const KVector2* varray, size_t count, const KMatrix& m, KVector4* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVector2TransformStream((DirectX::XMFLOAT4*)resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT2), count, M);
	}

	void KVector2::TransformNormal(const KVector2& v, const KMatrix& m, KVector2& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector2TransformNormal(v1, M);
		XMStoreFloat2(&result, X);
	}

	KVector2 KVector2::TransformNormal(const KVector2& v, const KMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector2TransformNormal(v1, M);

		KVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void KVector2::TransformNormal(const KVector2* varray, size_t count, const KMatrix& m, KVector2* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVector2TransformNormalStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
	}


	/****************************************************************************
	*
	* KVector3
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------
	bool KVector3::operator == (const KVector3& V) const
	{
		DirectX::XMVECTOR v1 = XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = XMLoadFloat3(&V);
		return DirectX::XMVector3Equal(v1, v2);
	}

	bool KVector3::operator != (const KVector3& V) const
	{
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		return XMVector3NotEqual(v1, v2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	KVector3& KVector3::operator+= (const KVector3& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorAdd(v1, v2);
		XMStoreFloat3(this, X);
		return *this;
	}

	KVector3& KVector3::operator-= (const KVector3& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		XMStoreFloat3(this, X);
		return *this;
	}

	KVector3& KVector3::operator*= (const KVector3& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		XMStoreFloat3(this, X);
		return *this;
	}

	KVector3& KVector3::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVectorScale(v1, S);
		XMStoreFloat3(this, X);
		return *this;
	}

	KVector3& KVector3::operator/= (float S)
	{
		using namespace DirectX;
		assert(S != 0.0f);
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVectorScale(v1, 1.f / S);
		XMStoreFloat3(this, X);
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	KVector3 KVector3::operator- () const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVectorNegate(v1);
		KVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	KVector3 operator+ (const KVector3& V1, const KVector3& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V1);
		XMVECTOR v2 = XMLoadFloat3(&V2);
		XMVECTOR X = XMVectorAdd(v1, v2);
		KVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	KVector3 operator- (const KVector3& V1, const KVector3& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V1);
		XMVECTOR v2 = XMLoadFloat3(&V2);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		KVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	KVector3 operator* (const KVector3& V1, const KVector3& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V1);
		XMVECTOR v2 = XMLoadFloat3(&V2);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		KVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	KVector3 operator* (const KVector3& V, float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		KVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	KVector3 operator/ (const KVector3& V1, const KVector3& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V1);
		XMVECTOR v2 = XMLoadFloat3(&V2);
		XMVECTOR X = XMVectorDivide(v1, v2);
		KVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	KVector3 operator* (float S, const KVector3& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		KVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Vector operations
	//------------------------------------------------------------------------------

	bool KVector3::InBounds(const KVector3& Bounds) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&Bounds);
		return XMVector3InBounds(v1, v2);
	}

	float KVector3::Length() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVector3Length(v1);
		return XMVectorGetX(X);
	}

	float KVector3::LengthSquared() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVector3LengthSq(v1);
		return XMVectorGetX(X);
	}

	float KVector3::Dot(const KVector3& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR X = XMVector3Dot(v1, v2);
		return XMVectorGetX(X);
	}

	void KVector3::Cross(const KVector3& V, KVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR R = XMVector3Cross(v1, v2);
		XMStoreFloat3(&result, R);
	}

	KVector3 KVector3::Cross(const KVector3& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR R = XMVector3Cross(v1, v2);

		KVector3 result;
		XMStoreFloat3(&result, R);
		return result;
	}

	void KVector3::Normalize()
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVector3Normalize(v1);
		XMStoreFloat3(this, X);
	}

	void KVector3::Normalize(KVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVector3Normalize(v1);
		XMStoreFloat3(&result, X);
	}

	void KVector3::Clamp(const KVector3& vmin, const KVector3& vmax)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&vmin);
		XMVECTOR v3 = XMLoadFloat3(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat3(this, X);
	}

	void KVector3::Clamp(const KVector3& vmin, const KVector3& vmax, KVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&vmin);
		XMVECTOR v3 = XMLoadFloat3(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat3(&result, X);
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------
	float KVector3::Distance(const KVector3& v1, const KVector3& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector3Length(V);
		return XMVectorGetX(X);
	}

	float KVector3::DistanceSquared(const KVector3& v1, const KVector3& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector3LengthSq(V);
		return XMVectorGetX(X);
	}

	void KVector3::Min(const KVector3& v1, const KVector3& v2, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::Min(const KVector3& v1, const KVector3& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::Max(const KVector3& v1, const KVector3& v2, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::Max(const KVector3& v1, const KVector3& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::Lerp(const KVector3& v1, const KVector3& v2, float t, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::Lerp(const KVector3& v1, const KVector3& v2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::SmoothStep(const KVector3& v1, const KVector3& v2, float t, KVector3& result)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t * (3.f - 2.f * t);
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::SmoothStep(const KVector3& v1, const KVector3& v2, float t)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t * (3.f - 2.f * t);
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::Barycentric(const KVector3& v1, const KVector3& v2, const KVector3& v3, float f, float g, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR x3 = XMLoadFloat3(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::Barycentric(const KVector3& v1, const KVector3& v2, const KVector3& v3, float f, float g)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR x3 = XMLoadFloat3(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::CatmullRom(const KVector3& v1, const KVector3& v2, const KVector3& v3, const KVector3& v4, float t, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR x3 = XMLoadFloat3(&v3);
		XMVECTOR x4 = XMLoadFloat3(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::CatmullRom(const KVector3& v1, const KVector3& v2, const KVector3& v3, const KVector3& v4, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR x3 = XMLoadFloat3(&v3);
		XMVECTOR x4 = XMLoadFloat3(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::Hermite(const KVector3& v1, const KVector3& t1, const KVector3& v2, const KVector3& t2, float t, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&t1);
		XMVECTOR x3 = XMLoadFloat3(&v2);
		XMVECTOR x4 = XMLoadFloat3(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::Hermite(const KVector3& v1, const KVector3& t1, const KVector3& v2, const KVector3& t2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&t1);
		XMVECTOR x3 = XMLoadFloat3(&v2);
		XMVECTOR x4 = XMLoadFloat3(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::Reflect(const KVector3& ivec, const KVector3& nvec, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat3(&ivec);
		XMVECTOR n = XMLoadFloat3(&nvec);
		XMVECTOR X = XMVector3Reflect(i, n);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::Reflect(const KVector3& ivec, const KVector3& nvec)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat3(&ivec);
		XMVECTOR n = XMLoadFloat3(&nvec);
		XMVECTOR X = XMVector3Reflect(i, n);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::Refract(const KVector3& ivec, const KVector3& nvec, float refractionIndex, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat3(&ivec);
		XMVECTOR n = XMLoadFloat3(&nvec);
		XMVECTOR X = XMVector3Refract(i, n, refractionIndex);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::Refract(const KVector3& ivec, const KVector3& nvec, float refractionIndex)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat3(&ivec);
		XMVECTOR n = XMLoadFloat3(&nvec);
		XMVECTOR X = XMVector3Refract(i, n, refractionIndex);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::Transform(const KVector3& v, const KQuaternion& quat, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMVECTOR q = XMLoadFloat4((XMFLOAT4*)&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::Transform(const KVector3& v, const KQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMVECTOR q = XMLoadFloat4((XMFLOAT4*)&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void KVector3::Transform(const KVector3& v, const KMatrix& m, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3TransformCoord(v1, M);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::Transform(const KVector3& v, const KMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3TransformCoord(v1, M);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void KVector3::Transform(const KVector3* varray, size_t count, const KMatrix& m, KVector3* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVector3TransformCoordStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
	}

	void KVector3::Transform(const KVector3& v, const KMatrix& m, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3Transform(v1, M);
		XMStoreFloat4(&result, X);
	}

	_Use_decl_annotations_
		void KVector3::Transform(const KVector3* varray, size_t count, const KMatrix& m, KVector4* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVector3TransformStream((XMFLOAT4*)resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT3), count, M);
	}

	void KVector3::TransformNormal(const KVector3& v, const KMatrix& m, KVector3& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3TransformNormal(v1, M);
		XMStoreFloat3(&result, X);
	}

	KVector3 KVector3::TransformNormal(const KVector3& v, const KMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3TransformNormal(v1, M);

		KVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void KVector3::TransformNormal(const KVector3* varray, size_t count, const KMatrix& m, KVector3* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVector3TransformNormalStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
	}


	///****************************************************************************
	//*
	//* KVector4
	//*
	//****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool KVector4::operator == (const KVector4& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		return XMVector4Equal(v1, v2);
	}

	bool KVector4::operator != (const KVector4& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		return XMVector4NotEqual(v1, v2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	KVector4& KVector4::operator+= (const KVector4& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorAdd(v1, v2);
		XMStoreFloat4(this, X);
		return *this;
	}

	KVector4& KVector4::operator-= (const KVector4& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		XMStoreFloat4(this, X);
		return *this;
	}

	KVector4& KVector4::operator*= (const KVector4& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		XMStoreFloat4(this, X);
		return *this;
	}

	KVector4& KVector4::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVectorScale(v1, S);
		XMStoreFloat4(this, X);
		return *this;
	}

	KVector4& KVector4::operator/= (float S)
	{
		using namespace DirectX;
		assert(S != 0.0f);
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVectorScale(v1, 1.f / S);
		XMStoreFloat4(this, X);
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	KVector4 KVector4::operator- () const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVectorNegate(v1);
		KVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	KVector4 operator+ (const KVector4& V1, const KVector4& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V1);
		XMVECTOR v2 = XMLoadFloat4(&V2);
		XMVECTOR X = XMVectorAdd(v1, v2);
		KVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	KVector4 operator- (const KVector4& V1, const KVector4& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V1);
		XMVECTOR v2 = XMLoadFloat4(&V2);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		KVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	KVector4 operator* (const KVector4& V1, const KVector4& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V1);
		XMVECTOR v2 = XMLoadFloat4(&V2);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		KVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	KVector4 operator* (const KVector4& V, float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		KVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	KVector4 operator/ (const KVector4& V1, const KVector4& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V1);
		XMVECTOR v2 = XMLoadFloat4(&V2);
		XMVECTOR X = XMVectorDivide(v1, v2);
		KVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	KVector4 operator* (float S, const KVector4& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		KVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Vector operations
	//------------------------------------------------------------------------------

	bool KVector4::InBounds(const KVector4& Bounds) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&Bounds);
		return XMVector4InBounds(v1, v2);
	}

	float KVector4::Length() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVector4Length(v1);
		return XMVectorGetX(X);
	}

	float KVector4::LengthSquared() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVector4LengthSq(v1);
		return XMVectorGetX(X);
	}

	float KVector4::Dot(const KVector4& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		XMVECTOR X = XMVector4Dot(v1, v2);
		return XMVectorGetX(X);
	}

	void KVector4::Cross(const KVector4& v1, const KVector4& v2, KVector4& result) const
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(this);
		XMVECTOR x2 = XMLoadFloat4(&v1);
		XMVECTOR x3 = XMLoadFloat4(&v2);
		XMVECTOR R = XMVector4Cross(x1, x2, x3);
		XMStoreFloat4(&result, R);
	}

	KVector4 KVector4::Cross(const KVector4& v1, const KVector4& v2) const
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(this);
		XMVECTOR x2 = XMLoadFloat4(&v1);
		XMVECTOR x3 = XMLoadFloat4(&v2);
		XMVECTOR R = XMVector4Cross(x1, x2, x3);

		KVector4 result;
		XMStoreFloat4(&result, R);
		return result;
	}

	void KVector4::Normalize()
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVector4Normalize(v1);
		XMStoreFloat4(this, X);
	}

	void KVector4::Normalize(KVector4& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVector4Normalize(v1);
		XMStoreFloat4(&result, X);
	}

	void KVector4::Clamp(const KVector4& vmin, const KVector4& vmax)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&vmin);
		XMVECTOR v3 = XMLoadFloat4(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat4(this, X);
	}

	void KVector4::Clamp(const KVector4& vmin, const KVector4& vmax, KVector4& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&vmin);
		XMVECTOR v3 = XMLoadFloat4(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat4(&result, X);
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	float KVector4::Distance(const KVector4& v1, const KVector4& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector4Length(V);
		return XMVectorGetX(X);
	}

	float KVector4::DistanceSquared(const KVector4& v1, const KVector4& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector4LengthSq(V);
		return XMVectorGetX(X);
	}

	void KVector4::Min(const KVector4& v1, const KVector4& v2, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Min(const KVector4& v1, const KVector4& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Max(const KVector4& v1, const KVector4& v2, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Max(const KVector4& v1, const KVector4& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Lerp(const KVector4& v1, const KVector4& v2, float t, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Lerp(const KVector4& v1, const KVector4& v2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::SmoothStep(const KVector4& v1, const KVector4& v2, float t, KVector4& result)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t * (3.f - 2.f * t);
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::SmoothStep(const KVector4& v1, const KVector4& v2, float t)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t * (3.f - 2.f * t);
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Barycentric(const KVector4& v1, const KVector4& v2, const KVector4& v3, float f, float g, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR x3 = XMLoadFloat4(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Barycentric(const KVector4& v1, const KVector4& v2, const KVector4& v3, float f, float g)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR x3 = XMLoadFloat4(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::CatmullRom(const KVector4& v1, const KVector4& v2, const KVector4& v3, const KVector4& v4, float t, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR x3 = XMLoadFloat4(&v3);
		XMVECTOR x4 = XMLoadFloat4(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::CatmullRom(const KVector4& v1, const KVector4& v2, const KVector4& v3, const KVector4& v4, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR x3 = XMLoadFloat4(&v3);
		XMVECTOR x4 = XMLoadFloat4(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Hermite(const KVector4& v1, const KVector4& t1, const KVector4& v2, const KVector4& t2, float t, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&t1);
		XMVECTOR x3 = XMLoadFloat4(&v2);
		XMVECTOR x4 = XMLoadFloat4(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Hermite(const KVector4& v1, const KVector4& t1, const KVector4& v2, const KVector4& t2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&t1);
		XMVECTOR x3 = XMLoadFloat4(&v2);
		XMVECTOR x4 = XMLoadFloat4(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Reflect(const KVector4& ivec, const KVector4& nvec, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat4(&ivec);
		XMVECTOR n = XMLoadFloat4(&nvec);
		XMVECTOR X = XMVector4Reflect(i, n);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Reflect(const KVector4& ivec, const KVector4& nvec)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat4(&ivec);
		XMVECTOR n = XMLoadFloat4(&nvec);
		XMVECTOR X = XMVector4Reflect(i, n);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Refract(const KVector4& ivec, const KVector4& nvec, float refractionIndex, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat4(&ivec);
		XMVECTOR n = XMLoadFloat4(&nvec);
		XMVECTOR X = XMVector4Refract(i, n, refractionIndex);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Refract(const KVector4& ivec, const KVector4& nvec, float refractionIndex)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat4(&ivec);
		XMVECTOR n = XMLoadFloat4(&nvec);
		XMVECTOR X = XMVector4Refract(i, n, refractionIndex);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Transform(const KVector2& v, const KQuaternion& quat, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Transform(const KVector2& v, const KQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Transform(const KVector3& v, const KQuaternion& quat, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Transform(const KVector3& v, const KQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Transform(const KVector4& v, const KQuaternion& quat, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Transform(const KVector4& v, const KQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void KVector4::Transform(const KVector4& v, const KMatrix& m, KVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector4Transform(v1, M);
		XMStoreFloat4(&result, X);
	}

	KVector4 KVector4::Transform(const KVector4& v, const KMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector4Transform(v1, M);

		KVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void KVector4::Transform(const KVector4* varray, size_t count, const KMatrix& m, KVector4* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVector4TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT4), count, M);
	}


	/****************************************************************************
	*
	* KMatrix
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool KMatrix::operator == (const KMatrix& M) const
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		return (XMVector4Equal(x1, y1)
			&& XMVector4Equal(x2, y2)
			&& XMVector4Equal(x3, y3)
			&& XMVector4Equal(x4, y4)) != 0;
	}

	bool KMatrix::operator != (const KMatrix& M) const
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		return (XMVector4NotEqual(x1, y1)
			|| XMVector4NotEqual(x2, y2)
			|| XMVector4NotEqual(x3, y3)
			|| XMVector4NotEqual(x4, y4)) != 0;
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	KMatrix::KMatrix(const XMFLOAT3X3& M)
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
	}

	KMatrix::KMatrix(const XMFLOAT4X3& M)
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
	}

	KMatrix& KMatrix::operator= (const XMFLOAT3X3& M)
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
		return *this;
	}

	KMatrix& KMatrix::operator= (const XMFLOAT4X3& M)
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
		return *this;
	}

	KMatrix& KMatrix::operator+= (const KMatrix& M)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorAdd(x1, y1);
		x2 = XMVectorAdd(x2, y2);
		x3 = XMVectorAdd(x3, y3);
		x4 = XMVectorAdd(x4, y4);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	KMatrix& KMatrix::operator-= (const KMatrix& M)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorSubtract(x1, y1);
		x2 = XMVectorSubtract(x2, y2);
		x3 = XMVectorSubtract(x3, y3);
		x4 = XMVectorSubtract(x4, y4);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	KMatrix& KMatrix::operator*= (const KMatrix& M)
	{
		using namespace DirectX;
		XMMATRIX M1 = XMLoadFloat4x4(this);
		XMMATRIX M2 = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMMATRIX X = XMMatrixMultiply(M1, M2);
		XMStoreFloat4x4(this, X);
		return *this;
	}

	KMatrix& KMatrix::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		x1 = XMVectorScale(x1, S);
		x2 = XMVectorScale(x2, S);
		x3 = XMVectorScale(x3, S);
		x4 = XMVectorScale(x4, S);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	KMatrix& KMatrix::operator/= (float S)
	{
		using namespace DirectX;
		assert(S != 0.f);
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		float rs = 1.f / S;

		x1 = XMVectorScale(x1, rs);
		x2 = XMVectorScale(x2, rs);
		x3 = XMVectorScale(x3, rs);
		x4 = XMVectorScale(x4, rs);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	KMatrix& KMatrix::operator/= (const KMatrix& M)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorDivide(x1, y1);
		x2 = XMVectorDivide(x2, y2);
		x3 = XMVectorDivide(x3, y3);
		x4 = XMVectorDivide(x4, y4);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	KMatrix KMatrix::operator- () const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		v1 = XMVectorNegate(v1);
		v2 = XMVectorNegate(v2);
		v3 = XMVectorNegate(v3);
		v4 = XMVectorNegate(v4);

		KMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), v1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), v2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), v3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), v4);
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	KMatrix operator+ (const KMatrix& M1, const KMatrix& M2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorAdd(x1, y1);
		x2 = XMVectorAdd(x2, y2);
		x3 = XMVectorAdd(x3, y3);
		x4 = XMVectorAdd(x4, y4);

		KMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	KMatrix operator- (const KMatrix& M1, const KMatrix& M2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorSubtract(x1, y1);
		x2 = XMVectorSubtract(x2, y2);
		x3 = XMVectorSubtract(x3, y3);
		x4 = XMVectorSubtract(x4, y4);

		KMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	KMatrix operator* (const KMatrix& M1, const KMatrix& M2)
	{
		using namespace DirectX;
		XMMATRIX m1 = XMLoadFloat4x4(&M1);
		XMMATRIX m2 = XMLoadFloat4x4((XMFLOAT4X4*)&M2);
		XMMATRIX X = XMMatrixMultiply(m1, m2);

		KMatrix R;
		XMStoreFloat4x4(&R, X);
		return R;
	}

	KMatrix operator* (const KMatrix& M, float S)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorScale(x1, S);
		x2 = XMVectorScale(x2, S);
		x3 = XMVectorScale(x3, S);
		x4 = XMVectorScale(x4, S);

		KMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	KMatrix operator/ (const KMatrix& M, float S)
	{
		using namespace DirectX;
		assert(S != 0.f);

		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		float rs = 1.f / S;

		x1 = XMVectorScale(x1, rs);
		x2 = XMVectorScale(x2, rs);
		x3 = XMVectorScale(x3, rs);
		x4 = XMVectorScale(x4, rs);

		KMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	KMatrix operator/ (const KMatrix& M1, const KMatrix& M2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorDivide(x1, y1);
		x2 = XMVectorDivide(x2, y2);
		x3 = XMVectorDivide(x3, y3);
		x4 = XMVectorDivide(x4, y4);

		KMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	KMatrix operator* (float S, const KMatrix& M)
	{
		using namespace DirectX;

		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorScale(x1, S);
		x2 = XMVectorScale(x2, S);
		x3 = XMVectorScale(x3, S);
		x4 = XMVectorScale(x4, S);

		KMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	//------------------------------------------------------------------------------
	// KMatrix operations
	//------------------------------------------------------------------------------


	bool KMatrix::Decompose(KVector3& scale, KQuaternion& rotation, KVector3& translation)
	{
		using namespace DirectX;

		XMVECTOR s, r, t;

		if (!XMMatrixDecompose(&s, &r, &t, *(XMMATRIX*)this))
			return false;

		XMStoreFloat3(&scale, s);
		XMStoreFloat4(&rotation, r);
		XMStoreFloat3(&translation, t);

		return true;
	}

	KMatrix KMatrix::Transpose() const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixTranspose(M));
		return R;
	}

	void KMatrix::Transpose(KMatrix& result) const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		XMStoreFloat4x4(&result, XMMatrixTranspose(M));
	}

	KMatrix KMatrix::Invert() const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		KMatrix R;
		XMVECTOR det;
		XMStoreFloat4x4(&R, XMMatrixInverse(&det, M));
		return R;
	}

	void KMatrix::Invert(KMatrix& result) const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		XMVECTOR det;
		XMStoreFloat4x4(&result, XMMatrixInverse(&det, M));
	}

	float KMatrix::Determinant() const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		return XMVectorGetX(XMMatrixDeterminant(M));
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	_Use_decl_annotations_
		KMatrix KMatrix::CreateBillboard(const KVector3& object, const KVector3& cameraPosition, const KVector3& cameraUp, const KVector3* cameraForward)
	{
		using namespace DirectX;
		XMVECTOR O = XMLoadFloat3(&object);
		XMVECTOR C = XMLoadFloat3(&cameraPosition);
		XMVECTOR Z = XMVectorSubtract(O, C);

		XMVECTOR N = XMVector3LengthSq(Z);
		if (XMVector3Less(N, g_XMEpsilon))
		{
			if (cameraForward)
			{
				XMVECTOR F = XMLoadFloat3(cameraForward);
				Z = XMVectorNegate(F);
			}
			else
				Z = g_XMNegIdentityR2;
		}
		else
		{
			Z = XMVector3Normalize(Z);
		}

		XMVECTOR up = XMLoadFloat3(&cameraUp);
		XMVECTOR X = XMVector3Cross(up, Z);
		X = XMVector3Normalize(X);

		XMVECTOR Y = XMVector3Cross(Z, X);

		XMMATRIX M;
		M.r[0] = X;
		M.r[1] = Y;
		M.r[2] = Z;
		M.r[3] = XMVectorSetW(O, 1.f);

		KMatrix R;
		XMStoreFloat4x4(&R, M);
		return R;
	}

	_Use_decl_annotations_
		KMatrix KMatrix::CreateConstrainedBillboard(const KVector3& object, const KVector3& cameraPosition, const KVector3& rotateAxis,
			const KVector3* cameraForward, const KVector3* objectForward)
	{
		using namespace DirectX;

		static const XMVECTORF32 s_minAngle = { { { 0.99825467075f, 0.99825467075f, 0.99825467075f, 0.99825467075f } } }; // 1.0 - XMConvertToRadians( 0.1f );

		XMVECTOR O = XMLoadFloat3(&object);
		XMVECTOR C = XMLoadFloat3(&cameraPosition);
		XMVECTOR faceDir = XMVectorSubtract(O, C);

		XMVECTOR N = XMVector3LengthSq(faceDir);
		if (XMVector3Less(N, g_XMEpsilon))
		{
			if (cameraForward)
			{
				XMVECTOR F = XMLoadFloat3(cameraForward);
				faceDir = XMVectorNegate(F);
			}
			else
				faceDir = g_XMNegIdentityR2;
		}
		else
		{
			faceDir = XMVector3Normalize(faceDir);
		}

		XMVECTOR Y = XMLoadFloat3(&rotateAxis);
		XMVECTOR X, Z;

		XMVECTOR dot = XMVectorAbs(XMVector3Dot(Y, faceDir));
		if (XMVector3Greater(dot, s_minAngle))
		{
			if (objectForward)
			{
				Z = XMLoadFloat3(objectForward);
				dot = XMVectorAbs(XMVector3Dot(Y, Z));
				if (XMVector3Greater(dot, s_minAngle))
				{
					dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
					Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
				}
			}
			else
			{
				dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
				Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
			}

			X = XMVector3Cross(Y, Z);
			X = XMVector3Normalize(X);

			Z = XMVector3Cross(X, Y);
			Z = XMVector3Normalize(Z);
		}
		else
		{
			X = XMVector3Cross(Y, faceDir);
			X = XMVector3Normalize(X);

			Z = XMVector3Cross(X, Y);
			Z = XMVector3Normalize(Z);
		}

		XMMATRIX M;
		M.r[0] = X;
		M.r[1] = Y;
		M.r[2] = Z;
		M.r[3] = XMVectorSetW(O, 1.f);

		KMatrix R;
		XMStoreFloat4x4(&R, M);
		return R;
	}

	KMatrix KMatrix::CreateTranslation(const KVector3& position)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixTranslation(position.x, position.y, position.z));
		return R;
	}

	KMatrix KMatrix::CreateTranslation(float x, float y, float z)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixTranslation(x, y, z));
		return R;
	}

	KMatrix KMatrix::CreateScale(const KVector3& scales)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixScaling(scales.x, scales.y, scales.z));
		return R;
	}

	KMatrix KMatrix::CreateScale(float xs, float ys, float zs)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixScaling(xs, ys, zs));
		return R;
	}

	KMatrix KMatrix::CreateScale(float scale)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixScaling(scale, scale, scale));
		return R;
	}

	KMatrix KMatrix::CreateRotationX(float radians)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixRotationX(radians));
		return R;
	}

	KMatrix KMatrix::CreateRotationY(float radians)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixRotationY(radians));
		return R;
	}

	KMatrix KMatrix::CreateRotationZ(float radians)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixRotationZ(radians));
		return R;
	}

	KMatrix KMatrix::CreateFromAxisAngle(const KVector3& axis, float angle)
	{
		using namespace DirectX;
		KMatrix R;
		XMVECTOR a = XMLoadFloat3(&axis);
		XMStoreFloat4x4(&R, XMMatrixRotationAxis(a, angle));
		return R;
	}

	KMatrix KMatrix::CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
		return R;
	}

	KMatrix KMatrix::CreatePerspective(float width, float height, float nearPlane, float farPlane)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixPerspectiveLH(width, height, nearPlane, farPlane));
		return R;
	}

	KMatrix KMatrix::CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixPerspectiveOffCenterLH(left, right, bottom, top, nearPlane, farPlane));
		return R;
	}

	KMatrix KMatrix::CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixOrthographicLH(width, height, zNearPlane, zFarPlane));
		return R;
	}

	KMatrix KMatrix::CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixOrthographicOffCenterLH(left, right, bottom, top, zNearPlane, zFarPlane));
		return R;
	}

	KMatrix KMatrix::CreateLookAt(const KVector3& eye, const KVector3& target, const KVector3& up)
	{
		using namespace DirectX;
		KMatrix R;
		XMVECTOR eyev = XMLoadFloat3(&eye);
		XMVECTOR targetv = XMLoadFloat3(&target);
		XMVECTOR upv = XMLoadFloat3(&up);
		XMStoreFloat4x4(&R, XMMatrixLookAtLH(eyev, targetv, upv));
		return R;
	}

	KMatrix KMatrix::CreateWorld(const KVector3& position, const KVector3& forward, const KVector3& up)
	{
		using namespace DirectX;
		XMVECTOR zaxis = XMVector3Normalize(XMVectorNegate(XMLoadFloat3(&forward)));
		XMVECTOR yaxis = XMLoadFloat3(&up);
		XMVECTOR xaxis = XMVector3Normalize(XMVector3Cross(yaxis, zaxis));
		yaxis = XMVector3Cross(zaxis, xaxis);

		KMatrix R;
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._11), xaxis);
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._21), yaxis);
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._31), zaxis);
		R._14 = R._24 = R._34 = 0.f;
		R._41 = position.x; R._42 = position.y; R._43 = position.z;
		R._44 = 1.f;
		return R;
	}

	KMatrix KMatrix::CreateFromQuaternion(const KQuaternion& rotation)
	{
		using namespace DirectX;
		KMatrix R;
		XMVECTOR quatv = XMLoadFloat4(&rotation);
		XMStoreFloat4x4(&R, XMMatrixRotationQuaternion(quatv));
		return R;
	}

	KMatrix KMatrix::CreateFromYawPitchRoll(float yaw, float pitch, float roll)
	{
		using namespace DirectX;
		KMatrix R;
		XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
		return R;
	}

	KMatrix KMatrix::CreateShadow(const KVector3& lightDir, const KPlane& plane)
	{
		using namespace DirectX;
		KMatrix R;
		XMVECTOR light = XMLoadFloat3(&lightDir);
		XMVECTOR planev = XMLoadFloat4(&plane);
		XMStoreFloat4x4(&R, XMMatrixShadow(planev, light));
		return R;
	}

	KMatrix KMatrix::CreateReflection(const KPlane& plane)
	{
		using namespace DirectX;
		KMatrix R;
		XMVECTOR planev = XMLoadFloat4(&plane);
		XMStoreFloat4x4(&R, XMMatrixReflect(planev));
		return R;
	}

	void KMatrix::Lerp(const KMatrix& M1, const KMatrix& M2, float t, KMatrix& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorLerp(x1, y1, t);
		x2 = XMVectorLerp(x2, y2, t);
		x3 = XMVectorLerp(x3, y3, t);
		x4 = XMVectorLerp(x4, y4, t);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
	}

	KMatrix KMatrix::Lerp(const KMatrix& M1, const KMatrix& M2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorLerp(x1, y1, t);
		x2 = XMVectorLerp(x2, y2, t);
		x3 = XMVectorLerp(x3, y3, t);
		x4 = XMVectorLerp(x4, y4, t);

		KMatrix result;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
		return result;
	}

	void KMatrix::Transform(const KMatrix& M, const KQuaternion& rotation, KMatrix& result)
	{
		using namespace DirectX;
		XMVECTOR quatv = XMLoadFloat4(&rotation);

		XMMATRIX M0 = XMLoadFloat4x4(&M);
		XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

		XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
	}

	KMatrix KMatrix::Transform(const KMatrix& M, const KQuaternion& rotation)
	{
		using namespace DirectX;
		XMVECTOR quatv = XMLoadFloat4(&rotation);

		XMMATRIX M0 = XMLoadFloat4x4(&M);
		XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

		KMatrix result;
		XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
		return result;
	}


	/****************************************************************************
	*
	* KPlane
	*
	****************************************************************************/

	KPlane::KPlane(const KVector3& point1, const KVector3& point2, const KVector3& point3)
	{
		using namespace DirectX;
		XMVECTOR P0 = XMLoadFloat3(&point1);
		XMVECTOR P1 = XMLoadFloat3(&point2);
		XMVECTOR P2 = XMLoadFloat3(&point3);
		XMStoreFloat4(this, XMPlaneFromPoints(P0, P1, P2));
	}

	KPlane::KPlane(const KVector3& point, const KVector3& normal)
	{
		using namespace DirectX;
		XMVECTOR P = XMLoadFloat3(&point);
		XMVECTOR N = XMLoadFloat3(&normal);
		XMStoreFloat4(this, XMPlaneFromPointNormal(P, N));
	}

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool KPlane::operator == (const KPlane& p) const
	{
		using namespace DirectX;
		XMVECTOR p1 = XMLoadFloat4(this);
		XMVECTOR p2 = XMLoadFloat4(&p);
		return XMPlaneEqual(p1, p2);
	}

	bool KPlane::operator != (const KPlane& p) const
	{
		using namespace DirectX;
		XMVECTOR p1 = XMLoadFloat4(this);
		XMVECTOR p2 = XMLoadFloat4(&p);
		return XMPlaneNotEqual(p1, p2);
	}

	//------------------------------------------------------------------------------
	// KPlane operations
	//------------------------------------------------------------------------------

	void KPlane::Normalize()
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMStoreFloat4(this, XMPlaneNormalize(p));
	}

	void KPlane::Normalize(KPlane& result) const
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMPlaneNormalize(p));
	}

	float KPlane::Dot(const KVector4& v) const
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMVECTOR v0 = XMLoadFloat4(&v);
		return XMVectorGetX(XMPlaneDot(p, v0));
	}

	float KPlane::DotCoordinate(const KVector3& position) const
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMVECTOR v0 = XMLoadFloat3(&position);
		return XMVectorGetX(XMPlaneDotCoord(p, v0));
	}

	float KPlane::DotNormal(const KVector3& normal) const
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMVECTOR n0 = XMLoadFloat3(&normal);
		return XMVectorGetX(XMPlaneDotNormal(p, n0));
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	void KPlane::Transform(const KPlane& plane, const KMatrix& M, KPlane& result)
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(&plane);
		XMMATRIX m0 = XMLoadFloat4x4(&M);
		XMStoreFloat4(&result, XMPlaneTransform(p, m0));
	}

	KPlane KPlane::Transform(const KPlane& plane, const KMatrix& M)
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(&plane);
		XMMATRIX m0 = XMLoadFloat4x4(&M);

		KPlane result;
		XMStoreFloat4(&result, XMPlaneTransform(p, m0));
		return result;
	}

	void KPlane::Transform(const KPlane& plane, const KQuaternion& rotation, KPlane& result)
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(&plane);
		XMVECTOR q = XMLoadFloat4(&rotation);
		XMVECTOR X = XMVector3Rotate(p, q);
		X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d
		XMStoreFloat4(&result, X);
	}

	KPlane KPlane::Transform(const KPlane& plane, const KQuaternion& rotation)
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(&plane);
		XMVECTOR q = XMLoadFloat4(&rotation);
		XMVECTOR X = XMVector3Rotate(p, q);
		X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d

		KPlane result;
		XMStoreFloat4(&result, X);
		return result;
	}


	/****************************************************************************
	*
	* KQuaternion
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool KQuaternion::operator == (const KQuaternion& q) const
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		return XMQuaternionEqual(q1, q2);
	}

	bool KQuaternion::operator != (const KQuaternion& q) const
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		return XMQuaternionNotEqual(q1, q2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	KQuaternion& KQuaternion::operator+= (const KQuaternion& q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		XMStoreFloat4(this, XMVectorAdd(q1, q2));
		return *this;
	}

	KQuaternion& KQuaternion::operator-= (const KQuaternion& q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		XMStoreFloat4(this, XMVectorSubtract(q1, q2));
		return *this;
	}

	KQuaternion& KQuaternion::operator*= (const KQuaternion& q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
		return *this;
	}

	KQuaternion& KQuaternion::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(this, XMVectorScale(q, S));
		return *this;
	}

	KQuaternion& KQuaternion::operator/= (const KQuaternion& q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		q2 = XMQuaternionInverse(q2);
		XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	KQuaternion KQuaternion::operator- () const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);

		KQuaternion R;
		XMStoreFloat4(&R, XMVectorNegate(q));
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	KQuaternion operator+ (const KQuaternion& Q1, const KQuaternion& Q2)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q1);
		XMVECTOR q2 = XMLoadFloat4(&Q2);

		KQuaternion R;
		XMStoreFloat4(&R, XMVectorAdd(q1, q2));
		return R;
	}

	KQuaternion operator- (const KQuaternion& Q1, const KQuaternion& Q2)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q1);
		XMVECTOR q2 = XMLoadFloat4(&Q2);

		KQuaternion R;
		XMStoreFloat4(&R, XMVectorSubtract(q1, q2));
		return R;
	}

	KQuaternion operator* (const KQuaternion& Q1, const KQuaternion& Q2)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q1);
		XMVECTOR q2 = XMLoadFloat4(&Q2);

		KQuaternion R;
		XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
		return R;
	}

	KQuaternion operator* (const KQuaternion& Q, float S)
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(&Q);

		KQuaternion R;
		XMStoreFloat4(&R, XMVectorScale(q, S));
		return R;
	}

	KQuaternion operator/ (const KQuaternion& Q1, const KQuaternion& Q2)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q1);
		XMVECTOR q2 = XMLoadFloat4(&Q2);
		q2 = XMQuaternionInverse(q2);

		KQuaternion R;
		XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
		return R;
	}

	KQuaternion operator* (float S, const KQuaternion& Q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q);

		KQuaternion R;
		XMStoreFloat4(&R, XMVectorScale(q1, S));
		return R;
	}

	//------------------------------------------------------------------------------
	// KQuaternion operations
	//------------------------------------------------------------------------------

	float KQuaternion::Length() const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		return XMVectorGetX(XMQuaternionLength(q));
	}

	float KQuaternion::LengthSquared() const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		return XMVectorGetX(XMQuaternionLengthSq(q));
	}

	void KQuaternion::Normalize()
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(this, XMQuaternionNormalize(q));
	}

	void KQuaternion::Normalize(KQuaternion& result) const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMQuaternionNormalize(q));
	}

	void KQuaternion::Conjugate()
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(this, XMQuaternionConjugate(q));
	}

	void KQuaternion::Conjugate(KQuaternion& result) const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMQuaternionConjugate(q));
	}

	void KQuaternion::Inverse(KQuaternion& result) const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMQuaternionInverse(q));
	}

	float KQuaternion::Dot(const KQuaternion& q) const
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		return XMVectorGetX(XMQuaternionDot(q1, q2));
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	KQuaternion KQuaternion::CreateFromAxisAngle(const KVector3& axis, float angle)
	{
		using namespace DirectX;
		XMVECTOR a = XMLoadFloat3(&axis);

		KQuaternion R;
		XMStoreFloat4(&R, XMQuaternionRotationAxis(a, angle));
		return R;
	}

	KQuaternion KQuaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll)
	{
		using namespace DirectX;
		KQuaternion R;
		XMStoreFloat4(&R, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
		return R;
	}

	KQuaternion KQuaternion::CreateFromRotationMatrix(const KMatrix& M)
	{
		using namespace DirectX;
		XMMATRIX M0 = XMLoadFloat4x4(&M);

		KQuaternion R;
		XMStoreFloat4(&R, XMQuaternionRotationMatrix(M0));
		return R;
	}

	void KQuaternion::Lerp(const KQuaternion& q1, const KQuaternion& q2, float t, KQuaternion& result)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);

		XMVECTOR dot = XMVector4Dot(Q0, Q1);

		XMVECTOR R;
		if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
		{
			R = XMVectorLerp(Q0, Q1, t);
		}
		else
		{
			XMVECTOR tv = XMVectorReplicate(t);
			XMVECTOR t1v = XMVectorReplicate(1.f - t);
			XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
			XMVECTOR X1 = XMVectorMultiply(Q1, tv);
			R = XMVectorSubtract(X0, X1);
		}

		XMStoreFloat4(&result, XMQuaternionNormalize(R));
	}

	KQuaternion KQuaternion::Lerp(const KQuaternion& q1, const KQuaternion& q2, float t)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);

		XMVECTOR dot = XMVector4Dot(Q0, Q1);

		XMVECTOR R;
		if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
		{
			R = XMVectorLerp(Q0, Q1, t);
		}
		else
		{
			XMVECTOR tv = XMVectorReplicate(t);
			XMVECTOR t1v = XMVectorReplicate(1.f - t);
			XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
			XMVECTOR X1 = XMVectorMultiply(Q1, tv);
			R = XMVectorSubtract(X0, X1);
		}

		KQuaternion result;
		XMStoreFloat4(&result, XMQuaternionNormalize(R));
		return result;
	}

	void KQuaternion::Slerp(const KQuaternion& q1, const KQuaternion& q2, float t, KQuaternion& result)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);
		XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
	}

	KQuaternion KQuaternion::Slerp(const KQuaternion& q1, const KQuaternion& q2, float t)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);

		KQuaternion result;
		XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
		return result;
	}

	void KQuaternion::Concatenate(const KQuaternion& q1, const KQuaternion& q2, KQuaternion& result)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);
		XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
	}

	KQuaternion KQuaternion::Concatenate(const KQuaternion& q1, const KQuaternion& q2)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);

		KQuaternion result;
		XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
		return result;
	}


	/****************************************************************************
	*
	* KColor
	*
	****************************************************************************/

	KColor::KColor(const DirectX::PackedVector::XMCOLOR& Packed)
	{
		using namespace DirectX;
		XMStoreFloat4(this, PackedVector::XMLoadColor(&Packed));
	}

	KColor::KColor(const DirectX::PackedVector::XMUBYTEN4& Packed)
	{
		using namespace DirectX;
		XMStoreFloat4(this, PackedVector::XMLoadUByteN4(&Packed));
	}

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------
	bool KColor::operator == (const KColor& c) const
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		return XMColorEqual(c1, c2);
	}

	bool KColor::operator != (const KColor& c) const
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		return XMColorNotEqual(c1, c2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	KColor& KColor::operator= (const DirectX::PackedVector::XMCOLOR& Packed)
	{
		using namespace DirectX;
		XMStoreFloat4(this, PackedVector::XMLoadColor(&Packed));
		return *this;
	}

	KColor& KColor::operator= (const DirectX::PackedVector::XMUBYTEN4& Packed)
	{
		using namespace DirectX;
		XMStoreFloat4(this, PackedVector::XMLoadUByteN4(&Packed));
		return *this;
	}

	KColor& KColor::operator+= (const KColor& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorAdd(c1, c2));
		return *this;
	}

	KColor& KColor::operator-= (const KColor& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorSubtract(c1, c2));
		return *this;
	}

	KColor& KColor::operator*= (const KColor& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorMultiply(c1, c2));
		return *this;
	}

	KColor& KColor::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMVectorScale(c, S));
		return *this;
	}

	KColor& KColor::operator/= (const KColor& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorDivide(c1, c2));
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	KColor KColor::operator- () const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		KColor R;
		XMStoreFloat4(&R, XMVectorNegate(c));
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	KColor operator+ (const KColor& C1, const KColor& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		KColor R;
		XMStoreFloat4(&R, XMVectorAdd(c1, c2));
		return R;
	}

	KColor operator- (const KColor& C1, const KColor& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		KColor R;
		XMStoreFloat4(&R, XMVectorSubtract(c1, c2));
		return R;
	}

	KColor operator* (const KColor& C1, const KColor& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		KColor R;
		XMStoreFloat4(&R, XMVectorMultiply(c1, c2));
		return R;
	}

	KColor operator* (const KColor& C, float S)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(&C);
		KColor R;
		XMStoreFloat4(&R, XMVectorScale(c, S));
		return R;
	}

	KColor operator/ (const KColor& C1, const KColor& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		KColor R;
		XMStoreFloat4(&R, XMVectorDivide(c1, c2));
		return R;
	}

	KColor operator* (float S, const KColor& C)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C);
		KColor R;
		XMStoreFloat4(&R, XMVectorScale(c1, S));
		return R;
	}

	//------------------------------------------------------------------------------
	// KColor operations
	//------------------------------------------------------------------------------

	DirectX::PackedVector::XMCOLOR KColor::BGRA() const
	{
		using namespace DirectX;
		XMVECTOR clr = XMLoadFloat4(this);
		PackedVector::XMCOLOR Packed;
		PackedVector::XMStoreColor(&Packed, clr);
		return Packed;
	}

	DirectX::PackedVector::XMUBYTEN4 KColor::RGBA() const
	{
		using namespace DirectX;
		XMVECTOR clr = XMLoadFloat4(this);
		PackedVector::XMUBYTEN4 Packed;
		PackedVector::XMStoreUByteN4(&Packed, clr);
		return Packed;
	}

	KVector3 KColor::ToVector3() const
	{
		return KVector3(x, y, z);
	}

	KVector4 KColor::ToVector4() const
	{
		return KVector4(x, y, z, w);
	}

	void KColor::Negate()
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMColorNegative(c));
	}

	void KColor::Negate(KColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMColorNegative(c));
	}

	void KColor::Saturate()
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMVectorSaturate(c));
	}

	void KColor::Saturate(KColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMVectorSaturate(c));
	}

	void KColor::Premultiply()
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMVECTOR a = XMVectorSplatW(c);
		a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
		XMStoreFloat4(this, XMVectorMultiply(c, a));
	}

	void KColor::Premultiply(KColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMVECTOR a = XMVectorSplatW(c);
		a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
		XMStoreFloat4(&result, XMVectorMultiply(c, a));
	}

	void KColor::AdjustSaturation(float sat)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMColorAdjustSaturation(c, sat));
	}

	void KColor::AdjustSaturation(float sat, KColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMColorAdjustSaturation(c, sat));
	}

	void KColor::AdjustContrast(float contrast)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMColorAdjustContrast(c, contrast));
	}

	void KColor::AdjustContrast(float contrast, KColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMColorAdjustContrast(c, contrast));
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	void KColor::Modulate(const KColor& c1, const KColor& c2, KColor& result)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);
		XMStoreFloat4(&result, XMColorModulate(C0, C1));
	}

	KColor KColor::Modulate(const KColor& c1, const KColor& c2)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);

		KColor result;
		XMStoreFloat4(&result, XMColorModulate(C0, C1));
		return result;
	}

	void KColor::Lerp(const KColor& c1, const KColor& c2, float t, KColor& result)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);
		XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
	}

	KColor KColor::Lerp(const KColor& c1, const KColor& c2, float t)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);

		KColor result;
		XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
		return result;
	}


	/****************************************************************************
	*
	* TRay
	*
	****************************************************************************/

	//-----------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------
	bool TRay::operator == (const TRay& r) const
	{
		using namespace DirectX;
		XMVECTOR r1p = XMLoadFloat3(&position);
		XMVECTOR r2p = XMLoadFloat3(&r.position);
		XMVECTOR r1d = XMLoadFloat3(&direction);
		XMVECTOR r2d = XMLoadFloat3(&r.direction);
		return XMVector3Equal(r1p, r2p) && XMVector3Equal(r1d, r2d);
	}

	bool TRay::operator != (const TRay& r) const
	{
		using namespace DirectX;
		XMVECTOR r1p = XMLoadFloat3(&position);
		XMVECTOR r2p = XMLoadFloat3(&r.position);
		XMVECTOR r1d = XMLoadFloat3(&direction);
		XMVECTOR r2d = XMLoadFloat3(&r.direction);
		return XMVector3NotEqual(r1p, r2p) && XMVector3NotEqual(r1d, r2d);
	}

	//-----------------------------------------------------------------------------
	// TRay operators
	//------------------------------------------------------------------------------

	bool TRay::Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const
	{
		return sphere.Intersects(position, direction, Dist);
	}

	bool TRay::Intersects(const BoundingBox& box, _Out_ float& Dist) const
	{
		return box.Intersects(position, direction, Dist);
	}

	bool TRay::Intersects(const KVector3& tri0, const KVector3& tri1, const KVector3& tri2, _Out_ float& Dist) const
	{
		return DirectX::TriangleTests::Intersects(position, direction, tri0, tri1, tri2, Dist);
	}

	bool TRay::Intersects(const KPlane& plane, _Out_ float& Dist) const
	{
		using namespace DirectX;

		XMVECTOR p = XMLoadFloat4(&plane);
		XMVECTOR dir = XMLoadFloat3(&direction);

		XMVECTOR nd = XMPlaneDotNormal(p, dir);

		if (XMVector3LessOrEqual(XMVectorAbs(nd), g_RayEpsilon))
		{
			Dist = 0.f;
			return false;
		}
		else
		{
			// t = -(dot(n,origin) + D) / dot(n,dir)
			XMVECTOR pos = XMLoadFloat3(&position);
			XMVECTOR v = XMPlaneDotNormal(p, pos);
			v = XMVectorAdd(v, XMVectorSplatW(p));
			v = XMVectorDivide(v, nd);
			float dist = -XMVectorGetX(v);
			if (dist < 0)
			{
				Dist = 0.f;
				return false;
			}
			else
			{
				Dist = dist;
				return true;
			}
		}
	}


	/****************************************************************************
	*
	* TViewport
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool TViewport::operator == (const TViewport& vp) const
	{
		return (x == vp.x && y == vp.y
			&& width == vp.width && height == vp.height
			&& minDepth == vp.minDepth && maxDepth == vp.maxDepth);
	}

	bool TViewport::operator != (const TViewport& vp) const
	{
		return (x != vp.x || y != vp.y
			|| width != vp.width || height != vp.height
			|| minDepth != vp.minDepth || maxDepth != vp.maxDepth);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	TViewport& TViewport::operator= (const RECT& rct)
	{
		x = float(rct.left); y = float(rct.top);
		width = float(rct.right - rct.left);
		height = float(rct.bottom - rct.top);
		minDepth = 0.f; maxDepth = 1.f;
		return *this;
	}

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
	TViewport& TViewport::operator= (const D3D11_VIEWPORT& vp)
	{
		x = vp.TopLeftX; y = vp.TopLeftY;
		width = vp.Width; height = vp.Height;
		minDepth = vp.MinDepth; maxDepth = vp.MaxDepth;
		return *this;
	}
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__)
	TViewport& TViewport::operator= (const D3D12_VIEWPORT& vp)
	{
		x = vp.TopLeftX; y = vp.TopLeftY;
		width = vp.Width; height = vp.Height;
		minDepth = vp.MinDepth; maxDepth = vp.MaxDepth;
		return *this;
	}
#endif

	//------------------------------------------------------------------------------
	// TViewport operations
	//------------------------------------------------------------------------------

	float TViewport::AspectRatio() const
	{
		if (width == 0.f || height == 0.f)
			return 0.f;

		return (width / height);
	}

	KVector3 TViewport::Project(const KVector3& p, const KMatrix& proj, const KMatrix& view, const KMatrix& world) const
	{
		using namespace DirectX;
		XMVECTOR v = XMLoadFloat3(&p);
		XMMATRIX projection = XMLoadFloat4x4(&proj);
		v = XMVector3Project(v, x, y, width, height, minDepth, maxDepth, projection,
			(CXMMATRIX)view, (CXMMATRIX)world);
		KVector3 result;
		XMStoreFloat3(&result, v);
		return result;
	}

	void TViewport::Project(const KVector3& p, const KMatrix& proj, const KMatrix& view, const KMatrix& world, KVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v = XMLoadFloat3(&p);
		XMMATRIX projection = XMLoadFloat4x4(&proj);
		v = XMVector3Project(v, x, y, width, height, minDepth, maxDepth, projection, (CXMMATRIX)view, (CXMMATRIX)world);
		XMStoreFloat3(&result, v);
	}

	KVector3 TViewport::Unproject(const KVector3& p, const KMatrix& proj, const KMatrix& view, const KMatrix& world) const
	{
		using namespace DirectX;
		XMVECTOR v = XMLoadFloat3(&p);
		XMMATRIX projection = XMLoadFloat4x4(&proj);
		v = XMVector3Unproject(v, x, y, width, height, minDepth, maxDepth, projection, (CXMMATRIX)view, (CXMMATRIX)world);
		KVector3 result;
		XMStoreFloat3(&result, v);
		return result;
	}

	void TViewport::Unproject(const KVector3& p, const KMatrix& proj, const KMatrix& view, const KMatrix& world, KVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v = XMLoadFloat3(&p);
		XMMATRIX projection = XMLoadFloat4x4(&proj);
		v = XMVector3Unproject(v, x, y, width, height, minDepth, maxDepth, projection, (CXMMATRIX)view, (CXMMATRIX)world);
		XMStoreFloat3(&result, v);
	}
}
