//===================================================
//
// プレイヤーカメラヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _CAMERA_PLAYER_H_	//このマクロが定義されてなかったら
#define _CAMERA_PLAYER_H_	//2重インクルード防止のマクロ定義

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "camera.h"

//================================
// プレイヤーカメラクラスの定義
//================================
class CCameraPlayer : public CCamera
{
public:
	//------------------------------
	// プレイヤー番号の列挙型
	//------------------------------
	enum NUMPLAYER
	{
		NUMPLAYER_ONE = 0,
		NUMPLAYER_TWO,
		NUMPLAYER_THREE,
		NUMPLAYER_FOUR,
		NUMPLAYER_MAX
	};

	CCameraPlayer();			//コンストラクタ
	~CCameraPlayer() override;	//デストラクタ

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
	static CCameraPlayer* Create(DWORD X, DWORD Y, DWORD Width, DWORD Height);

	//------------------
	// セッター
	//------------------
	void SetNumPlayer(int nNum);	//プレイヤー番号の設定

private:
	void EachPlayer();	//プレイヤーごとの処理
	void Turn(int nLeftKey, int nRightKey);	//旋回
	void Move(int nUpKey, int nDownKey, int nLeftKey, int nRightKey);	//移動
	void Following();				//追従

private:
	//------------------
	// メンバ変数
	//------------------
	int m_nNumPlayer;			//プレイヤー番号
	int m_nNumFieldCamera;		//フィールドカメラのプレイヤー番号
	int m_nChangeTime;			//フィールドカメラ切り替え時間
};

#endif