#include "KNode.h"

void KNode::AddObject(KMapObject* obj)
{
	obj->obj_node = this;
	m_StaticObjectList.push_back(obj);
}

void KNode::DeleteObject(KMapObject* obj)
{
	std::list<KMapObject*> ::iterator iter;
	for (iter = m_StaticObjectList.begin(); iter!= m_StaticObjectList.end();)
	{
		if(*iter == obj)
		{
			m_StaticObjectList.erase(iter);
			break;
		}
		iter++;
	}
}

void KNode::AddDynamicObject(KMapObject* obj)
{
	m_DynamicObjectList.push_back(obj);
}

