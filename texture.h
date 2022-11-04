//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>

//==================================================
// 定義
//==================================================
class CTexture
{
public: /* 定義 */
	enum TEXTURE
	{
		TEXTURE_GROUND = 0,		//地面
		TEXTURE_SHADOW,			//影
		TEXTURE_BULLET,			//弾
		TEXTURE_NUMBER,			//数字テクスチャ
		TEXTURE_TITLELOGO,		//タイトルロゴ
		TEXTURE_TITLELOGO2,		//タイトルロゴ
		TEXTURE_RANKING,		//ランキング
		TEXTURE_HUMAN,			//プレイヤー

		/* ↓ メッセージ ↓ */
		TEXTURE_COUNT_THREE,	//カウント3
		TEXTURE_COUNT_TWO,		//カウント2
		TEXTURE_COUNT_ONE,		//カウント1
		TEXTURE_START,			//スタート
		TEXTURE_FINISH,			//フィニッシュ

		TEXTURE_SELECT_COMENT,	//セレクト画面の解説
		TEXTURE_SELECT_BG,		//セレクト画面の背景
		TEXTURE_SELECT_SELECT,	//セレクト画面のセレクトの説明文
		TEXTURE_MAX,
		TEXTURE_NONE,			//使用しない
	};

	static const char* s_FileName[];	// ファイルパス

public:
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

public: /* メンバ関数 */
	void LoadAll();										// 全ての読み込み
	void Load(TEXTURE inTexture);						// 指定の読み込み
	void ReleaseAll();									// 全ての破棄
	void Release(TEXTURE inTexture);					// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// 情報の取得

private: /* メンバ変数 */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// テクスチャの情報
};

#endif // !_TEXTURE_H_
