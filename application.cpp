//===================================================
//
// アプリケーションの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "application.h"
#include "renderer.h"
#include "object3d.h"
#include "texture.h"
#include "sound.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "camera.h"
#include "light.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CTitle*				CApplication::m_pTitle = nullptr;		//タイトルクラス
CGame*				CApplication::m_pGame = nullptr;		//ゲームクラス
CResult*			CApplication::m_pResult = nullptr;		//リザルトクラス
CTutorial*			CApplication::m_pTutorial = nullptr;	//チュートリアルクラス
CFade*				CApplication::m_pFade = nullptr;		//フェードクラス
CApplication::MODE	CApplication::m_mode = MODE_MAX;		//ゲームモード

CRenderer*	CApplication::m_pRenderer = nullptr;//レンダラー
CInput*		CApplication::m_pInput = nullptr;	//インプット
CTexture*	CApplication::m_pTexture = nullptr;	//テクスチャ
CSound*		CApplication::m_pSound = nullptr;	//サウンド
CCamera*	CApplication::m_pCamera = nullptr;	//カメラ
CLight*		CApplication::m_pLight = nullptr;	//ライト

//===========================
// コンストラクタ
//===========================
CApplication::CApplication()
{

}

//===========================
// デストラクタ
//===========================
CApplication::~CApplication()
{

}

//===========================
// 初期化
//===========================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	//--------------------------
	// インスタンスの生成
	//--------------------------
	m_pRenderer = new CRenderer;	//レンダリング
	m_pInput = new CInput;			//インプット
	m_pTexture = new CTexture;		//テクスチャ
	m_pSound = new CSound;			//サウンド
	m_pCamera = new CCamera;		//カメラ
	m_pLight = new CLight;			//ライト

	//レンダリングの初期化
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//インプットの初期化
	m_pInput->Init(hInstance, hWnd);

	//サウンドの初期化
	//m_pSound->Init(hWnd);

	//カメラの初期化
	m_pCamera->Init();

	//ライトの初期化
	m_pLight->Init(GetRenderer()->GetDevice());

	//モードの設定
	SetMode(MODE_TITLE);

	return S_OK;
}

//===========================
// 終了
//===========================
void CApplication::Uninit()
{
	//オブジェクトの全解放
	CObject::ReleaseAll(true);

	//テクスチャの終了
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	//レンダリングの終了
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	//インプットの終了
	if (m_pInput != nullptr)
	{
		m_pInput->Uninit();
		delete m_pInput;
		m_pInput = nullptr;
	}

	//サウンドの終了
	/*if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}*/

	//カメラの終了
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	//ライトの終了
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}
}

//===========================
// 更新
//===========================
void CApplication::Update()
{
	//インプットの更新
	m_pInput->Update();	//最初にやる

	//レンダリングの更新
	m_pRenderer->Update();

	//カメラの更新
	m_pCamera->Update();

	//モードごとの更新
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
		break;

	case MODE_GAME:
		m_pGame->Update();
		break;

	case MODE_RESULT:
		m_pResult->Update();
		break;

	default:
		break;
	}

	//フェードの更新
	m_pFade->Update();
}

//===========================
// 描画
//===========================
void CApplication::Draw()
{
	//レンダリングの描画
	m_pRenderer->Draw();

	//フェードの描画
	m_pFade->Draw();
}

//===========================
// モードの設定
//===========================
void CApplication::SetMode(MODE mode)
{
	//現在のモードを終了
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Uninit();
		break;

	case MODE_GAME:
		m_pGame->Uninit();
		break;

	case MODE_RESULT:
		m_pResult->Uninit();
		break;

	default:
		break;
	}

	//オブジェクトの全解放
	CObject::ReleaseAll(true);

	//モードの切り替え
	m_mode = mode;

	//新しいモードの生成
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle = nullptr;
		m_pTitle = new CTitle;
		m_pTitle->Init();
		break;

	case MODE_GAME:
		m_pGame = nullptr;
		m_pGame = new CGame;
		m_pGame->Init();
		break;

	case MODE_RESULT:
		m_pResult = nullptr;
		m_pResult = new CResult;
		m_pResult->Init();
		break;

	default:
		break;
	}

	//フェードの生成
	m_pFade = nullptr;
	m_pFade = new CFade;
	m_pFade->Init(m_mode);
}

//===========================
// モードの取得
//===========================
CApplication::MODE CApplication::GetMode()
{
	return m_mode;
}

//===========================
// レンダリングの取得
//===========================
CRenderer *CApplication::GetRenderer()
{
	return m_pRenderer;
}

//===========================
// インプットの取得
//===========================
CInput *CApplication::GetInput()
{
	return m_pInput;
}

//===========================
// テクスチャの取得
//===========================
CTexture *CApplication::GetTexture()
{
	return m_pTexture;
}

//===========================
// サウンドの取得
//===========================
CSound *CApplication::GetSound()
{
	return m_pSound;
}

//===========================
// カメラの取得
//===========================
CCamera *CApplication::GetCamera()
{
	return m_pCamera;
}

//===========================
// ライトの取得
//===========================
CLight *CApplication::GetLight()
{
	return m_pLight;
}

//===========================
// フェードの取得
//===========================
CFade *CApplication::GetFade()
{
	return m_pFade;
}