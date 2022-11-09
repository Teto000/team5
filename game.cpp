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
#include "camera_player.h"
#include "time.h"
#include "Goal.h"
#include "message.h"
#include "motion_parts.h"
#include "3dobject.h"
#include "Editor.h"
#include "debug_proc.h"
#include "Map.h"
#include "read.h"
#include "num_block.h"
#include "Score.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CPolygon*	CGame::pPolygon = nullptr;
CPolygon2d*	CGame::pPolygon2d = nullptr;
CPlayer*	CGame::pPlayer[MAX_PLAYER] = {};
CMeshField*	CGame::pMeshField = nullptr;
CCameraPlayer*	CGame::m_pCameraPlayer[nDefaultMaxCamera] = {};		//�J����
CMessage*	CGame::m_pMessage;	//���b�Z�[�W
CEditor*	CGame::m_pEditor = nullptr;
CDebugProc*	CGame::m_pProc = nullptr;
CMap*		CGame::m_pMap = nullptr;

int	 CGame::m_nEnumCamera = 0;	//�J�����̗񋓌^�̐�
int	 CGame::m_nGroundNum = 0;		//��
int	 CGame::m_player = 0;		//�v���C���[�̐�
bool CGame::m_bFinish = false;	//�I���t���O

//===========================
// �R���X�g���N�^
//===========================
CGame::CGame()
{
	m_nMaxCamera = 0;		//�J�����̍ő吔
	m_bStop = false;		//�v���O�������~����
	mode = GAMEMODE_START;	//�Q�[���̏��
}

//===========================
// �f�X�g���N�^
//===========================
CGame::~CGame()
{
	m_nMaxCamera = 0;
}

//===========================
// ������
//===========================
HRESULT CGame::Init()
{
	//�ϐ��̏�����
	m_bFinish = false;

	//�J�����̐���
	CreateCamera((CGame::NUMCAMERA)m_player);

	//----------------------------
	// �J�����̍ő吔�̐ݒ�
	//----------------------------
	if (m_nEnumCamera == NUMCAMERA_THREE)
	{//�J�����񋓌^��3�Ȃ�
		//�J�����̍ő吔��1���炷
		m_nMaxCamera = 3;
	}

	//�J�����̍ő吔���v���C���[�𐶐�
	for (int nCnt = 0; nCnt < m_nMaxCamera; nCnt++)
	{
		pPlayer[nCnt] = CPlayer::Create(nCnt);

		//�J�����ɑΉ�����v���C���[�̔ԍ��̐ݒ�
		GetCameraPlayer(nCnt)->SetNumPlayer(nCnt);
	}

	SetPlayerPosition(D3DXVECTOR3(300.0f, 50.0f, -100.0f));

	//���b�V���t�B�[���h�̐���
	//pMeshField = CMeshField::Create();
	//m_pMap = CMap::Create(D3DXVECTOR3(0.0f, 0.0f, 100.0f));

	//�^�C�}�[�̐���
	CTime *pTime = CTime::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

	//�G�f�B�^�̍쐬�Ɠǂݍ���
	m_pEditor = CEditor::Create();

	//���b�Z�[�W�̐���
	m_pMessage = CMessage::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
									CMessage::MESSAGE_COUNT_THREE);

	//�T�E���h����
	CSound::PlaySound(CSound::SOUND_LABEL_GAME1);

	CRead cRead;
	m_nGroundNum = cRead.ReadMotion("data/MOTION/motionground.txt");

	//CObjectX::Create("data\\MODEL\\X_File\\Neptune_000.x", D3DXVECTOR3(200.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//===========================
// �I��
//===========================
void CGame::Uninit()
{
	//�T�E���h��~
	CSound::StopSound();

	//-----------------------------
	// �J�����̏I��
	//-----------------------------
	if (m_nEnumCamera == NUMCAMERA_THREE)
	{//�J�����񋓌^��3�Ȃ�
		//�J�����̍ő吔��߂�
		m_nMaxCamera = 4;
	}

	for (int i = 0; i < m_nMaxCamera; i++)
	{
		if (m_pCameraPlayer[i] != nullptr)
		{//�J������null����Ȃ��Ȃ� 
			m_pCameraPlayer[i]->Uninit();
			delete m_pCameraPlayer[i];
			m_pCameraPlayer[i] = nullptr;
		}
	}

	C3DObject::UninitAllModel();

	CMotionParts::ALLUninit();

	//�G�f�B�^�̍폜
	m_pEditor->Uninit();
	delete m_pEditor;
	m_pEditor = nullptr;
}

//===========================
// �X�V
//===========================
void CGame::Update()
{
	static int nCnt = 0;
	nCnt++;

	//�G�f�B�^�̍X�V
	m_pEditor->Update();

	//-----------------------------
	// �J�����̍X�V
	//-----------------------------
	if (m_nEnumCamera == NUMCAMERA_THREE)
	{//�J�����񋓌^��3�Ȃ�
	 //�J�����̍ő吔��߂�
		m_nMaxCamera = 4;
	}

	for (int i = 0; i < m_nMaxCamera; i++)
	{
		m_pCameraPlayer[i]->Update();
	}

	//�Q�[���I�����̏���
	if (CGoal::GetGoalFrag())
	{
		//���b�Z�[�W�̐���
		//m_pMessage = CMessage::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		//								CMessage::MESSAGE_FINISH);

		FinishGame();

		//��񂾂��ʂ�悤�ɂ������c
		const int ClearTime = CTime::GetTime();
		CScore::SetCurrentScore(ClearTime);
	}

	CMotionParts::ALLUpdate();

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
		m_nEnumCamera = CRenderer::SetMaxCamera(num);

		DWORD fWidth = SCREEN_WIDTH / 2;
		DWORD fHeight = SCREEN_HEIGHT / 2;

		switch (m_nEnumCamera)
		{
		case NUMCAMERA_ONE:
			//�J�����̐���1�Ȃ�
			m_pCameraPlayer[0] = CCameraPlayer::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			break;

		case NUMCAMERA_TWO:
			//�J�����̐���2�Ȃ�
			m_pCameraPlayer[0] = CCameraPlayer::Create(0, 0, fWidth, SCREEN_HEIGHT);
			m_pCameraPlayer[1] = CCameraPlayer::Create(fWidth, 0, fWidth, SCREEN_HEIGHT);
			break;

		case NUMCAMERA_THREE:
		case NUMCAMERA_FOUR:
			//�J�����̐���4�Ȃ�
			m_pCameraPlayer[0] = CCameraPlayer::Create(0, 0, fWidth, fHeight);				//����
			m_pCameraPlayer[1] = CCameraPlayer::Create(fWidth, 0, fWidth, fHeight);			//�E��
			m_pCameraPlayer[2] = CCameraPlayer::Create(0, fHeight, fWidth, fHeight);		//����
			m_pCameraPlayer[3] = CCameraPlayer::Create(fWidth, fHeight, fWidth, fHeight);	//�E��
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

	m_bFinish = true;	//�I���t���O�𗧂Ă�

	if (nFirstNumber >= 0)
	{//1�ʂ�null����Ȃ��Ȃ�

		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

		//-----------------------
		// �r���[�|�[�g���g��
		//-----------------------
		switch (nFirstNumber)
		{
			//-----------------------
			// 1�l�ڂ�������
			//-----------------------
		case CCameraPlayer::NUMPLAYER_ONE:
			m_pCameraPlayer[nFirstNumber]->AddViewSize(0, 0, nSpeed_X, nSpeed_Y);
			break;

			//-----------------------
			// 2�l�ڂ�������
			//-----------------------
		case CCameraPlayer::NUMPLAYER_TWO:
			m_pCameraPlayer[nFirstNumber]->AddViewSize(-nSpeed_X, 0, nSpeed_X, nSpeed_Y);
			break;

			//-----------------------
			// 3�l�ڂ�������
			//-----------------------
		case CCameraPlayer::NUMPLAYER_THREE:
			m_pCameraPlayer[nFirstNumber]->AddViewSize(0, -nSpeed_Y, nSpeed_X, nSpeed_Y);
			break;

			//-----------------------
			// 4�l�ڂ�������
			//-----------------------
		case CCameraPlayer::NUMPLAYER_FOUR:
			m_pCameraPlayer[nFirstNumber]->AddViewSize(-nSpeed_X, -nSpeed_Y, nSpeed_X, nSpeed_Y);
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

	switch (m_nEnumCamera)
	{
	case NUMCAMERA_ONE:
		//�J�����̐���1�Ȃ�
		m_pCameraPlayer[0]->SetViewSize(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;

	case NUMCAMERA_TWO:
		//�J�����̐���2�Ȃ�
		m_pCameraPlayer[0]->SetViewSize(0, 0, fWidth, SCREEN_HEIGHT);
		m_pCameraPlayer[1]->SetViewSize(fWidth, 0, fWidth, SCREEN_HEIGHT);
		break;

	case NUMCAMERA_THREE:
	case NUMCAMERA_FOUR:
		//�J�����̐���4�Ȃ�
		m_pCameraPlayer[0]->SetViewSize(0, 0, fWidth, fHeight);				//����
		m_pCameraPlayer[1]->SetViewSize(fWidth, 0, fWidth, fHeight);			//�E��
		m_pCameraPlayer[2]->SetViewSize(0, fHeight, fWidth, fHeight);			//����
		m_pCameraPlayer[3]->SetViewSize(fWidth, fHeight, fWidth, fHeight);	//�E��
		break;

	default:
		//�v���O������~�t���O�𗧂Ă�
		m_bStop = true;
		exit(m_bStop);	//�v���O�������~����
	}
}

//===========================
// �J�����񋓌^�ԍ����擾
//===========================
int CGame::GetEnumCamera()
{
	return m_nEnumCamera;
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
CCameraPlayer *CGame::GetCameraPlayer(int nCnt)
{
	return m_pCameraPlayer[nCnt];
}

//===========================
// �I���t���O�̎擾
//===========================
bool CGame::GetFinish()
{
	return m_bFinish;
}

//===========================
// �v���C���[�̏������W�ݒ�
//===========================
void CGame::SetPlayerPosition(D3DXVECTOR3 pos)
{
	//���������Ƃɂ��������_���Ȓl�𐶐�
	srand((unsigned int)time(NULL));

	//int nNumCamera = CRenderer::GetMaxCamera();

	m_nEnumCamera = CGame::GetEnumCamera();

	if (m_nEnumCamera == NUMCAMERA_THREE)
	{//�J�����񋓌^��3�Ȃ�
	 //�J�����̍ő吔��1���炷
		m_nMaxCamera = 3;
	}

	// �v���C���[�̏����ʒu�̐ݒ�
	D3DXVECTOR3 FirstPos = pos;

	// FirstPos���瓙�Ԋu�ɂق��̃v���C���[�̍��W��ݒ肷��B(�ő�4)
	D3DXVECTOR3 PlayerPos[4] = {};

	for (int nCnt = 0; nCnt < m_nMaxCamera; nCnt++)
	{
		// �v���C���[�̍��W��ݒ�
		PlayerPos[nCnt] = D3DXVECTOR3(pos.x + nCnt * 100, pos.y + nCnt, pos.z);
	}

	// �����ۑ��p
	// �����l��0���Ɩ�肪����̂�99
	int Rand[4] = { 99,99,99,99 };
	int RandCheck[4] = { 99,99,99,99 };
	int nCnt2 = 0;

	for (int nCnt = 0; nCnt < m_nMaxCamera; nCnt++)
	{
		// �����_���̒l
		Rand[nCnt] = rand() % m_nMaxCamera;

		while (1)
		{
			// Rand�����܂ł̒l�̂ǂꂩ�Ɠ�����������
			if (Rand[nCnt] == RandCheck[nCnt2])
			{
				// �����_���̒l
				Rand[nCnt] = rand() % m_nMaxCamera;
				nCnt2 = 0;

				continue;
			}
			// �ʂ����񐔂𑫂�
			nCnt2++;

			// �S�Ă̔��̃`�F�b�N���I�������
			if (nCnt2 == m_nMaxCamera)
			{
				break;
			}
		}

		// �����h�`�F�b�N�ɒl������
		RandCheck[nCnt] = Rand[nCnt];
	}

	for (int nCnt = 0; nCnt < m_nMaxCamera; nCnt++)
	{
		// �ݒ肵�����W�Ƀ����_���ȃv���C���[���ړ�������B
		pPlayer[nCnt]->SetPosition(PlayerPos[Rand[nCnt]]);
	}
}