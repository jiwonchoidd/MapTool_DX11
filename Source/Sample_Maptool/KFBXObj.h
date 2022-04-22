#pragma once
#include "K3DAsset.h"
#include <fbxsdk.h>
struct KBoneWorld
{
	KMatrix  matBoneWorld[255];
};
struct KTrack
{
	UINT	iFrame;
	KMatrix matTrack;
	//��� ������ ���� Ʈ������ SRT�� ���� ����
	KVector3	s;
	KQuaternion	r;
	KVector3	t;
};
struct KWeight
{
	std::vector<int>	Index;  // ������ ��ġ�� ����� �ε���
	std::vector<float>  Weight; // ����ġ
	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < Index.size(); ++i)
		{
			if (fBoneWeight > Weight[i])
			{
				for (DWORD j = (Index.size() - 1); j > i; --j)
				{
					Index[j] = Index[j - 1];
					Weight[j] = Weight[j - 1];
				}
				Index[i] = iBoneIndex;
				Weight[i] = fBoneWeight;
				break;
			}
		}
	}
	KWeight()
	{
		Index.resize(8);
		Weight.resize(8);
	}
};
//��Ƽ ��Ʈ������ ������ ���������
struct IW_VERTEX
{
	float  i[4];
	float  w[4];
	IW_VERTEX()
	{
		i[0] = i[1] = i[2] = i[3] = 0;
		w[0] = w[1] = w[2] = w[3] = 0.0f;
	}
};
struct KScene_Animation
{
	UINT   iStart;
	UINT   iEnd;
	UINT   iFrameSpeed;
};
struct KTexturePack
{
	KTexture* texAlbedo;
	KTexture* texSpecular;
	KTexture* texNormal;
	KTexture* texAlpha = nullptr;
};
class KFBXObj : public K3DAsset
{
public:
	FbxNode*	 m_pFbx_ParentNode;
	FbxNode*	 m_pFbx_ThisNode;
	KFBXObj*	 m_pFbx_ParentObj;
public:
	int					m_iIndex = -1;
	bool				m_bSkinned;
	KMatrix				m_matLocal;
	KMatrix				m_matAnim;
	std::vector<KTrack>	m_AnimTrack;
public:
	std::vector<ID3D11Buffer*>   m_pVBList;
	std::vector<ID3D11Buffer*>   m_pVBWeightList;
	std::vector<ID3D11Buffer*>   m_pVBBTList;
	KTexture*					 m_pTexturePack[3];
	std::vector<KTexturePack>	 m_pTextureList;
	//std::vector<std::wstring>		 m_strTexList;
	std::map<std::wstring, KMatrix>		 m_MatrixBindPoseMap; // �ε����� ������ ���ε����� �� 
public:
	//�� ��ü�� ���� ������ ��������
	using KSubVertex = std::vector<PNCT_VERTEX>;
	using KSubBT = std::vector<BT_VERTEX>;
	using KSubVertexIW = std::vector<IW_VERTEX>;
	std::vector<KSubVertex>      m_pSubVertexList; // ���� PNCT
	std::vector<KSubBT>			 m_pSubBTList; // ���� BT
	std::vector<KSubVertexIW>    m_pSubIWVertexList; // ���� Weight
	std::vector<KWeight>		 m_WeightList;
public:
	virtual bool		PreRender(ID3D11DeviceContext* context)override;
	virtual bool		Render(ID3D11DeviceContext* context)override;
	virtual bool		PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)override;
	virtual bool		Release() override;
	virtual bool		CheckVertexData()override;
	virtual bool		CreateVertexData()override;
	virtual bool		CreateIndexData()override;
	virtual HRESULT		CreateVertexLayout()override;
	virtual HRESULT		CreateVertexBuffer()override;

public:
	KFBXObj();
	~KFBXObj();
};

