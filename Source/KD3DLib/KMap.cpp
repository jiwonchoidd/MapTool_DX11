#include "KMap.h"
//2�� 2�� + 1
//init �Լ��� �ڵ����� ����Ʈ ���� �޾� ���� �����.
bool KMap::Init(ID3D11DeviceContext* context, std::wstring heightmap)
{
	m_pContext = context;
	m_tex_offset = 1.0f;
	//���̸� �о��
	CreateHeightMap(heightmap);
	//���̸����� ���� ũ�� ����
	CreateMap(m_num_row, m_num_col, 10.0f);
	return true;
}
bool KMap::CreateMap(UINT width, UINT height, float distance)
{
	m_cell_distance = distance;
	m_num_col = width;
	m_num_row = height;
	m_num_vertex = m_num_col * m_num_row;
	m_num_cell_col = m_num_col - 1;
	m_num_cell_row = m_num_row - 1;
	m_num_face = m_num_cell_col * m_num_cell_row * 2;

	//todo: �ؽ��� ũ�� �ڽ� ũ��� �ؾ����� 
	m_BoxCollision.max.x = (m_num_col / 2 * m_cell_distance);
	m_BoxCollision.min.x = -m_BoxCollision.max.x;
	m_BoxCollision.max.z = (m_num_row / 2 * m_cell_distance);
	m_BoxCollision.min.z = -m_BoxCollision.max.z;

	m_BoxCollision.size.x = (m_BoxCollision.max.x - m_BoxCollision.min.x);
	m_BoxCollision.size.z = (m_BoxCollision.max.z - m_BoxCollision.min.z);
	return true;
}
//context�� �ε�Ǿ�� �����
bool KMap::CreateHeightMap(std::wstring heightmap)
{
	if (heightmap.empty()&&m_pContext!=nullptr)
	{
		return false;
	}
	HRESULT hr;
	wrl::ComPtr<ID3D11Resource> pTexture;
	size_t maxsize = 0;
	if (FAILED(hr = CreateWICTextureFromFileEx(g_pd3dDevice,
		heightmap.c_str(),
		maxsize, 
		D3D11_USAGE_STAGING,
		NULL,
		D3D11_CPU_ACCESS_WRITE|D3D11_CPU_ACCESS_READ,
		NULL,
		WIC_LOADER_DEFAULT,
		pTexture.GetAddressOf(), nullptr)))
	{
		return false;
	}
	ID3D11Texture2D* pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);

	//�ؽ��� ũ�Ⱑ �� ���� ũ��.
	m_HeightList.resize(desc.Height * desc.Width);

	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		//ũ��ƼĮ ����ó�� unmap �ϱ������� ���� ����
		if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_VERTEX	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT colStart = col * 4;
					UINT byte_height = pTexels[rowStart + colStart + 0];
					//byte�� �����Ҽ��ִ� �ִ밪�� 0~255
					//���� ���̸� �����Ϸ��� ������ DWORD�̹Ƿ� pitch/4
					m_HeightList[row * desc.Width + col] = (static_cast<float>(byte_height))-30.0f;
				}
			}
			m_pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1)); 
		}
	}

	m_num_row = desc.Height;
	m_num_col = desc.Width;

	if (pTexture2D) pTexture2D->Release();
	return true;
}
bool KMap::CreateVertexData()
{
	bool bHasHeightMap = false;
	m_VertexList.resize(m_num_vertex);
	m_BTList.resize(m_num_vertex);
	if (m_HeightList.size() > 0)bHasHeightMap = true;

	float  hHalfCol = (m_num_col - 1) / 2.0f;
	float  hHalfRow = (m_num_row - 1) / 2.0f;
	//������ �����ؼ� �ؽ��� ũ�� ���� ������
	float  offsetU = m_tex_offset / (m_num_col - 1);
	float  offsetV = m_tex_offset / (m_num_row - 1);
	for (int iRow = 0; iRow < m_num_row; iRow++)
	{
		for (int iCol = 0; iCol < m_num_col; iCol++)
		{
			int index = iRow * m_num_col + iCol;
			m_VertexList[index].pos.x = (iCol - hHalfCol) * m_cell_distance; //

			if (bHasHeightMap)
			{
				m_VertexList[index].pos.y = m_HeightList[index];
			}
			else
			{
				m_VertexList[index].pos.y = 0;
			}
			m_VertexList[index].pos.z = -((iRow - hHalfRow)* m_cell_distance);//
			m_VertexList[index].color = KVector4(1, 1, 1, 1);
			m_VertexList[index].tex = KVector2(offsetU * iCol, offsetV * iRow);
			m_VertexList[index].normal =KVector3(0,1,0);
		}
	}


	return true;
}

bool KMap::CreateIndexData()
{
	m_IndexList.resize(m_num_face * 3);
	UINT iIndex = 0;
	for (int iRow = 0; iRow < m_num_cell_row; iRow++)
	{
		for (int iCol = 0; iCol < m_num_cell_col; iCol++)
		{
			m_IndexList[iIndex + 0] = iRow * m_num_col + iCol;
			m_IndexList[iIndex + 1] = (iRow * m_num_col + iCol) + 1;
			m_IndexList[iIndex + 2] = (iRow + 1) * m_num_col + iCol;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = m_IndexList[iIndex + 2] + 1;

			iIndex += 6;
		}
	}

	for (int triangle = 0; triangle < m_IndexList.size(); triangle += 3)
	{
		KVector3 T, B, N;
		K3DAsset::CreateTangentSpace(&m_VertexList[m_IndexList[triangle]].pos, &m_VertexList[m_IndexList[triangle+1]].pos, &m_VertexList[m_IndexList[triangle + 2]].pos,
			&m_VertexList[m_IndexList[triangle]].tex, &m_VertexList[m_IndexList[triangle+1]].tex, &m_VertexList[m_IndexList[triangle+2]].tex, &N, &T, &B);
		m_BTList[m_IndexList[triangle]].tangent = T;
		m_BTList[m_IndexList[triangle]].binormal= B;
		m_VertexList[m_IndexList[triangle]].normal	= N;

		K3DAsset::CreateTangentSpace(&m_VertexList[m_IndexList[triangle+1]].pos, &m_VertexList[m_IndexList[triangle+2]].pos, &m_VertexList[m_IndexList[triangle]].pos,
			&m_VertexList[m_IndexList[triangle+1]].tex, &m_VertexList[m_IndexList[triangle+2]].tex, &m_VertexList[m_IndexList[triangle]].tex, &N, &T, &B);
		m_BTList[m_IndexList[triangle+1]].tangent = T;
		m_BTList[m_IndexList[triangle+1]].binormal = B;
		m_VertexList[m_IndexList[triangle+1]].normal   = N;
		
		K3DAsset::CreateTangentSpace(&m_VertexList[m_IndexList[triangle+2]].pos, &m_VertexList[m_IndexList[triangle]].pos, &m_VertexList[m_IndexList[triangle+1]].pos,
			&m_VertexList[m_IndexList[triangle+2]].tex, &m_VertexList[m_IndexList[triangle]].tex, &m_VertexList[m_IndexList[triangle+1]].tex, &N, &T, &B);
		m_BTList[m_IndexList[triangle+2]].tangent = T;
		m_BTList[m_IndexList[triangle+2]].binormal = B;
		m_VertexList[m_IndexList[triangle+2]].normal   = N;

	}

	return true;
}

bool KMap::PreRender(ID3D11DeviceContext* context)
{
	if (!m_bVisibility)return false;

	if (m_VertexList.size() <= 0) return true;
	//���ҽ� ������Ʈ �����Ϳ� ���ҽ� ������ ����
	context->UpdateSubresource(
		m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);

	context->UpdateSubresource(
		m_pConstantBuffer_EX.Get(), 0, NULL, &m_cbDataEX, 0, 0);

	context->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	context->VSSetConstantBuffers(3, 1, m_pConstantBuffer_EX.GetAddressOf());
	context->PSSetConstantBuffers(3, 1, m_pConstantBuffer_EX.GetAddressOf());

	//�ؽ��� ���ҽ��� 0�� ���� - sprite��� SRV //1�� ���� - ����ŧ�� //2�� ���� - �븻
	if(m_pTexture_Diffuse != nullptr)
		context->PSSetShaderResources(0, 1, m_pTexture_Diffuse->m_pSRVTexture.GetAddressOf());//�⺻ �ؽ���

	if (m_pTexture_Specular != nullptr)
		context->PSSetShaderResources(1, 1, m_pTexture_Specular->m_pSRVTexture.GetAddressOf());

	if (m_pTexture_Normal != nullptr)
		context->PSSetShaderResources(2, 1, m_pTexture_Normal->m_pSRVTexture.GetAddressOf());

	for (int subtex = 0; subtex < m_pSubTextureList.size(); subtex++)
	{
		if (m_pSubTextureList[subtex] != nullptr)
			context->PSSetShaderResources(subtex+7, 1, m_pSubTextureList[subtex]->m_pSRVTexture.GetAddressOf());//�⺻ �ؽ���
	}
	if (m_pMapAlphaResultSRV != nullptr)
		context->PSSetShaderResources(11, 1, &m_pMapAlphaResultSRV);

	//���̴�
	context->VSSetShader(m_pVS->m_pVertexShader.Get(), NULL, 0);

	if (m_pPS_Swaped != nullptr)
	{
		context->PSSetShader(m_pPS_Swaped->m_pPixelShader.Get(), NULL, 0);
		m_pPS_Swaped = nullptr;
	}
	else
	{
		context->PSSetShader(m_pPS->m_pPixelShader.Get(), NULL, 0);
	}

	context->IASetInputLayout(m_pVertexLayout.Get());
	UINT pStrides[2] = { sizeof(PNCT_VERTEX), sizeof(BT_VERTEX) };
	UINT pOffsets[2] = { 0, };

	//�������� ���ε� �ε������� ���ε� 0������
	//�迭�� 0~1 ������ �Ҵ�
	ID3D11Buffer* buffer[2] = { m_pVertexBuffer.Get(), m_pVertexBTBuffer.Get() };
	context->IASetVertexBuffers(0, 2, buffer,
		pStrides, pOffsets);

	context->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	return true;
}

KMap::KMap()
{
	m_num_col = 0;
	m_num_row = 0;
	m_num_vertex = 0;
	m_num_cell_col = 0;
	m_num_cell_row = 0;
	m_num_face = 0;
	m_num_face=0;
	m_cell_distance = 1.0f;
}

bool KMap::Render(ID3D11DeviceContext* context)
{
	KObject::Render(context);
	return true;
}

float KMap::GetHeight(float xpos, float ypos)
{
	// fPosX/fPosZ�� ��ġ�� �ش��ϴ� ���̸ʼ��� ã�´�.
		// m_iNumCols��m_iNumRows�� ����/������ ���� ũ�Ⱚ��.
	float fcell_x = (float)(m_num_cell_row * m_cell_distance/ 2.0f + xpos);
	float fcell_z = (float)(m_num_cell_col * m_cell_distance / 2.0f - ypos);

	// ���� ũ��� ������ 0~1 ������ ������ �ٲپ� ���̸� �迭�� �����Ѵ�.
	fcell_x /= (float)m_cell_distance;
	fcell_z /= (float)m_cell_distance;

	// fCellX, fCellZ ������ �۰ų� ���� �ִ� ����( �Ҽ��κ��� �߶󳽴�.)
	float fVertexCol = ::floorf(fcell_x);
	float fVertexRow = ::floorf(fcell_z);

	// ���̸� ������ ����� ������ �ʱ�ȭ �Ѵ�.
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_num_col - 2) < fVertexCol)	fVertexCol = (float)(m_num_col - 2);
	if ((float)(m_num_row - 2) < fVertexRow)	fVertexRow = (float)(m_num_row - 2);

	// ���� ���� �÷��� �����ϴ� 4�� ������ ���̰��� ã�´�. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightMap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightMap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A������ ��ġ���� ������ ��(������)�� ����Ѵ�. 0 ~ 1.0f
	float fDeltaX = fcell_x - fVertexCol;
	float fDeltaZ = fcell_z - fVertexRow;
	// �����۾��� ���� ���� ���ս��� ã�´�. 
	float fHeight = 0.0f;
	// �����̽��� �������� �����Ѵ�.
	// fDeltaZ + fDeltaX < 1.0f
	if (fDeltaZ < (1.0f - fDeltaX))  //ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C	
						  // �� ������ ���̰��� ���̸� ���Ͽ� ��ŸX�� ���� ���� �������� ã�´�.		
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// �Ʒ����̽��� �������� �����Ѵ�.
	else // DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
						  // �� ������ ���̰��� ���̸� ���Ͽ� ��ŸZ�� ���� ���� �������� ã�´�.		
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}
float KMap::Lerp(float start, float end, float tangent)
{
	return start - (start * tangent) + (end * tangent);
}

float KMap::GetHeightMap(int row, int col)
{
	return m_VertexList[row * m_num_row + col].pos.y;
}

KMap::~KMap()
{
	
}


