// =====================================================================================
// 
//       Filename:  capturethread.cpp
//
//    Description:  后台捕获数据的多线程类实现文件
//
//        Version:  1.0
//        Created:  2013年01月24日 21时44分49秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include "capturethread.h"
#include "listtreeview.h"
#include "sniffertype.h"
#include "Sniffer.h"

#ifdef WIN32
	#pragma warning(disable:4996)
#endif

CaptureThread::CaptureThread()
{
	bStopped = false;
}

void CaptureThread::run()
{
	int res;
	struct tm *ltime;
	char szNum[10];
	char szLength[6];
	char timestr[16];
	time_t local_tv_sec;

	static int num = 1;
	SnifferData tmpSnifferData;

	while (bStopped != true && (res = sniffer->captureOnce()) >= 0) {
		if (res == 0) {
			continue;
		}
		sprintf(szNum, "%d", num);
		tmpSnifferData.strNum = szNum;
		num++;

		local_tv_sec = sniffer->header->ts.tv_sec;
		ltime = localtime(&local_tv_sec);
		strftime(timestr, sizeof(timestr), "%H:%M:%S", ltime);

		tmpSnifferData.strTime = timestr;

		sprintf(szLength, "%d", sniffer->header->len);
		tmpSnifferData.strLength = szLength;

		mainTree->addOneCaptureItem(tmpSnifferData.strNum.c_str(), tmpSnifferData.strTime.c_str(),
			 				tmpSnifferData.strSIP.c_str(),tmpSnifferData.strDIP.c_str(),
			 				tmpSnifferData.strProto.c_str(), tmpSnifferData.strLength.c_str());

		sniffer->snifferDataVector.push_back(tmpSnifferData);
	}
}

void CaptureThread::stop()
{
	bStopped = true;
}