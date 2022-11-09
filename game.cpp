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
#include "camera_player.h"
#include "time.h"
#include "Goal.h"
#include "message.h"
#include "motion_parts.h"
#include "3dobject.h"
#include "Editor.h"
#include "debug_proc.h"
#include "Map.h"
#include "read.h"
#include "num_block.h"
#include "Score.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CPolygon*	CGame::pPolygon = nullptr;
CPolygon2d*	CGame::pPolygon2d = nullptr;
CPlayer*	CGame::pPlayer[MAX_PLAYER] = {};
CMeshField*	CGame::pMeshField = nullptr;
CCameraPlayer*	CGame::m_pCameraPlayer[nDefaultMaxCamera] = {};		//カメラ
CMessage*	CGame::m_pMessage;	//メッセージ
CEditor*	CGame::m_pEditor = nullptr;
CDebugProc*	CGame::m_pProc = nullptr;
CMap*		CGame::m_pMap = nullptr;

int	 CGame::m_nEnumCamera = 0;	//カメラの列挙型の数
int	 CGame::m_nGroundNum = 0;		//床
int	 CGame::m_player = 0;		//プレイヤーの数
bool CGame::m_bFinish = false;	//終了フラグ

//===========================
// コンストラクタ
//===========================
CGame::CGame()
{
	m_nMaxCamera = 0;		//カメラの最大数
	m_bStop = false;		//プログラムを停止する
	mode = GAMEMODE_START;	//ゲームの状態
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
	//変数の初期化
	m_bFinish = false;

	//カメラの生成
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
		GetCameraPlayer(nCnt)->SetNumPlayer(nCnt);
	}

	SetPlayerPosition(D3DXVECTOR3(300.0f, 50.0f, -100.0f));

	//メッシュフィールドの生成
	//pMeshField = CMeshField::Create();
	//m_pMap = CMap::Create(D3DXVECTOR3(0.0f, 0.0f, 100.0f));

	//タイマーの生成
	CTime *pTime = CTime::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

	//エディタの作成と読み込み
	m_pEditor = CEditor::Create();

	//メッセージの生成
	m_pMessage = CMessage::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
									CMessage::MESSAGE_COUNT_THREE);

	//サウンド生成
	CSound::PlaySound(CSound::SOUND_LABEL_GAME1);

	CRead cRead;
	m_nGroundNum = cRead.ReadMotion("data/MOTION/motionground.txt");

	//CObjectX::Create("data\\MODEL\\X_File\\Neptune_000.x", D3DXVECTOR3(200.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//===========================
// 終了
//===========================
void CGame::Uninit()
{
	//サウンド停止
	CSound::StopSound();

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
		if (m_pCameraPlayer[i] != nullptr)
		{//カメラがnullじゃないなら 
			m_pCameraPlayer[i]->Uninit();
			delete m_pCameraPlayer[i];
			m_pCameraPlayer[i] = nullptr;
		}
	}

	C3DObject::UninitAllModel();

	CMotionParts::ALLUninit();

	//エディタの削除
	m_pEditor->Uninit();
	delete m_pEditor;
	m_pEditor = nullptr;
}

//===========================
// 更新
//===========================
void CGame::Update()
{
	static int nCnt = 0;
	nCnt++;

	//エディタの更新
	m_pEditor->Update();

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
		m_pCameraPlayer[i]->Update();
	}

	//ゲーム終了時の処理
	if (CGoal::GetGoalFrag())
	{
		//メッセージの生成
		//m_pMessage = CMessage::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		//								CMessage::MESSAGE_FINISH);

		FinishGame();

		//一回だけ通るようにしたい…
		const int ClearTime = CTime::GetTime();
		CScore::SetCurrentScore(ClearTime);
	}

	CMotionParts::ALLUpdate();

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
			m_pCameraPlayer[0] = CCameraPlayer::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			break;

		case NUMCAMERA_TWO:
			//カメラの数が2つなら
			m_pCameraPlayer[0] = CCameraPlayer::Create(0, 0, fWidth, SCREEN_HEIGHT);
			m_pCameraPlayer[1] = CCameraPlayer::Create(fWidth, 0, fWidth, SCREEN_HEIGHT);
			break;

		case NUMCAMERA_THREE:
		case NUMCAMERA_FOUR:
			//カメラの数が4つなら
			m_pCameraPlayer[0] = CCameraPlayer::Create(0, 0, fWidth, fHeight);				//左上
			m_pCameraPlayer[1] = CCameraPlayer::Create(fWidth, 0, fWidth, fHeight);			//右上
			m_pCameraPlayer[2] = CCameraPlayer::Create(0, fHeight, fWidth, fHeight);		//左下
			m_pCameraPlayer[3] = CCameraPlayer::Create(fWidth, fHeight, fWidth, fHeight);	//右下
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
		case CCameraPlayer::NUMPLAYER_ONE:
			m_pCameraPlayer[nFirstNumber]->AddViewSize(0, 0, nSpeed_X, nSpeed_Y);
			break;

			//-----------------------
			// 2人目が勝った
			//-----------------------
		case CCameraPlayer::NUMPLAYER_TWO:
			m_pCameraPlayer[nFirstNumber]->AddViewSize(-nSpeed_X, 0, nSpeed_X, nSpeed_Y);
			break;

			//-----------------------
			// 3人目が勝った
			//-----------------------
		case CCameraPlayer::NUMPLAYER_THREE:
			m_pCameraPlayer[nFirstNumber]->AddViewSize(0, -nSpeed_Y, nSpeed_X, nSpeed_Y);
			break;

			//-----------------------
			// 4人目が勝った
			//-----------------------
		case CCameraPlayer::NUMPLAYER_FOUR:
			m_pCameraPlayer[nFirstNumber]->AddViewSize(-nSpeed_X, -nSpeed_Y, nSpeed_X, nSpeed_Y);
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
		m_pCameraPlayer[0]->SetViewSize(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;

	case NUMCAMERA_TWO:
		//カメラの数が2つなら
		m_pCameraPlayer[0]->SetViewSize(0, 0, fWidth, SCREEN_HEIGHT);
		m_pCameraPlayer[1]->SetViewSize(fWidth, 0, fWidth, SCREEN_HEIGHT);
		break;

	case NUMCAMERA_THREE:
	case NUMCAMERA_FOUR:
		//カメラの数が4つなら
		m_pCameraPlayer[0]->SetViewSize(0, 0, fWidth, fHeight);				//左上
		m_pCameraPlayer[1]->SetViewSize(fWidth, 0, fWidth, fHeight);			//右上
		m_pCameraPlayer[2]->SetViewSize(0, fHeight, fWidth, fHeight);			//左下
		m_pCameraPlayer[3]->SetViewSize(fWidth, fHeight, fWidth, fHeight);	//右下
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
CCameraPlayer *CGame::GetCameraPlayer(int nCnt)
{
	return m_pCameraPlayer[nCnt];
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

	//int nNumCamera = CRenderer::GetMaxCamera();

	m_nEnumCamera = CGame::GetEnumCamera();

	if (m_nEnumCamera == NUMCAMERA_THREE)
	{//カメラ列挙型が3なら
	 //カメラの最大数を1減らす
		m_nMaxCamera = 3;
	}

	// プレイヤーの初期位置の設定
	D3DXVECTOR3 FirstPos = pos;

	// FirstPosから等間隔にほかのプレイヤーの座標を設定する。(最大4)
	D3DXVECTOR3 PlayerPos[4] = {};

	for (int nCnt = 0; nCnt < m_nMaxCamera; nCnt++)
	{
		// プレイヤーの座標を設定
		PlayerPos[nCnt] = D3DXVECTOR3(pos.x + nCnt * 100, pos.y + nCnt, pos.z);
	}

	// 数字保存用
	// 初期値が0だと問題があるので99
	int Rand[4] = { 99,99,99,99 };
	int RandCheck[4] = { 99,99,99,99 };
	int nCnt2 = 0;

	for (int nCnt = 0; nCnt < m_nMaxCamera; nCnt++)
	{
		// ランダムの値
		Rand[nCnt] = rand() % m_nMaxCamera;

		while (1)
		{
			// Randが今までの値のどれかと同じだった時
			if (Rand[nCnt] == RandCheck[nCnt2])
			{
				// ランダムの値
				Rand[nCnt] = rand() % m_nMaxCamera;
				nCnt2 = 0;

				continue;
			}
			// 通った回数を足す
			nCnt2++;

			// 全ての箱のチェックが終わったら
			if (nCnt2 == m_nMaxCamera)
			{
				break;
			}
		}

		// ランドチェックに値を入れる
		RandCheck[nCnt] = Rand[nCnt];
	}

	for (int nCnt = 0; nCnt < m_nMaxCamera; nCnt++)
	{
		// 設定した座標にランダムなプレイヤーを移動させる。
		pPlayer[nCnt]->SetPosition(PlayerPos[Rand[nCnt]]);
	}
}