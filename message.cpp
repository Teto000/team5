//===================================================
//
// ポリゴン2Dの処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "message.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"

//=======================
// コンストラクタ
//=======================
CMessage::CMessage() : CObject2D(0)
{

}

//=======================
// デストラクタ
//=======================
CMessage::~CMessage()
{

}

//=======================
// 初期化
//=======================
HRESULT CMessage::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	CObject2D::SetSize(500.0f, 500.0f);

	CObject2D::SetTexture(CTexture::TEXTURE_COUNT_THREE);

	return S_OK;
}

//=======================
// 終了
//=======================
void CMessage::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// 更新
//=======================
void CMessage::Update()
{
	CObject2D::Update();

	if (CInputKeyboard::Trigger(DIK_P))
	{
		Uninit();
		return;
	}
}

//=======================
// 描画
//=======================
void CMessage::Draw()
{
	CObject2D::Draw();
}

//=======================
// 生成
//=======================
CMessage *CMessage::Create(D3DXVECTOR3 pos)
{
	CMessage *pMessage = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pMessage = new CMessage;	//生成

	if (pMessage != nullptr)
	{//NULLチェック
	 //初期化
		pMessage->Init(D3DXVECTOR3(pos));
		//pMessage->SetObjType(OBJTYPE_ENEMY);
	}

	return pMessage;
}