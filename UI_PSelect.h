//=========================================
//
// プレイヤー選択画面のUI.h
// Auther：冨所知生
//
//=========================================
#ifndef _UI_PSELECT_H_		// このマクロ定義がされてなかったら
#define _UI_PSELECT_H_		// 二重インク―ルード防止のマクロ定義

#include "object2D.h"


//=========================================
// スコアクラスを定義
//=========================================
class CUI_PSelect :public CObject2D
{
public:
	//フェードの状態
	enum UI
	{
		UI_NONE = 0,	//何もない
		UI_COMENT,		//解説
		UI_PLAYER,		//プレイヤーUI
		UI_BG,			//背景
		UI_SELECT,		//選択
		UI_MAX
	};

	CUI_PSelect();			//コンストラクタ
	virtual ~CUI_PSelect() override;			//デストラクタ

	//初期化処理
	HRESULT Init(D3DXVECTOR3 pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//セッター
	void Set(UI ui) { m_Tui = ui; }

	//クリエイト処理
	static CUI_PSelect* Create(const D3DXVECTOR3 &pos,const UI &texture);

private:
	int m_texture;
	UI m_Tui;			// UI
};

#endif