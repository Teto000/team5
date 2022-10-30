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
#include  "camera.h"
#include "time.h"
#include "GoalFragManager.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CPolygon* CGame::pPolygon = nullptr;
CPolygon2d* CGame::pPolygon2d = nullptr;
CPlayer*  CGame::pPlayer[MAX_PLAYER] = {};
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
	//�J�����̍ő吔�̐ݒ�
	{
		int nNumCamera = CRenderer::GetMaxCamera();

		//�J�����̍ő吔���v���C���[�𐶐�
		for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
		{
			pPlayer[nCnt] = CPlayer::Create(nCnt);

			//�J�����ɑΉ�����v���C���[�̔ԍ��̐ݒ�
			CApplication::GetCamera(nCnt)->SetNumPlayer(nCnt);
		}
	}
	CTime *pTime = CTime::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

	pMeshField = CMeshField::Create();

	CGoalFragManager*pGoal = CGoalFragManager::Create(D3DXVECTOR3(90.0f, 40.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	

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
CPlayer* CGame::GetPlayer(int NumPlayer)
{
	return pPlayer[NumPlayer];
}