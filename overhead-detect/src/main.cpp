#include <windows.h>  
#include <mmsystem.h>  
#pragma comment(lib,"winmm.lib") 

#include "help.cpp"
#include "modules/counter.hpp"
#include "modules/view.hpp"

class CCMyPlayerControl
{
public:
	CCMyPlayerControl(void) {}
	~CCMyPlayerControl(void){}
public:
	BOOL Open(LPCSTR lpFileName)//���ļ�  
	{
		//����д򿪵�MCI�豸�͹ر�  
		if (mciOpen.wDeviceID)Close();
		//��ʼ��MCI_OPEN_PARMS�ṹ�е��ļ�����  
		mciOpen.lpstrDeviceType = NULL;
		//�����ļ�·��  
		mciOpen.lpstrElementName = lpFileName;//"C:\\Users\\Administrator\\Desktop\\show\\MP3\\CB_MP3\\CB_MP3\\�³��� - ����ʱ������.mp3";  
		//��MCI�豸����������Ϣ���ڴ��豸ʱ���豸��Ϊ0��  
		if (mciSendCommand(0, MCI_OPEN, MCI_DEVTYPE_WAVEFORM_AUDIO, (DWORD)&mciOpen))
		{
			return FALSE;
		}
		dwFrom = MCI_MAKE_HMS(0, 0, 0);
		return TRUE;
	}
	void Play()//����  
	{
		//���Ų����ṹ  
		MCI_PLAY_PARMS mciplayparms;
		//�õ��ļ���С  
		DWORD cdlen = GetLength(MCI_STATUS_LENGTH);
		DWORD cdto = MCI_MAKE_HMS(MCI_HMS_HOUR(cdlen), MCI_HMS_MINUTE(cdlen), MCI_HMS_SECOND(cdlen));//���ļ��ж����Ĵ�Сת��Ϊʱ������  
		mciplayparms.dwCallback = NULL;
		mciplayparms.dwFrom = dwFrom;//������ʼλ��  
		mciplayparms.dwTo = cdto;//������ֹλ��  
		if (mciOpen.wDeviceID != 0)//�ж��Ƿ���ļ�  
		{//��������  
			mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_TO | MCI_FROM, (DWORD)(LPVOID)&mciplayparms);
		}
	}
	void Close()//�ر�  
	{
		if (mciOpen.wDeviceID)
		{
			//ִ��MCI_CLOSE�������ر�MCI�豸  
			mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
		}
	}
	void Stop()//ֹͣ  
	{
		if (mciOpen.wDeviceID)
		{//ִ��MCI_STOP������ֹͣ��������  
			mciSendCommand(mciOpen.wDeviceID, MCI_STOP, NULL, NULL);
			//�Ѳ���λ���趨Ϊ�����ļ��Ŀ�ͷ����һ�β��Ų������ļ���ͷλ�ÿ�ʼ��  
			mciSendCommand(mciOpen.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
		}
		dwFrom = MCI_MAKE_HMS(0, 0, 0);
	}
	void Pause()//��ͣ  
	{
		if (mciOpen.wDeviceID)
		{//ִ��MCI_PAUSE��������ͣ��������  
			DWORD dwsf = GetLength(MCI_STATUS_POSITION);
			dwFrom = MCI_MAKE_MSF(MCI_MSF_MINUTE(dwsf), MCI_MSF_SECOND(dwsf), MCI_MSF_FRAME(dwsf));
			//ִ��MCI_PAUSE��������ͣ��������  
			mciSendCommand(mciOpen.wDeviceID, MCI_PAUSE, NULL, NULL);
		}
	}
	DWORD GetLength(DWORD dwItem)//��ȡ��������  
	{
		//�õ���ǰ�ļ�״̬  
		MCI_STATUS_PARMS mcistatusparms;
		mcistatusparms.dwCallback = (DWORD)m_hWnd;
		mcistatusparms.dwItem = dwItem;  //dwItem wItem
		mcistatusparms.dwReturn = 0;
		mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mcistatusparms);
		return mcistatusparms.dwReturn;
	}
	void SetWindowHwnd(HWND hWnd)//���������ھ�� 
	{
		m_hWnd = hWnd;
	}
private:
	MCI_OPEN_PARMS mciOpen;//���豸����  
	HWND m_hWnd;//�����ھ��  
	DWORD dwFrom;//������ʼ�� 
};

CCMyPlayerControl player;
BOOL playerOK = FALSE;

class CAlarm :public IAlarm {
public:
	virtual void alarm() {
		printf("%d,some one in...\n", time(0));
		if (playerOK == TRUE) {
			player.Play();
		}
	}
};

int main(int argc, char* argv[]) {
	const int params = 5;
    if (argc < params) return help();
	int type = atoi(argv[1]);
	int deviceIdx = atoi(argv[2]);
	int y1 = atoi(argv[3]);
	int y2 = atoi(argv[4]);
	char* vedioFile = argv[5];
	CAlarm * pAlarm = new CAlarm();
	BOOL ret = player.Open("warn.wav");
	if (TRUE == ret) {
		//player.Play();
		playerOK = TRUE;
	}
	else {
		printf("player open err\n");
	}
	VideoCapturePeopleCounter* counter = NULL;
	if (type == 1) {
		counter = new VideoCapturePeopleCounter(vedioFile, pAlarm);
	}
	else {
		counter = new VideoCapturePeopleCounter(pAlarm, deviceIdx);
	}
    counter->delegate = new WindowController(counter);
    //counter->setRefLineY(180,540);
	counter->setRefLineY(y1, y2);
    counter->start();


	getchar();
}