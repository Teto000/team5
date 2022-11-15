//=========================================
//
// ランキング処理
// Auther：Arita Meigen
//
//=========================================
#ifndef _RANKING_H_		// このマクロ定義がされてなかったら
#define _RANKING_H_		// 二重インク―ルード防止のマクロ定義

#include "object2D.h"

class CScore;

#define MAX_DIGITS		(8)			// スコアの桁数
#define MAX_RANK		(5)			//表示するランキングの最大数

//=========================================
// スコアクラスを定義
//=========================================
class CRanking
{
public:
	CRanking();							//コンストラクタ
	virtual ~CRanking() ;				//デストラクタ

	
	HRESULT Init() ;	//初期化
	void Update() ;		//更新
	void Uninit() ;		//終了

	//クリエイト処理
	static CRanking* Create();

	void Save();
	void Load();
	void Ranking();

	//セッター
	static void SetCurrentScore(int Time);

private:
	D3DXVECTOR3 m_pos;					// 座標
	static int	m_Time;					// 時間
	int			m_nRankUpdate;			//更新ランクNo.
	CScore*		m_apScore[MAX_RANK];	// スコアの最大数

};

#endif

