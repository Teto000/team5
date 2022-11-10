//===================================================
//
// �v���C���[�w�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _GIMMICK_H_
#define _GIMMICK_H_	//��d�C���N���[�h�h�~

#define MAX_BLOCK (1000)
//--------------------------------
// �C���N���[�h
//--------------------------------
#include "object.h"

//--------------------------------
// �O���錾
//--------------------------------
class CNumBlock;	//�u���b�N���̕\��
class CRank;		//�����N

//--------------------------------
// �}�N����`
//--------------------------------
#define MAX_SPEED	(3.0f)	//�ő呬�x

//================================
// �v���C���[�N���X�̒�`
//================================
class CGimmick : public CObject
{
public:
	CGimmick();				//�R���X�g���N�^
	~CGimmick() override;	//�f�X�g���N�^

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
	static CGimmick* Create(char* FileName,D3DXVECTOR3 pos);

private:
	void Move();				//�ړ�

private:
	//----------------
	// �萔
	//----------------
	static const float fPlayerSpeed;	//�v���C���[�̑��x
	static const float fGravity;		//�d��

	//----------------
	// �����o�ϐ�
	//----------------
	D3DXVECTOR3 m_pos;						//�ʒu
	D3DXVECTOR3 m_posold;					//1F�O�̈ʒu
	D3DXVECTOR3 m_move;						//�ړ���
	D3DXVECTOR3 m_rot;						//����
	D3DXVECTOR3 m_rotDest;					//�ړI�̌���
	char*		m_modelName;				//���f���̃p�X

	/* �� �v���C���[�Ǘ���� */
	int m_nPlayerNum;			//�v���C���[�̑���ԍ�

	/* ���@�g���Ă郂�[�V�������f���ԍ��@*/
	int m_nMotionNum;
};

#endif
