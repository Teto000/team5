//===================================================
//
// アプリケーションヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_	

//------------------------------
// インクルード
//------------------------------
#include <d3dx9.h>
#include "renderer.h"

//------------------------------
// 前方宣言
//------------------------------
class CGame;		//ゲーム
class CTitle;		//タイトル
class CResult;		//リザルト
class CTutorial;	//チュートリアル
class CFade;		//フェード

class CRenderer;	//レンダラー
class CInput;		//インプット
class CTexture;		//テクスチャ
class CSound;		//サウンド
class CCamera;		//カメラ
class CLight;		//ライト

//================================
// アプリケーションクラスの定義
//================================
class CApplication
{
public:
	//--------------------------------
	// ゲームモードの列挙型の定義
	//--------------------------------
	enum MODE
	{
		MODE_TITLE = 0,		//タイトル
		MODE_GAME,			//ゲーム
		MODE_RESULT,		//リザルト
		MODE_FADE,			//フェード
		MODE_MAX
	};

	enum GAMEMODE
	{
		GAMEMODE_TIME = 0,
		GAMEMODE_SCORE,
		GAMEMODE_MAX
	};

	CApplication();		//コンストラクタ
	~CApplication();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	//------------------
	// 静的メンバ関数
	//------------------
	static void SetMode(MODE mode);			//モードの設定
	static MODE GetMode();					//モードの取得

	static CRenderer *GetRenderer();		//レンダラーの取得
	static CInput	 *GetInput();			//インプットの取得
	static CTexture  *GetTexture();			//テクスチャの取得
	static CSound	 *GetSound();			//サウンドの取得
	static CCamera	 *GetCamera(int nCnt);	//カメラの取得
	static CLight	 *GetLight();			//ライトの取得
	static CFade	 *GetFade();			//フェードの取得

private:
	//------------------
	// 静的メンバ変数
	//------------------
	static CGame*		m_pGame;				//ゲームクラス
	static CTitle*		m_pTitle;				//タイトルクラス
	static CResult*		m_pResult;				//リザルトクラス
	static CTutorial*	m_pTutorial;			//チュートリアル
	static CFade*		m_pFade;				//フェードクラス
	static MODE			m_mode;					//モード
	static GAMEMODE		m_gameMode;				//ゲームモード

	static CRenderer*	m_pRenderer;			//レンダラークラス
	static CInput*		m_pInput;				//インプットクラス
	static CTexture*	m_pTexture;				//テクスチャクラス
	static CSound*		m_pSound;				//サウンドクラス
	static CCamera*		m_pCamera[MAX_CAMERA];	//カメラクラス
	static CLight*		m_pLight;				//ライトクラス

	static bool m_bCamera;	//カメラを使用するかどうか
};

#endif // !_APPLICATION_H_