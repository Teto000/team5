//===================================================
//
// レンダリングヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_	//二重インクルード防止

//--------------------
// インクルード
//--------------------
#include <tchar.h> // _T
#include <d3dx9.h>
#include "game.h"

//--------------------
// 前方宣言
//--------------------
class CCamera;

//*****************************************************************************
// 定数定義
//*****************************************************************************
namespace
{
	// ウインドウのクラス名
	LPCTSTR CLASS_NAME = _T("AppClass");
	// ウインドウのキャプション名
	LPCTSTR WINDOW_NAME = _T("光る！鳴る！DXショトカイザー！！");
}

// スクリーンの幅
const int SCREEN_WIDTH = 1280;
// スクリーンの高さ
const int SCREEN_HEIGHT = 720;
// 頂点フォーマット
const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点データ
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
};

struct VERTEX_3D
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャへのポインタ
};

//==============================
// レンダリングクラスの定義
//==============================
class CRenderer
{
public:
	CRenderer();	//コンストラクタ
	~CRenderer();	//デストラクタ

	//-----------------
	// メンバ関数
	//-----------------
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	//デバイスの取得
	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }

	//-----------------
	// 静的メンバ関数
	//-----------------
	static int SetMaxCamera(CGame::NUMCAMERA nNumCamera);
	static int GetMaxCamera();

private:
	void DrawFPS();
	void AddAcpect(int nNumCamera, float fov, float x);	//アスペクト比の加算

private:
	//-----------------
	// 定数
	//-----------------
	static const float fDefaultFov;		//基本の視野角
	static const float fDefaultAspectX;	//基本のアスペクト比X

	//-----------------
	// メンバ変数
	//-----------------
	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Deviceオブジェクト
	LPD3DXFONT m_pFont;					// フォント
	bool m_bWIRE;	//ワイヤーフレーム
	bool m_nFinish;	//終了フラグ
	int m_viewPortOrder[nDefaultMaxCamera];

	//-------------------
	// 静的メンバ変数
	//-------------------
	static int m_nMaxCamera;	//カメラの最大数
	static float m_fAspectFov;	//視野角
	static float m_fAspectX;	//アスペクト比X
	static CCamera*	m_pCamera[nDefaultMaxCamera];	//カメラクラス
};

#endif // !_RENDERER_H_