

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
   
    // ��÷�
WAVEFORMATEX waveformat;
waveformat.wFormatTag=WAVE_FORMAT_PCM;
waveformat.nChannels=1;
waveformat.nSamplesPerSec=8000;
waveformat.nAvgBytesPerSec=8000;
waveformat.nBlockAlign=1;
waveformat.wBitsPerSample=8; //ָ��¼����ʽ
waveformat.cbSize=0;
 
  wsprintf( lpTemp, "WAVEFORMATEX size = %lu", sizeof(WAVEFORMATEX) );
  //log.write(lpTemp);
  cout<<lpTemp<<endl;
 
 
 
HWAVEIN  m_hWaveIn;
 
 if ( waveInGetNumDevs() ) cout<<"�п���ʹ�õ� WaveIn ͨ��"<<endl;//log.write("�п���ʹ�õ� WaveIn ͨ��");  
 else cout<<"û�п���ʹ�õ� waveIn ͨ��"<<endl;//log.write("û�п���ʹ�õ� waveIn ͨ��");
 
 int res=waveInOpen(&m_hWaveIn,WAVE_MAPPER, &waveformat, (DWORD)NULL,0L,CALLBACK_WINDOW); //��¼���豸
 
 if ( res == MMSYSERR_NOERROR ) cout<<"�� waveIn �ɹ�"<<endl;/*log.write("�� waveIn �ɹ�")*/  // ��֤�����Ƿ�ɹ�
 else  {
     wsprintf(lpTemp, "�� waveIn ͨ��ʧ�ܣ�Error_Code = 0x%x", res );
     //log.write(lpTemp);
	 cout<<lpTemp<<endl;
 } // End of ��֤�����Ƿ�ɹ�
 
 
 
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
 
 
 int resPrepare = waveInPrepareHeader( m_hWaveIn, &m_pWaveHdr, sizeof(WAVEHDR) ); //׼���ڴ��¼��
 
 if ( resPrepare == MMSYSERR_NOERROR) //log.write("׼��¼����ͷ�ļ��ɹ�");
	 cout<<"׼��¼����ͷ�ļ��ɹ�"<<endl;
  else  {
     wsprintf(lpTemp, "���ܿ���¼��ͷ�ļ���Error_Code = 0x%03X", resPrepare );
     //log.write(lpTemp);
	 cout<<lpTemp<<endl;
 } // End of ��֤���ٻ���
 
 
 
  resPrepare = waveInAddBuffer( m_hWaveIn, &m_pWaveHdr, sizeof(WAVEHDR) );
 
 if ( resPrepare == MMSYSERR_NOERROR) //log.write("׼��¼�����ڴ�ɹ�");
	 cout<<"׼��¼�����ڴ�ɹ�"<<endl;
  else  {
     wsprintf(lpTemp, "���ܿ���¼���û��壬Error_Code = 0x%03X", resPrepare );
     //log.write(lpTemp);
	 cout<<lpTemp<<endl;
 } // End of ��֤���ٻ���
 
 
 
 
  //log.write("");  // д����ַ������Է���
  cout<<endl;
 
 
  if (! waveInStart(m_hWaveIn) ) //log.write("��ʼ¼��"); 
	  cout<<"��ʼ¼��"<<endl;
  else //log.write("��ʼ¼��ʧ��");
	  cout<<"��ʼ¼��ʧ��"<<endl;
 
  Sleep(3200);
 
 
MMTIME mmt;
mmt.wType = TIME_BYTES;
//log.numberwrite( "sizeof(MMTIME) =", sizeof(MMTIME) );
cout<<"sizeof(MMTIME) ="<<sizeof(MMTIME)<<endl;
//log.numberwrite( "sizeof(UINT) =", sizeof(UINT) );
cout<<"sizeof(UINT) ="<<sizeof(UINT)<<endl;
 
 if ( waveInGetPosition(m_hWaveIn, &mmt, sizeof(MMTIME)) ) //log.write("����ȡ����Ƶ����");
	 cout<<"����ȡ����Ƶ����"<<endl;
  else //log.numberwrite( "ȡ��������Ƶλ�� =", mmt.u.cb );
	  cout<<"ȡ��������Ƶλ�� ="<<mmt.u.cb<<endl;
 
if (mmt.wType == TIME_BYTES) //{ log.write("�õ��� TIME_BYTES ��ʽ����Ƶ����"); }
	cout<<"�õ��� TIME_BYTES ��ʽ����Ƶ����"<<endl;
   else //log.write("ָ���� TIME_BYTES ��ʽ��Ƶ���Ȳ�֧��");
	   cout<<"ָ���� TIME_BYTES ��ʽ��Ƶ���Ȳ�֧��"<<endl;
 
 
	//if (! waveInStop(m_hWaveIn) )  log.write("ֹͣ¼��"); else   log.write("ֹͣ¼��ʧ��");
 
	if (! waveInReset(m_hWaveIn) )  //log.write("�����ڴ����ɹ�"); 
		cout<<"�����ڴ����ɹ�"<<endl;
	else //log.write("�����ڴ���ʧ��");
		cout<<"�����ڴ���ʧ��"<<endl;
 
 
	m_pWaveHdr.dwBytesRecorded = mmt.u.cb;
 
 
 
	DWORD NumToWrite=0;  DWORD dwNumber = 0;
 
	HANDLE FileHandle =
			CreateFile( "myTest.wav", GENERIC_WRITE, FILE_SHARE_READ, NULL,
                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
 
//  memset(m_pWaveHdr.lpData, 0, datasize);
 

//��Ƶ�ļ���ͷ��Ϣ
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
CloseHandle( FileHandle );    FileHandle = INVALID_HANDLE_VALUE; // ��β�رվ��
 
//log.write("Ӧ�������� myTest.wav �ļ�");
cout<<"Ӧ�������� myTest.wav �ļ�"<<endl;
 
 
  if ( waveInUnprepareHeader(m_hWaveIn, &m_pWaveHdr, sizeof(WAVEHDR)) ) //log.write("Un_Prepare Header ʧ��"); 
	  cout<<"Un_Prepare Header ʧ��"<<endl;
  else //log.write("Un_Prepare Header �ɹ�");
	  cout<<"Un_Prepare Header �ɹ�"<<endl;
 
  if ( GlobalFree(GlobalHandle( m_pWaveHdr.lpData )) ) //log.write("Global Free ʧ��"); 
	  cout<<"Global Free ʧ��"<<endl;
  else //log.write("Global Free �ɹ�");
	  cout<<"Global Free �ɹ�"<<endl;
 
 
 
 if (res == MMSYSERR_NOERROR )  //�ر�¼���豸
 if (waveInClose(m_hWaveIn)==MMSYSERR_NOERROR)/*log.write("�����ر�¼���豸");*/cout<<"�����ر�¼���豸"<<endl;
 else /*log.write("�������ر�¼���豸");*/cout<<"�������ر�¼���豸"<<endl;
 
 
 
    //log.last(true);
    // ExitProcess(0);
    return 0;
}