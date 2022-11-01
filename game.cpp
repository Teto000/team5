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
CCamera* CGame::m_pCamera[nDefaultMaxCamera] = {};		//�J����

int CGame::m_nNumCamera = 0;	//�J�����̗񋓌^�̐�
int CGame::m_nMaxCamera = 0;	//�J�����̍ő吔
bool CGame::m_bStop = false;	//�v���O�������~����

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
	//�J�����̐���
	CreateCamera(NUMCAMERA_TWO);

	//�J�����̍ő吔�̐ݒ�
	{
		//�J�����̍ő吔���v���C���[�𐶐�
		for (int nCnt = 0; nCnt < m_nMaxCamera; nCnt++)
		{
			pPlayer[nCnt] = CPlayer::Create(nCnt);

			//�J�����ɑΉ�����v���C���[�̔ԍ��̐ݒ�
			GetCamera(nCnt)->SetNumPlayer(nCnt);
		}

		/*SetPlayerPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));*/
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
	//�J�����̏I��
	{
		for (int i = 0; i < m_nMaxCamera; i++)
		{
			if (m_pCamera[i] != nullptr)
			{//�J������null����Ȃ��Ȃ� 
				m_pCamera[i]->Uninit();
				delete m_pCamera[i];
				m_pCamera[i] = nullptr;
			}
		}
	}
}

//===========================
// �X�V
//===========================
void CGame::Update()
{
	//�J�����̍X�V
	{
		for (int i = 0; i < m_nMaxCamera; i++)
		{
			m_pCamera[i]->Update();
		}

		//�Q�[���I�����̏���
		if (CGoal::GetGoalFrag())
		{
			FinishGame();
		}
	}

	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enter�Ŏ��̉�ʂɑJ�ڂ���
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}
}

//===========================
// �J�����̐���
// (�J�����̐��񋓌^)
//===========================
void CGame::CreateCamera(CGame::NUMCAMERA num)
{
	//----------------------------
	// �J�����̐����Ə�����
	//----------------------------
	{
		//�J�����̍ő吔�̐ݒ�
		m_nNumCamera = CRenderer::SetMaxCamera(num);

		DWORD fWidth = SCREEN_WIDTH / 2;
		DWORD fHeight = SCREEN_HEIGHT / 2;

		switch (m_nNumCamera)
		{
		case NUMCAMERA_ONE:
			//�J�����̐���1�Ȃ�
			m_pCamera[0] = CCamera::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			break;

		case NUMCAMERA_TWO:
			//�J�����̐���2�Ȃ�
			m_pCamera[0] = CCamera::Create(0, 0, fWidth, SCREEN_HEIGHT);
			m_pCamera[1] = CCamera::Create(fWidth, 0, fWidth, SCREEN_HEIGHT);
			break;

		case NUMCAMERA_FOUR:
			//�J�����̐���4�Ȃ�
			m_pCamera[0] = CCamera::Create(0, 0, fWidth, fHeight);				//����
			m_pCamera[1] = CCamera::Create(fWidth, 0, fWidth, fHeight);			//�E��
			m_pCamera[2] = CCamera::Create(0, fHeight, fWidth, fHeight);		//����
			m_pCamera[3] = CCamera::Create(fWidth, fHeight, fWidth, fHeight);	//�E��
			break;

		default:
			//�v���O������~�t���O�𗧂Ă�
			m_bStop = true;
			exit(m_bStop);	//�v���O�������~����
		}

		//�J�����̍ő吔���擾
		m_nMaxCamera = CRenderer::GetMaxCamera();
	}
}

//===========================
// �Q�[���I�����̏���
//===========================
void CGame::FinishGame()
{
	//-------------------------------
	// 1�ʂ̃r���[�|�[�g���g�傷��
	//-------------------------------

	/* 1�ʂ̃v���C���[�ԍ����擾 */
	int nFirstNumber = CGoal::GetWinner();

	if (nFirstNumber >= 0 /* 1�ʂ�null����Ȃ��Ȃ� */)
	{//Z��������Ă���Ȃ�

	 //-----------------------
	 // �r���[�|�[�g���g��
	 //-----------------------
		switch (nFirstNumber)
		{
			//-----------------------
			// 1�l�ڂ�������
			//-----------------------
		case CCamera::NUMPLAYER_ONE:
			m_pCamera[nFirstNumber]->AddViewSize(0, 0, nSpeed_X, nSpeed_Y);
			break;

			//-----------------------
			// 2�l�ڂ�������
			//-----------------------
		case CCamera::NUMPLAYER_TWO:
			m_pCamera[nFirstNumber]->AddViewSize(-nSpeed_X, 0, nSpeed_X, nSpeed_Y);
			break;

			//-----------------------
			// 3�l�ڂ�������
			//-----------------------
		case CCamera::NUMPLAYER_THREE:
			m_pCamera[nFirstNumber]->AddViewSize(0, -nSpeed_Y, nSpeed_X, nSpeed_Y);
			break;

			//-----------------------
			// 4�l�ڂ�������
			//-----------------------
		case CCamera::NUMPLAYER_FOUR:
			m_pCamera[nFirstNumber]->AddViewSize(-nSpeed_X, -nSpeed_Y, nSpeed_X, nSpeed_Y);
			break;

		default:
			break;
		}
	}
}

//===========================
// �J�����̑傫���̃��Z�b�g
//===========================
void CGame::ResetCameraSize()
{
	DWORD fWidth = SCREEN_WIDTH / 2;
	DWORD fHeight = SCREEN_HEIGHT / 2;

	switch (m_nNumCamera)
	{
	case NUMCAMERA_ONE:
		//�J�����̐���1�Ȃ�
		m_pCamera[0]->SetViewSize(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;

	case NUMCAMERA_TWO:
		//�J�����̐���2�Ȃ�
		m_pCamera[0]->SetViewSize(0, 0, fWidth, SCREEN_HEIGHT);
		m_pCamera[1]->SetViewSize(fWidth, 0, fWidth, SCREEN_HEIGHT);
		break;

	case NUMCAMERA_FOUR:
		//�J�����̐���4�Ȃ�
		m_pCamera[0]->SetViewSize(0, 0, fWidth, fHeight);				//����
		m_pCamera[1]->SetViewSize(fWidth, 0, fWidth, fHeight);			//�E��
		m_pCamera[2]->SetViewSize(0, fHeight, fWidth, fHeight);			//����
		m_pCamera[3]->SetViewSize(fWidth, fHeight, fWidth, fHeight);	//�E��
		break;

	default:
		//�v���O������~�t���O�𗧂Ă�
		m_bStop = true;
		exit(m_bStop);	//�v���O�������~����
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
// �J�����̎擾
//===========================
CCamera *CGame::GetCamera(int nCnt)
{
	return m_pCamera[nCnt];
}

//===========================
// �v���C���[�̏������W�ݒ�
//===========================
void CGame::SetPlayerPosition(D3DXVECTOR3 pos)
{
	//���������Ƃɂ��������_���Ȓl�𐶐�
	srand((unsigned int)time(NULL));

	int nNumCamera = CRenderer::GetMaxCamera();

	// �v���C���[�̏����ʒu�̐ݒ�
	D3DXVECTOR3 FirstPos = pos;

	// FirstPos���瓙�Ԋu�ɂق��̃v���C���[�̍��W��ݒ肷��B(�ő�4)
	D3DXVECTOR3 PlayerPos[4] = {};

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// �v���C���[�̍��W��ݒ�
		PlayerPos[nCnt] = D3DXVECTOR3(pos.x + nCnt * 100, pos.y + nCnt, pos.z);
	}

	// �����ۑ��p
	// �����l��0���Ɩ�肪����̂�99
	int Rand[4] = { 99,99,99,99 };
	int RandCheck[4] = { 99,99,99,99 };
	int nCnt2 = 0;

	for (int nCnt = 0; nCnt < nNumCamera; nCnt++)
	{
		// �����_���̒l
		Rand[nCnt] = rand() % 4;

		while (1)
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
			if (nCnt2 == nNumCamera)
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