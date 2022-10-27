//========================================================
//スコア画面の処理
//========================================================
#include "number.h"

//=========================================
//コンストラクタ
//=========================================
CNumber::CNumber()
{
}

//=========================================
//デストラクタ
//=========================================
CNumber::~CNumber()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CNumber::Init(D3DXVECTOR3 pos)
{
	//サイズの設定
	CObject2D::SetSize(50.0f, 50.0f);

	// オブジェクトの初期化
	CObject2D::Init(pos);

	// テクスチャの設定
	CObject2D::SetTexture(CTexture::TEXTURE_NUMBER);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CNumber::Update()
{
	D3DXVECTOR3 pos;
	pos = GetPosition();
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CNumber::Uninit()
{
	CObject2D::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CNumber::Draw()
{
	CObject2D::Draw();
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CNumber* CNumber::Create(const D3DXVECTOR3 &pos)
{
	CNumber* pCNumber = nullptr;

	pCNumber = new CNumber;

	if (pCNumber != nullptr)
	{
		pCNumber->Init(pos);
	}

	return pCNumber;
}

//========================================================
//数字の設定処理
//========================================================
void CNumber::Set(int nDigits)
{
	SetAnim(nDigits, 12);
}
