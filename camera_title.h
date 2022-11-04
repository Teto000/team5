//===================================================
//
// タイトルカメラヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _CAMERA_TITLE_H_	//このマクロが定義されてなかったら
#define _CAMERA_TITLE_H_	//2重インクルード防止のマクロ定義

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "camera.h"

//================================
// タイトルカメラクラスの定義
//================================
class CCameraTitle : public CCamera
{
public:
	CCameraTitle();				//コンストラクタ
	~CCameraTitle() override;	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void SetCamera(LPDIRECT3DDEVICE9 pDevice) override;

	//------------------
	// 静的メンバ関数
	//------------------
	static CCameraTitle* Create(DWORD X, DWORD Y, DWORD Width, DWORD Height);
};

#endif