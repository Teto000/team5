//===================================================
//
// ���f���w�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _MODEL_H_
#define _MODEL_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "object.h"

//================================
// ���f���N���X�̒�`
//================================
class CModel
{
public:
	CModel();	//�R���X�g���N�^
	~CModel();	//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	//----------------
	// �Z�b�^�[
	//----------------
	void SetPos(D3DXVECTOR3 pos);		//�ʒu�̐ݒ�
	void SetRot(D3DXVECTOR3 rot);		//�����̐ݒ�
	void SetParent(CModel* pModel);		//�e���f���̐ݒ�

	//----------------
	// �Q�b�^�[
	//----------------
	D3DXVECTOR3 GetPos();		//�ʒu�̎擾
	D3DXVECTOR3 GetRot();		//�����̎擾
	D3DXMATRIX GetmtxWorld();	//���[���h�}�g���b�N�X�̎擾

	//----------------
	// �ÓI�����o�֐�
	//----------------
	static CModel* Create(LPCTSTR text,D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	void DrawShadow();

private:
	//----------------
	// �����o�ϐ�
	//----------------
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxParent;		//�e�̃}�g���b�N�X
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_rot;			//����
	CModel* m_pModel;			//�e���f���ւ̃|�C���^
	LPCTSTR m_modelName;		//���f���̃p�X

	/* �� ���f����� �� */
	LPD3DXMESH m_pMesh;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;	//�}�e���A�����ւ̃|�C���^
	DWORD m_nNumMat;			//�}�e���A�����̐�
};

#endif