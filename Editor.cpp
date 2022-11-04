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
//更新
//=============================================================================
void CEditor::Update()
{

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
					else if (strncmp(strLine, "Player", 5) == 0)
					{
						bFlag = true;
						m_type = OBJ_PLAYER;
					}
					else if (strncmp(strLine, "Pos", 3) == 0)
					{
						fscanf(fp, "%f%f%f", &m_pos.x, &m_pos.y, &m_pos.z);
					}
					else if (strncmp(strLine, "End", 3) == 0)
					{
						int nMaxCamera = 0;

						switch (m_type)
						{//タイプによって分ける
						case OBJ_PLAYER:

							nMaxCamera = CRenderer::GetMaxCamera();
							if (nNumplayer < nMaxCamera&&bFlag==true)
							{
								CPlayer::Create(nNumplayer);
								CGame::GetCameraPlayer(nNumplayer);
								nNumplayer++;
								bFlag = false;	//フラグリセット
							}
						default:
							break;
						}
						bEnd = true;
						break;
					}
				}
			}
		}
		fclose(fp);
	}
}
