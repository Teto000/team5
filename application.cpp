//===================================================
//
// アプリケーションの処理
// Author : Sato Teruto
//
//===================================================

 //------------------------
// インクルード
//------------------------
#include <stdlib.h>
#include "application.h"
#include "renderer.h"
#include "object3d.h"
#include "texture.h"
#include "sound.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "tutorial.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "Goal.h"
#include "select_player.h"
#include "debug_proc.h"
#include "joypad.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CTitle*				CApplication::m_pTitle = nullptr;		//タイトルクラス
CPSelect*			CApplication::m_pPSelect = nullptr;		//プレイヤー人数指定クラス
CGame*				CApplication::m_pGame = nullptr;		//ゲームクラス
CResult*			CApplication::m_pResult = nullptr;		//リザルトクラス
CTutorial*			CApplication::m_pTutorial = nullptr;	//チュートリアルクラス
CFade*				CApplication::m_pFade = nullptr;		//フェードクラス
CApplication::MODE	CApplication::m_mode = MODE_MAX;		//ゲームモード

CRenderer*			CApplication::m_pRenderer = nullptr;	//レンダラー
CInput*				CApplication::m_pInput = nullptr;		//インプット
CTexture*			CApplication::m_pTexture = nullptr;		//テクスチャ
CSound*				CApplication::m_pSound = nullptr;		//サウンド
CLight*				CApplication::m_pLight = nullptr;		//ライト
CDebugProc*			CApplication::m_pDebugproc = nullptr;	//デバッグ用文字
CJoypad*			CApplication::m_pJoyPad = nullptr;		//ジョイパッド

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
	//----------------------------
	// レンダリングの生成と初期化
	//----------------------------
	m_pRenderer = new CRenderer;
	if (FAILED(m_pRenderer->Init(hWnd, FALSE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//----------------------------
	// インプットの生成と初期化
	//----------------------------
	m_pInput = new CInput;
	m_pInput->Init(hInstance, hWnd);

	//----------------------------
	// テクスチャの生成
	//----------------------------
	m_pTexture = new CTexture;

	//----------------------------
	// サウンドの生成と初期化
	----------------------------
	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	//----------------------------
	// ライトの生成と初期化
	//----------------------------
	m_pLight = new CLight;
	m_pLight->Init(GetRenderer()->GetDevice());

	//----------------------------
	// ジョイパッドの生成と初期化
	//----------------------------
	m_pJoyPad = new CJoypad;
	m_pJoyPad->Init(hInstance,hWnd,MAX_PLAYER);

	//----------------------------
	// モードの設定
	//----------------------------
	m_pFade = new CFade;
	SetMode(MODE_TITLE);
	m_pFade->Init(MODE_TITLE);

	//----------------------------
	// デバッグ用文字の生成
	//----------------------------
	m_pDebugproc->Init();


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
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	//ライトの終了
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	//タイトルの終了
	if (m_pTitle != nullptr)
	{
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = nullptr;
	}

	//チュートリアルの終了
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = nullptr;
	}

	//ゲームの終了
	if (m_pGame != nullptr)
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}

	//リザルトの終了
	if (m_pResult != nullptr)
	{
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
	}

	//フェードの終了
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	//デバッグ用文字の終了
	if (m_pDebugproc != nullptr)
	{
		m_pDebugproc->Uninit();
		delete m_pDebugproc;
		m_pDebugproc = nullptr;
	}

	//ジョイパッドの終了処理
	if (m_pJoyPad != nullptr)
	{
		m_pJoyPad->Uninit();
		delete m_pJoyPad;
		m_pJoyPad = nullptr;
	}
}

//===========================
// 更新
//===========================
void CApplication::Update()
{
	//インプットの更新
	m_pInput->Update();	//最初にやる

	//ジョイパッドの更新
	m_pJoyPad->Update();

	//レンダリングの更新
	m_pRenderer->Update();

	//モードごとの更新
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
		break;

	case MODE_PSELECT:
		m_pPSelect->Update();
		break;

	case MODE_TUTORIAL:
		m_pTutorial->Update();
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
	//m_pFade->Draw();

	//デバッグ用文字の描画
	//m_pDebugproc->Draw();
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
		delete m_pTitle;
		m_pTitle = nullptr;
		break;

	case MODE_PSELECT:
		m_pPSelect->Uninit();
		delete m_pPSelect;
		m_pPSelect = nullptr;
		break;

	case MODE_TUTORIAL:
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = nullptr;
		break;

	case MODE_GAME:
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
		break;

	case MODE_RESULT:
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
		break;

	default:
		break;
	}

	//オブジェクトの全解放
	CObject::ReleaseAll(true);
	//m_pFade->Create2DObject();

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

	case MODE_PSELECT:
		m_pPSelect = nullptr;
		m_pPSelect = new CPSelect;
		m_pPSelect->Init();
		break;

	case MODE_TUTORIAL:
		m_pTutorial = nullptr;
		m_pTutorial = new CTutorial;
		m_pTutorial->Init();
		break;

	case MODE_GAME:
		m_pGame = nullptr;
		m_pGame = new CGame;
		m_pGame->Init();

		//カメラの大きさのリセット
		m_pGame->ResetCameraSize();
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
	/*m_pFade = nullptr;
	m_pFade = new CFade;
	m_pFade->Init(m_mode);*/
}

//===========================
// モードの取得
//===========================
CApplication::MODE CApplication::GetMode()
{
	return m_mode;
}

//===========================
// ゲームの取得
//===========================
CGame* CApplication::GetGame()
{
	return m_pGame;
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

//===========================
// セレクトの取得
//===========================
CPSelect *CApplication::GetPSelect()
{
	return m_pPSelect;
}