//=============================================================================
//
// プレイヤー
// Author : 有田明玄
//
//=============================================================================

#ifndef _EDITOR_H_
#define _EDITOR_H_

//インクルード
#include"Application.h"

//マクロ定義
#define NUM_STAGE	(1)									//ステージ数
#define FILE_MAPDATA	"data\\TXT\\Edit.txt"	//読み込むファイル名


//前方宣言
class CEnemyManager;
class CPlayer;
class CGoal;
class CMeshField;
class CObject;

//=============================================================================
// 構造体定義
//=============================================================================
class CEditor
{
public:
	enum OBJ
	{
		OBJ_GOAL = 0,
		OBJ_MAP,
		OBJ_GIMMICK,
		OBJ_MAX
	};

	CEditor();
	~CEditor();
	void Init();
	void Update();
	void Load();
	void Uninit();
	static CEditor*Create();
	void SaveObject();

private:
	//メンバ関数
	OBJ					m_type;			//オブジェクトの種類
	D3DXVECTOR3			m_pos;			//出現座標
	D3DXVECTOR3			m_rot;			//出現した際の角度
	bool				m_bEnd;			//終了フラグ
	bool				m_bFlag;		//生成フラグ
	int					m_nNumber;		//現在設定するブロックのタイプ

	//スタティック関数
	static	CPlayer*	pPlayer;
	static	CGoal*		m_pGoal;		//ゴール
	static	CMeshField*	m_Map;			//マップ
	static	CObject*	m_SelectObj;	//選択中のオブジェクト
};
#endif 



#pragma once
