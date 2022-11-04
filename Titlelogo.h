//===================================================
//
// �^�C�g�����S
// Author : Arita Meigen
//
//===================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_	//��d�C���N���[�h�h�~

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "object2d.h"

//================================
// ���b�Z�[�W�N���X
//================================
class CTitlelogo : public CObject2D
{
public:

	CTitlelogo();		//�R���X�g���N�^
	~CTitlelogo();		//�f�X�g���N�^

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
	static CTitlelogo* Create(D3DXVECTOR3 pos, CTexture::TEXTURE tex);

private:
	//------------------
	// �����o�ϐ�
	//------------------
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXCOLOR m_col;	//�F
	float m_fWidth;		//��
	float m_fHeight;	//����
	
};

#endif