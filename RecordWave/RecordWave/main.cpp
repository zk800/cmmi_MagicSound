

#include <windows.h>
#include <Mmsystem.h>
#include <iostream>
using namespace std;

char lpTemp[256];


DWORD FCC(LPSTR lpStr)
{
   DWORD Number = lpStr[0] + lpStr[1] *0x100 + lpStr[2] *0x10000 + lpStr[3] *0x1000000 ;
   return Number;
}


//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
//                    LPSTR lpCmdLine, int nCmdShow )
int main()
{
 
  CreateMutex( NULL, false, "MyMutex");
   if ( GetLastError() == ERROR_ALREADY_EXISTS )
   { /*log.write("Exists and Exit"); log.last(); */
	   cout<<"Exists and Exit"<<endl;
	   ExitProcess( NULL); }
 
 
    //log.write("Program Start.");
    //log.nobuff = true;
   cout<<"Program Start"<<endl;
 
DWORD datasize = 48000;
   
    // 最常用法
WAVEFORMATEX waveformat;
waveformat.wFormatTag=WAVE_FORMAT_PCM;
waveformat.nChannels=1;
waveformat.nSamplesPerSec=8000;
waveformat.nAvgBytesPerSec=8000;
waveformat.nBlockAlign=1;
waveformat.wBitsPerSample=8; //指定录音格式
waveformat.cbSize=0;
 
  wsprintf( lpTemp, "WAVEFORMATEX size = %lu", sizeof(WAVEFORMATEX) );
  //log.write(lpTemp);
  cout<<lpTemp<<endl;
 
 
 
HWAVEIN  m_hWaveIn;
 
 if ( waveInGetNumDevs() ) cout<<"有可以使用的 WaveIn 通道"<<endl;//log.write("有可以使用的 WaveIn 通道");  
 else cout<<"没有可以使用的 waveIn 通道"<<endl;//log.write("没有可以使用的 waveIn 通道");
 
 int res=waveInOpen(&m_hWaveIn,WAVE_MAPPER, &waveformat, (DWORD)NULL,0L,CALLBACK_WINDOW); //打开录音设备
 
 if ( res == MMSYSERR_NOERROR ) cout<<"打开 waveIn 成功"<<endl;/*log.write("打开 waveIn 成功")*/  // 验证创建是否成功
 else  {
     wsprintf(lpTemp, "打开 waveIn 通道失败，Error_Code = 0x%x", res );
     //log.write(lpTemp);
	 cout<<lpTemp<<endl;
 } // End of 验证创建是否成功
 
 
 
 WAVEHDR m_pWaveHdr;
  
  m_pWaveHdr.lpData = (char *)GlobalLock( GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE, datasize) );
  memset(m_pWaveHdr.lpData, 0, datasize );
  m_pWaveHdr.dwBufferLength = datasize;
  m_pWaveHdr.dwBytesRecorded = 0;
  m_pWaveHdr.dwUser = 0;
  m_pWaveHdr.dwFlags = 0;
  m_pWaveHdr.dwLoops = 0;
 
 
  wsprintf( lpTemp, "WAVEHDR size = %lu", sizeof(WAVEHDR) );
  //log.write(lpTemp);
  cout<<lpTemp<<endl;
 
 
 int resPrepare = waveInPrepareHeader( m_hWaveIn, &m_pWaveHdr, sizeof(WAVEHDR) ); //准备内存块录音
 
 if ( resPrepare == MMSYSERR_NOERROR) //log.write("准备录音用头文件成功");
	 cout<<"准备录音用头文件成功"<<endl;
  else  {
     wsprintf(lpTemp, "不能开辟录音头文件，Error_Code = 0x%03X", resPrepare );
     //log.write(lpTemp);
	 cout<<lpTemp<<endl;
 } // End of 验证开辟缓冲
 
 
 
  resPrepare = waveInAddBuffer( m_hWaveIn, &m_pWaveHdr, sizeof(WAVEHDR) );
 
 if ( resPrepare == MMSYSERR_NOERROR) //log.write("准备录音用内存成功");
	 cout<<"准备录音用内存成功"<<endl;
  else  {
     wsprintf(lpTemp, "不能开辟录音用缓冲，Error_Code = 0x%03X", resPrepare );
     //log.write(lpTemp);
	 cout<<lpTemp<<endl;
 } // End of 验证开辟缓冲
 
 
 
 
  //log.write("");  // 写入空字符串可以分行
  cout<<endl;
 
 
  if (! waveInStart(m_hWaveIn) ) //log.write("开始录音"); 
	  cout<<"开始录音"<<endl;
  else //log.write("开始录音失败");
	  cout<<"开始录音失败"<<endl;
 
  Sleep(3200);
 
 
MMTIME mmt;
mmt.wType = TIME_BYTES;
//log.numberwrite( "sizeof(MMTIME) =", sizeof(MMTIME) );
cout<<"sizeof(MMTIME) ="<<sizeof(MMTIME)<<endl;
//log.numberwrite( "sizeof(UINT) =", sizeof(UINT) );
cout<<"sizeof(UINT) ="<<sizeof(UINT)<<endl;
 
 if ( waveInGetPosition(m_hWaveIn, &mmt, sizeof(MMTIME)) ) //log.write("不能取得音频长度");
	 cout<<"不能取得音频长度"<<endl;
  else //log.numberwrite( "取得现在音频位置 =", mmt.u.cb );
	  cout<<"取得现在音频位置 ="<<mmt.u.cb<<endl;
 
if (mmt.wType == TIME_BYTES) //{ log.write("得到的 TIME_BYTES 格式的音频长度"); }
	cout<<"得到的 TIME_BYTES 格式的音频长度"<<endl;
   else //log.write("指定的 TIME_BYTES 格式音频长度不支持");
	   cout<<"指定的 TIME_BYTES 格式音频长度不支持"<<endl;
 
 
	//if (! waveInStop(m_hWaveIn) )  log.write("停止录音"); else   log.write("停止录音失败");
 
	if (! waveInReset(m_hWaveIn) )  //log.write("重置内存区成功"); 
		cout<<"重置内存区成功"<<endl;
	else //log.write("重置内存区失败");
		cout<<"重置内存区失败"<<endl;
 
 
	m_pWaveHdr.dwBytesRecorded = mmt.u.cb;
 
 
 
	DWORD NumToWrite=0;  DWORD dwNumber = 0;
 
	HANDLE FileHandle =
			CreateFile( "myTest.wav", GENERIC_WRITE, FILE_SHARE_READ, NULL,
                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
 
//  memset(m_pWaveHdr.lpData, 0, datasize);
 

//音频文件的头信息
dwNumber = FCC("RIFF");
WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
 
dwNumber = m_pWaveHdr.dwBytesRecorded + 18 + 20;
WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
 
dwNumber = FCC("WAVE");
WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
 
dwNumber = FCC("fmt ");
WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
 
dwNumber = 18L;
WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
 
WriteFile(FileHandle, &waveformat, sizeof(WAVEFORMATEX), &NumToWrite, NULL);
 
dwNumber = FCC("data");
WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
 
dwNumber = m_pWaveHdr.dwBytesRecorded;
WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
 
WriteFile(FileHandle, m_pWaveHdr.lpData, m_pWaveHdr.dwBytesRecorded, &NumToWrite, NULL);
 
SetEndOfFile(FileHandle);
CloseHandle( FileHandle );    FileHandle = INVALID_HANDLE_VALUE; // 收尾关闭句柄
 
//log.write("应该已生成 myTest.wav 文件");
cout<<"应该已生成 myTest.wav 文件"<<endl;
 
 
  if ( waveInUnprepareHeader(m_hWaveIn, &m_pWaveHdr, sizeof(WAVEHDR)) ) //log.write("Un_Prepare Header 失败"); 
	  cout<<"Un_Prepare Header 失败"<<endl;
  else //log.write("Un_Prepare Header 成功");
	  cout<<"Un_Prepare Header 成功"<<endl;
 
  if ( GlobalFree(GlobalHandle( m_pWaveHdr.lpData )) ) //log.write("Global Free 失败"); 
	  cout<<"Global Free 失败"<<endl;
  else //log.write("Global Free 成功");
	  cout<<"Global Free 成功"<<endl;
 
 
 
 if (res == MMSYSERR_NOERROR )  //关闭录音设备
 if (waveInClose(m_hWaveIn)==MMSYSERR_NOERROR)/*log.write("正常关闭录音设备");*/cout<<"正常关闭录音设备"<<endl;
 else /*log.write("非正常关闭录音设备");*/cout<<"非正常关闭录音设备"<<endl;
 
 
 
    //log.last(true);
    // ExitProcess(0);
    return 0;
}