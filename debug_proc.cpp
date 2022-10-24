//===================================================
//
// �f�o�b�O�̏���
// Author : Sato Teruto
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include <stdarg.h>
#include <string>
#include "debug_proc.h"
#include "main.h"
#include "renderer.h"
#include "application.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
LPD3DXFONT CDebugProc::m_pFont = nullptr;
std::string CDebugProc::m_Str = {};
int CDebugProc::m_nCntData = 0;

//===========================
// �R���X�g���N�^
//===========================
CDebugProc::CDebugProc()
{

}

//===========================
// �f�X�g���N�^
//===========================
CDebugProc::~CDebugProc()
{

}

//===========================
// ������
//===========================
HRESULT CDebugProc::Init()
{
	return S_OK;
}

//===========================
// �I��
//===========================
void CDebugProc::Uninit()
{

}

//===========================
// ������̓o�^
//===========================
void CDebugProc::Print(const char* pFormat, ...)
{
	va_list ap;	//�ψ����\���̂̒�`

	va_start(ap, pFormat);	//�ψ����\���̂̏�����

	for (int i = 0; i < (signed)strlen(pFormat); i++)
	{//������̐�������
		if (pFormat[i] == '%')
		{//������%�Ȃ�
			if (pFormat[i + 1] == 'd')
			{//���̒l��d�Ȃ�
				//-------------------
				// int�^�̒l������
				//-------------------
				int type = va_arg(ap, int);	//��������l�����o��
				m_Str += std::to_string(type);
			}
			else if (pFormat[i + 1] == 'f')
			{//���̒l��f�Ȃ�
				//-------------------
				// float�^�̒l������
				//-------------------
				double type = va_arg(ap, double);
				m_Str += std::to_string(type);
			}

			i++;	//�����ԍ��̉��Z
		}
		else
		{
			m_Str += pFormat[i];	//����������
		}
	}

	va_end(ap);	//�ψ����\���̂̏���
}

//===========================
// �`��
//===========================
void CDebugProc::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];

	wsprintf(str, &m_Str[0]);

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}