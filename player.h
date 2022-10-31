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

//--------------------------------
// �O���錾
//--------------------------------
class CShadow;	//�e
class CBullet;	//�e
class CModel;	//���f��

//--------------------------------
// �}�N����`
//--------------------------------
#define MAX_PARTS	(7)		//���f���p�[�c�̍ő吔
#define MAX_KEY		(2)		//�L�[�̍ő吔
#define MAX_SPEED	(3.0f)	//�ő呬�x

//================================
// �v���C���[�N���X�̒�`
//================================
class CPlayer : public CObject
{
public:
	//�L�[�v�f
	struct KEY
	{
		float fPosX;	//�ʒu
		float fPosY;
		float fPosZ;
		float fRotX;	//����
		float fRotY;
		float fRotZ;
	};

	//�L�[���
	struct KEY_SET
	{
		int nFrame;	//�t���[����
		KEY aKey[MAX_PARTS];
	};

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
	void SetModel();			//���f���̐ݒ�
	void SetMotion(bool bLoop);	//���[�V�����̐ݒ�
	void Move();				//�ړ�
	void Jump();				//�W�����v
	void P1MoveKey();			//�v���C���[1�̃L�[����ݒ�
	void P2MoveKey();			//�v���C���[2�̃L�[����ݒ�

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
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posold;		//1F�O�̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�ړI�̌���
	STATE		m_state;		//�v���C���[�̏��
	bool		m_bJump;		//�W�����v���������Ă��Ȃ���

	/* �� ���[�V������� �� */
	int m_nCurrentKey;			//���݂̃L�[�ԍ�
	int m_nCntMotion;			//���[�V�����J�E���^�[
	int m_nJumpCount;			//�W�����v�J�E���g

	/* �� �v���C���[�Ǘ���� */
	int m_nPlayerNum;			//�v���C���[�̑���ԍ�

	/* �� �N���X��� �� */
	static CShadow* m_pShadow;	//�e
	static CBullet* m_pBullet;	//�e
	static CModel*  m_pModel[MAX_PARTS];	//���f��
};

#endif