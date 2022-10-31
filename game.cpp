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
#include "Goal.h"

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

		SetPlayerPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	pMeshField = CMeshField::Create();

	CGoal*pGoal = CGoal::Create(D3DXVECTOR3(90.0f, 40.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	CTime *pTime = CTime::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

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

//===========================
// �v���C���[�̏������W�ݒ�
//===========================
void CGame::SetPlayerPosition(D3DXVECTOR3 pos)
{
	int nNumCamera = CRenderer::GetMaxCamera();

	//���������Ƃɂ��������_���Ȓl�𐶐�
	srand((unsigned int)time(NULL));

	// �v���C���[�̏����ʒu�̐ݒ�
	D3DXVECTOR3 FirstPos = pos;

	// FirstPos���瓙�Ԋu�ɂق��̃v���C���[�̍��W��ݒ肷��B(�ő�4)
	D3DXVECTOR3 PlayerPos[4] = {};

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// �v���C���[�̍��W��ݒ�
		PlayerPos[nCnt] = D3DXVECTOR3(pos.x + nCnt * 100, pos.y + nCnt,pos.z);
	}

	// �����ۑ��p
	// �����l��0���Ɩ�肪����̂�99
	int Rand[4] = {99,99,99,99};
	int RandCheck[4] = {99,99,99,99};
	int nCnt2 = 0;

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// �����_���̒l
		Rand[nCnt] = rand() % 4;

		while(1)
		{
			// Rand�����܂ł̒l�̂ǂꂩ�Ɠ�����������
			if (Rand[nCnt] == RandCheck[nCnt2])
			{
				// �����_���̒l
				Rand[nCnt] = rand() % 4;
				nCnt2 = 0;

				continue;
			}
			// �ʂ����񐔂𑫂�
			nCnt2++;

			// �S�Ă̔��̃`�F�b�N���I�������
			if (nCnt2 == PlayerAmount)
			{
				break;
			}
		}

		// �����h�`�F�b�N�ɒl������
		RandCheck[nCnt] = Rand[nCnt];
	}

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// �ݒ肵�����W�Ƀ����_���ȃv���C���[���ړ�������B
		pPlayer[nCnt]->SetPosition(PlayerPos[Rand[nCnt]]);
	}
}