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

//静的メンバ変数
CPlayer*	CEditor::pPlayer = nullptr;
CGoal*		CEditor::m_pGoal = nullptr;			//ゴール
CMeshField*	CEditor::m_Map = nullptr;			//マップ
//=============================================================================
// コンストラクタ
//=============================================================================
CEditor::CEditor()
{
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
	m_Map = nullptr;
	pPlayer = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//出現座標
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//出現した際の角度
	bEnd = false;									//終了フラグ
	bFlag = false;									//生成フラグ
	nNumber = 0;									//現在設定するブロックのタイプ
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
	if (m_Map != nullptr)
	{
		m_Map = nullptr;
	}
	if (pPlayer != nullptr)
	{
		pPlayer = nullptr;
	}
}

//=============================================================================
//更新
//=============================================================================
void CEditor::Update()
{
	if (CInputKeyboard::Trigger(DIK_P) == true)
	{
		if (m_pGoal == nullptr)
		{
			pPlayer = CGame::GetPlayer(0);
			D3DXVECTOR3 pos = pPlayer->GetPosition();
			m_pGoal = CGoal::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			pPlayer = CGame::GetPlayer(0);
			D3DXVECTOR3 pos = pPlayer->GetPosition();
			m_pGoal->SetPosition(pos);
		}
	}

	if (CInputKeyboard::Trigger(DIK_O) == true)
	{
		SaveObject();
	}

	//CDebugProc::Print("現在置くブロック\n");
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
				bEnd = false;
				while (bEnd == false)
				{
					fscanf(fp, "%s", &strLine[0]);	//読み込んだ文字ごとに設定する

					if (strncmp(strLine, "#", 1) == 0)
					{
						return;
					}
					else if (strncmp(strLine, "Goal", 4) == 0)
					{
						bFlag = true;
						m_type = OBJ_GOAL;
					}
					else if (strncmp(strLine, "Map", 3) == 0)
					{
						bFlag = true;
						m_type = OBJ_MAP;
					}
					else if (strncmp(strLine, "Gimmick", 7) == 0)
					{
						bFlag = true;
						m_type = OBJ_GIMMICK;
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
						if (bFlag == true)
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
									m_Map = CMeshField::Create();
								}
								break;

							default:
								break;
							}
							bEnd = true;
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
