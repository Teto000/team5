#include "Map.h"
#include "object.h"
#include "renderer.h"


//===========================
// コンストラクタ
//===========================
CMap::CMap(int nPriority):CObject(nPriority)
{

}

//===========================
// デストラクタ
//===========================
CMap::~CMap()
{

}

//===========================
// 初期化
//===========================
HRESULT CMap::Init(D3DXVECTOR3 pos)
{
	//-----------------------
	// デバイスの取得
	//-----------------------
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//-----------------------
	// Xファイルの読み込み
	//-----------------------
	D3DXLoadMeshFromX("data\\MODEL\\X_File\\Map_001.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//===========================
// 終了
//===========================
void CMap::Uninit()
{
	//-----------------------
	// メッシュの破棄
	//-----------------------
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	//-----------------------
	// マテリアルの破壊
	//-----------------------
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
}

//===========================
// 更新
//===========================
void CMap::Update()
{

}

//===========================
// 描画
//===========================
void CMap::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用

	D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

							//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//プレイヤーパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保持しているマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// 生成
//========================
CMap* CMap::Create( D3DXVECTOR3 pos)
{//モデルのパス , 親モデルから見た位置 , 親モデルから見た向き
	CMap *pMap = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pMap = new CMap(0);	//生成

	if (pMap != nullptr)
	{//NULLチェック
	 //メンバ変数に代入
		pMap->m_pos = pos;

		//初期化
		pMap->Init(pMap->m_pos);
	}

	return pMap;
}


//========================
// 位置の設定
//========================
void CMap::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}


//========================
// 位置の取得
//========================
D3DXVECTOR3 CMap::GetPosition()
{
	return m_pos;
}


//========================
// 横幅の取得
//========================
float CMap::GetWidth()
{
	return 0.0f;
}


//========================
// 縦幅の取得
//========================
float CMap::GetHeight()
{
	return 0.0f;
}
