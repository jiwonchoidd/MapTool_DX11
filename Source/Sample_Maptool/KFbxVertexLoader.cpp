#include "KFbxLoader.h"
void KFbxLoader::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv)
{
	FbxLayerElementUV* pFbxLayerElementUV = pUVSet;
	if (pFbxLayerElementUV == nullptr) {
		return;
	}

	switch (pFbxLayerElementUV->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = pFbxLayerElementUV->GetIndexArray().GetAt(vertexIndex);
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(id);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		}
		break;
	}
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
			// Always enters this part for the example model
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[1];
			break;
		}
		}
		break;
	}
	}
}

FbxColor KFbxLoader::ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet, DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
	{
		// Crack apart the FBX dereferencing system for Color coordinates		
		switch (pVertexColorSet->GetMappingMode())
		{
		case FbxLayerElement::eByControlPoint:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
		case FbxLayerElement::eByPolygonVertex:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				int iColorIndex = dwVertexIndex;
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
			break;
		}
	}
	return Value;
}

int KFbxLoader::GetSubMaterialIndex(int iPoly,
	FbxLayerElementMaterial* pMaterialSetList)
{
	// ���ι��
	//eNone,
	//eByControlPoint,  // ������
	//eByPolygonVertex, //  
	//eByPolygon, // �����︶�� �ٸ��� �ִ�.
	//eAllSame - ��üǥ�鿡 1���� ������ǥ�� �ִ�.
	int iSubMtrl = 0;
	if (pMaterialSetList != nullptr)
	{
		switch (pMaterialSetList->GetMappingMode())
		{
		case FbxLayerElement::eByPolygon:
		{
			// ���� ������ �迭�� ����Ǵ� ���
			switch (pMaterialSetList->GetReferenceMode())
			{
			case FbxLayerElement::eIndex:
			{
				iSubMtrl = iPoly;
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				iSubMtrl = pMaterialSetList->GetIndexArray().GetAt(iPoly);
			}break;
			}
		}
		default:
		{
			break;
		}
		}
	}
	return iSubMtrl;
}

// ���� �븻�� �д� �Լ� 
FbxVector4 KFbxLoader::ReadNormal(const FbxMesh* mesh,
	int controlPointIndex,
	int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	// �븻 ȹ�� 
	FbxVector4 result;
	// �븻 ���͸� ������ ���� 
	switch (vertexNormal->GetMappingMode()) 	// ���� ��� 
	{
		// ������ ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// ���� ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}

FbxVector4 KFbxLoader::ReadTangent(const FbxMesh* mesh,
	int controlPointIndex,
	int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);
	// �븻 ȹ�� 
	FbxVector4 result;
	// �븻 ���͸� ������ ���� 
	switch (vertexTangent->GetMappingMode()) 	// ���� ��� 
	{
		// ������ ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(controlPointIndex);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// ���� ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}

FbxVector4 KFbxLoader::ReadBinormal(const FbxMesh* mesh,
	int controlPointIndex,
	int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);
	// �븻 ȹ�� 
	FbxVector4 result;
	// �븻 ���͸� ������ ���� 
	switch (vertexBinormal->GetMappingMode()) 	// ���� ��� 
	{
		// ������ ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexBinormal->GetIndexArray().GetAt(controlPointIndex);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// ���� ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result[1] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result[2] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexBinormal->GetIndexArray().GetAt(vertexCounter);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}
