#include "KObjObject.h"
#include "ImGuiManager.h"
//Init���� �ε���� �ع���
bool KObjObject::Init(ID3D11DeviceContext* context, 
	std::wstring vs, std::wstring ps, std::wstring tex, std::wstring objfile)
{
	m_pContext = context;
	ObjParse(objfile);
	K2DAsset::CreateObject_Mask(vs, ps, tex, L"");
	return true;
}

bool KObjObject::Frame()
{
	//
	if (ImGui::Begin("test"))
	{
		ImGui::Text("model %d %d", m_rtColl.max.x, m_rtColl.max.y);
	}
	ImGui::End();
	KObject::Frame();
	return true;
}

bool KObjObject::Render(ID3D11DeviceContext* context)
{
	KObject::Render(context);
	return true;
}

// v ���ؽ� ��ǥ��
// vn �������� ��
// vt �ؽ��� ��ǥ�� 
// g �׷�
// f ���ؽ�. �ؽ���, �븻�� 
bool KObjObject::ObjParse(std::wstring	objfile)
{
	FILE* fp = nullptr;
	_tfopen_s(&fp, objfile.c_str(), _T("rt"));
	if (fp == NULL)
	{
		//������ ����
		return false;
	}
	TCHAR buffer[256] = { 0, };
	std::wstring filename;

	//������ ��� 0 ���� EOF�� -1
	//������ �о��
	while (_fgetts(buffer, _countof(buffer), fp)!=0)
	{
		TCHAR type[6] = { 0, };
		TCHAR value[256] = { 0, };

		_stscanf_s(buffer, _T("%s"), type,(unsigned int)_countof(type));
		//���ڿ��� ���� �� üũ�ϴ� �Լ� strcmp �� ����
		if (_tcscmp(type, L"v") == 0)
		{
			KVector3 vertex;
			_stscanf_s(buffer, _T("%s %f %f %f\n"), value, (unsigned int)_countof(value),
				&vertex.x, &vertex.y, &vertex.z);
			m_vlist.push_back(vertex);
		}
		else if (_tcscmp(type, L"vt") == 0)
		{
			KVector2 uv;
			_stscanf_s(buffer, _T("%s %f %f \n"), value, (unsigned int)_countof(value),
				&uv.x, &uv.y);
			m_vtlist.push_back(uv);
		}
		else if (_tcscmp(type, L"vn") == 0) {
			KVector3 normal;
			_stscanf_s(buffer, _T("%s %f %f %f\n"), value, (unsigned int)_countof(value),
				&normal.x, &normal.y, &normal.z);
			m_vnlist.push_back(normal);
		}
	
		else if (_tcscmp(type, L"f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = _stscanf_s(buffer, _T("%s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n"),
				value, (unsigned int)_countof(type),
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 10) {
				printf("File can't be read by our simple parser \n");
				continue;
			}
			m_vertexindex.push_back(vertexIndex[0]);
			m_vertexindex.push_back(vertexIndex[1]);
			m_vertexindex.push_back(vertexIndex[2]);
			m_uvindex.push_back(uvIndex[0]);
			m_uvindex.push_back(uvIndex[1]);
			m_uvindex.push_back(uvIndex[2]);
			m_normalindex.push_back(normalIndex[0]);
			m_normalindex.push_back(normalIndex[1]);
			m_normalindex.push_back(normalIndex[2]);
		}
	}
	

	fclose(fp);
	return true;
}

bool KObjObject::SetVertexData()
{
	for (unsigned int index = 0; index < m_vertexindex.size(); index++)
	{
		unsigned int vertexindex = m_vertexindex[index];
		unsigned int uvindex = m_uvindex[index];
		unsigned int normalindex = m_normalindex[index];

		PNCT_VERTEX pnct;
		pnct.pos = m_vlist[vertexindex-1];
		pnct.tex = m_vtlist[uvindex - 1];
		pnct.normal = m_vnlist[normalindex - 1];
		m_VertexList.push_back(pnct);
	}
	return true;
}
//�ε��� ���� �Ⱦ� 
//���Ŀ� �߰���
bool KObjObject::SetIndexData()
{

	return true;
}

KObjObject::KObjObject()
{
}

KObjObject::~KObjObject()
{
}
