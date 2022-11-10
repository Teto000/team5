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
#define NUM_STAGE	(1)							//ステージ数
#define FILE_MAPDATA	"data\\TXT\\Edit.txt"	//読み込むファイル名
#define	MAX_PLANET	(10)						//配置する惑星の数
#define	MAX_GIMMICK	(4)							//ギミックの種類
#define MAX_STACK_BLOCK (100)					//重なっているブロックの設置数



//前方宣言
class CEnemyManager;
class CPlayer;
class CGoal;
class CMeshField;
class CObject;
class CGimmick;
class CMap;
class CObjectX;
class CGimmick;

class CSBlock;
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
		OBJ_PLANET,
		OBJ_STACK_BLOCK,
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
	void Pass();
	void Input();

	static CGoal* GetGoal();	//ゴールの取得

private:
	//メンバ関数
	OBJ					m_type;							//オブジェクトの種類
	D3DXVECTOR3			m_pos;							//出現座標
	D3DXVECTOR3			m_rot;							//出現した際の角度
	bool				m_bEnd;							//終了フラグ
	bool				m_bFlag;						//生成フラグ
	int					m_nNumber;						//現在設定するブロックのタイプ
	int					m_nNumpla;						//現在の惑星の番号
	int					m_nNumgim;						//現在のギミックの番号

	LPCTSTR				m_nPlaFileName[MAX_PLANET];		//惑星のファイルの名前


	//スタティック関数
	CPlayer*	m_pPlayer;
	static CGoal*		m_pGoal;			//ゴール
	CMap*		m_pMap;						//マップ
	CObject*	m_pSelectObj;				//選択中のオブジェクト
	CObjectX*	m_pPlanet[MAX_PLANET];		//背景の星
	static CSBlock*	m_pStackBlock[MAX_STACK_BLOCK];	//重なっているブロックの処理
};
#endif



#pragma once
