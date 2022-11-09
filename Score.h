//=========================================
//
// スコア処理
// Auther：冨所知生
//
//=========================================
#ifndef _SCORE_H_		// このマクロ定義がされてなかったら
#define _SCORE_H_		// 二重インク―ルード防止のマクロ定義

#include "object2D.h"
#include "number.h"

#define MAX_DIGITS		(8)			// スコアの桁数
#define MAX_RANK		(5)			//表示するランキングの最大数

//=========================================
// スコアクラスを定義
//=========================================
class CScore :public CObject2D
{
public:
	CScore();							//コンストラクタ
	virtual ~CScore() override;			//デストラクタ

										//初期化処理
	HRESULT Init(D3DXVECTOR3 pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CScore* Create(const D3DXVECTOR3 &pos);

	//スコアの設定
	void Set();
	static void SetCurrentScore(int Time);
	static int GetTime() { return m_Time; }
	static int GetSec()	 { return m_Seconds; }
	static int GetMin()	 { return m_Minutes; }

private:
	D3DXVECTOR3 m_pos;					// 座標
	CNumber	*m_pNumber[MAX_DIGITS];		// ナンバー1
	static int m_Time;					// 時間
	static int m_Seconds;				// 現在の時間(秒)
	static int m_Minutes;				// 現在の時間(分)
	int m_Score[MAX_RANK];				// スコアの最大数
};

#endif
