//=============================================================================
//
// マップデータ読み込み
// Author : Arita Meigen
//
//=============================================================================

//インクルード
#include"Editor.h"
#include<string>
#include"player.h"
#include"game.h"
#include"renderer.h"
#include"Goal.h"
#include"meshfield.h"
#include"debug_proc.h"
#include"input_keybord.h"
#include"input.h"
#include"Map.h"
#include"objectX.h"


//静的メンバ変数
//CPlayer*	CEditor::pPlayer = nullptr;
//CGoal*		CEditor::m_pGoal = nullptr;						//ゴール
//CMap*		CEditor::m_pMap = nullptr;						//マップ
//CObject*	CEditor::m_pSelectObj = nullptr;				//選択中のオブジェクト
//CModel*		CEditor::m_pPlanet[MAX_PLANET] = { nullptr };	//設置するオブジェクト
//=============================================================================
// コンストラクタ
//=============================================================================
CEditor::CEditor()
{
	m_pPlayer = nullptr;
	m_pGoal = nullptr;						//ゴール
	m_pMap = nullptr;						//マップ
	m_pSelectObj = nullptr;					//選択中のオブジェクト
	ZeroMemory(&m_pPlanet[0],sizeof(m_pPlanet[0])*MAX_PLANET);
}

//=============================================================================
// デストラクタ
//=============================================================================
CEditor::~CEditor()
{
}


//=============================================================================
//初期化
//=============================================================================
void CEditor::Init()
{
	//初期化
	m_pGoal = nullptr;
	m_pMap = nullptr;
	m_pPlayer = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//出現座標
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//出現した際の角度
	m_bEnd = false;									//終了フラグ
	m_bFlag = false;								//生成フラグ
	m_nNumber = 0;									//現在設定するブロックのタイプ
	m_nNumpla = 0;

	SelectPlanet();
}

//=============================================================================
//終了
//=============================================================================
void CEditor::Uninit()
{
	if (m_pGoal != nullptr)
	{
		m_pGoal = nullptr;
	}

	if (m_pMap != nullptr)
	{
		m_pMap = nullptr;
	}

	if (m_pPlayer != nullptr)
	{
		m_pPlayer = nullptr;
	}

	for (int i = 0; i < MAX_PLANET; i++)
	{
		if (m_pPlanet[i] != nullptr)
		{
			m_pPlanet[i] = nullptr;
		}
	}

	m_nNumber = NULL;
	m_bEnd = NULL;			//終了フラグ
	m_bFlag = NULL;			//生成フラグ
}

//=============================================================================
//更新
//=============================================================================
void CEditor::Update()
{
	Input();

	switch (m_nNumber)
	{
	case OBJ_GOAL:
		m_pSelectObj = m_pGoal;
		break;

	case OBJ_MAP:
		//m_pSelectObj = m_pMap;
		break;

	case OBJ_GIMMICK:
		//m_pSelectObj = m_pMap;
		break;
	default:
		break;

	}

//	D3DXVECTOR3 Selectpos = m_pSelectObj->GetPosition();

	CDebugProc::Print("現在置くオブジェクト:%d 0(ゼロ)キーで種類を変更", m_nNumber);
	CDebugProc::Print("O(オー)キーでオブジェクトの座標ファイルに出力する");
	CDebugProc::Print("Pキーでオブジェクトの生成/移動");
	CDebugProc::Print("現在設置する惑星の種類:%d 8/9で種類を変更", m_nNumpla);


	//CDebugProc::Print("現在のオブジェクトの座標:x:%f y:%f z:%f", Selectpos.x, Selectpos.y, Selectpos.z);

}

//=============================================================================
// マップデータ読み込み
//=============================================================================
void CEditor::Load()
{
	const int lenLine = 1024;	//1単語の最大数
	char strLine[lenLine];		//読み込み用の文字列
								//DEBUG_MAPDATA
								//FILE_MAPDATA
	FILE*fp = fopen(FILE_MAPDATA, "r");		//ファイル読み込み
	if (fp != NULL)
	{
		for (int i = 0; i < lenLine; i++)
		{
			fgets(strLine, lenLine, fp);
			if (strncmp(strLine, "Object", 6) == 0)	//六文字比較して一致するかどうか調べる
			{
				m_bEnd = false;
				while (m_bEnd == false)
				{
					fscanf(fp, "%s", &strLine[0]);	//読み込んだ文字ごとに設定する

					if (strncmp(strLine, "#", 1) == 0)
					{
						return;
					}
					else if (strncmp(strLine, "Goal", 4) == 0)
					{
						m_bFlag = true;
						m_type = OBJ_GOAL;
					}
					else if (strncmp(strLine, "Map", 3) == 0)
					{
						m_bFlag = true;
						m_type = OBJ_MAP;
					}
					else if (strncmp(strLine, "Gimmick", 7) == 0)
					{
						m_bFlag = true;
						m_type = OBJ_GIMMICK;
					}
					else if (strncmp(strLine, "Planet", 7) == 0)
					{
						m_bFlag = true;
						m_type = OBJ_PLANET;
					}
					else if (strncmp(strLine, "Pos", 3) == 0)
					{
						fscanf(fp, "%f%f%f", &m_pos.x, &m_pos.y, &m_pos.z);
					}
					else if (strncmp(strLine, "Rot", 3) == 0)
					{
						fscanf(fp, "%f%f%f", &m_rot.x, &m_rot.y, &m_rot.z);
					}
					else if (strncmp(strLine, "End", 3) == 0)
					{
						if (m_bFlag == true)
						{
							switch (m_type)
							{//タイプによって分ける
							case OBJ_GOAL:
								if (m_pGoal == nullptr)
								{
									m_pGoal = CGoal::Create(D3DXVECTOR3(m_pos), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
								}
								break;

							case OBJ_GIMMICK:
								break;

							case OBJ_MAP:
								if (m_pGoal == nullptr)
								{
									m_pMap = CMap::Create(m_pos);
								}
								break;

							case OBJ_PLANET:
								if (m_pPlanet[m_nNumpla] == nullptr)
								{
									m_pPlanet[m_nNumpla] = CObjectX::Create(m_nPlaFileName[m_nNumpla], m_pos, m_rot);
								}
								break;

							default:
								break;
							}
							m_bEnd = true;
							break;
						}
					}
				}
			}
		}
		fclose(fp);
	}
}

//=============================================================================
//生成
//=============================================================================
CEditor * CEditor::Create()
{
	CEditor *pEditor = new CEditor;

	pEditor->Init();
	pEditor->Load();

	return pEditor;
}

//=============================================================================
//オブジェクトの位置の保存
//=============================================================================
void CEditor::SaveObject()
{
	FILE*fp = fopen("data\\TXT\\ObjectPos.txt", "w");		//ファイル読み込み

	fprintf(fp, "Object\n");
	fprintf(fp, "Goal\n");
	fprintf(fp, "Pos %.1f %.1f %.1f\n", m_pGoal->GetPosition().x, m_pGoal->GetPosition().y, m_pGoal->GetPosition().z);
	fprintf(fp, "Rot 0.0f 0.0f 0.0f\n");
	fprintf(fp, "End\n");

	fclose(fp);
}

//=============================================================================
//オブジェクトの位置の保存
//=============================================================================
void CEditor::SelectPlanet()
{
	m_nPlaFileName[0] = "data\\MODEL\\X_File\\Earth_000.x";
	m_nPlaFileName[2] = "data\\MODEL\\X_File\\Sun_000.x";
	m_nPlaFileName[1] = "data\\MODEL\\X_File\\Moon_000.x";
	m_nPlaFileName[3] = "data\\MODEL\\X_File\\Mars_000.x";
	m_nPlaFileName[4] = "data\\MODEL\\X_File\\Jupiter_000.x";
	m_nPlaFileName[5] = "data\\MODEL\\X_File\\Venus_000.x";
	m_nPlaFileName[6] = "data\\MODEL\\X_File\\Saturn_000.x";
	m_nPlaFileName[7] = "data\\MODEL\\X_File\\Neptune_000.x";
	m_nPlaFileName[8] = "data\\MODEL\\X_File\\Uranus_000.x";
	m_nPlaFileName[9] = "data\\MODEL\\X_File\\Pluto_000.x";
}


//=============================================================================
//入力処理
//=============================================================================
void CEditor::Input()
{
	if (CInputKeyboard::Trigger(DIK_O) == true)
	{//O(オー)キーを押したとき
		SaveObject();
	}

	if (CInputKeyboard::Trigger(DIK_0) == true)
	{//0(ゼロ)キーを押したとき
		if (m_nNumber++ >= OBJ_MAX - 1)
		{
			m_nNumber = 0;
		}
	}

	if (CInputKeyboard::Trigger(DIK_9) == true)
	{//9キーを押したとき
		if (m_nNumpla++ >= MAX_PLANET-1)
		{
			m_nNumpla = 0;
		}
	}
	if (CInputKeyboard::Trigger(DIK_8) == true)
	{//9キーを押したとき
		if (m_nNumpla-- <= 0)
		{
			m_nNumpla = MAX_PLANET - 1;
		}
	}

	//生成/移動
	if (CInputKeyboard::Trigger(DIK_P) == true)
	{
		m_pPlayer = CGame::GetPlayer(0);
		D3DXVECTOR3 pos = m_pPlayer->GetPosition();

		switch (m_nNumber)
		{
		case OBJ_GOAL:
			if (m_pGoal == nullptr)
			{//ゴールの位置を現在のプレイヤーの位置に移動
				m_pGoal = CGoal::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else
			{
				m_pGoal->SetPosition(pos);
			}
			break;

		case OBJ_MAP:
			break;

		case OBJ_GIMMICK:
			break;

		case OBJ_PLANET:
			if (m_pPlanet[m_nNumpla] == nullptr)
			{//生成されてない場合生成
				m_pPlanet[m_nNumpla] = CObjectX::Create(m_nPlaFileName[m_nNumpla], pos, m_rot);
			}
			else
			{//中身がある場合移動
				m_pPlanet[m_nNumpla]->SetPos(pos);
			}
			break;

		default:
			break;

		}
	}
}
