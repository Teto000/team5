//===================================================
//
// �Q�[���̏���
// Author : Sato Teruto
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include <time.h>
#include "game.h"
#include "renderer.h"
#include "application.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "sound.h"
#include "player.h"
#include "meshfield.h"
#include "debug_proc.h"
#include "fade.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CPolygon* CGame::pPolygon = nullptr;
CPolygon2d* CGame::pPolygon2d = nullptr;
CPlayer*  CGame::pPlayer = nullptr;
CMeshField* CGame::pMeshField = nullptr;

//===========================
// �R���X�g���N�^
//===========================
CGame::CGame()
{
	
}

//===========================
// �f�X�g���N�^
//===========================
CGame::~CGame()
{

}

//===========================
// ������
//===========================
HRESULT CGame::Init()
{
	pPlayer = CPlayer::Create(0);

	pMeshField = CMeshField::Create();

	return S_OK;
}

//===========================
// �I��
//===========================
void CGame::Uninit()
{

}

//===========================
// �X�V
//===========================
void CGame::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enter�Ŏ��̉�ʂɑJ�ڂ���
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}
}

//===========================
// �v���C���[�̎擾
//===========================
CPlayer* CGame::GetPlayer()
{
	return pPlayer;
}