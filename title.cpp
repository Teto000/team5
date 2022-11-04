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
#include "camera_title.h"
#include "player.h"
#include "Titlelogo.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CMeshField*   CTitle::m_pMeshField = nullptr;
CCameraTitle* CTitle::m_pCameraTitle = nullptr;
CTitlelogo*	  CTitle::m_pTitlelogo = nullptr;


//===========================
// コンストラクタ
//===========================
CTitle::CTitle()
{
	m_pObject2D = nullptr;
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
	//メッシュフィールドの生成
	m_pMeshField = CMeshField::Create();

	//カメラの生成
	m_pCameraTitle = CCameraTitle::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//タイトルロゴの生成
	m_pTitlelogo = CTitlelogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pTitlelogo->SetSize((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2);

	return S_OK;
}

//===========================
// 終了
//===========================
void CTitle::Uninit()
{
	if (m_pCameraTitle != nullptr)
	{//カメラがnullじゃないなら 
		m_pCameraTitle->Uninit();
		delete m_pCameraTitle;
		m_pCameraTitle = nullptr;
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

	//カメラの更新
	m_pCameraTitle->Update();
}

//===========================
// タイトルカメラの取得
//===========================
CCameraTitle* CTitle::GetCameraTitle()
{
	return m_pCameraTitle;
}