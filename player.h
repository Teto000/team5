//===================================================
//
// プレイヤーヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_	//二重インクルード防止

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
class CBlock;
class CPlayer : public CObject
{
public:

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
	D3DXVECTOR3 GetPosition() override;					//位置の取得
	D3DXVECTOR3 GetRot();								//向きの取得
	float GetWidth() override;							//幅の取得
	float GetHeight() override;							//高さの取得
	D3DXMATRIX GetmtxWorld();							//ワールドマトリックスの取得
	D3DXVECTOR3 GetPosOld();							//1F前の位置の習得
	void Add(int Add);									//加算処理

	//----------------
	// 静的メンバ関数
	//----------------
	static CPlayer* Create(int PlayerNum);

private:
	void Move();				//移動
	void Jump();				//ジャンプ
	void SetBlock();			//ブロックの設置
	void Gravity();				//重力

	// 移動キー
	// 引数 : 上のキー、左のキー、下のキー、右のキー
	void MoveKey(int UPKey, int LEFTKey, int DOWNKey, int RIGHTKey, int JUMPKey, int nPlayerNum);
	CNumBlock*	m_pNumBlock;	//ブロック数の表示

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
	D3DXVECTOR3 m_respornPos;				//リスポーン地点
	D3DXVECTOR3 m_move;						//移動量
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_rotDest;					//目的の向き
	STATE		m_state;					//プレイヤーの状態
	int			m_nNumBlock;				//ブロック数
	bool		m_bJump;					//ジャンプしたかしていないか
	bool		m_PlayerBlockCollision;		//プレイヤーがブロックと当たっているかいないかを入れる箱

	CBlock		*m_pModel[MAX_BLOCK];		//ブロック
	int			m_BlockCnt;					//ブロックをカウントする処理
	int			m_BlockHave;				//ブロックを所持している個数

	int			m_nJumpCount;
	CRank*		m_pRank;		//順位

	/* ↓ プレイヤー管理情報 */
	int m_nPlayerNum;			//プレイヤーの操作番号

	/* ↓　使ってるモーションモデル番号　*/
	int m_nMotionNum;
};

#endif