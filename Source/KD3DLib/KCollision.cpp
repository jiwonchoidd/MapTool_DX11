#include "KCollision.h"
bool KCollision::SphereToPoint(KSphere sp, float x, float y, float z)
{
	float fDistance = (sp.vCenter -KVector3(x, y, z)).Length();
	if (fDistance <= sp.fRadius)
	{
		return true;
	}
	return false;
}
bool KCollision::SphereToPoint(KSphere sp, KVector3 v)
{
	float fDistance = (sp.vCenter - v).Length();
	if (fDistance <= sp.fRadius)
	{
		return true;
	}
	return false;
}
bool   KCollision::RectToPoint(KRect rt, int x, int y)
{
	if (rt.min.x <= x && rt.max.x >= x &&
		rt.min.y <= y && rt.max.y >= y)
	{
		return true;
	}
	return false;
}
bool   KCollision::RectToPoint(KRect rt, KVector2 v)
{
	if (rt.min.x <= v.x && rt.max.x >= v.x &&
		rt.min.y <= v.y && rt.max.y >= v.y)
	{
		return true;
	}
	return false;
}
KRect   KCollision::UnionRect(KRect rt1, KRect rt2)
{
	KRect rt;
	rt.min.x = rt1.min.x < rt2.min.x ? rt1.min.x : rt2.min.x;
	rt.min.y = rt1.min.y < rt2.min.y ? rt1.min.y : rt2.min.y;
	rt.max.x = rt1.max.x < rt2.max.x ? rt2.max.x : rt1.max.x;
	rt.max.y = rt1.max.y < rt2.max.y ? rt2.max.y : rt1.max.y;
	rt.size.x = rt.max.x - rt.min.x;
	rt.size.y = rt.max.y - rt.min.y;
	rt.middle = (rt.max + rt.min);
	rt.middle /= 2.0f;
	return rt;
}
bool   KCollision::IntersectRect(
	KRect rt1, KRect rt2, KRect* pRect)
{
	KRect rt;
	KRect rtUnion = UnionRect(rt1, rt2);
	if (rtUnion.size.x <= (rt1.size.x + rt2.size.x) &&
		rtUnion.size.y <= (rt1.size.y + rt2.size.y))
	{
		if (pRect != nullptr)
		{
			pRect->min.x = rt1.min.x < rt2.min.x ?
				rt2.min.x : rt1.min.x;
			pRect->min.y = rt1.min.y < rt2.min.y ?
				rt2.min.y : rt1.min.y;

			pRect->max.x = rt1.max.x < rt2.max.x ?
				rt1.max.x : rt2.max.x;
			pRect->max.y = rt1.max.y < rt2.max.y ?
				rt1.max.y : rt2.max.y;

			pRect->size.x = pRect->max.x - pRect->min.x;
			pRect->size.y = pRect->max.y - pRect->min.y;
			pRect->middle = (pRect->max + pRect->min);
			pRect->middle /= 2.0f;
		}
		return true;
	}
	return false;
}
// 0 :  떨어져 있다.
// 1 :  안에 있다.
// 2 :  걸쳐 있다.
KCollisionResult   KCollision::RectToRect(KRect rt1,
	KRect rt2)
{
	KRect rtInterction;
	int iRet = IntersectRect(rt1, rt2, &rtInterction);
	if (iRet <= 0)
	{
		return RECT_OUT;
	}
	if (rtInterction == rt2) return RECT_OVERLAP;
	return RECT_IN;
}

KCollisionResult   KCollision::ToRect(KRect rt1, KRect rt2)
{
	// 거리 판정
	float fDistanceX;
	float fDistanceY;
	fDistanceX = fabs(rt1.middle.x - rt2.middle.x);
	fDistanceY = fabs(rt1.middle.y - rt2.middle.y);
	float fToX = rt1.size.x / 2.0f + rt1.size.x / 2.0f;
	float fToY = rt1.size.y / 2.0f + rt1.size.y / 2.0f;
	if (fDistanceX < fToX && fDistanceY < fToY)
	{
		return RECT_OVERLAP;
	}
	return RECT_OUT;
}
bool   KCollision::BoxToPoint(KBox rt, int x, int y, int z)
{
	if (rt.min.x <= x && rt.max.x >= x &&
		rt.min.y <= y && rt.max.y >= y &&
		rt.min.z <= z && rt.max.z >= z)
	{
		return true;
	}
	return false;
}
bool   KCollision::BoxToPoint(KBox rt, KVector3 v)
{
	if (rt.min.x <= v.x && rt.max.x >= v.x &&
		rt.min.y <= v.y && rt.max.y >= v.y &&
		rt.min.z <= v.z && rt.max.z >= v.z)
	{
		return true;
	}
	return false;
}
KBox   KCollision::UnionBox(KBox rt1, KBox rt2)
{
	KBox rt;
	rt.min.x = rt1.min.x < rt2.min.x ? rt1.min.x : rt2.min.x;
	rt.min.y = rt1.min.y < rt2.min.y ? rt1.min.y : rt2.min.y;
	rt.min.z = rt1.min.z < rt2.min.z ? rt1.min.z : rt2.min.z;

	rt.max.x = rt1.max.x < rt2.max.x ? rt2.max.x : rt1.max.x;
	rt.max.y = rt1.max.y < rt2.max.y ? rt2.max.y : rt1.max.y;
	rt.max.z = rt1.max.z < rt2.max.z ? rt2.max.z : rt1.max.z;

	rt.size = rt.max - rt.min;
	rt.middle = (rt.max + rt.min);
	rt.middle /= 2.0f;
	return rt;
}
bool   KCollision::IntersectBox(
	KBox rt1, KBox rt2, KBox* pRect)
{
	KBox rt;
	KBox rtUnion = UnionBox(rt1, rt2);
	if (rtUnion.size.x <= (rt1.size.x + rt2.size.x) &&
		rtUnion.size.y <= (rt1.size.y + rt2.size.y) &&
		rtUnion.size.z <= (rt1.size.z + rt2.size.z))
	{
		if (pRect != nullptr)
		{
			pRect->min.x = rt1.min.x < rt2.min.x ?
				rt2.min.x : rt1.min.x;
			pRect->min.y = rt1.min.y < rt2.min.y ?
				rt2.min.y : rt1.min.y;
			pRect->min.z = rt1.min.z < rt2.min.z ?
				rt2.min.z : rt1.min.z;

			pRect->max.x = rt1.max.x < rt2.max.x ?
				rt1.max.x : rt2.max.x;
			pRect->max.y = rt1.max.y < rt2.max.y ?
				rt1.max.y : rt2.max.y;
			pRect->max.z = rt1.max.z < rt2.max.z ?
				rt1.max.z : rt2.max.z;

			pRect->size = pRect->max - pRect->min;
			pRect->middle = (pRect->max + pRect->min);
			pRect->middle /= 2.0f;
		}
		return true;
	}
	return false;
}
// 0 :  떨어져 있다.
// 1 :  안에 있다.
// 2 :  걸쳐 있다.
KCollisionResult   KCollision::BoxToBox(KBox rt1, KBox rt2)
{
	KBox rtInterction;
	int iRet = IntersectBox(rt1, rt2, &rtInterction);
	if (iRet <= 0)
	{
		return RECT_OUT;
	}
	if (rtInterction == rt2) return RECT_OVERLAP;
	return RECT_IN;
}