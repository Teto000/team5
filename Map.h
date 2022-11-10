//===================================================
//
// �}�b�v
// Author : Arita Meigen
//
//===================================================
#ifndef _MAP_H_
#define _MAP_H_	

//-------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "application.h"
#include "model.h"
#include "object.h"

//================================
// �t�F�[�h�N���X�̒�`
//================================
class CMap :CObject
{
public:
	explicit CMap(int nPriority);	//�R���X�g���N�^
	~CMap();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(D3DXVECTOR3 pos)override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CMap * Create(D3DXVECTOR3 pos);

	//�Z�b�^�[
	void SetPosition(D3DXVECTOR3 pos);

	//�Q�b�^�[
	D3DXVECTOR3 GetPosition();
	float GetWidth()override;
	float GetHeight()override;

private:
	//----------------
	// �����o�ϐ�
	//----------------
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_rot;			//����
	CModel* m_pModel;			//�e���f���ւ̃|�C���^
	LPCTSTR m_modelName;		//���f���̃p�X

	/* �� ���f����� �� */
	LPD3DXMESH m_pMesh;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;	//�}�e���A�����ւ̃|�C���^
	DWORD m_nNumMat;			//�}�e���A�����̐�
	D3DXVECTOR3 vtxMax;			//���f���̍ő���W
	D3DXVECTOR3 vtxMin;			//���f���̍��W�̍ŏ��l

};

#endif

