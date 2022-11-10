//===================================================
//
// プレイヤーの人数のセレクト.cpp
// Author : Tomoki Tomidokoro
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <time.h>
#include "select_player.h"
#include "renderer.h"
#include "application.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "sound.h"
#include "player.h"
#include "meshfield.h"
#include "debug_proc.h"
#include "camera.h"
#include "time.h"
#include "UI_PSelect.h"
#include "fade.h"

#define MAX_WINDOW (3)
//===========================
// コンストラクタ
//===========================
CPSelect::CPSelect()
{
	m_PlayerSelectNum = 0;

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_PUI[nCnt] = nullptr;
	}
}

//===========================
// デストラクタ
//===========================
CPSelect::~CPSelect()
{

}

//===========================
// 初期化
//===========================
HRESULT CPSelect::Init()
{
	m_BG = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f,0.0f), CUI_PSelect::UI_BG);
	m_title = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 4.0f, 0.0f), CUI_PSelect::UI_COMENT);
	m_Select = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 200.0f, 0.0f), CUI_PSelect::UI_SELECT);
	UI_Create();
	//サウンド生成
	CSound::PlaySound(CSound::SOUND_LABEL_SELECT);
	return S_OK;
}

//===========================
// 終了
//===========================
void CPSelect::Uninit()
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_PUI[nCnt] != nullptr)
		{
			m_PUI[nCnt]->Uninit();
			m_PUI[nCnt] = nullptr;
		}
	}

	if (m_title != nullptr)
	{
		m_title->Uninit();
		m_title = nullptr;
	}
	if (m_BG != nullptr)
	{
		m_BG->Uninit();
		m_BG = nullptr;
	}
	if (m_Select != nullptr)
	{
		m_Select->Uninit();
		m_Select = nullptr;
	}
	//サウンド停止
	CSound::StopSound();
}

//===========================
// 更新
//===========================
void CPSelect::Update()
{
	// プレイヤーの数を指定する
	SelectPlayerNum();

	// カメラの個数の指定
	CGame::SetPlayerNum(m_PlayerSelectNum);

	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enterで次の画面に遷移する
		CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
	}
}

//===========================
// プレイヤー数の選択
//===========================
void CPSelect::SelectPlayerNum()
{
	// 左
	if (CInputKeyboard::Trigger(DIK_LEFT))
	{// 例外処理
		if (m_PlayerSelectNum == 0)
		{// プレイヤーの数が0だった時、減らしたら4になる処理
			m_PlayerSelectNum = MAX_WINDOW;
			// 0になったので再生成
			UI_Create();
		}
		else
		{
			// 右を押せばプレイヤー数が1減る
			m_PlayerSelectNum--;
			//UIの生成
			UI_Create();
		}
	}
	// 右
	else if (CInputKeyboard::Trigger(DIK_RIGHT))
	{
		// 例外処理
		if (m_PlayerSelectNum == MAX_WINDOW)
		{
			// プレイヤーの数がMAX_PLAYERだった時、増やしたら0になる処理
			m_PlayerSelectNum = 0;
			// 0になったので再生成
			UI_Create();
		}
		else
		{
		// 右を押せばプレイヤー数が1増える
			m_PlayerSelectNum++;
		//UIの生成
			UI_Create();
		}
	}
}

//===========================
// プレイヤー数に応じたUIの生成
//===========================
void CPSelect::UI_Create()
{
	if (m_PUI[0] != nullptr)
	{
		// 表示していたUIの削除
		UI_Uninit();
	}

	switch (m_PlayerSelectNum)
	{
	case 0:
		//UIの生成
		m_PUI[0] = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CUI_PSelect::UI_PLAYER);
		break;

	case 1:
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			m_PUI[nCnt] = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 100.0f + nCnt * 200.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CUI_PSelect::UI_PLAYER);
		}
		break;

	case 2:
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			m_PUI[nCnt] = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 150.0f + nCnt * 150.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CUI_PSelect::UI_PLAYER);
		}
		break;

	case 3:
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			m_PUI[nCnt] = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 300.0f + nCnt * 200.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CUI_PSelect::UI_PLAYER);
		}
		break;
	}
}

//===========================
// 全ての破棄
//===========================
void CPSelect::UI_Uninit()
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_PUI[nCnt] != nullptr)
		{
			m_PUI[nCnt]->Uninit();
			m_PUI[nCnt] = nullptr;
		}
	}
}