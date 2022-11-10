//===================================================
//
// プレイヤーヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _GIMMICK_H_
#define _GIMMICK_H_	//二重インクルード防止

#define MAX_BLOCK (1000)
//--------------------------------
// インクルード
//--------------------------------
#include "object.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CNumBlock;	//ブロック数の表示
class CRank;		//ランク

//--------------------------------
// マクロ定義
//--------------------------------
#define MAX_SPEED	(3.0f)	//最大速度

//================================
// プレイヤークラスの定義
//================================
class CGimmick : public CObject
{
public:
	CGimmick();				//コンストラクタ
	~CGimmick() override;	//デストラクタ

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
	static CGimmick* Create(char* FileName,D3DXVECTOR3 pos);

private:
	void Move();				//移動

private:
	//----------------
	// 定数
	//----------------
	static const float fPlayerSpeed;	//プレイヤーの速度
	static const float fGravity;		//重力

	//----------------
	// メンバ変数
	//----------------
	D3DXVECTOR3 m_pos;						//位置
	D3DXVECTOR3 m_posold;					//1F前の位置
	D3DXVECTOR3 m_move;						//移動量
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_rotDest;					//目的の向き
	char*		m_modelName;				//モデルのパス

	/* ↓ プレイヤー管理情報 */
	int m_nPlayerNum;			//プレイヤーの操作番号

	/* ↓　使ってるモーションモデル番号　*/
	int m_nMotionNum;
};

#endif
