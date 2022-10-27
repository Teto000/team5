//===================================================
//
// ゲームの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <time.h>
#include "game.h"
#include "renderer.h"
#include "application.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "sound.h"
#include "player.h"
#include "meshfield.h"
#include "debug_proc.h"
#include "fade.h"
#include "time.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CPolygon* CGame::pPolygon = nullptr;
CPolygon2d* CGame::pPolygon2d = nullptr;
CPlayer*  CGame::pPlayer[MAX_PLAYER] = {};
CMeshField* CGame::pMeshField = nullptr;

//===========================
// コンストラクタ
//===========================
CGame::CGame()
{
	
}

//===========================
// デストラクタ
//===========================
CGame::~CGame()
{

}

//===========================
// 初期化
//===========================
HRESULT CGame::Init()
{
	CTime *pTime = CTime::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		pPlayer[nCnt] = CPlayer::Create(nCnt);
	}
	pMeshField = CMeshField::Create();

	return S_OK;
}

//===========================
// 終了
//===========================
void CGame::Uninit()
{

}

//===========================
// 更新
//===========================
void CGame::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enterで次の画面に遷移する
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}
}

//===========================
// プレイヤーの取得
//===========================
CPlayer* CGame::GetPlayer(int NumPlayer)
{
	return pPlayer[NumPlayer];
}