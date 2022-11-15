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
#include "read.h"
#include "3dobject.h"
#include "motion_parts.h"
#include "joypad.h"

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
	//m_pMeshField = CMeshField::Create();
	CRead cRead;
	int nNum = cRead.ReadMotion("data/MOTION/motionground.txt");
	CMotionParts::MoveMotionModel(D3DXVECTOR3(0.0f, -500.0f, 5000.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), nNum, 0);

	//カメラの生成
	m_pCameraTitle = CCameraTitle::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//タイトルロゴの生成
	m_pTitlelogo = CTitlelogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),CTexture::TEXTURE_TITLELOGO);
	m_pTitlelogo = CTitlelogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), CTexture::TEXTURE_TITLELOGO2);

	//サウンド生成
	CSound::PlaySound(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//===========================
// 終了
//===========================
void CTitle::Uninit()
{
	//サウンド停止
	CSound::StopSound();

	C3DObject::UninitAllModel();

	CMotionParts::ALLUninit();

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
void CTitle::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//サウンド生成
		CSound::PlaySound(CSound::SOUND_LABEL_SE_GAME_START);
		CApplication::GetFade()->SetFade(CApplication::MODE_PSELECT);
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
CCameraTitle* CTitle::GetCameraTitle()
{
	return m_pCameraTitle;
}