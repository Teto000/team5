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
#include "Goal.h"

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

		SetPlayerPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	pMeshField = CMeshField::Create();

	CGoal*pGoal = CGoal::Create(D3DXVECTOR3(90.0f, 40.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	CTime *pTime = CTime::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

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

//===========================
// プレイヤーの初期座標設定
//===========================
void CGame::SetPlayerPosition(D3DXVECTOR3 pos)
{
	int nNumCamera = CRenderer::GetMaxCamera();

	//時刻をもとにしたランダムな値を生成
	srand((unsigned int)time(NULL));

	// プレイヤーの初期位置の設定
	D3DXVECTOR3 FirstPos = pos;

	// FirstPosから等間隔にほかのプレイヤーの座標を設定する。(最大4)
	D3DXVECTOR3 PlayerPos[4] = {};

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// プレイヤーの座標を設定
		PlayerPos[nCnt] = D3DXVECTOR3(pos.x + nCnt * 100, pos.y + nCnt,pos.z);
	}

	// 数字保存用
	// 初期値が0だと問題があるので99
	int Rand[4] = {99,99,99,99};
	int RandCheck[4] = {99,99,99,99};
	int nCnt2 = 0;

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// ランダムの値
		Rand[nCnt] = rand() % 4;

		while(1)
		{
			// Randが今までの値のどれかと同じだった時
			if (Rand[nCnt] == RandCheck[nCnt2])
			{
				// ランダムの値
				Rand[nCnt] = rand() % 4;
				nCnt2 = 0;

				continue;
			}
			// 通った回数を足す
			nCnt2++;

			// 全ての箱のチェックが終わったら
			if (nCnt2 == PlayerAmount)
			{
				break;
			}
		}

		// ランドチェックに値を入れる
		RandCheck[nCnt] = Rand[nCnt];
	}

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// 設定した座標にランダムなプレイヤーを移動させる。
		pPlayer[nCnt]->SetPosition(PlayerPos[Rand[nCnt]]);
	}
}