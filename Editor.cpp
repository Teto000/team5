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
#include"objectX.h"
#include"Gimmick.h"
#include"stack_block.h"

//静的メンバ変数
CGoal*		CEditor::m_pGoal = nullptr;										//ゴール
CSBlock*	CEditor::m_pStackBlock[MAX_STACK_BLOCK] = { nullptr };			//ブロックの山
//=============================================================================
// コンストラクタ
//=============================================================================
CEditor::CEditor()
{
	m_pGoal = nullptr;						//ゴール
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
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//出現座標
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//出現した際の角度
	m_bEnd = false;									//終了フラグ
	m_bFlag = false;								//生成フラグ
	m_nNumber = 0;									//現在設定するブロックのタイプ
	m_nNumpla = 0;

	for (int nCnt = 0; nCnt < MAX_STACK_BLOCK; nCnt++)
	{
		if (m_pStackBlock[nCnt] != nullptr)
		{
			m_pStackBlock[nCnt] = nullptr;
		}
	}
	Pass();
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

	for (int i = 0; i < MAX_PLANET; i++)
	{
		if (m_pPlanet[i] != nullptr)
		{
			m_pPlanet[i] = nullptr;
		}
	}

	for (int nCnt = 0; nCnt < MAX_STACK_BLOCK; nCnt++)
	{
		if (m_pStackBlock[nCnt] != nullptr)
		{
			m_pStackBlock[nCnt] = nullptr;
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
#ifdef _DEBUG
	Input();

	//デバッグ用文字の表示
	CDebugProc::Print("現在置くオブジェクト:%d 0(ゼロ)キーで種類を変更", m_nNumber);
	CDebugProc::Print("O(オー)キーでオブジェクトの座標ファイルに出力する");
	CDebugProc::Print("Pキーでオブジェクトの生成/移動");

	//選択中のオブジェクトに応じて表示される文字を変更
	if (m_nNumber == OBJ_PLANET)
	{
		CDebugProc::Print("現在設置する惑星の種類:%d 8/9で種類を変更", m_nNumpla);
	}
	else if (m_nNumber == OBJ_GIMMICK)
	{
		CDebugProc::Print("現在設置するギミックの種類:%d 8/9で種類を変更", m_nNumgim);
	}
#endif // !_DEBUG_
}

//=============================================================================
// マップデータ読み込み
//=============================================================================
void CEditor::Load()
{
	const int lenLine = 1024;	//1単語の最大数
	char strLine[lenLine];		//読み込み用の文字列
	m_gim = 0;
								
	FILE*fp = fopen(FILE_MAPDATA, "r");		//ファイル読み込み
	if (fp != NULL)
	{
		for (int i = 0; i < lenLine; i++)
		{
			fgets(strLine, lenLine, fp);
			if (strncmp(strLine, "Object", 6) == 0)	//Objectを読み込むまでループ
			{
				m_bEnd = false;
				while (m_bEnd == false)
				{
					fscanf(fp, "%s", &strLine[0]);	//読み込んだ文字ごとに設定する

					if (strncmp(strLine, "#", 1) == 0)
					{//コメントアウト
						continue;
					}
					else if (strncmp(strLine, "Goal", 4) == 0)
					{//生成するオブジェクトをゴールに指定
						m_bFlag = true;
						m_type = OBJ_GOAL;
					}
					else if (strncmp(strLine, "StackBlock", 10) == 0)
					{//生成するオブジェクトをブロックに指定
						m_bFlag = true;
						m_type = OBJ_STACK_BLOCK;
					}
					else if (strncmp(strLine, "Gimmick", 7) == 0)
					{//生成するオブジェクトを障害物に指定
						m_bFlag = true;
						m_type = OBJ_GIMMICK;
					}
					else if (strncmp(strLine, "Planet", 7) == 0)
					{//生成するオブジェクトを惑星に指定
						m_bFlag = true;
						m_type = OBJ_PLANET;
					}
					else if (strncmp(strLine, "Pos", 3) == 0)
					{//生成するオブジェクトの位置を指定
						fscanf(fp, "%f%f%f", &m_pos.x, &m_pos.y, &m_pos.z);
					}
					else if (strncmp(strLine, "Rot", 3) == 0)
					{//生成するオブジェクトの向きを指定
						fscanf(fp, "%f%f%f", &m_rot.x, &m_rot.y, &m_rot.z);
					}
					else if (strncmp(strLine, "Type", 4) == 0)
					{//生成するオブジェクトの種類を指定
						fscanf(fp, "%d", &m_nNumgim);
						m_nNumpla = m_nNumgim;
					}
					else if (strncmp(strLine, "End", 3) == 0)
					{//オブジェクトの生成
						if (m_bFlag == true)
						{
							switch (m_type)
							{//タイプによって分ける
							case OBJ_GOAL:
								if (m_pGoal == nullptr)
								{//ゴールは一体のみ生成されるようにする
									m_pGoal = CGoal::Create(D3DXVECTOR3(m_pos), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
								}
								break;

							case OBJ_GIMMICK:
								if (m_nNumgim >= 0 && m_nNumgim < MAX_GIMMICK)
								{//障害物
									m_pGimmick[m_gim]=	CGimmick::Create(m_nNumgim, m_pos, m_rot);
									m_gim++;
								}
								break;

							case OBJ_PLANET:
								if (m_pPlanet[m_nNumpla] == nullptr)
								{//惑星
									m_pPlanet[m_nNumpla] = CObjectX::Create(m_nPlaFileName[m_nNumpla], m_pos, m_rot);
								}
								break;

							case OBJ_STACK_BLOCK:
								for (int nCnt = 0; nCnt < MAX_STACK_BLOCK; nCnt++)
								{//ブロック
									if (m_pStackBlock[nCnt] == nullptr)
									{
										m_pStackBlock[nCnt] = CSBlock::Create(m_pos,m_rot);
										break;
									}
								}
								break;

							default:
								break;
							}
							m_bEnd = true;	//オブジェクトを生成したのでループ終了
							break;
						}
					}
				}
			}
		}
		fclose(fp);
	}

	//数値リセット
	m_nNumgim = 0;
	m_nNumpla = 0;
}

//=============================================================================
//生成
//=============================================================================
CEditor * CEditor::Create()
{
	CEditor *pEditor = new CEditor;

	pEditor->Init();	//初期化
	pEditor->Load();	//読み込み

	return pEditor;
}

//=============================================================================
//オブジェクトの位置の保存
//=============================================================================
void CEditor::SaveObject()
{
	//オブジェクトの情報保存用
	CObject* pObj;		//オブジェクトの情報
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き

	FILE*fp = fopen("data\\TXT\\ObjectPos.txt", "w");		//ファイル読み込み

	//ゴールの情報を保存
	fprintf(fp, "Object\n");
	fprintf(fp, "Goal\n");
	fprintf(fp, "Pos %.1f %.1f %.1f\n", m_pGoal->GetPosition().x, m_pGoal->GetPosition().y, m_pGoal->GetPosition().z);
	fprintf(fp, "Rot %.2f %.2f %.2f\n", m_pGoal->GetBaseRot().x, m_pGoal->GetBaseRot().y, m_pGoal->GetBaseRot().z);
	fprintf(fp, "End\n\n");

	//障害物の情報を保存
	pObj = CObject::GetTop(CObject::OBJTYPE_GIMMICK);		//障害物のリスト構造の先頭を取得

	while (pObj)
	{//pObjがnullじゃないなら
	 //次のオブジェクトを保存
		CObject* pObjNext = pObj->GetNext();

		//オブジェクトの位置と向きを取得
		 pos= pObj->GetPosition();
		 rot = pObj->GetBaseRot();

		fprintf(fp, "Object\n");
		fprintf(fp, "Gimmick\n");
		fprintf(fp, "Pos %.1f %.1f %.1f\n", pos.x, pos.y, pos.z);
		fprintf(fp, "Rot %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);
		fprintf(fp, "Type %d\n",pObj->GetType());
		fprintf(fp, "End\n\n");

		//次のオブジェクトのアドレスを代入
		pObj = pObjNext;
	}

	//惑星の情報を保存
	pObj = CObject::GetTop(CObject::OBJTYPE_PLANET);		//惑星のリスト構造の先頭を取得

	while (pObj)
	{//pObjがnullじゃないなら
	 //次のオブジェクトを保存
		CObject* pObjNext = pObj->GetNext();

		//終了処理
		pos = pObj->GetPosition();
		rot = pObj->GetBaseRot();

		fprintf(fp, "Object\n");
		fprintf(fp, "Planet\n");
		fprintf(fp, "Pos %.1f %.1f %.1f\n", pos.x, pos.y, pos.z);
		fprintf(fp, "Rot %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);
		fprintf(fp, "Type %d\n", pObj->GetType());
		fprintf(fp, "End\n\n");

		//次のオブジェクトのアドレスを代入
		pObj = pObjNext;
	}

	fclose(fp);
}

//=============================================================================
//惑星のパスを読み込み
//=============================================================================
void CEditor::Pass()
{
	//星のオブジェクト
	m_nPlaFileName[0] = "data\\MODEL\\X_File\\Earth_000.x";
	m_nPlaFileName[1] = "data\\MODEL\\X_File\\Sun_000.x";
	m_nPlaFileName[2] = "data\\MODEL\\X_File\\Moon_000.x";
	m_nPlaFileName[3] = "data\\MODEL\\X_File\\Mars_000.x";
	m_nPlaFileName[4] = "data\\MODEL\\X_File\\Jupiter_000.x";
	m_nPlaFileName[5] = "data\\MODEL\\X_File\\Venus_000.x";
	m_nPlaFileName[6] = "data\\MODEL\\X_File\\Saturn_000.x";
	m_nPlaFileName[7] = "data\\MODEL\\X_File\\Neptune_000.x";
	m_nPlaFileName[8] = "data\\MODEL\\X_File\\Uranus_000.x";
	m_nPlaFileName[9] = "data\\MODEL\\X_File\\Pluto_000.x";
	m_nPlaFileName[10] = "data\\MODEL\\X_File\\BG_000.x";
}

//=============================================================================
//入力処理
//=============================================================================
void CEditor::Input()
{
	//オブジェクトの位置を保存
	if (CInputKeyboard::Trigger(DIK_O) == true)
	{//O(オー)キーを押したとき
		SaveObject();
	}

	//ナンバーの切り替え
	if (CInputKeyboard::Trigger(DIK_0) == true)
	{//0(ゼロ)キーを押したとき
		if (m_nNumber++ >= OBJ_MAX - 1)
		{
			m_nNumber = 0;
		}
	}
	
	//オブジェクトの種類の変更
	if (CInputKeyboard::Trigger(DIK_9)) 
	{//9キーを押したとき
		switch (m_nNumber)
		{
		case OBJ_PLANET:
			m_nNumpla++;

			if (m_nNumpla >= MAX_PLANET)
			{//最大数を超えたとき0にリセット
				m_nNumpla = 0;
			}
			break;

		case OBJ_GIMMICK:
			m_nNumgim++;

			if (m_nNumgim>=MAX_GIMMICK)
			{//最大数を超えたとき0にリセット
				m_nNumgim = 0;
			}
		default:
			break;
		}
	}
	else if (CInputKeyboard::Trigger(DIK_8))
	{//8キーを押したとき
		switch (m_nNumber)
		{
		case OBJ_PLANET:
			m_nNumpla--;

			if (m_nNumpla < 0)
			{//下限を超えたら上限値にする
				m_nNumpla = MAX_PLANET - 1;
			}
			break;

		case OBJ_GIMMICK:
			m_nNumgim--;

			if (m_nNumgim < 0)
			{//下限を超えたら上限値にする
				m_nNumgim = MAX_GIMMICK - 1;
			}
		default:
			break;
		}
	}
	
	//Pキーで生成/移動
	if (CInputKeyboard::Trigger(DIK_P))
	{
		CPlayer* m_pPlayer = CGame::GetPlayer(0);
		D3DXVECTOR3 pos = m_pPlayer->GetPosition();
		D3DXVECTOR3 rot = m_pPlayer->GetRot();

		switch (m_nNumber)
		{
		case OBJ_GOAL:
			if (m_pGoal == nullptr)
			{//生成されていない場合のみ生成
				m_pGoal = CGoal::Create(pos, rot);
			}
			else
			{//中身がある場合現在のプレイヤーの位置に移動
				m_pGoal->SetPosition(pos);
			}
			break;

		case OBJ_GIMMICK:
			CGimmick::Create(m_nNumgim, pos, rot);	//生成
			break;

		case OBJ_PLANET:
			if (m_pPlanet[m_nNumpla] == nullptr)
			{//生成されていない場合のみ生成
				m_pPlanet[m_nNumpla] = CObjectX::Create(m_nPlaFileName[m_nNumpla], pos, rot);
			}
			else
			{//中身がある場合現在のプレイヤーの位置に移動
				m_pPlanet[m_nNumpla]->SetPos(pos);
			}
			break;

		case OBJ_STACK_BLOCK:
			if (m_pStackBlock[0] == nullptr)
			{//テスト用に一個だけ生成
				m_pStackBlock[0] = CSBlock::Create(m_pos, m_rot);
				break;
			}

		default:
			break;
		}
	}
}

//=============================================================================
// ゴールの取得
//=============================================================================
CGoal* CEditor::GetGoal()
{
	return m_pGoal;
}