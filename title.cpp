//===================================================
//
// タイトルの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "title.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "application.h"
#include "sound.h"
#include "game.h"
#include "fade.h"
#include "meshfield.h"
#include "camera.h"
#include "player.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CMeshField*	CTitle::pMeshField = nullptr;
CCamera*	CTitle::m_pCamera = nullptr;	//カメラクラス

//===========================
// コンストラクタ
//===========================
CTitle::CTitle()
{

}

//===========================
// デストラクタ
//===========================
CTitle::~CTitle()
{

}

//===========================
// 初期化
//===========================
HRESULT CTitle::Init()
{
	m_pObject2D = new CObject2D;
	m_pObject2D->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObject2D->SetTexture(CTexture::TEXTURE_TITLELOGO);
	m_pObject2D->SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	//メッシュフィールドの生成
	pMeshField = CMeshField::Create();

	//カメラの生成
	//カメラ・プレイヤーの人数設定はここ
	// カメラの数が4つなら
	m_pCamera = CCamera::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);				//左上

	return S_OK;
}

//===========================
// 終了
//===========================
void CTitle::Uninit()
{
	if (m_pCamera != nullptr)
	{//カメラがnullじゃないなら 
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

//===========================
// 更新
//===========================
void CTitle::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_PSELECT);
	}

	m_pCamera->Update();
}