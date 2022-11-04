//===================================================
//
// �����_�����O�̏���
// Author : Sato Teruto
//
//===================================================

//--------------------
// �C���N���[�h
//--------------------
#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>
#include <assert.h>
#include "renderer.h"
#include "main.h"
#include "object3d.h"
#include "application.h"
#include "camera_player.h"
#include "camera_title.h"
#include "debug_proc.h"
#include "input.h"
#include "input_keybord.h"
#include "Goal.h"
#include "title.h"

//-----------------------
// �ÓI�����o�ϐ��錾
//-----------------------
const float CRenderer::fDefaultFov = 45.0f;						//��{�̎���p
const float CRenderer::fDefaultAspectX = (float)SCREEN_WIDTH;	//��{�̃A�X�y�N�g��X

int CRenderer::m_nMaxCamera = nDefaultMaxCamera;		//�J�����̍ő吔
float CRenderer::m_fAspectFov = 0.0f;					//����p
float CRenderer::m_fAspectX = 0.0f;						//�A�X�y�N�g��X
CCameraPlayer* CRenderer::m_pCameraPlayer[nDefaultMaxCamera] = {};	//�J����
CCameraTitle* CRenderer::m_pCameraTitle = nullptr;	//�^�C�g���J����

//=========================
// �R���X�g���N�^
//=========================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;		// Direct3D�I�u�W�F�N�g
	m_pD3DDevice = nullptr;	// Device�I�u�W�F�N�g

	m_pFont = nullptr;		// �t�H���g
	m_bWIRE = false;
	m_nFinish = false;
}

//=========================
// �f�X�g���N�^
//=========================
CRenderer::~CRenderer()
{
	assert(m_pD3D == nullptr);
	assert(m_pD3DDevice == nullptr);
	assert(m_pFont == nullptr);
}

//=============================================================================
// ������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	//�A�X�y�N�g��ύX�p�ϐ��̏�����
	m_fAspectFov = 60.0f;
	m_fAspectX = (float)(SCREEN_WIDTH / 2);

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;				// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;							// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if ((FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// ��L�̐ݒ肪���s������
		// �`��ƒ��_������CPU�ōs�Ȃ�
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))))
	{
		// �������s
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�r���[�|�[�g�̕`�揇��������
	for (int i = 0; i < m_nMaxCamera; i++)
	{
		m_viewPortOrder[i] = i;
	}

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
#endif

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CRenderer::Uninit()
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif // _DEBUG

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CRenderer::Update()
{
	//�I�u�W�F�N�g�̍X�V
	CObject::UpdateAll();

	if (CInputKeyboard::Trigger(DIK_F4))
	{//F4�L�[�������ꂽ
		if (m_bWIRE == false)
		{//���C���[�t���[������Ȃ��Ȃ�
		 //���C���[�t���[���̐ݒ�
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else if (m_bWIRE == true)
		{//���C���[�t���[���Ȃ�
		 //���C���[�t���[�������̏�Ԃɂ���
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		m_bWIRE = !m_bWIRE;		//���C���[�̏�Ԃ�؂�ւ���
	}
}

//=============================================================================
// �`��
//=============================================================================
void CRenderer::Draw()
{
	//-------------------------------
	// 1�ʂ̃r���[�|�[�g��O�ʂɏo��
	//-------------------------------
	/* 1�ʂ̃v���C���[�ԍ����擾 */
	int nFirstNumber = CGoal::GetWinner();

	if (nFirstNumber <= CRenderer::GetMaxCamera() - 1)
	{//�v���C���[�ԍ����ő吔�𒴂��Ă��Ȃ��Ȃ�
		if (nFirstNumber >= 0 /* 1�ʂ�null����Ȃ��Ȃ� */)
		{//Z��������Ă���Ȃ�
			//-----------------------------
			// �`�揇�̔z������ւ���
			//-----------------------------
			int nviewData = 0;	//�ۑ��p�ϐ�

			//�Ō�̒l��ۑ�
			nviewData = m_viewPortOrder[m_nMaxCamera - 1];

			//�z��̍Ō�Ɠ���ւ���
			m_viewPortOrder[m_nMaxCamera - 1] = nFirstNumber;

			//�ۑ����Ă����l����
			m_viewPortOrder[nFirstNumber] = nviewData;
		}
	}

	//-------------------------------
	// �`�揈��
	//-------------------------------
	for (int i = 0; i < m_nMaxCamera; i++)
	{
		int nOrder = m_viewPortOrder[i];

		//-------------------------
		// �J�����̐ݒ�
		//-------------------------
		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{//�Q�[����ʂȂ�
			//�v���C���[�J�����̐ݒ�
			SetCameraPlayer(nOrder);
		}
		else if (CApplication::GetMode() == CApplication::MODE_TITLE)
		{//�^�C�g����ʂȂ�
			//�^�C�g���J�����̐ݒ�
			SetCameraTitle();
		}

		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		m_pD3DDevice->Clear(0,
							NULL,
							(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
							D3DCOLOR_RGBA(0, 0, 0, 0),
							1.0f,
							0);

		// Direct3D�ɂ��`��̊J�n
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{
			//�I�u�W�F�N�g�̕`��
			CObject::DrawAll();

#ifdef _DEBUG
			// FPS�\��
			DrawFPS();

			CDebugProc::Draw();
#endif // _DEBUG

			// Direct3D�ɂ��`��̏I��
			m_pD3DDevice->EndScene();
		}
	}

	//�r���[�|�[�g�̕`�揇��������
	for (int i = 0; i < m_nMaxCamera; i++)
	{
		m_viewPortOrder[i] = i;
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �J�����̍ő吔�̐ݒ�E�擾
//=============================================================================
int CRenderer::SetMaxCamera(CGame::NUMCAMERA nNumCamera)
{
	switch (nNumCamera)
	{
	case CGame::NUMCAMERA_ONE:
		//�J�����̍ő吔��1�ɂ���
		m_nMaxCamera = 1;
		break;

	case CGame::NUMCAMERA_TWO:
		//�J�����̍ő吔��2�ɂ���
		m_nMaxCamera = 2;
		break;

	case CGame::NUMCAMERA_THREE:
	case CGame::NUMCAMERA_FOUR:
		//�J�����̍ő吔��4�ɂ���
		m_nMaxCamera = 4;
		break;

	default:
		m_nMaxCamera = 0;
		break;
	}

	return nNumCamera;
}

//=============================================================================
// �A�X�y�N�g��̉��Z
//=============================================================================
void CRenderer::AddAcpect(int nNumCamera, float fov, float x)
{
	if (m_fAspectFov >= fDefaultFov)
	{//����p��45�ȏ�Ȃ�
		//����p������
		m_fAspectFov -= fov;
	}
	if (m_fAspectX <= fDefaultAspectX)
	{//�A�X�y�N�g��X���X�N���[���̕��ȓ��Ȃ�
		//�A�X�y�N�g��X�����Z
		m_fAspectX += x;
	}

	//�J�����̃A�X�y�N�g���ύX
	m_pCameraPlayer[nNumCamera]->SetAspect(m_pD3DDevice, m_fAspectFov,
		m_fAspectX, (float)SCREEN_HEIGHT);
}

//=============================================================================
// �J�����̍ő吔�̎擾
//=============================================================================
int CRenderer::GetMaxCamera()
{
	return m_nMaxCamera;
}

//=============================================================================
// �v���C���[�J�����̐ݒ�
//=============================================================================
void CRenderer::SetCameraPlayer(int nOrder)
{
	//�v���C���[�J�����̎擾
	m_pCameraPlayer[nOrder] = CGame::GetCameraPlayer(nOrder);

	if (m_pCameraPlayer[nOrder] != nullptr)
	{//�J������null����Ȃ��Ȃ�
		//-------------------------
		// �J�����̐ݒ�
		//-------------------------
		m_pCameraPlayer[nOrder]->SetCamera(m_pD3DDevice);

		//-------------------------
		// �J������2�̏ꍇ
		//-------------------------
		if (m_nMaxCamera == 2)
		{//�J�����̐���2�Ȃ�
		 //�J�����̃A�X�y�N�g���ύX
			m_pCameraPlayer[nOrder]->SetAspect(m_pD3DDevice, 60.0f,
				(float)(SCREEN_WIDTH / 2), (float)SCREEN_HEIGHT);

			//-------------------------
			// �g�傷�鎞�̏���
			//-------------------------
			if (CGame::GetFinish() == true)
			{//�I���t���O�������Ă���Ȃ�
			 //�A�X�y�N�g��̉��Z
				AddAcpect(nOrder, (15.0f / 60.0f), (640.0f / 120.0f));
			}
		}

		//-------------------------
		// �r���[�|�[�g�̏���
		//-------------------------
		//�r���[�|�[�g�̐ݒ�
		D3DVIEWPORT9 viewport = m_pCameraPlayer[nOrder]->GetViewport();
		m_pD3DDevice->SetViewport(&viewport);
	}
}

//=============================================================================
// �^�C�g���J�����̐ݒ�
//=============================================================================
void CRenderer::SetCameraTitle()
{
	//�^�C�g���J�����̎擾
	m_pCameraTitle = CTitle::GetCameraTitle();

	if (m_pCameraTitle != nullptr)
	{//�J������null����Ȃ��Ȃ�
		//-------------------------
		// �J�����̐ݒ�
		//-------------------------
		m_pCameraTitle->SetCamera(m_pD3DDevice);

		//-------------------------
		// �r���[�|�[�g�̏���
		//-------------------------
		//�r���[�|�[�g�̐ݒ�
		D3DVIEWPORT9 viewport = m_pCameraTitle->GetViewport();
		m_pD3DDevice->SetViewport(&viewport);
	}
}

#ifdef _DEBUG
//=============================================================================
// FPS�\��
//=============================================================================
void CRenderer::DrawFPS()
{
	int nCntFPS = GetFPS();	//FPS���擾

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];

	wsprintf(str, _T("FPS : %d\n"), nCntFPS);

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif // _DEBUG