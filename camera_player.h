//===================================================
//
// �v���C���[�J�����w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _CAMERA_PLAYER_H_	//���̃}�N������`����ĂȂ�������
#define _CAMERA_PLAYER_H_	//2�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "camera.h"

//================================
// �v���C���[�J�����N���X�̒�`
//================================
class CCameraPlayer : public CCamera
{
public:
	//------------------------------
	// �v���C���[�ԍ��̗񋓌^
	//------------------------------
	enum NUMPLAYER
	{
		NUMPLAYER_ONE = 0,
		NUMPLAYER_TWO,
		NUMPLAYER_THREE,
		NUMPLAYER_FOUR,
		NUMPLAYER_MAX
	};

	CCameraPlayer();			//�R���X�g���N�^
	~CCameraPlayer() override;	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void SetCamera(LPDIRECT3DDEVICE9 pDevice) override;

	//------------------
	// �ÓI�����o�֐�
	//------------------
	static CCameraPlayer* Create(DWORD X, DWORD Y, DWORD Width, DWORD Height);

	//------------------
	// �Z�b�^�[
	//------------------
	void SetNumPlayer(int nNum);	//�v���C���[�ԍ��̐ݒ�

private:
	void EachPlayer();	//�v���C���[���Ƃ̏���
	void Turn(int nLeftKey, int nRightKey);	//����
	void Move(int nUpKey, int nDownKey, int nLeftKey, int nRightKey);	//�ړ�
	void Following();				//�Ǐ]

private:
	//------------------
	// �����o�ϐ�
	//------------------
	int m_nNumPlayer;			//�v���C���[�ԍ�
	int m_nNumFieldCamera;		//�t�B�[���h�J�����̃v���C���[�ԍ�
	int m_nChangeTime;			//�t�B�[���h�J�����؂�ւ�����
};

#endif