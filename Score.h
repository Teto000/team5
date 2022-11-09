//=========================================
//
// �X�R�A����
// Auther�F�y���m��
//
//=========================================
#ifndef _SCORE_H_		// ���̃}�N����`������ĂȂ�������
#define _SCORE_H_		// ��d�C���N�\���[�h�h�~�̃}�N����`

#include "object2D.h"
#include "number.h"

#define MAX_DIGITS		(8)			// �X�R�A�̌���
#define MAX_RANK		(5)			//�\�����郉���L���O�̍ő吔

//=========================================
// �X�R�A�N���X���`
//=========================================
class CScore :public CObject2D
{
public:
	CScore();							//�R���X�g���N�^
	virtual ~CScore() override;			//�f�X�g���N�^

										//����������
	HRESULT Init(D3DXVECTOR3 pos) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;
	//�I������
	void Uninit(void) override;
	//�N���G�C�g����
	static CScore* Create(const D3DXVECTOR3 &pos);

	//�X�R�A�̐ݒ�
	void Set();
	static void SetCurrentScore(int Time);
	static int GetTime() { return m_Time; }
	static int GetSec()	 { return m_Seconds; }
	static int GetMin()	 { return m_Minutes; }

private:
	D3DXVECTOR3 m_pos;					// ���W
	CNumber	*m_pNumber[MAX_DIGITS];		// �i���o�[1
	static int m_Time;					// ����
	static int m_Seconds;				// ���݂̎���(�b)
	static int m_Minutes;				// ���݂̎���(��)
	int m_Score[MAX_RANK];				// �X�R�A�̍ő吔
};

#endif
