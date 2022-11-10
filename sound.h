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
		SOUND_LABEL_GAME = 0,				//ゲーム
		SOUND_LABEL_TITLE,					//タイトル
		SOUND_LABEL_RESULT,					//リザルト
		SOUND_LABEL_SELECT,					//セレクト
		SOUND_LABEL_SE_BLOCK,				//ブロック設置音
		SOUND_LABEL_SE_JUMP1,				//ジャンプ音１
		SOUND_LABEL_SE_JUMP2,				//ジャンプ音２
		SOUND_LABEL_SE_FALL1,				//落下音１
		SOUND_LABEL_SE_FALL2,				//落下音２
		SOUND_LABEL_SE_UP,					//ジャンプオブジェクト用音
		SOUND_LABEL_SE_HIT1,				//衝撃音軽１
		SOUND_LABEL_SE_HIT2,				//衝撃音軽２
		SOUND_LABEL_SE_HIT3,				//衝撃音重
		SOUND_LABEL_SE_GAME_START,			//スタート音声
		SOUND_LABEL_SE_START_VOICE,			//スタート音声
		SOUND_LABEL_SE_START_LIGHT,			//カウントダウン低音
		SOUND_LABEL_SE_START_HEAVY,			//カウントダウン高音
		SOUND_LABEL_SE_SELECT,				//選択音
		SOUND_LABEL_SE_SELECT_DECISION,		//選択決定音
		SOUND_LABEL_SE_FINISH,				//ゴール音声
		SOUND_LABEL_SE_1PWIN,				//１Ｐ勝利音声
		SOUND_LABEL_SE_2PWIN,				//２Ｐ勝利音声
		SOUND_LABEL_SE_3PWIN,				//３Ｐ勝利音声
		SOUND_LABEL_SE_4PWIN,				//４Ｐ勝利音声
		SOUND_LABEL_MAX,					//最大数
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