#include "KMapUtil.h"

bool KMapUtil::SaveKMap(KMapSpace* pSpace,std::string filename)
{
	FILE* fpWrite = nullptr;
	//������ ������ 0 ����
	//�� ������, ���̰�, ����ŷ ��, �ؽ��� ���, ������Ʈ ����, ��İ�
	if (fopen_s(&fpWrite,filename.c_str(), "w") == 0)
	{
		bool bRet = true;
		//������ ����
		std::string header = "#MapSize";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\t", pSpace->m_pMap->m_num_col);
		bRet = fprintf_s(fpWrite, "%d\n", pSpace->m_pMap->m_num_row);

		//���̰� ����
		header = "#Mapheight";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\n", pSpace->m_pMap->m_VertexList.size());
		for (auto vertex : pSpace->m_pMap->m_VertexList)
		{
			bRet = fprintf_s(fpWrite, "%f\t", vertex.pos.y);
		}
		//������Ʈ ����
		//���̰� ����
		header = "#MapObject";
		bRet = fprintf_s(fpWrite, "\n%s\n", header.c_str());
		for (auto object: pSpace->m_ObjectMap)
		{
			bRet = fprintf_s(fpWrite, "%s\t", to_wm(object.first).c_str());
			bRet = fprintf_s(fpWrite, "%f\t", object.second->obj_pos.x);
			bRet = fprintf_s(fpWrite, "%f\t", object.second->obj_pos.y);
			bRet = fprintf_s(fpWrite, "%f\t", object.second->obj_pos.z);
			bRet = fprintf_s(fpWrite, "%f\t", object.second->obj_RollPitchYaw.x);
			bRet = fprintf_s(fpWrite, "%f\t", object.second->obj_RollPitchYaw.y);
			bRet = fprintf_s(fpWrite, "%f\t", object.second->obj_RollPitchYaw.z);
			bRet = fprintf_s(fpWrite, "%f\t", object.second->obj_scale.x);
			bRet = fprintf_s(fpWrite, "%f\t", object.second->obj_scale.y);
			bRet = fprintf_s(fpWrite, "%f\n", object.second->obj_scale.z);
		}
		header = "#MapTexture";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%s\t", to_wm(pSpace->m_pMap->m_pSubTextureList[0]->m_Name).c_str());
		bRet = fprintf_s(fpWrite, "%s\t", to_wm(pSpace->m_pMap->m_pSubTextureList[1]->m_Name).c_str());
		bRet = fprintf_s(fpWrite, "%s\t", to_wm(pSpace->m_pMap->m_pSubTextureList[2]->m_Name).c_str());
		bRet = fprintf_s(fpWrite, "%s\n", to_wm(pSpace->m_pMap->m_pSubTextureList[3]->m_Name).c_str());
		header = "#MapAlpha";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%s\n", filename.c_str());

	fclose(fpWrite);
	}

	return true;
}
bool KMapUtil::LoadKMap(std::string filename)
{
	m_Mapsize[0] = 0;
	m_Mapsize[1] = 0;
	m_MapHeight.clear();
	m_MapObject.clear();
	m_SubTexture.clear();
	FILE* fpRead = nullptr;

	if (fopen_s(&fpRead, filename.c_str(), "rt") == 0)
	{
		TCHAR buffer[256] = { 0, };
		//���� ���� ����
		while (_fgetts(buffer, _countof(buffer), fpRead) != 0)
		{
			TCHAR type[36] = { 0, };
			TCHAR value[256] = { 0, };

			_stscanf_s(buffer, _T("%s"), type, (unsigned int)_countof(type));
			//���ڿ��� ���� �� üũ�ϴ� �Լ� strcmp �� ����
			if (_tcscmp(type, L"#MapSize") == 0)
			{
				_fgetts(buffer, _countof(buffer), fpRead); //�� �� ��
				_stscanf_s(buffer, _T("\n %d %d "),
					&m_Mapsize[0], &m_Mapsize[1]);
			}
			else if (_tcscmp(type, L"#Mapheight") == 0)
			{
				_fgetts(buffer, _countof(buffer), fpRead); //�� �� ��
				int vertexSize = 0;
				_stscanf_s(buffer, _T("%d"), &vertexSize);

				for (int i = 0; i < vertexSize; i++)
				{
					float height = 0.0f;
					_fgetts(buffer, _countof(buffer), fpRead); //�� �� ��
					_stscanf_s(buffer, _T("%f"), &height);
					m_MapHeight.push_back(height);
				}
			}
			else if (_tcscmp(type, L"#MapObject") == 0)
			{
				KMatrix mat;
				_stscanf_s(buffer, _T("%s %f %f %f %f %f %f %f %f %f\n"), value, (unsigned int)_countof(value),
					&mat._11, &mat._12, &mat._13, &mat._21, &mat._22, &mat._23, &mat._31, &mat._32, &mat._33);
				m_MapObject.insert(std::make_pair(value, mat));
			}
			else if (_tcscmp(type, L"#MapTexture") == 0)
			{
				KMatrix mat;
				_stscanf_s(buffer, _T("%s %f %f %f %f %f %f %f %f %f\n"), value, (unsigned int)_countof(value),
					&mat._11, &mat._12, &mat._13, &mat._21, &mat._22, &mat._23, &mat._31, &mat._32, &mat._33);
				m_MapObject.insert(std::make_pair(value, mat));
			}
			else if (_tcscmp(type, L"#MapAlpha") == 0)
			{
				KMatrix mat;
				_stscanf_s(buffer, _T("%s %f %f %f %f %f %f %f %f %f\n"), value, (unsigned int)_countof(value),
					&mat._11, &mat._12, &mat._13, &mat._21, &mat._22, &mat._23, &mat._31, &mat._32, &mat._33);
				m_MapObject.insert(std::make_pair(value, mat));
			}
		}
		fclose(fpRead);
	}

	return true;
}
void KMapUtil::SaveTexture(ID3D11DeviceContext* pContext , ID3D11Texture2D* texture, std::wstring filename)
{
	g_TextureMananger.SaveFile(pContext, filename, texture);
}
KMapUtil::KMapUtil()
{
	
}

KMapUtil::~KMapUtil()
{
}
