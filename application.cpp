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
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "player.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CTitle*				CApplication::m_pTitle = nullptr;		//タイトルクラス
CGame*				CApplication::m_pGame = nullptr;		//ゲームクラス
CResult*			CApplication::m_pResult = nullptr;		//リザルトクラス
CTutorial*			CApplication::m_pTutorial = nullptr;	//チュートリアルクラス
CFade*				CApplication::m_pFade = nullptr;		//フェードクラス
CApplication::MODE	CApplication::m_mode = MODE_MAX;		//ゲームモード

CRenderer*	CApplication::m_pRenderer = nullptr;			//レンダラー
CInput*		CApplication::m_pInput = nullptr;				//インプット
CTexture*	CApplication::m_pTexture = nullptr;				//テクスチャ
CSound*		CApplication::m_pSound = nullptr;				//サウンド
CCamera*	CApplication::m_pCamera[nDefaultMaxCamera] = {};		//カメラ
CLight*		CApplication::m_pLight = nullptr;				//ライト

bool CApplication::m_bStop = false;	//プログラムを停止する

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
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
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
	//----------------------------
	//m_pSound = new CSound;
	//m_pSound->Init(hWnd);

	//----------------------------
	// カメラの生成と初期化
	//----------------------------
	{
		//カメラの最大数の設定
		int nNumCamera = CRenderer::SetMaxCamera(NUMCAMERA_ONE);

		DWORD fWidth = SCREEN_WIDTH / 2;
		DWORD fHeight = SCREEN_HEIGHT / 2;

		switch (nNumCamera)
		{
		case NUMCAMERA_ONE:
			//カメラの数が1つなら
			m_pCamera[0] = CCamera::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			break;
			
		case NUMCAMERA_TWO:
			//カメラの数が2つなら
			m_pCamera[0] = CCamera::Create(0, 0, fWidth, SCREEN_HEIGHT);
			m_pCamera[1] = CCamera::Create(fWidth, 0, fWidth, SCREEN_HEIGHT);
			break;

		case NUMCAMERA_FOUR:
			//カメラの数が4つなら
			m_pCamera[0] = CCamera::Create(0, 0, fWidth, fHeight);				//左上
			m_pCamera[1] = CCamera::Create(fWidth, 0, fWidth, fHeight);			//右上
			m_pCamera[2] = CCamera::Create(0, fHeight, fWidth, fHeight);		//左下
			m_pCamera[3] = CCamera::Create(fWidth, fHeight, fWidth, fHeight);	//右下
			break;

		default:
			//プログラムを停止する
			m_bStop = true;

			//警告
			MessageBox(hWnd, "カメラの分割数が正常ではありません", "警告！", MB_ICONWARNING);
			exit(m_bStop);	//プログラムを停止する
		}
	}

	//----------------------------
	// ライトの生成と初期化
	//----------------------------
	m_pLight = new CLight;
	m_pLight->Init(GetRenderer()->GetDevice());

	//----------------------------
	// モードの設定
	//----------------------------

	m_pFade = new CFade;
	SetMode(MODE_GAME);
	m_pFade->Init(MODE_GAME);
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
	{
		int nNumCamera = CRenderer::GetMaxCamera();
		for (int i = 0; i < nNumCamera; i++)
		{
			if (m_pCamera[i] != nullptr)
			{//カメラがnullじゃないなら 
				m_pCamera[i]->Uninit();
				delete m_pCamera[i];
				m_pCamera[i] = nullptr;
			}
		}
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
	{
		int nNumCamera = CRenderer::GetMaxCamera();
		for (int i = 0; i < nNumCamera; i++)
		{
			m_pCamera[i]->Update();
		}

		//ゲーム終了時の処理
		FinishGame();
	}

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
		m_pGame = nullptr;
		break;

	case MODE_RESULT:
		m_pResult->Uninit();
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
	/*m_pFade = nullptr;
	m_pFade = new CFade;
	m_pFade->Init(m_mode);*/
}

//===========================
// ゲーム終了時の処理
//===========================
void CApplication::FinishGame()
{
	//-------------------------------
	// 1位のビューポートを拡大する
	//-------------------------------

	/* 1位のプレイヤー番号を取得 */
	int nFirstNumber = 1;

	if (CInputKeyboard::Press(DIK_Z) /* 1位がnullじゃないなら */)
	{//Zが押されているなら

		//-----------------------
		// ビューポートを拡大
		//-----------------------
		switch (nFirstNumber)
		{
		//-----------------------
		// 1人目が勝った
		//-----------------------
		case CCamera::NUMPLAYER_ONE:
			m_pCamera[nFirstNumber]->AddViewSize(0, 0, nSpeed_X, nSpeed_Y);
			break;

		//-----------------------
		// 2人目が勝った
		//-----------------------
		case CCamera::NUMPLAYER_TWO:
			m_pCamera[nFirstNumber]->AddViewSize(-nSpeed_X, 0, nSpeed_X, nSpeed_Y);
			break;

		//-----------------------
		// 3人目が勝った
		//-----------------------
		case CCamera::NUMPLAYER_THREE:
			m_pCamera[nFirstNumber]->AddViewSize(0, -nSpeed_Y, nSpeed_X, nSpeed_Y);
			break;

		//-----------------------
		// 4人目が勝った
		//-----------------------
		case CCamera::NUMPLAYER_FOUR:
			m_pCamera[nFirstNumber]->AddViewSize(-nSpeed_X, -nSpeed_Y, nSpeed_X, nSpeed_Y);
			break;

		default:
			break;
		}
	}
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
// カメラの取得
//===========================
CCamera *CApplication::GetCamera(int nCnt)
{
	return m_pCamera[nCnt];
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