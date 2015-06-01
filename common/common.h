﻿
#ifndef COMMON_H
#define COMMON_H

#include "version.h"

#define MAX_KRNLOBJNAME		256
#define CONV_POINT_NUM		256
#define KEYRELEN			256
#define MAX_PRESERVEDKEY	8
#define MAX_SELKEY_C		9

#define MAX_SKKSERVER_HOST	(255 + 1)
#define MAX_SKKSERVER_PORT	(5 + 1)

#define MAX_WIDTH_DEFAULT	800

#define PIPEBUFSIZE			0x2000	//named pipe, 16KB with wchar_t
#define DICBUFSIZE			0x1000	//entry, 8KB with wchar_t
#define READBUFSIZE			0x800	//read skk dictionary, 4KB with wchar_t/char
#define RECVBUFSIZE			0x800	//receive from skk server, 2KB with char

//request
#define REQ_SEARCH			L'1'	//辞書検索
#define REQ_SEARCHUSER		L'2'	//辞書検索(ユーザー辞書のみ)
#define REQ_COMPLEMENT		L'4'	//補完
#define REQ_CONVERTKEY		L'5'	//見出し語変換
#define REQ_CONVERTCND		L'6'	//候補変換
#define REQ_USER_ADD_0		L'A'	//ユーザー辞書追加(送りあり、補完なし)
#define REQ_USER_ADD_1		L'B'	//ユーザー辞書追加(送りなし、補完あり)
#define REQ_USER_DEL_0		L'C'	//ユーザー辞書削除(送りあり、補完なし)
#define REQ_USER_DEL_1		L'D'	//ユーザー辞書削除(送りなし、補完あり)
#define REQ_USER_SAVE		L'S'	//ユーザー辞書保存
#define REQ_BUSHU			L'b'	//部首合成変換
#ifdef _DEBUG
#define REQ_DEBUGOUT_ON		L'['
#define REQ_DEBUGOUT_OFF	L']'
#endif
//reply
#define REP_OK				L'T'	//hit
#define REP_FALSE			L'F'	//nothing

#define CORVUSMGREXE		L"imtutmgr.exe"
#define CORVUSCNFEXE		L"imtutcnf.exe"
#ifndef _DEBUG
#define CORVUSKRNLOBJ		L"tsf-tutcode-"
#else
#define CORVUSKRNLOBJ		L"tsf-tutcode-debug-"
#endif
#define CORVUSMGRMUTEX		CORVUSKRNLOBJ L"mgr-"
#define CORVUSCNFMUTEX		CORVUSKRNLOBJ L"cnf-"
#define CORVUSMGRPIPE		L"\\\\.\\pipe\\" CORVUSKRNLOBJ

typedef struct {
	BYTE digest[16];
} MD5_DIGEST;

extern LPCWSTR RccsUTF16;
extern LPCWSTR WccsUTF16;
extern LPCWSTR RccsUTF8;
extern LPCWSTR WccsUTF8;
extern LPCWSTR RB;
extern LPCWSTR WB;

extern LPCWSTR fnconfigxml;	//設定
extern LPCWSTR fnuserdic;	//ユーザー辞書
extern LPCWSTR fnskkdic;	//取込SKK辞書
extern LPCWSTR fnskkidx;	//取込SKK辞書インデックス
extern LPCWSTR fninitlua;	//init.lua

#define DISPLAY_COLOR_NUM	8
#define DISPLAYATTRIBUTE_INFO_NUM	7

extern const TF_DISPLAYATTRIBUTE c_daDisplayAttributeInputMark;
extern const TF_DISPLAYATTRIBUTE c_daDisplayAttributeInputText;
extern const TF_DISPLAYATTRIBUTE c_daDisplayAttributeInputOkuri;
extern const TF_DISPLAYATTRIBUTE c_daDisplayAttributeConvMark;
extern const TF_DISPLAYATTRIBUTE c_daDisplayAttributeConvText;
extern const TF_DISPLAYATTRIBUTE c_daDisplayAttributeConvOkuri;
extern const TF_DISPLAYATTRIBUTE c_daDisplayAttributeConvAnnot;
extern const BOOL c_daDisplayAttributeSeries[DISPLAYATTRIBUTE_INFO_NUM];

BOOL IsVersion62AndOver();
BOOL IsVersion63AndOver();
BOOL GetSidMD5Digest(LPWSTR *ppszDigest);
BOOL GetMD5(MD5_DIGEST *digest, CONST BYTE *data, DWORD datalen);
BOOL GetUserSid(LPWSTR *ppszUserSid);
BOOL GetLogonSid(LPWSTR *ppszLogonSid);

#define FORWARD_ITERATION_I(iterator, container) \
	for(auto (iterator) = (container).begin(); (iterator) != (container).end(); ++(iterator))
#define FORWARD_ITERATION(iterator, container) \
	for(auto (iterator) = (container).begin(); (iterator) != (container).end(); )
#define REVERSE_ITERATION_I(reverse_iterator, container) \
	for(auto (reverse_iterator) = (container).rbegin(); (reverse_iterator) != (container).rend(); ++(reverse_iterator))
#define REVERSE_ITERATION(reverse_iterator, container) \
	for(auto (reverse_iterator) = (container).rbegin(); (reverse_iterator) != (container).rend(); )

template<class T> void SafeRelease(T **ppT)
{
	if(*ppT != NULL)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

#endif //COMMON_H
