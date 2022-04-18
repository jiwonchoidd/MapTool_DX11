#pragma once
#include "KStd.h"
enum KCollisionResult
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};
//위치랑 w h
//화면 좌표계를 이용하며, 왼쪽 상단이 원점인 KRedct2D
struct KRect2D
{
	KVector2 min;
	KVector2 max;
	KVector2 middle;
	KVector2 size;
	bool operator == (const KRect2D& v)
	{
		if (fabs((min - v.min).Length()) < 0.0001f)
		{
			if (fabs((max - v.max).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	KRect2D() {};
	KRect2D(KVector2 min, KVector2 max)
	{
		this->min = min;
		this->max = max;
		middle = (max + min);
		middle /= 2.0f;
		size.x = max.x - min.x;
		size.y = max.y - min.y;
	}
	KRect2D(KVector2 v, float w, float h)
	{
		this->min = v;
		this->max = min + KVector2(w, h);
		middle = (max + min);
		middle /= 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
}; 
//화면 좌표계 중앙이 원점이 KRect
struct KRect
{
	KVector2 min;
	KVector2 max;
	KVector2 middle;
	KVector2 size;
	bool operator == (const KRect& v)
	{
		if (fabs((min - v.min).Length()) < 0.0001f)
		{
			if (fabs((max - v.max).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	KRect() {};
	KRect(KVector2 min, KVector2 max)
	{
		this->min = min;
		this->max = max;
		middle = (max + min);
		middle /= 2.0f;
		size.x = max.x - min.x;
		size.y = max.y - min.y;
	}
	KRect(KVector2 v, float w, float h)
	{
		middle = v;
		this->min = middle - KVector2(w / 2.0f, h / 2.0f);
		this->max = middle + KVector2(w / 2.0f, h / 2.0f);
		this->size.x = w;
		this->size.y = h;
	}
};
struct KBox
{
	KVector3 min;
	KVector3 max;
	KVector3 middle;
	KVector3 size;
	KVector3 List[8];
	KVector3 Axis[3];
	bool operator == (const KBox& v)
	{
		if (fabs((min - v.min).Length()) < 0.0001f)
		{
			if (fabs((max - v.max).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	KBox() {};
	KBox(KVector3 min, KVector3 max)
	{
		this->min = min;
		this->max = max;
		middle = (max + min);
		middle /= 2.0f;
		size.x = max.x - min.x;
		size.y = max.y - min.y;
		Axis[0] = { 1,0,0 };
		Axis[1] = { 0,1,0 };
		Axis[2] = { 0,0,1 };
	}
	KBox(KVector3 v, float w, float h, float q)
	{
		this->min = v;
		this->max = min + KVector3(w, h, q);
		middle = (max + min);
		middle /= 2.0f;
		this->size.x = w;
		this->size.y = h;
		Axis[0] = { 1,0,0 };
		Axis[1] = { 0,1,0 };
		Axis[2] = { 0,0,1 };
	}
};
struct KSphere
{
	KVector3 vCenter;
	float    fRadius;
	KSphere()
	{
		fRadius = 3.0f;
	}
};
class KCollision
{
public:
	static bool   SphereToPoint(KSphere sp, float x, float y, float z);
	static bool   SphereToPoint(KSphere sp, KVector3 v);
	static bool   RectToPoint(KRect rt, int x, int y);
	static bool   RectToPoint(KRect rt, KVector2 v);
	static KCollisionResult    RectToRect(KRect, KRect);
	static KCollisionResult    ToRect(KRect rt1, KRect rt2);
	static KRect  UnionRect(KRect rt1, KRect rt2);
	static bool  IntersectRect(KRect rt1, KRect rt2, KRect* rt);

	static bool   BoxToPoint(KBox rt, int x, int y, int z);
	static bool   BoxToPoint(KBox rt, KVector3 v);
	static KCollisionResult    BoxToBox(KBox, KBox);
	static KBox   UnionBox(KBox rt1, KBox rt2);
	static bool  IntersectBox(KBox rt1, KBox rt2, KBox* rt);
};

