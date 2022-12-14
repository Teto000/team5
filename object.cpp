//===================================================
//
// オブジェクトの処理
// Author : Sato Teruto
//
//===================================================

//--------------------
// インクルード
//--------------------
#include "object.h"
#include "object3d.h"
#include "input.h"
#include "input_keybord.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CObject* CObject::m_Top[nMaxPriority] = {};		//最初のオブジェクト
CObject* CObject::m_Current[nMaxPriority] = {};	//最後のオブジェクト

//=============================
// コンストラクタ
//=============================
CObject::CObject(int nPriority)
{
	m_nPriority = nPriority;	//プライオリティの番号
	m_bDeath = false;			//死亡フラグ
	m_bPause = false;			//ポーズ画面のON,OFF

	//-----------------------------
	// 先頭のオブジェクトを設定
	//-----------------------------
	if (m_Top[m_nPriority] == nullptr)
	{//先頭がないなら
		//先頭のオブジェクトに代入
		m_Top[m_nPriority] = this;
		m_pPrev = nullptr;
	} 

	//-----------------------------
	// 最後のオブジェクトを設定
	//-----------------------------
	if (m_pNext == nullptr)
	{//1つ後ろがnullなら
		//最後のオブジェクトに代入
		m_pPrev = m_Current[m_nPriority];
		m_Current[m_nPriority] = this;

		if (m_pPrev)
		{
			m_pPrev->m_pNext = this;
		}
	}
}

//=============================
// デストラクタ
//=============================
CObject::~CObject()
{

}

//=============================
// 全て解放
//=============================
void CObject::ReleaseAll(bool bFinish)
{
	for (int i = 0; i < nMaxPriority; i++)
	{
		if (!m_Top[i])
		{//Topがnullなら
			continue;
		}

		CObject* pObj = m_Top[i];

		while (pObj)
		{//pObjがnullじゃないなら
			//次のオブジェクトを保存
			CObject* pObjNext = pObj->m_pNext;

			//終了処理
			pObj->Release();

			//次のオブジェクトのアドレスを代入
			pObj = pObjNext;
		}

		//-------------------
		// 死亡処理
		//-------------------
		pObj = m_Top[i];

		while (pObj)
		{
			if (bFinish == true)
			{
				//次のオブジェクトを保存
				CObject* pObjNext = pObj->m_pNext;

				if (pObj->m_bDeath == true)
				{
					//消去処理
					pObj->Death(pObj);
				}

				//次のオブジェクトのアドレスを代入
				pObj = pObjNext;
			}
		}
	}
}

//=============================
// 全て更新
//=============================
void CObject::UpdateAll()
{
	for (int m_nPriority = 0; m_nPriority < OBJTYPE_MAX; m_nPriority++)
	{
		if (!m_Top[m_nPriority])
		{//Topがnullなら
			continue;
		}

		CObject* pObj = m_Top[m_nPriority];

		while (pObj)
		{//pObjがnullじゃないなら
			//次のオブジェクトを保存
			CObject* pObjNext = pObj->m_pNext;

			//-------------------------
			// ポーズ画面の切り替え
			//-------------------------
			/*if (CInputKeyboard::Trigger(DIK_P))
			{//Pキーを押したとき
				if (pObj->m_bPause == false)
				{//ポーズ中じゃないなら
					//ポーズする
					pObj->m_bPause = true;
				}
				else
				{//ポーズ中なら
					//ポーズを解除する
					pObj->m_bPause = false;
				}
			}*/

			if (pObj->m_bPause == false)
			{//ポーズ中じゃないなら
				//更新処理
				pObj->Update();
			}

			//次のオブジェクトのアドレスを代入
			pObj = pObjNext;
		}

		//-------------------
		// 死亡処理
		//-------------------
		pObj = m_Top[m_nPriority];

		while (pObj)
		{
			//次のオブジェクトを保存
			CObject* pObjNext = pObj->m_pNext;

			if (pObj->m_bDeath == true)
			{
				//消去処理
				pObj->Death(pObj);
			}

			//次のオブジェクトのアドレスを代入
			pObj = pObjNext;
		}
	}
}

//=============================
// 全て描画
//=============================
void CObject::DrawAll()
{
	for (int i = 0; i < OBJTYPE_MAX; i++)
	{
		if (!m_Top[i])
		{//Topがnullなら
			continue;
		}

		CObject* pObj = m_Top[i];

		while (pObj)
		{//pObjがnullじゃないなら
			//描画処理
			pObj->Draw();

			//次のオブジェクトのアドレスを代入
			pObj = pObj->m_pNext;
		}
	}
}

//=============================
// 解放する
//=============================
void CObject::Release()
{
	//死亡フラグを立てる
	m_bDeath = true;

	return;
}

//=============================
// 先頭のアドレスを取得
//=============================
CObject * CObject::GetTop(int priority)
{
	return m_Top[priority];
}

//=============================
// 次のオブジェクトを取得
//=============================
CObject * CObject::GetNext()
{
	return m_pNext;
}

int CObject::GetType()
{
	return m_nType;
}

//=============================
// 死亡処理
//=============================
void CObject::Death(CObject* pObj)
{
	//----------------------------------
	// 前後のオブジェクトの接続
	//----------------------------------
	if (m_pPrev != nullptr)
	{//1つ前がnullじゃないなら
	 //前のNextに次を代入
		m_pPrev->m_pNext = m_pNext;
	}

	if (m_pNext != nullptr)
	{//1つ後がnullじゃないなら
	 //次のPrevに前を代入
		m_pNext->m_pPrev = m_pPrev;
	}

	//----------------------------------
	// 先頭と最後のオブジェクトを設定
	//----------------------------------
	if (m_pPrev == nullptr)
	{//自分が先頭なら
	 //次のオブジェクトを先頭にする
		m_Top[m_nPriority] = m_pNext;
	}

	if (m_pNext == nullptr)
	{//自分が最後なら
	 //次のオブジェクトを最後にする
		m_Current[m_nPriority] = m_pPrev;
	}

	pObj->Uninit();

	delete pObj;
	pObj = nullptr;
}

//=============================
// 総数の取得
//=============================
int CObject::GetNumAll()
{
	return 0;
}

//=============================
// オブジェクトの取得
//=============================
//CObject *CObject::GETObject(int nPriority, int nCnt)
//{
//	return m_pObject;
//}

//=============================
// オブジェクトの種類の設定
//=============================
void CObject::SetObjType(EObjType ObjType)
{
	m_ObjType = ObjType;
}

//=============================
// オブジェクトの種類の取得
//=============================
CObject::EObjType CObject::GetObjType()
{
	return m_ObjType;
}

//=============================
// オブジェクトの種類の設定
//=============================
void CObject::SetType(int Type)
{
	m_nType = Type;
}

//=============================
// もともとの向きを設定
//=============================
void CObject::SetBaseRot(D3DXVECTOR3 rot)
{
	m_Baserot = rot;
}

//=============================
// ベースの向きを取得
//=============================
D3DXVECTOR3 CObject::GetBaseRot()
{
	return m_Baserot;
}
