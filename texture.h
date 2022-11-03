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
		TEXTURE_RANKING,		//ランキング
		TEXTURE_HUMAN,			//プレイヤー
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
