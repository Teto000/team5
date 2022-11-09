//===================================================
//
// �I�u�W�F�N�g�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_	

//-------------------------------
// �C���N���[�h
//-------------------------------
#include <d3dx9.h>

//-------------------------------
// �}�N����`
//-------------------------------
#define MAX_OBJECT		(512)	//�I�u�W�F�N�g�̍ő吔
#define MY_MAX_PRIORITY	(3)		//�v���C�I���e�B�̍ő吔

//================================
// �I�u�W�F�N�g�N���X
//================================
class CObject
{
public:
	//------------------------
	// �I�u�W�F�N�g�̎��
	//------------------------
	enum EObjType
	{
		OBJTYPE_MAP,
		OBJTYPE_GOAL,
		OBJTYPE_MODEL,
		OBJTYPE_UI,
		OBJTYPE_FADE,
		OBJTYPE_MAX,
	};

	explicit CObject(int nPriority);	//�R���X�g���N�^
	virtual ~CObject();					//�f�X�g���N�^

	//----------------
	// �����o�֐�
	//----------------
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void Release();

	//----------------
	// �ÓI�����o�֐�
	//----------------
	static void ReleaseAll(bool bFinish);
	static void UpdateAll();
	static void DrawAll();
	static int GetNumAll();

	//CObject *GETObject(int nPriority, int nCnt);
	void SetObjType(EObjType ObjType);	//��ނ̐ݒ�
	EObjType GetObjType();				//��ނ̎擾

	//�Q�b�^�[
	virtual D3DXVECTOR3 GetPosition() = 0;
	virtual float GetWidth() = 0;
	virtual float GetHeight() = 0;

private:
	void Death(CObject* pObj);

private:
	//----------------
	// �萔
	//----------------
	static const int nMaxPriority = OBJTYPE_MAX;

	//----------------
	// �����o�ϐ�
	//----------------
	CObject* m_pPrev;	//1�O�̃I�u�W�F�N�g
	CObject* m_pNext;	//1��̃I�u�W�F�N�g
	EObjType m_ObjType;	//�I�u�W�F�N�g�̎��
	bool m_bDeath;		//���S�t���O
	bool m_bPause;		//�|�[�Y��ON,OFF

	//----------------
	// �ÓI�����o�ϐ�
	//----------------
	static CObject* m_Top[nMaxPriority];		//�ŏ��̃I�u�W�F�N�g
	static CObject*	m_Current[nMaxPriority];	//�Ō�̃I�u�W�F�N�g
	int	   m_nPriority;	//�v���C�I���e�B�̔ԍ�
};

#endif // !_OBJECT_H_