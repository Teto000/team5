//===================================================
//
// �r���{�[�h�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "object.h"
#include "texture.h"

//================================
// �I�u�W�F�N�g3D�N���X�̒�`
//================================
class CBillBoard : public CObject
{
public:
	CBillBoard();
	explicit CBillBoard(int nPriority);	//�R���X�g���N�^
	virtual ~CBillBoard() override;		//�f�X�g���N�^

	//----------------
	// �����o�֐�
	//----------------
	virtual HRESULT Init(D3DXVECTOR3 pos) override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//----------------
	// �Z�b�^�[
	//----------------
	void SetPos(D3DXVECTOR3 pos);		//�ʒu�̐ݒ�
	D3DXVECTOR3 AddMove(D3DXVECTOR3 move);		//�ړ��ʂ̉��Z
	void SetSize(float fWidth, float fHeight);	//�傫���̐ݒ�

	/* ���e�N�X�`���� */
	void SetTexCIE(float left, float fRight);	//�e�N�X�`�����W�̐ݒ�
	void SetTexture(CTexture::TEXTURE texture);	//�e�N�X�`���̐ݒ�

	//----------------
	// �Q�b�^�[
	//----------------
	D3DXVECTOR3 GetPosition() override;	//�ʒu�̎擾
	float GetWidth() override;			//���̎擾
	float GetHeight() override;			//�����̎擾

private:
	//----------------
	// �����o�ϐ�
	//----------------
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_move;	//�ړ���
	D3DXVECTOR3 m_rot;	//����
	float m_fWidth;		//��
	float m_fHeight;	//����
	float m_fRot;		//��]�p�x
	float m_fScal;		//�傫��
	D3DXMATRIX m_mtxWorld;//���[���h�}�g���b�N�X

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	CTexture::TEXTURE m_texture;		//�e�N�X�`��
};

#endif // !_OBJECT_H_