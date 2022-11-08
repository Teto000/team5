//========================================================
// �v���C���[�I����ʂ�UI�̏���
// Author : �y���m��
//========================================================
#include "UI_PSelect.h"
#include "renderer.h"

//=========================================
//�R���X�g���N�^
//=========================================
CUI_PSelect::CUI_PSelect()
{
}

//=========================================
//�f�X�g���N�^
//=========================================
CUI_PSelect::~CUI_PSelect()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CUI_PSelect::Init(D3DXVECTOR3 pos)
{
	switch (m_Tui)
	{
	case UI_PLAYER:
		//�T�C�Y�̐ݒ�
		CObject2D::SetSize(100.0f, 100.0f);
		// �e�N�X�`���̐ݒ�
		CObject2D::SetTexture(CTexture::TEXTURE_HUMAN);
		break;

	case UI_COMENT:
		//�T�C�Y�̐ݒ�
		CObject2D::SetSize(500.0f, 200.0f);
		// �e�N�X�`���̐ݒ�
		CObject2D::SetTexture(CTexture::TEXTURE_SELECT_COMENT);
		break;

	case UI_BG:
		//�T�C�Y�̐ݒ�
		CObject2D::SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
		// �e�N�X�`���̐ݒ�
		CObject2D::SetTexture(CTexture::TEXTURE_SELECT_BG);
		break;

	case UI_SELECT:
		//�T�C�Y�̐ݒ�
		CObject2D::SetSize(400.0f, 50.0f);
		// �e�N�X�`���̐ݒ�
		// �e�N�X�`���̎w���΂����邯�ǋC�ɂ��Ȃ���
		CObject2D::SetTexture(CTexture::TEXTURE_SELECT_SELECT);
		break;
	}

	// �I�u�W�F�N�g�̏�����
	CObject2D::Init(pos);

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CUI_PSelect::Update()
{
	D3DXVECTOR3 pos;
	pos = GetPosition();
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CUI_PSelect::Uninit()
{
	CObject2D::Uninit();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CUI_PSelect::Draw()
{
	CObject2D::Draw();
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CUI_PSelect* CUI_PSelect::Create(const D3DXVECTOR3 &pos,const UI &texture)
{
	CUI_PSelect* pUI_PSelect = nullptr;

	pUI_PSelect = new CUI_PSelect;

	if (pUI_PSelect != nullptr)
	{
		pUI_PSelect->Set(texture);
		pUI_PSelect->Init(pos);
	}

	return pUI_PSelect;
}
