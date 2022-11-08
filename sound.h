//===================================================
//
// サウンドヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _SOUND_H_
#define _SOUND_H_	

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include <xaudio2.h>

//================================
// サウンドクラスの定義
//================================
class CSound
{
public:
	//*****************************************************************************
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	//*****************************************************************************
	// サウンドファイル
	//*****************************************************************************
	typedef enum
	{
		/*↓ BGM ↓*/
		SOUND_LABEL_GAME1 = 0,	// ゲーム
		SOUND_LABEL_GAME2,
		SOUND_LABEL_TITLE1,
		SOUND_LABEL_TITLE2,
		SOUND_LABEL_RESULT1,
		SOUND_LABEL_RESULT2,
		SOUND_LABEL_SELECT1,
		SOUND_LABEL_SELECT2,
		SOUND_LABEL_SE_BLOCK,
		SOUND_LABEL_SE_JUMP1,
		SOUND_LABEL_SE_JUMP2,
		SOUND_LABEL_SE_FALL1,
		SOUND_LABEL_SE_FALL2,
		SOUND_LABEL_SE_UP,
		SOUND_LABEL_SE_HIT1,
		SOUND_LABEL_SE_HIT2,
		SOUND_LABEL_SE_HIT3,
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();	//コンストラクタ
	~CSound();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(HWND hWnd);
	void Uninit(void);

	static HRESULT PlaySound(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);

	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	//------------------
	// 静的メンバ変数
	//------------------
	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// オーディオデータサイズ
};

#endif // !_SOUND_H_