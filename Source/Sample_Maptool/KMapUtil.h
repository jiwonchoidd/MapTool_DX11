#pragma once
/// <summary>
/// KMap Exporter Importer
/// </summary>
///
#include "KMapSpace.h"
class KMapUtil
{
public:
	//���̸�, ����ŷ ��, �ؽ��� ���, ������Ʈ ����, ��İ�
	UINT							m_Mapsize[2];
	std::vector<float>				m_MapHeight;
	std::map<std::wstring, KMatrix> m_MapObject;
	std::vector<std::wstring>		m_SubTexture;
public:
	bool	SaveKMap(KMapSpace* pSpace,std::string filename);
	bool	LoadKMap(std::string filename);
public:
	void	SaveTexture(ID3D11DeviceContext* pContext, ID3D11Texture2D* texture, std::wstring filename);
public:
	KMapUtil();
	~KMapUtil();
};

