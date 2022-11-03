//===================================================
//
// �|���S��2D�̏���
// Author : Sato Teruto
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "message.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"

//=======================
// �R���X�g���N�^
//=======================
CMessage::CMessage() : CObject2D(0)
{

}

//=======================
// �f�X�g���N�^
//=======================
CMessage::~CMessage()
{

}

//=======================
// ������
//=======================
HRESULT CMessage::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	CObject2D::SetSize(500.0f, 500.0f);

	CObject2D::SetTexture(CTexture::TEXTURE_COUNT_THREE);

	return S_OK;
}

//=======================
// �I��
//=======================
void CMessage::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// �X�V
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
// �`��
//=======================
void CMessage::Draw()
{
	CObject2D::Draw();
}

//=======================
// ����
//=======================
CMessage *CMessage::Create(D3DXVECTOR3 pos)
{
	CMessage *pMessage = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pMessage = new CMessage;	//����

	if (pMessage != nullptr)
	{//NULL�`�F�b�N
	 //������
		pMessage->Init(D3DXVECTOR3(pos));
		//pMessage->SetObjType(OBJTYPE_ENEMY);
	}

	return pMessage;
}