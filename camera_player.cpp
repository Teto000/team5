//==================================================
//
// �v���C���[�J�����̏���
// Author:Teruto Sato
//
//==================================================

//----------------------
// �C���N���[�h
//----------------------
#include "camera_player.h"
#include "camera.h"
#include "input.h"
#include "input_keybord.h"
#include "application.h"
#include "game.h"
#include "player.h"
#include "Goal.h"

//==================================================
// �R���X�g���N�^
//==================================================
CCameraPlayer::CCameraPlayer()
{
	m_nNumPlayer = -1;		//�Ή�����v���C���[�ԍ�
	m_nNumFieldCamera = 0;	//�t�B�[���h�J�����̃v���C���[�ԍ�
	m_nChangeTime = 0;		//�t�B�[���h�J�����؂�ւ�����
}

//==================================================
// �f�X�g���N�^
//==================================================
CCameraPlayer::~CCameraPlayer()
{

}

//==================================================
// �J�����̏���������
//==================================================
HRESULT CCameraPlayer::Init(void)
{
	//�J�����̏�����
	CCamera::Init();

	m_rot.y = D3DX_PI;			//��]�ʂ̑���
	//m_posR.x = D3DX_PI;	//x�̋���
	//m_posR.y = D3DX_PI;	//y�̋���
	//m_posR.z = D3DX_PI;	//z�̋���

	return S_OK;
}

//==================================================
// �J�����̏I������
//==================================================
void CCameraPlayer::Uninit(void)
{
	//�J�����̏I��
	CCamera::Uninit();
}

//==================================================
// �J�����̍X�V����
//==================================================
void CCameraPlayer::Update(void)
{
	//�J�����̍X�V
	CCamera::Update();

	//------------------------
	// �Q�[���I�����̏���
	//------------------------
	if (CGame::GetFinish())
	{//�I���t���O�������Ă���Ȃ�
		if (m_nNumPlayer == CGoal::GetWinner())
		{//�Ή�����v���C���[�ԍ������҂ƈꏏ�Ȃ�
			//��]������
			m_rot.y -= fTurnSpeed / 16;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}

	//------------------------
	// �v���C���[���Ƃ̏���
	//------------------------
	EachPlayer();

	//------------------------
	// �t�B�[���h�J�����̐���
	//------------------------
	if (CApplication::GetGame()->GetEnumCamera() == CGame::NUMCAMERA_THREE
		&& m_nNumPlayer == -1)
	{//�J�����񋓌^��three ���� �Ή�����v���C���[�ԍ���0�����Ȃ�
		m_rot.y -= fTurnSpeed / 16;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}

	//------------------------
	// �J�����̒Ǐ]
	//------------------------
	Following();
}

//==================================================
// �J�����̐ݒ菈��
// ���� : �f�o�C�X
//==================================================
void CCameraPlayer::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//�J�����̐ݒ�
	CCamera::SetCamera(pDevice);
}

//==================================================
// �J�����̐���
// ���� : ����̍��W X, ����̍��W Y, ��, ����
//==================================================
CCameraPlayer* CCameraPlayer::Create(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	CCameraPlayer *pCameraPlayer = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pCameraPlayer = new CCameraPlayer;	//����

	if (pCameraPlayer != nullptr)
	{//NULL�`�F�b�N
	 //�r���[�|�[�g�\���̕ۑ�
		pCameraPlayer->m_viewport.X = X;			//�J�n�ʒu(X)
		pCameraPlayer->m_viewport.Y = Y;			//�J�n�ʒu(Y)
		pCameraPlayer->m_viewport.Width = Width;	//��
		pCameraPlayer->m_viewport.Height = Height;	//����

		//������
		pCameraPlayer->Init();
	}

	return pCameraPlayer;
}

//==================================================
// �v���C���[���Ƃ̏���
//==================================================
void CCameraPlayer::EachPlayer()
{
	switch (m_nNumPlayer)
	{
		//------------------
		// �v���C���[1
		//------------------
	case NUMPLAYER_ONE:
		Turn(DIK_E, DIK_Q);	//����
		//Turn(DIK_Q, DIK_E);
		break;

		//------------------
		// �v���C���[2
		//------------------
	case NUMPLAYER_TWO:
		Turn(DIK_Y, DIK_R);	//����
		//Turn(DIK_R, DIK_Y);
		break;

		//------------------
		// �v���C���[3
		//------------------
	case NUMPLAYER_THREE:
		Turn(DIK_O, DIK_U);	//����
		//Turn(DIK_U, DIK_O);
		break;

		//------------------
		// �v���C���[4
		//------------------
	case NUMPLAYER_FOUR:
		Turn(DIK_RIGHT, DIK_LEFT);	//����
		//Turn(DIK_LEFT, DIK_RIGHT);
		break;

	default:
		break;
	}
}

//==================================================
// ����
// ���� : ����]�L�[,�E��]�L�[
//==================================================
void CCameraPlayer::Turn(int nLeftKey, int nRightKey)
{
	//-------------------
	// ����]
	//-------------------
	if (CInputKeyboard::Press(nLeftKey))
	{//�L�[�������ꂽ
		m_rot.y += fTurnSpeed;			//��]�ʂ̑���
		m_posR.x = m_posV.x + POLOR_X;	//x�̋���
		m_posR.y = m_posV.y + POLOR_Y;	//y�̋���
		m_posR.z = m_posV.z + POLOR_Z;	//z�̋���
	}
	//-------------------
	// �E��]
	//-------------------
	else if (CInputKeyboard::Press(nRightKey))	//�E��]
	{//�L�[�������ꂽ
		m_rot.y -= fTurnSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	/*if (CInputKeyboard::Press(DIK_UP))	//���]
	{//��L�[�������ꂽ
	if (m_rot.x <= 3.0f)
	{
	m_rot.x += fTurnSpeed;
	m_posV.x = m_posR.x - POLOR_X;
	m_posV.y = m_posR.y - POLOR_Y;
	m_posV.z = m_posR.z - POLOR_Z;
	}
	}
	else if (CInputKeyboard::Press(DIK_DOWN))	//����]
	{//���L�[�������ꂽ
	if (m_rot.x >= 1.8f)
	{
	m_rot.x -= fTurnSpeed;
	m_posV.x = m_posR.x - POLOR_X;
	m_posV.y = m_posR.y - POLOR_Y;
	m_posV.z = m_posR.z - POLOR_Z;
	}
	}*/
}

//==================================================
// �ړ�
// ���� : ��L�[�A���L�[�A���L�[�A�E�L�[
//==================================================
void CCameraPlayer::Move(int nUpKey, int nDownKey, int nLeftKey, int nRightKey)
{
	//-------------------
	// ���ړ�
	//-------------------
	if (CInputKeyboard::Press(nLeftKey))
	{//A�L�[�������ꂽ
		if (CInputKeyboard::Press(nUpKey))
		{//W�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.25f) * fMoveSpeed;	//�����ړ�
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.25f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//S�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.75f) * fMoveSpeed;	//���O�ړ�
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.75f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;	//���ړ�
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	//-------------------
	// �E�ړ�
	//-------------------
	else if (CInputKeyboard::Press(nRightKey))
	{//D�L�[�������ꂽ
		if (CInputKeyboard::Press(nUpKey))
		{//W�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.25f) * fMoveSpeed;	//�E���ړ�
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.25f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//S�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.75f) * fMoveSpeed;	//�E�O�ړ�
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.75f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;	//�E�ړ�
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	//-------------------
	// ���ړ�
	//-------------------
	else if (CInputKeyboard::Press(nUpKey))
	{//W�L�[�������ꂽ
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 1.0f) * fMoveSpeed;	//���ړ�
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 1.0f) * fMoveSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	//-------------------
	// ��O�ړ�
	//-------------------
	else if (CInputKeyboard::Press(nDownKey))
	{//S�L�[�������ꂽ
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.0f) * fMoveSpeed;	//�O�ړ�
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.0f) * fMoveSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	/*if (CInputKeyboard::Press(DIK_O))	//�㏸
	{//O�L�[�������ꂽ
	m_posV.y -= tanf(m_rot.x + D3DX_PI * 1.0f) * fMoveSpeed;	//��ړ�
	m_posR.x = m_posV.x + POLOR_X;
	m_posR.y = m_posV.y + POLOR_Y;
	m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_L))	//���~
	{//L�L�[�������ꂽ
	m_posV.y += tanf(m_rot.x + D3DX_PI * 0.0f) * fMoveSpeed;	//��ړ�
	m_posR.x = m_posV.x + POLOR_X;
	m_posR.y = m_posV.y + POLOR_Y;
	m_posR.z = m_posV.z + POLOR_Z;
	}*/
}

//==================================================
// �Ǐ]
//==================================================
void CCameraPlayer::Following()
{
	if (!CApplication::GetGame())
	{//�Q�[�����I�����Ă�Ȃ�
		return;
	}

	D3DXVECTOR3 playerPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	D3DXVECTOR3 playerRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//------------------------
	// �t�B�[���h�J�����̐ݒ�
	//------------------------
	if (CApplication::GetGame()->GetEnumCamera() == CGame::NUMCAMERA_THREE
		&& m_nNumPlayer == -1)
	{//�J�����񋓌^��three ���� �Ή�����v���C���[�ԍ���0�����Ȃ�

	 //�؂�ւ����Ԃ̉��Z
		m_nChangeTime++;

		if (m_nChangeTime % 360 == 0)
		{//��莞�Ԃ��o�߂�����
		 //�����_���ȃv���C���[�ɐ؂�ւ�
		 //m_nNumFieldCamera = rand() % 3;

		 //���̃v���C���[�ɐ؂�ւ�
			m_nNumFieldCamera++;

			if (m_nNumFieldCamera >= 3)
			{//�v���C���[�l�����ő�Ȃ�
			 //�ŏ��̃v���C���[��Ǐ]
				m_nNumFieldCamera = 0;
			}

			//�J�E���g�̃��Z�b�g
			m_nChangeTime = 0;
		}

		//�v���C���[�̏����擾
		playerPos = CGame::GetPlayer(m_nNumFieldCamera)->GetPosition();
		playerRot = CGame::GetPlayer(m_nNumFieldCamera)->GetRot();
	}
	else
	{//����ȊO�Ȃ�
	 //----------------------------
	 // �v���C���[�̏����擾
	 //----------------------------
		playerPos = CGame::GetPlayer(m_nNumPlayer)->GetPosition();
		playerRot = CGame::GetPlayer(m_nNumPlayer)->GetRot();
	}

	//----------------------------
	// �ړI�̒����_��ݒ�
	//----------------------------
	m_posRDest.x = playerPos.x + sinf(playerRot.x) * sinf(playerRot.y) * 50.0f;
	m_posRDest.y = playerPos.y + cosf(playerRot.x) * 50.0f;
	m_posRDest.z = playerPos.z + sinf(playerRot.x) * cosf(playerRot.y) * 50.0f;

	//----------------------------
	// �ړI�̎��_��ݒ�
	//----------------------------
	m_posVDest.x = playerPos.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = playerPos.y - cosf(m_rot.x) * m_fDistance;
	m_posVDest.z = playerPos.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	//----------------------------
	// �����_��ݒ�
	//----------------------------
	m_posR.x += (m_posRDest.x - m_posR.x) * 0.3f;
	m_posR.z += (m_posRDest.z - m_posR.z) * 0.3f;

	//----------------------------
	// ���_��ݒ�
	//----------------------------
	m_posV.x += (m_posVDest.x - m_posV.x) * 0.3f;
	m_posV.z += (m_posVDest.z - m_posV.z) * 0.3f;
}

//==================================================
// �v���C���[�ԍ��̐ݒ�
//==================================================
void CCameraPlayer::SetNumPlayer(int nNum)
{
	m_nNumPlayer = nNum;
}