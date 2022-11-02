//========================================================
// �v���C���[�I����ʂ�UI�̏���
// Author : �y���m��
//========================================================
#include "UI_PSelect.h"

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
	//�T�C�Y�̐ݒ�
	CObject2D::SetSize(50.0f, 50.0f);

	// �I�u�W�F�N�g�̏�����
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	CObject2D::SetTexture(CTexture::TEXTURE_HUMAN);

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
CUI_PSelect* CUI_PSelect::Create(const D3DXVECTOR3 &pos)
{
	CUI_PSelect* pUI_PSelect = nullptr;

	pUI_PSelect = new CUI_PSelect;

	if (pUI_PSelect != nullptr)
	{
		pUI_PSelect->Init(pos);
	}

	return pUI_PSelect;
}
