#pragma once
#include "K3DAsset.h"
//정점 개수 (2n승 +1)
class KMap : public K3DAsset
{
public:
	//mapsprite에서 받아올 포인터 변수
	ID3D11ShaderResourceView*	m_pMapAlphaResultSRV;
	std::vector<KTexture*>		m_pSubTextureList;
public:
	float		m_cell_distance;
public:
	UINT		m_num_col;
	UINT		m_num_row;
	UINT        m_num_vertex;
	UINT		m_num_cell_col;
	UINT		m_num_cell_row;
	UINT        m_num_face;
public:
	float		m_tex_offset;
public:
	std::vector<float>  m_HeightList;
public:
	virtual	bool Init(ID3D11DeviceContext* context, std::wstring heightmap);
	virtual bool CreateVertexData()override;
	virtual bool CreateIndexData()override;
	virtual bool PreRender(ID3D11DeviceContext* context)override;
	virtual bool Render(ID3D11DeviceContext* context);
public:
	virtual float GetHeight(float xpos, float ypos);
	virtual float GetHeightMap(int row, int col);
	virtual float Lerp(float start, float end, float tangent);
private:
	virtual bool CreateHeightMap(std::wstring heightmap=L"");
	virtual bool CreateMap(UINT width, UINT height, float distance);
public:
	KMap();
	virtual ~KMap();
};

