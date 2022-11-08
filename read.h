//=============================================================================
//
// read.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _READ_H_			//���̃}�N����`������Ȃ�������
#define _READ_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************

//*****************************************************************************
// �񋓌^��`
//*****************************************************************************
enum ModelTxt
{
	TitleModel = 0,
	StageSelectModel,
	GameStage01,
	GameStage02,
	GameStage03,
	GameStage04,
	GameStage05,
	GameStage06,
	ResultModel,
	ModelTxtMax
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRead
{
	static const int LINE_MAX_READING_LENGTH = 256;
public:

	CRead();
	~CRead();
	void ReadAnchorPoint();
	//void ReadEnemy(StageNum Stage);
	int ReadXFile(char *sXFilePath);
	void ReadModel(ModelTxt ModelTxt);
	int ReadMotion(char *sXFilePath);
	//void ReadRanking(StageNum Stage,CRanking *pRnking);
	//void SaveRanking(StageNum Stage,const int *pRanking ,const CRanking::NameData *pNameData, const int *pAirframe);

private:
	
};

#endif