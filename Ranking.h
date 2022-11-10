//=========================================
//
// �X�R�A����
// Auther�FArita Meigen
//
//=========================================
#ifndef _RANKING_H_		// ���̃}�N����`������ĂȂ�������
#define _RANKING_H_		// ��d�C���N�\���[�h�h�~�̃}�N����`

#include "object2D.h"

class CScore;

#define MAX_DIGITS		(8)			// �X�R�A�̌���
#define MAX_RANK		(5)			//�\�����郉���L���O�̍ő吔

//=========================================
// �X�R�A�N���X���`
//=========================================
class CRanking
{
public:
	CRanking();							//�R���X�g���N�^
	virtual ~CRanking() ;			//�f�X�g���N�^

	//����������
	HRESULT Init() ;

	//�X�V����
	void Update(void) ;

	//�`�揈��
	void Draw(void) ;

	//�I������
	void Uninit(void) ;

	//�N���G�C�g����
	static CRanking* Create();

	//�X�R�A�̐ݒ�
	static void SetCurrentScore(int Time);
	void Save();
	void Load();
	void Ranking();

private:
	D3DXVECTOR3 m_pos;					// ���W
	static int	m_Time;					// ����
	int			m_nRankUpdate;			//�X�V�����NNo.
	CScore*		m_apScore[MAX_RANK];	// �X�R�A�̍ő吔

};

#endif

