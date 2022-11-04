//===================================================
//
// レンダリングの処理
// Author : Sato Teruto
//
//===================================================

//--------------------
// インクルード
//--------------------
#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>
#include <assert.h>
#include "renderer.h"
#include "main.h"
#include "object3d.h"
#include "application.h"
#include "camera_player.h"
#include "camera_title.h"
#include "debug_proc.h"
#include "input.h"
#include "input_keybord.h"
#include "Goal.h"
#include "title.h"

//-----------------------
// 静的メンバ変数宣言
//-----------------------
const float CRenderer::fDefaultFov = 45.0f;						//基本の視野角
const float CRenderer::fDefaultAspectX = (float)SCREEN_WIDTH;	//基本のアスペクト比X

int CRenderer::m_nMaxCamera = nDefaultMaxCamera;		//カメラの最大数
float CRenderer::m_fAspectFov = 0.0f;					//視野角
float CRenderer::m_fAspectX = 0.0f;						//アスペクト比X
CCameraPlayer* CRenderer::m_pCameraPlayer[nDefaultMaxCamera] = {};	//カメラ
CCameraTitle* CRenderer::m_pCameraTitle = nullptr;	//タイトルカメラ

//=========================
// コンストラクタ
//=========================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;		// Direct3Dオブジェクト
	m_pD3DDevice = nullptr;	// Deviceオブジェクト

	m_pFont = nullptr;		// フォント
	m_bWIRE = false;
	m_nFinish = false;
}

//=========================
// デストラクタ
//=========================
CRenderer::~CRenderer()
{
	assert(m_pD3D == nullptr);
	assert(m_pD3DDevice == nullptr);
	assert(m_pFont == nullptr);
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	//アスペクト比変更用変数の初期化
	m_fAspectFov = 60.0f;
	m_fAspectX = (float)(SCREEN_WIDTH / 2);

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;				// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;							// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if ((FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画と頂点処理をCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))))
	{
		// 生成失敗
		return E_FAIL;
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//ビューポートの描画順を初期化
	for (int i = 0; i < m_nMaxCamera; i++)
	{
		m_viewPortOrder[i] = i;
	}

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
#endif

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CRenderer::Uninit()
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif // _DEBUG

	// デバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CRenderer::Update()
{
	//オブジェクトの更新
	CObject::UpdateAll();

	if (CInputKeyboard::Trigger(DIK_F4))
	{//F4キーが押された
		if (m_bWIRE == false)
		{//ワイヤーフレームじゃないなら
		 //ワイヤーフレームの設定
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else if (m_bWIRE == true)
		{//ワイヤーフレームなら
		 //ワイヤーフレームを元の状態にする
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		m_bWIRE = !m_bWIRE;		//ワイヤーの状態を切り替える
	}
}

//=============================================================================
// 描画
//=============================================================================
void CRenderer::Draw()
{
	//-------------------------------
	// 1位のビューポートを前面に出す
	//-------------------------------
	/* 1位のプレイヤー番号を取得 */
	int nFirstNumber = CGoal::GetWinner();

	if (nFirstNumber <= CRenderer::GetMaxCamera() - 1)
	{//プレイヤー番号が最大数を超えていないなら
		if (nFirstNumber >= 0 /* 1位がnullじゃないなら */)
		{//Zが押されているなら
			//-----------------------------
			// 描画順の配列を入れ替える
			//-----------------------------
			int nviewData = 0;	//保存用変数

			//最後の値を保存
			nviewData = m_viewPortOrder[m_nMaxCamera - 1];

			//配列の最後と入れ替える
			m_viewPortOrder[m_nMaxCamera - 1] = nFirstNumber;

			//保存していた値を代入
			m_viewPortOrder[nFirstNumber] = nviewData;
		}
	}

	//-------------------------------
	// 描画処理
	//-------------------------------
	for (int i = 0; i < m_nMaxCamera; i++)
	{
		int nOrder = m_viewPortOrder[i];

		//-------------------------
		// カメラの設定
		//-------------------------
		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{//ゲーム画面なら
			//プレイヤーカメラの設定
			SetCameraPlayer(nOrder);
		}
		else if (CApplication::GetMode() == CApplication::MODE_TITLE)
		{//タイトル画面なら
			//タイトルカメラの設定
			SetCameraTitle();
		}

		// バックバッファ＆Ｚバッファのクリア
		m_pD3DDevice->Clear(0,
							NULL,
							(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
							D3DCOLOR_RGBA(0, 0, 0, 0),
							1.0f,
							0);

		// Direct3Dによる描画の開始
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{
			//オブジェクトの描画
			CObject::DrawAll();

#ifdef _DEBUG
			// FPS表示
			DrawFPS();

			CDebugProc::Draw();
#endif // _DEBUG

			// Direct3Dによる描画の終了
			m_pD3DDevice->EndScene();
		}
	}

	//ビューポートの描画順を初期化
	for (int i = 0; i < m_nMaxCamera; i++)
	{
		m_viewPortOrder[i] = i;
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// カメラの最大数の設定・取得
//=============================================================================
int CRenderer::SetMaxCamera(CGame::NUMCAMERA nNumCamera)
{
	switch (nNumCamera)
	{
	case CGame::NUMCAMERA_ONE:
		//カメラの最大数を1にする
		m_nMaxCamera = 1;
		break;

	case CGame::NUMCAMERA_TWO:
		//カメラの最大数を2にする
		m_nMaxCamera = 2;
		break;

	case CGame::NUMCAMERA_THREE:
	case CGame::NUMCAMERA_FOUR:
		//カメラの最大数を4にする
		m_nMaxCamera = 4;
		break;

	default:
		m_nMaxCamera = 0;
		break;
	}

	return nNumCamera;
}

//=============================================================================
// アスペクト比の加算
//=============================================================================
void CRenderer::AddAcpect(int nNumCamera, float fov, float x)
{
	if (m_fAspectFov >= fDefaultFov)
	{//視野角が45以上なら
		//視野角を減少
		m_fAspectFov -= fov;
	}
	if (m_fAspectX <= fDefaultAspectX)
	{//アスペクト比Xがスクリーンの幅以内なら
		//アスペクト比Xを加算
		m_fAspectX += x;
	}

	//カメラのアスペクト比を変更
	m_pCameraPlayer[nNumCamera]->SetAspect(m_pD3DDevice, m_fAspectFov,
		m_fAspectX, (float)SCREEN_HEIGHT);
}

//=============================================================================
// カメラの最大数の取得
//=============================================================================
int CRenderer::GetMaxCamera()
{
	return m_nMaxCamera;
}

//=============================================================================
// プレイヤーカメラの設定
//=============================================================================
void CRenderer::SetCameraPlayer(int nOrder)
{
	//プレイヤーカメラの取得
	m_pCameraPlayer[nOrder] = CGame::GetCameraPlayer(nOrder);

	if (m_pCameraPlayer[nOrder] != nullptr)
	{//カメラがnullじゃないなら
		//-------------------------
		// カメラの設定
		//-------------------------
		m_pCameraPlayer[nOrder]->SetCamera(m_pD3DDevice);

		//-------------------------
		// カメラが2つの場合
		//-------------------------
		if (m_nMaxCamera == 2)
		{//カメラの数が2つなら
		 //カメラのアスペクト比を変更
			m_pCameraPlayer[nOrder]->SetAspect(m_pD3DDevice, 60.0f,
				(float)(SCREEN_WIDTH / 2), (float)SCREEN_HEIGHT);

			//-------------------------
			// 拡大する時の処理
			//-------------------------
			if (CGame::GetFinish() == true)
			{//終了フラグが立っているなら
			 //アスペクト比の加算
				AddAcpect(nOrder, (15.0f / 60.0f), (640.0f / 120.0f));
			}
		}

		//-------------------------
		// ビューポートの処理
		//-------------------------
		//ビューポートの設定
		D3DVIEWPORT9 viewport = m_pCameraPlayer[nOrder]->GetViewport();
		m_pD3DDevice->SetViewport(&viewport);
	}
}

//=============================================================================
// タイトルカメラの設定
//=============================================================================
void CRenderer::SetCameraTitle()
{
	//タイトルカメラの取得
	m_pCameraTitle = CTitle::GetCameraTitle();

	if (m_pCameraTitle != nullptr)
	{//カメラがnullじゃないなら
		//-------------------------
		// カメラの設定
		//-------------------------
		m_pCameraTitle->SetCamera(m_pD3DDevice);

		//-------------------------
		// ビューポートの処理
		//-------------------------
		//ビューポートの設定
		D3DVIEWPORT9 viewport = m_pCameraTitle->GetViewport();
		m_pD3DDevice->SetViewport(&viewport);
	}
}

#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void CRenderer::DrawFPS()
{
	int nCntFPS = GetFPS();	//FPSを取得

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];

	wsprintf(str, _T("FPS : %d\n"), nCntFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif // _DEBUG