//===================================================
//
// �Q�[���w�b�_�[
// Author : Sato Teruto
//
//====================================================
#ifndef _GAME_H_
#define _GAME_H_	

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "application.h"

//--------------------------------
// �O���錾
//--------------------------------
class CPolygon;		//�|���S��
class CPolygon2d;	//�|���S��2d
class CPlayer;		//�v���C���[
class CMeshField;	//���b�V���t�B�[���h
class CCamera;		//�J����

//--------------------
// �}�N����`
//--------------------
#define nDefaultMaxCamera	(4)
#define MAX_PLAYER (4)

//================================
// �Q�[���N���X�̒�`
//================================
class CGame
{
public:
	//�J�����̍ő吔
	enum NUMCAMERA
	{
		NUMCAMERA_ONE = 0,
		NUMCAMERA_TWO,
		NUMCAMERA_THREE,
		NUMCAMERA_FOUR,
		NUMCAMERA_MAX
	};

	CGame();	//�R���X�g���N�^
	~CGame();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();
	void SetPlayerPosition(D3DXVECTOR3 pos);
	void ResetCameraSize();	//�J�����̑傫���̃��Z�b�g

	//------------------
	// �Q�b�^�[
	//------------------
	static CPlayer*	GetPlayer(int NumPlayer);			//�v���C���[�̎擾
	static CMeshField* GetMesh() { return pMeshField; }	//���b�V���t�B�[���h�̎擾
	static CCamera	 *GetCamera(int nCnt);				//�J�����̎擾

	//�v���C���[�l���̎擾
	static int GetAmount() { return nDefaultMaxCamera; }

private:
	void CreateCamera(CGame::NUMCAMERA num);	//�J�����̐���
	void FinishGame();	//�Q�[���I�����̏���

private:
	//------------------
	// �萔
	//------------------
	static const int nSpeed_X = 9;
	static const int nSpeed_Y = 5;

	//-------------------
	// �ÓI�����o�ϐ�
	//-------------------
	static CPolygon* pPolygon;
	static CPolygon2d* pPolygon2d;
	static CPlayer*	 pPlayer[MAX_PLAYER];
	static CMeshField* pMeshField;
	static CCamera* m_pCamera[nDefaultMaxCamera];	//�J�����N���X

	static int m_nNumCamera;	//�J�����̗񋓌^�ԍ�
	static int m_nMaxCamera;	//�J�����̐�
	static bool m_bStop;		//�v���O�������~����
};

#endif