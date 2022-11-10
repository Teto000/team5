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
	void SetTime(int Time);
	int GetTime();
	int GetSec();
	int GetMin(); 
	void Load();
	//void Ranking();

private:
	D3DXVECTOR3 m_pos;					// ���W
	CNumber*	m_pNumber[MAX_DIGITS];	// �i���o�[1
	int			m_Time;					// ����
	int			m_Seconds;				// ���݂̎���(�b)
	int			m_Minutes;				// ���݂̎���(��)
	int			m_apScore[MAX_RANK];	// �X�R�A�̍ő吔
	int			m_nRankUpdate;			//�X�V�����NNo.
	int			m_nTimerRanking;		//�����L���O��ʕ\���^�C�}�[
	float		m_fPtnrank;				//�v���C���[�_��
	int			m_nCntrank;				//�_�ł̃J�E���g�p

};

#endif
