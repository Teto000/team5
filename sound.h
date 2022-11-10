//===================================================
//
// �T�E���h�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _SOUND_H_
#define _SOUND_H_	

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include <xaudio2.h>

//================================
// �T�E���h�N���X�̒�`
//================================
class CSound
{
public:
	//*****************************************************************************
	// �p�����[�^�\���̒�`
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	//*****************************************************************************
	// �T�E���h�t�@�C��
	//*****************************************************************************
	typedef enum
	{
		/*�� BGM ��*/
		SOUND_LABEL_GAME = 0,				//�Q�[��
		SOUND_LABEL_TITLE,					//�^�C�g��
		SOUND_LABEL_RESULT,					//���U���g
		SOUND_LABEL_SELECT,					//�Z���N�g
		SOUND_LABEL_SE_BLOCK,				//�u���b�N�ݒu��
		SOUND_LABEL_SE_JUMP1,				//�W�����v���P
		SOUND_LABEL_SE_JUMP2,				//�W�����v���Q
		SOUND_LABEL_SE_FALL1,				//�������P
		SOUND_LABEL_SE_FALL2,				//�������Q
		SOUND_LABEL_SE_UP,					//�W�����v�I�u�W�F�N�g�p��
		SOUND_LABEL_SE_HIT1,				//�Ռ����y�P
		SOUND_LABEL_SE_HIT2,				//�Ռ����y�Q
		SOUND_LABEL_SE_HIT3,				//�Ռ����d
		SOUND_LABEL_SE_GAME_START,			//�X�^�[�g����
		SOUND_LABEL_SE_START_VOICE,			//�X�^�[�g����
		SOUND_LABEL_SE_START_LIGHT,			//�J�E���g�_�E���ቹ
		SOUND_LABEL_SE_START_HEAVY,			//�J�E���g�_�E������
		SOUND_LABEL_SE_SELECT,				//�I����
		SOUND_LABEL_SE_SELECT_DECISION,		//�I�����艹
		SOUND_LABEL_SE_FINISH,				//�S�[������
		SOUND_LABEL_SE_1PWIN,				//�P�o��������
		SOUND_LABEL_SE_2PWIN,				//�Q�o��������
		SOUND_LABEL_SE_3PWIN,				//�R�o��������
		SOUND_LABEL_SE_4PWIN,				//�S�o��������
		SOUND_LABEL_MAX,					//�ő吔
	} SOUND_LABEL;

	CSound();	//�R���X�g���N�^
	~CSound();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(HWND hWnd);
	void Uninit(void);

	static HRESULT PlaySound(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);

	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
};

#endif // !_SOUND_H_