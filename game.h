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
class CCameraPlayer;//�v���C���[�J����
class CMessage;		//���b�Z�[�W
class CEditor;		//�G�f�B�^
class CDebugProc;	//�f�o�b�O�p����
class CMap;			//�}�b�v

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
	//�Q�[���̏��
	enum GAMEMODE
	{
		GAMEMODE_NORMAL = 0,	//�ʏ�
		GAMEMODE_START,			//�J�n
		GAMEMODE_FINISH,		//�I��
		GAMEMODE_PAUSE,			//�ꎞ��~
		GAMEMODE_MAX
	};

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
	static int GetEnumCamera();	//�J�����񋓌^�ԍ����擾

	static CPlayer*			GetPlayer(int NumPlayer);			//�v���C���[�̎擾
	static CMeshField*		GetMesh() { return pMeshField; }	//���b�V���t�B�[���h�̎擾
	static CCameraPlayer*	GetCameraPlayer(int nCnt);				//�J�����̎擾
	static bool				GetFinish();	//�I���t���O�̎擾
	static int GetGroundNum() { return m_nGroundNum; }
	
	//�v���C���[�l���̐ݒ�
	static void SetPlayerNum(int nPlayer) { m_player = nPlayer; }

private:
	void CreateCamera(CGame::NUMCAMERA num);	//�J�����̐���
	void FinishGame();							//�Q�[���I�����̏���

private:
	//------------------
	// �萔
	//------------------
	static const int nSpeed_X = 9;
	static const int nSpeed_Y = 5;

	//-------------------
	// �����o�ϐ�
	//-------------------
	static int m_nEnumCamera;	//�J�����̗񋓌^�̒l
	int m_nMaxCamera;	//�J�����̐�
	bool m_bStop;		//�v���O�������~����
	GAMEMODE mode;
	

	//-------------------
	// �ÓI�����o�ϐ�
	//-------------------
	static CPolygon*	pPolygon;
	static CPolygon2d*	pPolygon2d;
	static CPlayer*		pPlayer[MAX_PLAYER];
	static CMeshField*	pMeshField;
	static CCameraPlayer*	m_pCameraPlayer[nDefaultMaxCamera];	//�v���C���[�J�����N���X
	static CMessage*	m_pMessage;
	static CEditor*		m_pEditor;
	static CDebugProc*	m_pProc;
	static CMap*		m_pMap;

	static bool m_bFinish;		//�I���t���O
	static int  m_player;		//�v���C���[�̐�
	static int m_nGroundNum;
};

#endif