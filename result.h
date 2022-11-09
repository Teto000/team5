//===================================================
//
// リザルトヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _RESULT_H_
#define _RESULT_H_	

//-------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>

//================================
// 前方宣言
//================================
class CObject2D;
class CCameraTitle;
class CScore;

//================================
// リザルトクラスの定義
//================================
class CResult
{
public:
	CResult();	//コンストラクタ
	~CResult();	//デストラクタ

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

	//------------------
	// メンバ変数
	//------------------
	CObject2D* m_pObject2D;
	static CCameraTitle* m_pCameraTitle;	//タイトルカメラ
	CScore* apScore[5];
};

#endif