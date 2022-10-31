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
#include "camera.h"
#include "debug_proc.h"
#include "input.h"
#include "input_keybord.h"

//-----------------------
// 静的メンバ変数宣言
//-----------------------
int CRenderer::m_nMaxCamera = nDefaultMaxCamera;	//カメラの最大数
CCamera* CRenderer::m_pCamera[nDefaultMaxCamera] = {};	//カメラ

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
	int nFirstNumber = 0;

	if (nFirstNumber <= CRenderer::GetMaxCamera() - 1)
	{//プレイヤー番号が最大数を超えていないなら
		if (CInputKeyboard::Press(DIK_Z) /* 1位がnullじゃないなら */)
		{//Zが押されているなら
			//-----------------------------
			// 描画順の配列を入れ替える
			//-----------------------------
			int nviewData = m_viewPortOrder[nFirstNumber];

			if (m_nMaxCamera == 2)
			{//最大数が2なら
				//配列の1番目と入れ替える
				m_viewPortOrder[1] = nFirstNumber;
			}
			else if (m_nMaxCamera == 4)
			{//最大数が4なら
				//配列の3番目と入れ替える
				m_viewPortOrder[3] = nFirstNumber;
			}

			m_viewPortOrder[nFirstNumber] = nviewData;
		}
	}

	//-------------------------------
	// 描画処理
	//-------------------------------
	for (int i = 0; i < m_nMaxCamera; i++)
	{
		int nOrder = m_viewPortOrder[i];

		//カメラの取得
		m_pCamera[nOrder] = CApplication::GetCamera(nOrder);

		//カメラの設定
		m_pCamera[nOrder]->SetCamera(m_pD3DDevice);

		//-------------------------
		// ビューポートの処理
		//-------------------------
		//ビューポートの設定
		D3DVIEWPORT9 viewport = m_pCamera[nOrder]->GetViewport();
		m_pD3DDevice->SetViewport(&viewport);

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

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// カメラの最大数の設定・取得
//=============================================================================
int CRenderer::SetMaxCamera(CApplication::NUMCAMERA nNumCamera)
{
	switch (nNumCamera)
	{
	case CApplication::NUMCAMERA_ONE:
		//カメラの最大数を1にする
		m_nMaxCamera = 1;
		break;

	case CApplication::NUMCAMERA_TWO:
		//カメラの最大数を2にする
		m_nMaxCamera = 2;
		break;

	case CApplication::NUMCAMERA_FOUR:
		//カメラの最大数を4にする
		m_nMaxCamera = 4;
		break;

	default:
		break;
	}

	return nNumCamera;
}

//=============================================================================
// カメラの最大数の取得
//=============================================================================
int CRenderer::GetMaxCamera()
{
	return m_nMaxCamera;
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