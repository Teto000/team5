//===================================================
//
// メッセージの処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "message.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "Goal.h"

//=======================
// コンストラクタ
//=======================
CMessage::CMessage() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色
	m_nCntTime = 0;		//時間カウント
	m_fWidth = 0.0f;	//幅
	m_fHeight = 0.0f;	//高さ
	m_message = MESSAGE_COUNT_THREE;
}

//=======================
// デストラクタ
//=======================
CMessage::~CMessage()
{

}

//=======================
// 初期化
//=======================
HRESULT CMessage::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = 500.0f;
	m_fHeight = 500.0f;

	CObject2D::Init(m_pos);

	CObject2D::SetSize(500.0f, 500.0f);

	//テクスチャの設定
	SetTexture();

	return S_OK;
}

//=======================
// 終了
//=======================
void CMessage::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// 更新
//=======================
void CMessage::Update()
{
	CObject2D::Update();

	//-------------------------
	// 文字の切り替え処理
	//-------------------------
	if (m_message == MESSAGE_START && CGame::GetFinish())
	{//スタート文字が出た後 and 終了フラグが立っているなら
	 //メッセージの切り替え
		ChangeMessage();
	}

	//テクスチャの設定
	SetTexture();

	//時間カウント
	m_nCntTime++;

	//-------------------------
	// 透明にする
	//-------------------------
	if (m_message == MESSAGE_START || m_message == MESSAGE_FINISH)
	{//スタートかフィニッシュなら
		if (m_nCntTime >= 60)
		{//カウントが60以上なら
			m_col.a -= 0.02f;	//透明度の減少
		}
	}
	else if (m_message != MESSAGE_WINNER_ONE && m_message != MESSAGE_WINNER_TWO
		&& m_message != MESSAGE_WINNER_THREE && m_message != MESSAGE_WINNER_FOUR)
	{//勝者の文字じゃないなら
		if (m_nCntTime >= 30)
		{//カウントが30以上なら
			m_col.a -= 0.02f;	//透明度の減少
		}
	}

	//色の設定
	CObject2D::SetColor(m_col);

	if (m_col.a <= 0.0f)
	{//完全に透明になったら
		if (m_message != MESSAGE_START)
		{//スタート以外の文字なら
			//メッセージの切り替え
			ChangeMessage();
		}

		//カウントのリセット
		m_nCntTime = 0;

		return;
	}
}

//=======================
// 描画
//=======================
void CMessage::Draw()
{
	CObject2D::Draw();
}

//=======================
// 生成
//=======================
CMessage *CMessage::Create(D3DXVECTOR3 pos, MESSAGE message)
{
	CMessage *pMessage = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pMessage = new CMessage;	//生成

	if (pMessage != nullptr)
	{//NULLチェック
		//引数の代入
		pMessage->m_message = message;	//列挙型

		//初期化
		pMessage->Init(D3DXVECTOR3(pos));
		//pMessage->SetObjType(OBJTYPE_ENEMY);
	}

	return pMessage;
}

//=======================
// テクスチャの設定
//=======================
void CMessage::SetTexture()
{
	switch (m_message)
	{
	case MESSAGE_COUNT_ONE:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_ONE);
		break;

	case MESSAGE_COUNT_TWO:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_TWO);
		break;

	case MESSAGE_COUNT_THREE:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_THREE);
		break;

	case MESSAGE_START:
		CObject2D::SetTexture(CTexture::TEXTURE_START);
		break;

	case MESSAGE_FINISH:
		CObject2D::SetTexture(CTexture::TEXTURE_FINISH);
		break;

	case MESSAGE_WINNER_ONE:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_ONE);
		break;

	case MESSAGE_WINNER_TWO:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_TWO);
		break;

	case MESSAGE_WINNER_THREE:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_THREE);
		break;

	case MESSAGE_WINNER_FOUR:
		CObject2D::SetTexture(CTexture::TEXTURE_WINNER_FOUR);
		break;

	default:
		break;
	}
}

//=======================
// メッセージの変更
//=======================
void CMessage::ChangeMessage()
{
	switch (m_message)
	{
	case MESSAGE_COUNT_ONE:
		m_message = MESSAGE_START;
		break;

	case MESSAGE_COUNT_TWO:
		m_message = MESSAGE_COUNT_ONE;
		break;

	case MESSAGE_COUNT_THREE:
		m_message = MESSAGE_COUNT_TWO;
		break;

	case MESSAGE_START:
		m_message = MESSAGE_FINISH;
		break;

	case MESSAGE_FINISH:
		//勝利したプレイヤーの番号を取得
		switch (CGoal::GetWinner())
		{
		case 0:
			m_message = MESSAGE_WINNER_ONE;
			break;

		case 1:
			m_message = MESSAGE_WINNER_TWO;
			break;

		case 2:
			m_message = MESSAGE_WINNER_THREE;
			break;

		case 3:
			m_message = MESSAGE_WINNER_FOUR;
			break;

		default:
			break;
		}

		//位置の変更
		m_pos.y = 550.0f;
		CObject2D::SetPosition(m_pos);

		//サイズの変更
		m_fWidth = 700.0f;
		m_fHeight = 400.0f;
		CObject2D::SetSize(m_fWidth, m_fHeight);
		break;

	default:
		break;
	}

	//透明度を元に戻す
	m_col.a = 1.0f;
}