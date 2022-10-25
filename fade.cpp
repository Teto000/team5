//===================================================
//
// フェードの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "fade.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "application.h"
#include "renderer.h"
#include "object2d.h"

//===========================
// コンストラクタ
//===========================
CFade::CFade()
{

}

//===========================
// デストラクタ
//===========================
CFade::~CFade()
{

}

//===========================
// 初期化
//===========================
HRESULT CFade::Init(CApplication::MODE modeNext)
{
	//列挙型の初期化
	m_fade = FADE_IN;		//フェードイン状態
	m_modeNext = modeNext;	//次の画面(モード)を設定
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴン(不透明)にしておく

	CObject2D::Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	//CObject2D::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	//CObject2D::SetColor(m_col);

	//CObject2D::SetTexture(CTexture::TEXTURE_NONE);	//テクスチャの設定

	return S_OK;
}

//===========================
// 終了
//===========================
void CFade::Uninit()
{
	CObject2D::Uninit();
}

//===========================
// 更新
//===========================
void CFade::Update()
{
	CObject2D::Update();

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{//フェードイン状態
			m_col.a -= 0.02f;	//ポリゴンを透明にしていく

			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
				m_fade = FADE_NONE;	//何もしていない状態に
			}
		}
		else if (m_fade == FADE_OUT)
		{//フェードアウト状態
			m_col.a += 0.02f;	//ポリゴンを不透明にしていく

			if (m_col.a >= 1.0f)
			{
				m_col.a = 1.0f;
				m_fade = FADE_IN;	//フェードイン状態に

				CApplication::SetMode(m_modeNext);
			}
		}
		//CObject2D::SetColor(m_col);
	}
}

//===========================
// 描画
//===========================
void CFade::Draw()
{
	CObject2D::Draw();
}

//===========================
// 設定
//===========================
void CFade::SetFade(CApplication::MODE modeNext)
{
	m_fade = FADE_OUT;		//フェードアウト状態に
	m_modeNext = modeNext;	//次の画面を設定
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//===========================
// 取得
//===========================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}