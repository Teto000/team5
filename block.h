//===================================================
//
// �u���b�N�̏���
//	Auhotr : C�EGen�EGow
//
//===================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_	

//-------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "application.h"
#include "objectX.h"

//--------------------------------
// �O���錾
//--------------------------------
class CShadow;	//�e

//================================
// �S�[���N���X�̒�`
//================================
class CBlock : public CObject
{
public:
	explicit CBlock(int nPriority);	//�R���X�g���N�^
	~CBlock();	//�f�X�g���N�^

//------------------
// �����o�֐�
//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit();
	void Update();
	void Draw();
	bool Collision();
	static CBlock * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	//�Z�b�^�[
	void SetAbove() { m_bAbove = true; }
	void SetPosition(D3DXVECTOR3 pos);

	//�Q�b�^�[
	static bool GetBlockCollision() { return m_bAbove; }
	D3DXVECTOR3 GetPosition()override;
	float GetWidth()override;
	float GetHeight()override;

private:
	//------------------
	// �����o�ϐ�
	//------------------
	static bool m_bAbove;
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posold;		//1F�O�̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�ړI�̌���
	LPCTSTR		m_modelName;	//���f���̃p�X

	/* �� �N���X��� �� */
	LPD3DXMESH m_pMesh;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;	//�}�e���A�����ւ̃|�C���^
	DWORD m_nNumMat;			//�}�e���A�����̐�
	D3DXVECTOR3 m_vtxMax;			//���f���̍ő���W
	D3DXVECTOR3 m_vtxMin;			//���f���̍��W�̍ŏ��l

};

#endif
