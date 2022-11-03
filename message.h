//===================================================
//
// ���b�Z�[�W�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _POLYGON2D_H_
#define _POLYGON2D_H_	//��d�C���N���[�h�h�~

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "object2d.h"

//================================
// ���b�Z�[�W�N���X
//================================
class CMessage : public CObject2D
{
public:
	enum MESSAGE
	{
		MESSAGE_COUNT_ONE = 0,
		MESSAGE_COUNT_TWO,
		MESSAGE_COUNT_THREE,
		MESSAGE_START,
		MESSAGE_FINISH,
		MESSAGE_MAX
	};

	CMessage();		//�R���X�g���N�^
	~CMessage();	//�f�X�g���N�^

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
	static CMessage* Create(D3DXVECTOR3 pos);

private:
	void SetTexture();		//�e�N�X�`���̐ݒ�
	void ChangeMessage();	//���b�Z�[�W�̕ύX

private:
	//------------------
	// �����o�ϐ�
	//------------------
	D3DXCOLOR m_col;	//�F
	int m_nCntTime;		//���ԃJ�E���g
	float m_fWidth;		//��
	float m_fHeight;	//����
	MESSAGE m_message;	//���b�Z�[�W
};

#endif