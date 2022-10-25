//===================================================
//
// �t�F�[�h�̏���
// Author : Sato Teruto
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "fade.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "application.h"
#include "renderer.h"
#include "object2d.h"

//===========================
// �R���X�g���N�^
//===========================
CFade::CFade()
{

}

//===========================
// �f�X�g���N�^
//===========================
CFade::~CFade()
{

}

//===========================
// ������
//===========================
HRESULT CFade::Init(CApplication::MODE modeNext)
{
	//�񋓌^�̏�����
	m_fade = FADE_IN;		//�t�F�[�h�C�����
	m_modeNext = modeNext;	//���̉��(���[�h)��ݒ�
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//�����|���S��(�s����)�ɂ��Ă���

	CObject2D::Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	//CObject2D::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	//CObject2D::SetColor(m_col);

	//CObject2D::SetTexture(CTexture::TEXTURE_NONE);	//�e�N�X�`���̐ݒ�

	return S_OK;
}

//===========================
// �I��
//===========================
void CFade::Uninit()
{
	CObject2D::Uninit();
}

//===========================
// �X�V
//===========================
void CFade::Update()
{
	CObject2D::Update();

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{//�t�F�[�h�C�����
			m_col.a -= 0.02f;	//�|���S���𓧖��ɂ��Ă���

			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
				m_fade = FADE_NONE;	//�������Ă��Ȃ���Ԃ�
			}
		}
		else if (m_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���
			m_col.a += 0.02f;	//�|���S����s�����ɂ��Ă���

			if (m_col.a >= 1.0f)
			{
				m_col.a = 1.0f;
				m_fade = FADE_IN;	//�t�F�[�h�C����Ԃ�

				CApplication::SetMode(m_modeNext);
			}
		}
		//CObject2D::SetColor(m_col);
	}
}

//===========================
// �`��
//===========================
void CFade::Draw()
{
	CObject2D::Draw();
}

//===========================
// �ݒ�
//===========================
void CFade::SetFade(CApplication::MODE modeNext)
{
	m_fade = FADE_OUT;		//�t�F�[�h�A�E�g��Ԃ�
	m_modeNext = modeNext;	//���̉�ʂ�ݒ�
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//===========================
// �擾
//===========================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}