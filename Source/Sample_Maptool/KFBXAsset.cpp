#include "KFBXAsset.h"

bool KFBXAsset::Init()
{
	m_fAnimTime = 0;
	m_fAnimSpeed = 0.1f;
	return true;
}

bool KFBXAsset::Frame()
{
	return false;
}

bool KFBXAsset::Render(ID3D11DeviceContext* pContext)
{
	//만약에 애님로더가 비어있을 경우에는 일반 로더로 교체해줌
	if (m_pAnimLoader == nullptr)
	{
		m_pAnimLoader = m_pLoader;
	}

	//Private bool True? Time calculation
	if (m_bAnimPlay)
	{
		m_fAnimTime += g_fSecPerFrame * m_pAnimLoader->m_Scene.iFrameSpeed * m_fAnimDir* m_fAnimSpeed;
		//애니메이션 시간이 끝났을때 처음으로 다시 돌림
		if (m_fAnimTime >= m_pAnimLoader->m_Scene.iEnd)
		{
			m_fAnimTime = m_pAnimLoader->m_Scene.iStart;
		}
	}
	

	int iFrame = m_fAnimTime;
	iFrame = max(0, min(m_pAnimLoader->m_Scene.iEnd-1, iFrame));

	for (int iObj = 0; iObj < m_pLoader->m_MeshList.size(); iObj++)
	{
		KFBXObj* pFbxObj = m_pLoader->m_MeshList[iObj];
		if (_tcsstr(pFbxObj->m_ObjName.c_str(), L"LOD") != nullptr)// != L"SK_Mannequin_LOD0")
		{
			if (_tcsstr(pFbxObj->m_ObjName.c_str(), L"LOD0") == nullptr)// != L"SK_Mannequin_LOD0")
			{
				continue;
			}
		}
		//만약에 스키닝 캐릭터 오브젝트라면
		//트리를 타고 뼈 좌표계로 움직여야함
		if (pFbxObj->m_bSkinned)
		{
			//뼈대 하나 하나 모두 탐색
			for (auto data : m_pAnimLoader->m_pFbxObjMap)
			{
				std::wstring name = data.first;
				KFBXObj* pAnimObj = data.second;
				auto model = m_pLoader->m_pFbxObjMap.find(name);
				if (model == m_pLoader->m_pFbxObjMap.end())
				{
					continue; // error
				}
				KFBXObj* pTreeObj = model->second;
				if (pTreeObj == nullptr)
				{
					continue; // error
				}
				auto binepose = pFbxObj->m_MatrixBindPoseMap.find(name);
				if (binepose != pFbxObj->m_MatrixBindPoseMap.end() && pAnimObj)
				{
					KMatrix matInverseBindpose = binepose->second;
					m_matBoneArray.matBoneWorld[pTreeObj->m_iIndex] =
						matInverseBindpose * Interpolation(m_pAnimLoader, pAnimObj, m_fAnimTime);
						//pAnimObj->m_AnimTrack[iFrame].matTrack;
				}
				D3DKMatrixTranspose(&m_matBoneArray.matBoneWorld[pTreeObj->m_iIndex],
					&m_matBoneArray.matBoneWorld[pTreeObj->m_iIndex]);
			}
		}
		else
		{
			for (int inode = 0; inode < m_pLoader->m_FBXTreeList.size(); inode++)
			{
				KFBXObj* pFbxObj = m_pLoader->m_FBXTreeList[inode];
				if (pFbxObj->m_AnimTrack.size() > 0)
				{
					m_matBoneArray.matBoneWorld[inode] =
						Interpolation(m_pAnimLoader, pFbxObj, m_fAnimTime);

				}
				D3DKMatrixTranspose(&m_matBoneArray.matBoneWorld[inode],
					&m_matBoneArray.matBoneWorld[inode]);
			}
		}

		pContext->UpdateSubresource(m_pLoader->m_pBoneCB, 0, NULL, &m_matBoneArray, 0, 0);
		pContext->VSSetConstantBuffers(1, 1, &m_pLoader->m_pBoneCB);
		pFbxObj->m_cbDataEX.vLightColor = { m_cbDataEX.vLightColor.x,m_cbDataEX.vLightColor.y,m_cbDataEX.vLightColor.z,1.0f };
		pFbxObj->m_cbDataEX.vLightPos = { m_cbDataEX.vLightPos.x,m_cbDataEX.vLightPos.y,m_cbDataEX.vLightPos.z };
		pFbxObj->m_cbDataEX.vCamPos = { m_cbDataEX.vCamPos.x, m_cbDataEX.vCamPos.y, m_cbDataEX.vCamPos.z, 1.0f };
		pFbxObj->m_cbDataEX.vLightDir= { m_cbDataEX.vLightDir.x, m_cbDataEX.vLightDir.y, m_cbDataEX.vLightDir.z, 1.0f };
		D3DKMatrixInverse(&pFbxObj->m_cbData.matNormal, NULL,
			&pFbxObj->m_matWorld);
		pFbxObj->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		pFbxObj->SwapPSShader(m_pPS_Swaped);
		pFbxObj->Render(pContext);
	}
	return true;
}

bool KFBXAsset::Release()
{
	return false;
}

bool KFBXAsset::PlayAnimation()
{
	if(!m_bAnimPlay)m_bAnimPlay = true;
	return true;
}

bool KFBXAsset::StopAnimation()
{
	if (m_bAnimPlay)m_bAnimPlay = false;
	return true;
}

//애니메이션 변경시에 보간, 혹은 슬로우모션(프레임저하)
KMatrix KFBXAsset::Interpolation(KFbxLoader* pAnimLoader, KFBXObj* pFbxObj, float fTime)
{
	KMatrix  matAnim;
	KScene_Animation scene_Anim = pAnimLoader->m_Scene;
	int iStart = max(scene_Anim.iStart, fTime);
	int iEnd = min(scene_Anim.iEnd, fTime + 1);
	// 보간 = A ~ 7.5f ~ B
	//       9.5f <=10   ~     20 -> 20.1
	KTrack A = pFbxObj->m_AnimTrack[iStart];
	KTrack B = pFbxObj->m_AnimTrack[iEnd];
	float s = fTime - (float)iStart; // 0~1
	KVector3 pos;
	//위치 값 선형 보간 
	D3DXVec3Lerp(&pos, &A.t, &B.t, s);
	KVector3 scale;
	//스케일 값 선형 보간
	D3DXVec3Lerp(&scale, &A.s, &B.s, s);
	KQuaternion rotation;
	//회전 값 구면 보간
	D3DXQuaternionSlerp(&rotation, &A.r, &B.r, s);
	KMatrix matScale;
	D3DKMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	KMatrix matRotation;
	D3DKMatrixRotationQuaternion(&matRotation, &rotation);

	matAnim = matScale * matRotation;
	matAnim._41 = pos.x;
	matAnim._42 = pos.y;
	matAnim._43 = pos.z;
	return matAnim;
}
