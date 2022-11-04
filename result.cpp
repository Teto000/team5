//===================================================
//
// リザルトの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "result.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "application.h"
#include "sound.h"
#include "fade.h"
#include "camera_title.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CCameraTitle* CResult::m_pCameraTitle = nullptr;

//===========================
// コンストラクタ
//===========================
CResult::CResult()
{

}

//===========================
// デストラクタ
//===========================
CResult::~CResult()
{

}

//===========================
// 初期化
//===========================
HRESULT CResult::Init()
{
	//カメラの生成
	m_pCameraTitle = CCameraTitle::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	m_pObject2D = new CObject2D;
	m_pObject2D->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObject2D->SetTexture(CTexture::TEXTURE_RANKING);
	m_pObject2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return S_OK;
}

//===========================
// 終了
//===========================
void CResult::Uninit()
{
	//---------------------
	// カメラの終了
	//---------------------
	if (m_pCameraTitle != nullptr)
	{//カメラがnullじゃないなら 
		//終了
		m_pCameraTitle->Uninit();

		//消去
		delete m_pCameraTitle;
		m_pCameraTitle = nullptr;
	}
}

//===========================
// 更新
//===========================
void CResult::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enterで次の画面に遷移する
		CApplication::GetFade()->SetFade(CApplication::MODE_TITLE);
	}

	//---------------------
	// カメラの更新
	//---------------------
	if (m_pCameraTitle != nullptr)
	{//カメラがnullじゃないなら 
	 //更新
		m_pCameraTitle->Update();
	}
}

//===========================
// タイトルカメラの取得
//===========================
CCameraTitle* CResult::GetCameraTitle()
{
	return m_pCameraTitle;
}