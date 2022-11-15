//==================================================
//
// �v���C���[�J�����̏���
// Author:Teruto Sato
//
//==================================================

//------------------------------
// �C���N���[�h
//------------------------------
#include "camera_player.h"
#include "camera.h"
#include "input.h"
#include "input_keybord.h"
#include "application.h"
#include "game.h"
#include "player.h"
#include "Goal.h"
#include "joypad.h"

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

	//�J�����̏�����]�ʂ̐ݒ�
	m_rot.y = D3DX_PI;

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

	//------------------------------
	// �v���C���[���Ƃ̏���
	//------------------------------
	EachPlayer();

	//------------------------------
	// �J�����̒Ǐ]
	//------------------------------
	Following();

	//------------------------------
	// �t�B�[���h�J�����̐���
	//------------------------------
	if (CApplication::GetGame()->GetEnumCamera() == CGame::NUMCAMERA_THREE
		&& m_nNumPlayer == -1)
	{//�J�����񋓌^��three ���� �Ή�����v���C���[�ԍ���0�����Ȃ�
		//�J�����̉�]
		m_rot.y -= fTurnSpeed / 16;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}

	//------------------------------
	// �Q�[���I�����̏���
	//------------------------------
	if (CGame::GetFinish() && m_nNumPlayer == CGoal::GetWinner())
	{//�I���t���O�������Ă��� & �Ή�����v���C���[�ԍ������҂ƈꏏ�Ȃ�
		//�J�����̉�]
		m_rot.y -= fTurnSpeed / 16;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
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

	//------------------------------
	// �|���S���̐����Ə�����
	//------------------------------
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
	//------------------------------
	// �v���C���[1
	//------------------------------
	case NUMPLAYER_ONE:
		Turn(DIK_E, DIK_Q);	//����
		break;

	//------------------------------
	// �v���C���[2
	//------------------------------
	case NUMPLAYER_TWO:
		Turn(DIK_Y, DIK_R);	//����
		break;

	//------------------------------
	// �v���C���[3
	//------------------------------
	case NUMPLAYER_THREE:
		Turn(DIK_O, DIK_U);	//����
		break;

	//------------------------------
	// �v���C���[4
	//------------------------------
	case NUMPLAYER_FOUR:
		Turn(DIK_RIGHT, DIK_LEFT);	//����
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
	//------------------------------
	// ����]
	//-------------------
	// �W���C�p�b�h�ł̑���
	CJoypad *pJoypad = CApplication::GetJoyPad();

	if (pJoypad->Stick(CJoypad::JOYKEY_RIGHT_STICK, m_nNumPlayer, 0.5f))
	{
		m_rot.y += pJoypad->GetStickAngle(CJoypad::JOYKEY_RIGHT_STICK, m_nNumPlayer) / 180.0f;
		m_posR.x = m_posV.x + POLOR_X;	//x�̋���
		m_posR.y = m_posV.y + POLOR_Y;	//y�̋���
		m_posR.z = m_posV.z + POLOR_Z;	//z�̋���
	}

	if (CInputKeyboard::Press(nLeftKey))
	{//�L�[�������ꂽ
		m_rot.y += fTurnSpeed;			//��]�ʂ̑���
		m_posR.x = m_posV.x + POLOR_X;	//x�̋���
		m_posR.y = m_posV.y + POLOR_Y;	//y�̋���
		m_posR.z = m_posV.z + POLOR_Z;	//z�̋���
	}
	//------------------------------
	// �E��]
	//------------------------------
	else if (CInputKeyboard::Press(nRightKey))	//�E��]
	{//�L�[�������ꂽ
		m_rot.y -= fTurnSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
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

	//------------------------------
	// �t�B�[���h�J�����̐ݒ�
	//------------------------------
	if (CApplication::GetGame()->GetEnumCamera() == CGame::NUMCAMERA_THREE
		&& m_nNumPlayer == -1)
	{//�J�����񋓌^��three ���� �Ή�����v���C���[�ԍ���0�����Ȃ�

		//�؂�ւ����Ԃ̉��Z
		m_nChangeTime++;

		if (m_nChangeTime % 360 == 0)
		{//��莞�Ԃ��o�߂�����
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
	 //------------------------------
	 // �v���C���[�̏����擾
	 //------------------------------
		playerPos = CGame::GetPlayer(m_nNumPlayer)->GetPosition();
		playerRot = CGame::GetPlayer(m_nNumPlayer)->GetRot();
	}

	//------------------------------
	// �ړI�̒����_��ݒ�
	//------------------------------
	m_posRDest.x = playerPos.x + sinf(playerRot.x) * sinf(playerRot.y) * 50.0f;
	m_posRDest.y = playerPos.y + cosf(playerRot.x) * 50.0f;
	m_posRDest.z = playerPos.z + sinf(playerRot.x) * cosf(playerRot.y) * 50.0f;

	//------------------------------
	// �ړI�̎��_��ݒ�
	//------------------------------
	m_posVDest.x = playerPos.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = playerPos.y - cosf(m_rot.x) * m_fDistance;
	m_posVDest.z = playerPos.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	//------------------------------
	// �����_��ݒ�
	//------------------------------
	m_posR.x += (m_posRDest.x - m_posR.x) * 0.3f;
	m_posR.z += (m_posRDest.z - m_posR.z) * 0.3f;

	//------------------------------
	// ���_��ݒ�
	//------------------------------
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