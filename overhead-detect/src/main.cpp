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
	BOOL Open(LPCSTR lpFileName)//打开文件  
	{
		//如果有打开的MCI设备就关闭  
		if (mciOpen.wDeviceID)Close();
		//初始化MCI_OPEN_PARMS结构中的文件类型  
		mciOpen.lpstrDeviceType = NULL;
		//播放文件路径  
		mciOpen.lpstrElementName = lpFileName;//"C:\\Users\\Administrator\\Desktop\\show\\MP3\\CB_MP3\\CB_MP3\\陈楚生 - 风起时，想你.mp3";  
		//向MCI设备发送命令消息（在打开设备时，设备号为0）  
		if (mciSendCommand(0, MCI_OPEN, MCI_DEVTYPE_WAVEFORM_AUDIO, (DWORD)&mciOpen))
		{
			return FALSE;
		}
		dwFrom = MCI_MAKE_HMS(0, 0, 0);
		return TRUE;
	}
	void Play()//播放  
	{
		//播放参数结构  
		MCI_PLAY_PARMS mciplayparms;
		//得到文件大小  
		DWORD cdlen = GetLength(MCI_STATUS_LENGTH);
		DWORD cdto = MCI_MAKE_HMS(MCI_HMS_HOUR(cdlen), MCI_HMS_MINUTE(cdlen), MCI_HMS_SECOND(cdlen));//把文件中读出的大小转换为时间数量  
		mciplayparms.dwCallback = NULL;
		mciplayparms.dwFrom = dwFrom;//设置起始位置  
		mciplayparms.dwTo = cdto;//设置终止位置  
		if (mciOpen.wDeviceID != 0)//判断是否打开文件  
		{//播放音乐  
			mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_TO | MCI_FROM, (DWORD)(LPVOID)&mciplayparms);
		}
	}
	void Close()//关闭  
	{
		if (mciOpen.wDeviceID)
		{
			//执行MCI_CLOSE操作，关闭MCI设备  
			mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
		}
	}
	void Stop()//停止  
	{
		if (mciOpen.wDeviceID)
		{//执行MCI_STOP操作，停止播放音乐  
			mciSendCommand(mciOpen.wDeviceID, MCI_STOP, NULL, NULL);
			//把播放位置设定为音乐文件的开头（下一次播放操作从文件开头位置开始）  
			mciSendCommand(mciOpen.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
		}
		dwFrom = MCI_MAKE_HMS(0, 0, 0);
	}
	void Pause()//暂停  
	{
		if (mciOpen.wDeviceID)
		{//执行MCI_PAUSE操作，暂停播放音乐  
			DWORD dwsf = GetLength(MCI_STATUS_POSITION);
			dwFrom = MCI_MAKE_MSF(MCI_MSF_MINUTE(dwsf), MCI_MSF_SECOND(dwsf), MCI_MSF_FRAME(dwsf));
			//执行MCI_PAUSE操作，暂停播放音乐  
			mciSendCommand(mciOpen.wDeviceID, MCI_PAUSE, NULL, NULL);
		}
	}
	DWORD GetLength(DWORD dwItem)//获取歌曲长度  
	{
		//得到当前文件状态  
		MCI_STATUS_PARMS mcistatusparms;
		mcistatusparms.dwCallback = (DWORD)m_hWnd;
		mcistatusparms.dwItem = dwItem;  //dwItem wItem
		mcistatusparms.dwReturn = 0;
		mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mcistatusparms);
		return mcistatusparms.dwReturn;
	}
	void SetWindowHwnd(HWND hWnd)//设置主窗口句柄 
	{
		m_hWnd = hWnd;
	}
private:
	MCI_OPEN_PARMS mciOpen;//打开设备参数  
	HWND m_hWnd;//主窗口句柄  
	DWORD dwFrom;//播放起始点 
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