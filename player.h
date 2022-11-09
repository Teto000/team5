//===================================================
//
// �v���C���[�w�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "object.h"

#define MAX_BLOCK	(1000)	//�u���b�N�̍ő吔
#define MAX_SPEED	(3.0f)	//�ő呬�x

//================================
// �v���C���[�N���X�̒�`
//================================
class CBlock;
class CPlayer : public CObject
{
public:

	//�v���C���[�̏��
	enum STATE
	{
		IDOL_STATE = 0,
		JUMP_STATE,
		MAX_STATE
	};

	CPlayer();				//�R���X�g���N�^
	~CPlayer() override;	//�f�X�g���N�^

	//----------------
	// �����o�֐�
	//----------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//----------------
	// �Z�b�^�[
	//----------------
	void SetPosition(D3DXVECTOR3 pos);
	void SetGravity() { m_move.y = 0; }

	//----------------
	// �Q�b�^�[
	//----------------
	D3DXVECTOR3 GetPosition() override;	//�ʒu�̎擾
	D3DXVECTOR3 GetRot();				//�����̎擾
	float GetWidth() override;			//���̎擾
	float GetHeight() override;			//�����̎擾
	D3DXMATRIX GetmtxWorld();			//���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetPosOld();			//1F�O�̈ʒu�̏K��

	//----------------
	// �ÓI�����o�֐�
	//----------------
	static CPlayer* Create(int PlayerNum);

private:
	void Move();				//�ړ�
	void Jump();				//�W�����v
	void SetBlock();			//�u���b�N�̐ݒu

	// �ړ��L�[
	// ���� : ��̃L�[�A���̃L�[�A���̃L�[�A�E�̃L�[
	void MoveKey(int UPKey,int LEFTKey,int DOWNKey,int RIGHTKey,int JUMPKey);

private:
	//----------------
	// �萔
	//----------------
	static const float fPlayerSpeed;	//�v���C���[�̑��x
	static const float fGravity;		//�d��

	//----------------
	// �����o�ϐ�
	//----------------
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posold;		//1F�O�̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�ړI�̌���
	STATE		m_state;		//�v���C���[�̏��
	bool		m_bJump;		//�W�����v���������Ă��Ȃ���

	CBlock		*m_pModel[MAX_BLOCK];		//�u���b�N
	int			m_BlockCnt;					//�u���b�N���J�E���g���鏈��
	int			m_BlockHave;				//�u���b�N���������Ă����

	int m_nJumpCount;
	/* �� �v���C���[�Ǘ���� */
	int m_nPlayerNum;			//�v���C���[�̑���ԍ�

	/* ���@�g���Ă郂�[�V�������f���ԍ��@*/
	int m_nMotionNum;
};

#endif