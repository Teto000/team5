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
#include  "camera.h"
#include "time.h"
#include "GoalFragManager.h"

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
	//カメラの最大数の設定
	{
		int nNumCamera = CRenderer::GetMaxCamera();

		//カメラの最大数分プレイヤーを生成
		for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
		{
			pPlayer[nCnt] = CPlayer::Create(nCnt);

			//カメラに対応するプレイヤーの番号の設定
			CApplication::GetCamera(nCnt)->SetNumPlayer(nCnt);
		}
	}
	CTime *pTime = CTime::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

	pMeshField = CMeshField::Create();

	CGoalFragManager*pGoal = CGoalFragManager::Create(D3DXVECTOR3(90.0f, 40.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	

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