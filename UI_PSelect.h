//=========================================
//
// �v���C���[�I����ʂ�UI.h
// Auther�F�y���m��
//
//=========================================
#ifndef _UI_PSELECT_H_		// ���̃}�N����`������ĂȂ�������
#define _UI_PSELECT_H_		// ��d�C���N�\���[�h�h�~�̃}�N����`

#include "object2D.h"

//=========================================
// �X�R�A�N���X���`
//=========================================
class CUI_PSelect :public CObject2D
{
public:
	//�t�F�[�h�̏��
	enum UI
	{
		UI_NONE = 0,	//�����Ȃ�
		UI_PLAYER,		//�v���C���[UI
		UI_MAX
	};

	CUI_PSelect();			//�R���X�g���N�^
	virtual ~CUI_PSelect() override;			//�f�X�g���N�^

	//����������
	HRESULT Init(D3DXVECTOR3 pos) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;
	//�I������
	void Uninit(void) override;

	//�N���G�C�g����
	static CUI_PSelect* Create(const D3DXVECTOR3 &pos);

private:
	int m_texture;
};

#endif