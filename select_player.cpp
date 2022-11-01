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
#include "fade.h"
#include  "camera.h"
#include "time.h"

//===========================
// コンストラクタ
//===========================
CPSelect::CPSelect()
{
	m_PlayerSelectNum = 0;
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
	return S_OK;
}

//===========================
// 終了
//===========================
void CPSelect::Uninit()
{

}

//===========================
// 更新
//===========================
void CPSelect::Update()
{
	// プレイヤーの数を指定する
	SelectPlayerNum();

	// カメラの個数の指定
	CApplication::SetPlayerSelect(m_PlayerSelectNum);

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
			m_PlayerSelectNum = MAX_PLAYER;
		}
		else
		{// 右を押せばプレイヤー数が1減る
			m_PlayerSelectNum--;
		}
	}
	// 右
	else if (CInputKeyboard::Trigger(DIK_RIGHT))
	{
		// 例外処理
		if (m_PlayerSelectNum == MAX_PLAYER)
		{// プレイヤーの数がMAX_PLAYERだった時、増やしたら0になる処理
			m_PlayerSelectNum = 0;
		}
		else
		{// 右を押せばプレイヤー数が1増える
			m_PlayerSelectNum++;
		}
	}
}
