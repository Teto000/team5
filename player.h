//===================================================
//
// プレイヤーヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CShadow;	//影
class CBullet;	//弾
class CModel;	//モデル

//--------------------------------
// マクロ定義
//--------------------------------
#define MAX_PARTS	(7)		//モデルパーツの最大数
#define MAX_KEY		(2)		//キーの最大数
#define MAX_SPEED	(3.0f)	//最大速度

//================================
// プレイヤークラスの定義
//================================
class CPlayer : public CObject
{
public:
	//キー要素
	struct KEY
	{
		float fPosX;	//位置
		float fPosY;
		float fPosZ;
		float fRotX;	//向き
		float fRotY;
		float fRotZ;
	};

	//キー情報
	struct KEY_SET
	{
		int nFrame;	//フレーム数
		KEY aKey[MAX_PARTS];
	};

	//プレイヤーの状態
	enum STATE
	{
		IDOL_STATE = 0,
		JUMP_STATE,
		MAX_STATE
	};

	CPlayer();				//コンストラクタ
	~CPlayer() override;	//デストラクタ

	//----------------
	// メンバ関数
	//----------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//----------------
	// セッター
	//----------------
	void SetPosition(D3DXVECTOR3 pos);
	void SetGravity() { m_move.y = 0; }

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override;	//位置の取得
	D3DXVECTOR3 GetRot();				//向きの取得
	float GetWidth() override;			//幅の取得
	float GetHeight() override;			//高さの取得
	D3DXMATRIX GetmtxWorld();			//ワールドマトリックスの取得
	D3DXVECTOR3 GetPosOld();			//1F前の位置の習得

	//----------------
	// 静的メンバ関数
	//----------------
	static CPlayer* Create(int PlayerNum);

private:
	void SetModel();			//モデルの設定
	void SetMotion(bool bLoop);	//モーションの設定
	void Move();				//移動
	void Jump();				//ジャンプ
	void P1MoveKey();			//プレイヤー1のキー操作設定
	void P2MoveKey();			//プレイヤー2のキー操作設定

	// 移動キー
	// 引数 : 上のキー、左のキー、下のキー、右のキー
	void MoveKey(int UPKey,int LEFTKey,int DOWNKey,int RIGHTKey,int JUMPKey);

private:
	//----------------
	// 定数
	//----------------
	static const float fPlayerSpeed;	//プレイヤーの速度
	static const float fGravity;		//重力

	//----------------
	// メンバ変数
	//----------------
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posold;		//1F前の位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//目的の向き
	STATE		m_state;		//プレイヤーの状態
	bool		m_bJump;		//ジャンプしたかしていないか

	/* ↓ モーション情報 ↓ */
	int m_nCurrentKey;			//現在のキー番号
	int m_nCntMotion;			//モーションカウンター
	int m_nJumpCount;			//ジャンプカウント

	/* ↓ プレイヤー管理情報 */
	int m_nPlayerNum;			//プレイヤーの操作番号

	/* ↓ クラス情報 ↓ */
	static CShadow* m_pShadow;	//影
	static CBullet* m_pBullet;	//弾
	static CModel*  m_pModel[MAX_PARTS];	//モデル
};

#endif