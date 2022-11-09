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
	static CRank* Create(D3DXVECTOR3 pos);

private:
	//------------------
	// �����o�ϐ�
	//------------------
	D3DXVECTOR3	m_pos;		//�ʒu
	int			m_nRank;	//����
	int m_aPosTexU;			//���̌��̐��l
};

#endif