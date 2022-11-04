//===================================================
//
// タイトルヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _TITLE_H_
#define _TITLE_H_	

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>

//--------------------------------
// 前方宣言
//--------------------------------
class CObject2D;
class CMeshField;
class CCameraTitle;
class CPlayer;
class CTitlelogo;

//================================
// タイトルクラスの定義
//================================
class CTitle
{
public:
	CTitle();	//コンストラクタ
	~CTitle();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();

	//------------------
	// ゲッター
	//------------------
	static CCameraTitle* GetCameraTitle();	//カメラの取得

private:
	//------------------
	// メンバ変数
	//------------------
	CObject2D*		m_pObject2D;

	//------------------
	// 静的メンバ変数
	//------------------
	static CMeshField*	 m_pMeshField;		//メッシュフィールド
	static CCameraTitle* m_pCameraTitle;	//タイトルカメラ
	static CTitlelogo*	 m_pTitlelogo;

};

#endif