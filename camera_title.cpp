//==================================================
//
// タイトルタイトルカメラの処理
// Author:Teruto Sato
//
//==================================================

//----------------------
// インクルード
//----------------------
#include "camera_title.h"
#include "camera.h"
#include "input.h"
#include "input_keybord.h"
#include "application.h"

//==================================================
// コンストラクタ
//==================================================
CCameraTitle::CCameraTitle()
{

}

//==================================================
// デストラクタ
//==================================================
CCameraTitle::~CCameraTitle()
{

}

//==================================================
// タイトルカメラの初期化処理
//==================================================
HRESULT CCameraTitle::Init(void)
{
	//カメラの初期化
	CCamera::Init();

	return S_OK;
}

//==================================================
// タイトルカメラの終了処理
//==================================================
void CCameraTitle::Uninit(void)
{
	//カメラの終了
	CCamera::Uninit();
}

//==================================================
// タイトルカメラの更新処理
//==================================================
void CCameraTitle::Update(void)
{
	//カメラの更新
	CCamera::Update();
}

//==================================================
// タイトルカメラの設定処理
// 引数 : デバイス
//==================================================
void CCameraTitle::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//カメラの設定
	CCamera::SetCamera(pDevice);
}

//==================================================
// タイトルカメラの生成
// 引数 : 左上の座標 X, 左上の座標 Y, 幅, 高さ
//==================================================
CCameraTitle* CCameraTitle::Create(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	CCameraTitle *pCameraTitle = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pCameraTitle = new CCameraTitle;	//生成

	if (pCameraTitle != nullptr)
	{//NULLチェック
	 //ビューポート構成の保存
		pCameraTitle->m_viewport.X = X;				//開始位置(X)
		pCameraTitle->m_viewport.Y = Y;				//開始位置(Y)
		pCameraTitle->m_viewport.Width = Width;		//幅
		pCameraTitle->m_viewport.Height = Height;	//高さ

		//初期化
		pCameraTitle->Init();
	}

	return pCameraTitle;
}