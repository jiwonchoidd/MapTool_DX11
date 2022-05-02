#include "KMapUtil.h"

bool KMapUtil::SaveKMap(KMapSpace* pSpace,std::string filename)
{
	FILE* fpWrite = nullptr;
	//오류가 없으면 0 리턴
	//맵 사이즈, 높이값, 마스킹 맵, 텍스쳐 경로, 오브젝트 갯수, 행렬값
	if (fopen_s(&fpWrite,filename.c_str(), "w") == 0)
	{
		bool bRet = true;
		//사이즈 저장
		std::string header = "#MapSize";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\t", pSpace->m_pMap->m_num_col);
		bRet = fprintf_s(fpWrite, "%d\n", pSpace->m_pMap->m_num_row);

		//높이값 저장
		header = "#Mapheight";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\n", static_cast<int>(pSpace->m_pMap->m_VertexList.size())); //버텍스 개수

		pSpace->UpdateMapVertexList(); // 노드의 높이를 맵 높이와 같게 만듬
		for (auto vertex : pSpace->m_pMap->m_VertexList)
		{
			bRet = fprintf_s(fpWrite, "%f\n", vertex.pos.y);//버텍스 높이 
		}
		
		//오브젝트 갯수
		header = "#MapObject";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\n", static_cast<int>(pSpace->m_ObjectMap.size()));//오브젝트 개수
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
		std::string alphafile = filename + ".bmp";
		bRet = fprintf_s(fpWrite, "%s\n", alphafile.c_str());

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
		//한줄 한줄 읽음
		while (_fgetts(buffer, _countof(buffer), fpRead) != 0)
		{
			TCHAR type[36] = { 0, };

			_stscanf_s(buffer, _T("%s"), type, (unsigned int)_countof(type));
			//문자열이 같은 지 체크하는 함수 strcmp 와 같음
			if (_tcscmp(type, L"#MapSize") == 0)
			{
				_fgetts(buffer, _countof(buffer), fpRead); //한 줄 더
				_stscanf_s(buffer, _T("\n %d %d "),
					&m_Mapsize[0], &m_Mapsize[1]);
			}
			else if (_tcscmp(type, L"#Mapheight") == 0)
			{
				_fgetts(buffer, _countof(buffer), fpRead);
				int vertexSize = 0;
				_stscanf_s(buffer, _T("%d"), &vertexSize);//몇개인지

				for (int i = 0; i < vertexSize; i++)
				{
					_fgetts(buffer, _countof(buffer), fpRead); 
					float fheight = 0.0f;
					_stscanf_s(buffer, _T("%f"), &fheight);
					m_MapHeight.push_back(fheight);
				}
			}
			else if (_tcscmp(type, L"#MapObject") == 0)
			{
				TCHAR value[36] = { 0, };
				_fgetts(buffer, _countof(buffer), fpRead);
				int objectAmount = 0;
				_stscanf_s(buffer, _T("%d"), &objectAmount);//몇개인지
				for (int i = 0; i < objectAmount; i++)
				{
					KMatrix mat;
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%s %f %f %f %f %f %f %f %f %f\n"), value, (unsigned int)_countof(value),
						&mat._11, &mat._12, &mat._13, &mat._21, &mat._22, &mat._23, &mat._31, &mat._32, &mat._33);
					m_MapObject.insert(std::make_pair(value, mat));
				}
			}
			else if (_tcscmp(type, L"#MapTexture") == 0)
			{
				TCHAR tex1[36] = { 0, };
				TCHAR tex2[36] = { 0, };
				TCHAR tex3[36] = { 0, };
				TCHAR tex4[36] = { 0, };

				_fgetts(buffer, _countof(buffer), fpRead);
				_stscanf_s(buffer, _T("%s %s %s %s \n"),
					tex1, (unsigned int)_countof(tex1),
					tex2, (unsigned int)_countof(tex2),
					tex3, (unsigned int)_countof(tex3),
					tex4, (unsigned int)_countof(tex4));
				m_SubTexture.push_back(tex1);
				m_SubTexture.push_back(tex2);
				m_SubTexture.push_back(tex3);
				m_SubTexture.push_back(tex4);
			}
			else if (_tcscmp(type, L"#MapAlpha") == 0)
			{
				TCHAR tex[36] = { 0, };
				_fgetts(buffer, _countof(buffer), fpRead);
				_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));
				m_AlphaTexture = tex;
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
