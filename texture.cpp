//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "application.h"
#include "texture.h"
#include "renderer.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
const char* CTexture::s_FileName[] =
{// テクスチャのパス
	"data/TEXTURE/Ground.jpg",		//地面
	"data/TEXTURE/Shadow.jpg",		//影
	"data/TEXTURE/Bullet.png",		//弾
	"data/TEXTURE/number001.png",	//ナンバー
	"data/TEXTURE/Titlelogo000.png",//タイトルロゴ
	"data/TEXTURE/Titlelogo001.png",//タイトルロゴのPreseAnykey
	"data/TEXTURE/Ranking.png",		//ランキング
	"data/TEXTURE/human.png",		//プレイヤー画像

	/* ↓ メッセージ ↓ */
	"data/TEXTURE/3.png",			//カウント3
	"data/TEXTURE/2.png",			//カウント2
	"data/TEXTURE/1.png",			//カウント1
	"data/TEXTURE/START.png",		//スタート
	"data/TEXTURE/FINISH00.png",	//フィニッシュ

	/* ↓ 順位 ↓ */
	"data/TEXTURE/1st.png",			//1位
	"data/TEXTURE/2nd.png",			//2位
	"data/TEXTURE/3rd.png",			//3位
	"data/TEXTURE/4th.png",			//4位

	/* ↓ 勝利 ↓ */
	"data/TEXTURE/Player 1 Win.png",//1PWin
	"data/TEXTURE/Player 2 Win.png",//2PWin
	"data/TEXTURE/Player 3 Win.png",//3PWin
	"data/TEXTURE/Player 4 Win.png",//4PWin

	/* ↓ 人数選択画面 ↓ */
	"data/TEXTURE/PlayerSelect/title.png",	//人数選択画面のタイトル
	"data/TEXTURE/PlayerSelect/bg2.png",				//人数選択画面の背景
	"data/TEXTURE/PlayerSelect/select.png",	//人数選択画面の背景

	/* ↓チュートリアル画面 ↓ */
	"data/TEXTURE/Tutorial01.png"			//チュートリアル背景
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// 全ての読み込み
//--------------------------------------------------
void CTexture::LoadAll()
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// テクスチャの読み込みがされている
			continue;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// テクスチャの読み込みがされている
		return;
	}

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//--------------------------------------------------
// 全ての解放
//--------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// テクスチャの解放
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//--------------------------------------------------
// 解放
//--------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != NULL)
	{// テクスチャの解放
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = NULL;
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// テクスチャを使用しない
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// 読み込み
	Load(inTexture);

	return s_pTexture[inTexture];
}
