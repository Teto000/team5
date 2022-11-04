//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include <d3dx9.h>

//==================================================
// ��`
//==================================================
class CTexture
{
public: /* ��` */
	enum TEXTURE
	{
		TEXTURE_GROUND = 0,		//�n��
		TEXTURE_SHADOW,			//�e
		TEXTURE_BULLET,			//�e
		TEXTURE_NUMBER,			//�����e�N�X�`��
		TEXTURE_TITLELOGO,		//�^�C�g�����S
		TEXTURE_TITLELOGO2,		//�^�C�g�����S
		TEXTURE_RANKING,		//�����L���O
		TEXTURE_HUMAN,			//�v���C���[

		/* �� ���b�Z�[�W �� */
		TEXTURE_COUNT_THREE,	//�J�E���g3
		TEXTURE_COUNT_TWO,		//�J�E���g2
		TEXTURE_COUNT_ONE,		//�J�E���g1
		TEXTURE_START,			//�X�^�[�g
		TEXTURE_FINISH,			//�t�B�j�b�V��

		TEXTURE_SELECT_COMENT,	//�Z���N�g��ʂ̉��
		TEXTURE_SELECT_BG,		//�Z���N�g��ʂ̔w�i
		TEXTURE_SELECT_SELECT,	//�Z���N�g��ʂ̃Z���N�g�̐�����
		TEXTURE_MAX,
		TEXTURE_NONE,			//�g�p���Ȃ�
	};

	static const char* s_FileName[];	// �t�@�C���p�X

public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public: /* �����o�֐� */
	void LoadAll();										// �S�Ă̓ǂݍ���
	void Load(TEXTURE inTexture);						// �w��̓ǂݍ���
	void ReleaseAll();									// �S�Ă̔j��
	void Release(TEXTURE inTexture);					// �w��̔j��
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// ���̎擾

private: /* �����o�ϐ� */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
