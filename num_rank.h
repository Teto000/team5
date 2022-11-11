//===================================================
//
// ���ʃw�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _NUM_RANK_H_
#define _NUM_RANK_H_	//��d�C���N���[�h�h�~

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "object2d.h"
#include "game.h"

//================================
// �u���b�N���N���X
//================================
class CRank : public CObject2D
{
public:
	CRank();	//�R���X�g���N�^
	~CRank();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static CRank* Create(D3DXVECTOR3 pos, int nNumPlayer);

private:
	void SetTexture(int nNumPlayer);	//�e�N�X�`���̐ݒ�

private:
	//------------------
	// �����o�ϐ�
	//------------------
	D3DXVECTOR3	m_pos;					//�ʒu
	int			m_aPosTexU;				//���̌��̐��l
	int			m_nNumPlayer;			//�Ή�����v���C���[�ԍ�

	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static int	m_nRank[MAX_PLAYER];	//����
	static int	m_nDistance[MAX_PLAYER];//����
};

#endif