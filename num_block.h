//===================================================
//
// �u���b�N���w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _NUM_BLOCK_H_
#define _NUM_BLOCK_H_	//��d�C���N���[�h�h�~

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "object2d.h"

//-------------------------------
// �O���錾
//-------------------------------
class CNumber;	//���l

//================================
// �u���b�N���N���X
//================================
class CNumBlock : public CObject2D
{
public:
	CNumBlock();		//�R���X�g���N�^
	~CNumBlock();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	int AddNumber(int nNumber);	//���l�̉��Z

	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static CNumBlock* Create(D3DXVECTOR3 pos);

private:
	void SetNumber();	//���l�̐ݒ�

private:
	//------------------
	// �萔
	//------------------
	static const int nMaxDigits = 2;	//�ő包��

	//------------------
	// �����o�ϐ�
	//------------------
	D3DXVECTOR3	m_pos;					//�ʒu
	int			m_nNumBlock;			//�u���b�N�̐�
	int m_aPosTexU[nMaxDigits];			//���̌��̐��l
	CNumber*	m_pNumber[nMaxDigits];	//���l
};

#endif