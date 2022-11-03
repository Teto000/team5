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
CCamera* CGame::m_pCamera[nDefaultMaxCamera] = {};		//カメラ

int	 CGame::m_player = 0;		//プレイヤーの数
bool CGame::m_bFinish = false;	//終了フラグ

//===========================
// コンストラクタ
//===========================
CGame::CGame()
{
	m_nEnumCamera = 0;	//カメラの列挙型の数
	m_nMaxCamera = 0;	//カメラの最大数
	m_bStop = false;	//プログラムを停止する
}

//===========================
// デストラクタ
//===========================
CGame::~CGame()
{
	m_nMaxCamera = 0;
}

//===========================
// 初期化
//===========================
HRESULT CGame::Init()
{
	//カメラの生成
	//カメラ・プレイヤーの人数設定はここ
	CreateCamera((CGame::NUMCAMERA)m_player);

	//----------------------------
	// カメラの最大数の設定
	//----------------------------
	if (m_nEnumCamera == NUMCAMERA_THREE)
	{//カメラ列挙型が3なら
		//カメラの最大数を1減らす
		m_nMaxCamera = 3;
	}

	//カメラの最大数分プレイヤーを生成
	for (int nCnt = 0; nCnt < m_nMaxCamera; nCnt++)
	{
		pPlayer[nCnt] = CPlayer::Create(nCnt);

		//カメラに対応するプレイヤーの番号の設定
		GetCamera(nCnt)->SetNumPlayer(nCnt);
	}

	/*SetPlayerPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));*/

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
	//-----------------------------
	// カメラの終了
	//-----------------------------
	if (m_nEnumCamera == NUMCAMERA_THREE)
	{//カメラ列挙型が3なら
		//カメラの最大数を戻す
		m_nMaxCamera = 4;
	}

	for (int i = 0; i < m_nMaxCamera; i++)
	{
		if (m_pCamera[i] != nullptr)
		{//カメラがnullじゃないなら 
			m_pCamera[i]->Uninit();
			delete m_pCamera[i];
			m_pCamera[i] = nullptr;
		}
	}
}

//===========================
// 更新
//===========================
void CGame::Update()
{
	//-----------------------------
	// カメラの更新
	//-----------------------------
	if (m_nEnumCamera == NUMCAMERA_THREE)
	{//カメラ列挙型が3なら
		//カメラの最大数を戻す
		m_nMaxCamera = 4;
	}

	for (int i = 0; i < m_nMaxCamera; i++)
	{
		m_pCamera[i]->Update();
	}

	//ゲーム終了時の処理
	if (CGoal::GetGoalFrag())
	{
		FinishGame();
	}

	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enterで次の画面に遷移する
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}
}

//===========================
// カメラの生成
// (カメラの数列挙型)
//===========================
void CGame::CreateCamera(CGame::NUMCAMERA num)
{
	//----------------------------
	// カメラの生成と初期化
	//----------------------------
	{
		//カメラの最大数の設定
		m_nEnumCamera = CRenderer::SetMaxCamera(num);

		DWORD fWidth = SCREEN_WIDTH / 2;
		DWORD fHeight = SCREEN_HEIGHT / 2;

		switch (m_nEnumCamera)
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

		case NUMCAMERA_THREE:
		case NUMCAMERA_FOUR:
			//カメラの数が4つなら
			m_pCamera[0] = CCamera::Create(0, 0, fWidth, fHeight);				//左上
			m_pCamera[1] = CCamera::Create(fWidth, 0, fWidth, fHeight);			//右上
			m_pCamera[2] = CCamera::Create(0, fHeight, fWidth, fHeight);		//左下
			m_pCamera[3] = CCamera::Create(fWidth, fHeight, fWidth, fHeight);	//右下
			break;

		default:
			//プログラム停止フラグを立てる
			m_bStop = true;
			exit(m_bStop);	//プログラムを停止する
		}

		//カメラの最大数を取得
		m_nMaxCamera = CRenderer::GetMaxCamera();
	}
}

//===========================
// ゲーム終了時の処理
//===========================
void CGame::FinishGame()
{
	//-------------------------------
	// 1位のビューポートを拡大する
	//-------------------------------

	/* 1位のプレイヤー番号を取得 */
	int nFirstNumber = CGoal::GetWinner();

	m_bFinish = true;	//終了フラグを立てる

	if (nFirstNumber >= 0)
	{//1位がnullじゃないなら

		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

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
// カメラの大きさのリセット
//===========================
void CGame::ResetCameraSize()
{
	DWORD fWidth = SCREEN_WIDTH / 2;
	DWORD fHeight = SCREEN_HEIGHT / 2;

	switch (m_nEnumCamera)
	{
	case NUMCAMERA_ONE:
		//カメラの数が1つなら
		m_pCamera[0]->SetViewSize(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;

	case NUMCAMERA_TWO:
		//カメラの数が2つなら
		m_pCamera[0]->SetViewSize(0, 0, fWidth, SCREEN_HEIGHT);
		m_pCamera[1]->SetViewSize(fWidth, 0, fWidth, SCREEN_HEIGHT);
		break;

	case NUMCAMERA_THREE:
	case NUMCAMERA_FOUR:
		//カメラの数が4つなら
		m_pCamera[0]->SetViewSize(0, 0, fWidth, fHeight);				//左上
		m_pCamera[1]->SetViewSize(fWidth, 0, fWidth, fHeight);			//右上
		m_pCamera[2]->SetViewSize(0, fHeight, fWidth, fHeight);			//左下
		m_pCamera[3]->SetViewSize(fWidth, fHeight, fWidth, fHeight);	//右下
		break;

	default:
		//プログラム停止フラグを立てる
		m_bStop = true;
		exit(m_bStop);	//プログラムを停止する
	}
}

//===========================
// カメラ列挙型番号を取得
//===========================
int CGame::GetEnumCamera()
{
	return m_nEnumCamera;
}

//===========================
// プレイヤーの取得
//===========================
CPlayer* CGame::GetPlayer(int NumPlayer)
{
	return pPlayer[NumPlayer];
}

//===========================
// カメラの取得
//===========================
CCamera *CGame::GetCamera(int nCnt)
{
	return m_pCamera[nCnt];
}

//===========================
// 終了フラグの取得
//===========================
bool CGame::GetFinish()
{
	return m_bFinish;
}

//===========================
// プレイヤーの初期座標設定
//===========================
void CGame::SetPlayerPosition(D3DXVECTOR3 pos)
{
	//時刻をもとにしたランダムな値を生成
	srand((unsigned int)time(NULL));

	int nNumCamera = CRenderer::GetMaxCamera();

	// プレイヤーの初期位置の設定
	D3DXVECTOR3 FirstPos = pos;

	// FirstPosから等間隔にほかのプレイヤーの座標を設定する。(最大4)
	D3DXVECTOR3 PlayerPos[4] = {};

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// プレイヤーの座標を設定
		PlayerPos[nCnt] = D3DXVECTOR3(pos.x + nCnt * 100, pos.y + nCnt, pos.z);
	}

	// 数字保存用
	// 初期値が0だと問題があるので99
	int Rand[4] = { 99,99,99,99 };
	int RandCheck[4] = { 99,99,99,99 };
	int nCnt2 = 0;

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// ランダムの値
		Rand[nCnt] = rand() % 4;

		while (1)
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
			if (nCnt2 == nNumCamera)
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