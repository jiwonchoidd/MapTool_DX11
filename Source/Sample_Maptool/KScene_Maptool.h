#pragma once
#include "KScene.h"
#include "KCamera.h"
#include "KSoundManager.h"
#include "KMapSpace.h"
#include "KMiniMap.h"
#include "KRenderTarget.h"
#include "KMousePicker.h"
#include "KDepthShadow.h"
#include "KFBXAsset.h"
#include "KMapSprite.h"
#include "KMapUtil.h"
class KScene_Maptool : public KScene
{
public:
    bool           m_bImguiAddObject;
    bool           m_bImguiAddObject_Random;
   
public:
    KLight         m_Light;
    KDepthShadow   m_Shadow;
public:
    KFbxLoader	   m_FbxLoader;
    std::vector<KFBXAsset*> m_FBXList;
    std::vector<KTexture*>  m_TextureList;
public:
    KMap           m_Terrian;
    KMapSpace      m_Terrian_Space;
    KMapSprite     m_Terrian_Sprite;
    KMapUtil       m_Terrian_Util;
public:
    KCamera        m_TopView;
    KMiniMap       m_MiniMap_DebugCamera;
    KMiniMap       m_MiniMap_DebugShadow;
public:
    KMousePicker   m_MousePicker;
public:
    void    ImguiInspector();
    void    ImguiSaveLoad();
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

