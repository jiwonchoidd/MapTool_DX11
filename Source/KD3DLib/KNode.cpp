#include "KNode.h"

void KNode::AddObject(KMapObject* obj)
{
	m_StaticObjectList.push_back(obj);
}

void KNode::AddDynamicObject(KMapObject* obj)
{
	m_DynamicObjectList.push_back(obj);
}
