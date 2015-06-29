﻿
#include "imcrvtip.h"
#include "TextService.h"
#include "EditSession.h"
#include "CandidateList.h"
#include "LanguageBar.h"
#include "InputModeWindow.h"
#include "resource.h"

#define TEXTSERVICE_LANGBARITEMSINK_COOKIE 0x54ab516b

// langbar menu items
static const struct {
	int inputmode;
	UINT id;
	DWORD flag;
	LPCWSTR text;
} menuItems[] =
{
	{im_hiragana,		IDM_HIRAGANA,		0, L"［かな］"},
	{im_katakana,		IDM_KATAKANA,		0, L"［カナ］"},
	{im_katakana_ank,	IDM_KATAKANA_ANK,	0, L"［－ｶﾅ］"},
	{im_jlatin,			IDM_JLATIN,			0, L"［全英］"},
	{im_ascii,			IDM_ASCII,			0, L"［SKK］"},
	{im_default,		IDM_DEFAULT,		0, L"［－－］"},
	{im_disable,		IDM_NONE,			TF_LBMENUF_SEPARATOR, L""},
	{im_disable,		IDM_CONFIG,			0, L"設定"},
	{im_disable,		IDM_NONE,			TF_LBMENUF_SEPARATOR, L""},
	{im_disable,		IDM_NONE,			0, L"キャンセル"}
};

// 16 colors (black only) icons
static const WORD iconIDX[] =
{
	IDI_X_DEFAULT, IDI_X_HIRAGANA, IDI_X_KATAKANA, IDI_X_KATAKANA1, IDI_X_JLATIN, IDI_X_ASCII
};
// png icons
static const WORD iconIDZ[] =
{
	IDI_Z_DEFAULT, IDI_Z_HIRAGANA, IDI_Z_KATAKANA, IDI_Z_KATAKANA1, IDI_Z_JLATIN, IDI_Z_ASCII
};

CLangBarItemButton::CLangBarItemButton(CTextService *pTextService, REFGUID guid)
{
	DllAddRef();

	_cRef = 1;

	_LangBarItemInfo.clsidService = c_clsidTextService;
	_LangBarItemInfo.guidItem = guid;
	// GDI handle leak occurs when GetIcon function returns monochrome icon if TF_LBI_STYLE_TEXTCOLORICON flag is set.
	// Use 16 colors (black only) icon instead of monochrome icon.
	_LangBarItemInfo.dwStyle = TF_LBI_STYLE_SHOWNINTRAY |
		(IsEqualGUID(_LangBarItemInfo.guidItem, GUID_LBI_INPUTMODE) ? TF_LBI_STYLE_BTN_BUTTON : TF_LBI_STYLE_BTN_MENU) |
		(IsWindowsVersion62OrLater() ? 0 : TF_LBI_STYLE_TEXTCOLORICON);	//16 colors (black only) icon used on earlier than Windows 8
	_LangBarItemInfo.ulSort = 1;
	wcsncpy_s(_LangBarItemInfo.szDescription, LangbarItemDesc, _TRUNCATE);

	_pLangBarItemSink = NULL;

	_pTextService = pTextService;
	_pTextService->AddRef();
}

CLangBarItemButton::~CLangBarItemButton()
{
	SafeRelease(&_pTextService);

	DllRelease();
}

STDAPI CLangBarItemButton::QueryInterface(REFIID riid, void **ppvObj)
{
	if(ppvObj == NULL)
	{
		return E_INVALIDARG;
	}

	*ppvObj = NULL;

	if(IsEqualIID(riid, IID_IUnknown) ||
		IsEqualIID(riid, IID_ITfLangBarItem) ||
		IsEqualIID(riid, IID_ITfLangBarItemButton))
	{
		*ppvObj = (ITfLangBarItemButton *)this;
	}
	else if(IsEqualIID(riid, IID_ITfSource))
	{
		*ppvObj = (ITfSource *)this;
	}

	if(*ppvObj)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDAPI_(ULONG) CLangBarItemButton::AddRef()
{
	return ++_cRef;
}

STDAPI_(ULONG) CLangBarItemButton::Release()
{
	if(--_cRef == 0)
	{
		delete this;
		return 0;
	}

	return _cRef;
}

STDAPI CLangBarItemButton::GetInfo(TF_LANGBARITEMINFO *pInfo)
{
	if(pInfo == NULL)
	{
		return E_INVALIDARG;
	}

	*pInfo = _LangBarItemInfo;

	return S_OK;
}

STDAPI CLangBarItemButton::GetStatus(DWORD *pdwStatus)
{
	if(pdwStatus == NULL)
	{
		return E_INVALIDARG;
	}

	if(_pTextService->_IsKeyboardDisabled())
	{
		*pdwStatus = TF_LBI_STATUS_DISABLED;
	}
	else
	{
		*pdwStatus = 0;
	}

	return S_OK;
}

STDAPI CLangBarItemButton::Show(BOOL fShow)
{
	if(_pLangBarItemSink == NULL)
	{
		return E_FAIL;
	}

	return _pLangBarItemSink->OnUpdate(TF_LBI_STATUS);
}

STDAPI CLangBarItemButton::GetTooltipString(BSTR *pbstrToolTip)
{
	BSTR bstrToolTip;

	if(pbstrToolTip == NULL)
	{
		return E_INVALIDARG;
	}

	*pbstrToolTip = NULL;

	bstrToolTip = SysAllocString(LangbarItemDesc);

	if(bstrToolTip == NULL)
	{
		return E_OUTOFMEMORY;
	}

	*pbstrToolTip = bstrToolTip;

	return S_OK;
}

STDAPI CLangBarItemButton::OnClick(TfLBIClick click, POINT pt, const RECT *prcArea)
{
	if(IsEqualGUID(_LangBarItemInfo.guidItem, GUID_LBI_INPUTMODE))
	{
		switch(click)
		{
		case TF_LBI_CLK_RIGHT:
			{
				HMENU hMenu = LoadMenuW(g_hInst, MAKEINTRESOURCEW(IDR_SYSTRAY_MENU));
				if(hMenu)
				{
					UINT check = IDM_DEFAULT;
					for(int i = 0; i < _countof(menuItems); i++)
					{
						if(_pTextService->inputmode == menuItems[i].inputmode)
						{
							check = menuItems[i].id;
							break;
						}
					}
					CheckMenuRadioItem(hMenu, IDM_HIRAGANA, IDM_DEFAULT, check, MF_BYCOMMAND);
					HMENU hSubMenu = GetSubMenu(hMenu, 0);
					if(hSubMenu)
					{
						TPMPARAMS tpm;
						TPMPARAMS *ptpm = NULL;
						if(prcArea != NULL)
						{
							tpm.cbSize = sizeof(tpm);
							tpm.rcExclude = *prcArea;
							ptpm = &tpm;
						}
						BOOL bRet = TrackPopupMenuEx(hSubMenu,
							TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_VERTICAL,
							pt.x, pt.y, GetFocus(), ptpm);
						this->OnMenuSelect(bRet);
					}
					DestroyMenu(hMenu);
				}
			}
			break;
		case TF_LBI_CLK_LEFT:
			{
				BOOL fOpen = _pTextService->_IsKeyboardOpen();
				if(!fOpen)
				{
					_pTextService->inputmode = im_disable;
				}
				else
				{
					_pTextService->_ClearComposition();
				}
				_pTextService->_SetKeyboardOpen(fOpen ? FALSE : TRUE);
			}
			break;
		default:
			break;
		}
	}

	return S_OK;
}

STDAPI CLangBarItemButton::InitMenu(ITfMenu *pMenu)
{
	if(pMenu == NULL)
	{
		return E_INVALIDARG;
	}

	for(int i = 0; i < _countof(menuItems); i++)
	{
		pMenu->AddMenuItem(menuItems[i].id, menuItems[i].flag |
			(_pTextService->inputmode == menuItems[i].inputmode ? TF_LBMENUF_RADIOCHECKED : 0),
			NULL, NULL, menuItems[i].text, (ULONG)wcslen(menuItems[i].text), NULL);
	}

	return S_OK;
}

STDAPI CLangBarItemButton::OnMenuSelect(UINT wID)
{
	BOOL fOpen = _pTextService->_IsKeyboardOpen();
	switch(wID)
	{
	case IDM_CONFIG:
		_pTextService->_StartConfigure();
		break;
	case IDM_HIRAGANA:
	case IDM_KATAKANA:
	case IDM_KATAKANA_ANK:
	case IDM_JLATIN:
	case IDM_ASCII:
		for(int i = 0; i < _countof(menuItems); i++)
		{
			if(wID == menuItems[i].id)
			{
				_pTextService->inputmode = menuItems[i].inputmode;
				if(!fOpen)
				{
					_pTextService->_SetKeyboardOpen(TRUE);
				}
				else
				{
					_pTextService->_ClearComposition();
					_pTextService->_UpdateLanguageBar();
				}
				break;
			}
		}
		break;
	case IDM_DEFAULT:
		_pTextService->inputmode = im_default;
		if(fOpen)
		{
			_pTextService->_ClearComposition();
			_pTextService->_SetKeyboardOpen(FALSE);
		}
		break;
	default:
		break;
	}

	return S_OK;
}

STDAPI CLangBarItemButton::GetIcon(HICON *phIcon)
{
	_GetIcon(phIcon, IsWindowsVersion62OrLater());

	return (*phIcon != NULL) ? S_OK : E_FAIL;
}

STDAPI CLangBarItemButton::GetText(BSTR *pbstrText)
{
	BSTR bstrText;

	if(pbstrText == NULL)
	{
		return E_INVALIDARG;
	}

	*pbstrText = NULL;

	bstrText = SysAllocString(LangbarItemDesc);

	if(bstrText == NULL)
	{
		return E_OUTOFMEMORY;
	}

	*pbstrText = bstrText;

	return S_OK;
}

STDAPI CLangBarItemButton::AdviseSink(REFIID riid, IUnknown *punk, DWORD *pdwCookie)
{
	if(!IsEqualIID(IID_ITfLangBarItemSink, riid))
	{
		return CONNECT_E_CANNOTCONNECT;
	}

	if(_pLangBarItemSink != NULL)
	{
		return CONNECT_E_ADVISELIMIT;
	}

	if(punk->QueryInterface(IID_PPV_ARGS(&_pLangBarItemSink)) != S_OK)
	{
		_pLangBarItemSink = NULL;
		return E_NOINTERFACE;
	}

	*pdwCookie = TEXTSERVICE_LANGBARITEMSINK_COOKIE;

	return S_OK;
}

STDAPI CLangBarItemButton::UnadviseSink(DWORD dwCookie)
{
	if(dwCookie != TEXTSERVICE_LANGBARITEMSINK_COOKIE)
	{
		return CONNECT_E_NOCONNECTION;
	}

	if(_pLangBarItemSink == NULL)
	{
		return CONNECT_E_NOCONNECTION;
	}

	SafeRelease(&_pLangBarItemSink);

	return S_OK;
}

STDAPI CLangBarItemButton::_Update()
{
	VARIANT var;

	var.vt = VT_I4;

	var.lVal = TF_SENTENCEMODE_PHRASEPREDICT;
	_pTextService->_SetCompartment(GUID_COMPARTMENT_KEYBOARD_INPUTMODE_SENTENCE, &var);

	if(!_pTextService->_IsKeyboardDisabled() && _pTextService->_IsKeyboardOpen())
	{
		switch(_pTextService->inputmode)
		{
		case im_hiragana:
			var.lVal = TF_CONVERSIONMODE_NATIVE | TF_CONVERSIONMODE_FULLSHAPE |
				TF_CONVERSIONMODE_ROMAN;
			_pTextService->_SetCompartment(GUID_COMPARTMENT_KEYBOARD_INPUTMODE_CONVERSION, &var);
			break;
		case im_katakana:
			var.lVal = TF_CONVERSIONMODE_NATIVE | TF_CONVERSIONMODE_FULLSHAPE |
				TF_CONVERSIONMODE_ROMAN | TF_CONVERSIONMODE_KATAKANA;
			_pTextService->_SetCompartment(GUID_COMPARTMENT_KEYBOARD_INPUTMODE_CONVERSION, &var);
			break;
		case im_katakana_ank:
			var.lVal = TF_CONVERSIONMODE_NATIVE |
				TF_CONVERSIONMODE_ROMAN | TF_CONVERSIONMODE_KATAKANA;
			_pTextService->_SetCompartment(GUID_COMPARTMENT_KEYBOARD_INPUTMODE_CONVERSION, &var);
			break;
		case im_jlatin:
			var.lVal = TF_CONVERSIONMODE_ALPHANUMERIC | TF_CONVERSIONMODE_FULLSHAPE;
			_pTextService->_SetCompartment(GUID_COMPARTMENT_KEYBOARD_INPUTMODE_CONVERSION, &var);
			break;
		case im_ascii:
			var.lVal = TF_CONVERSIONMODE_ALPHANUMERIC;
			_pTextService->_SetCompartment(GUID_COMPARTMENT_KEYBOARD_INPUTMODE_CONVERSION, &var);
			break;
		default:
			break;
		}
	}

	if(_pLangBarItemSink == NULL)
	{
		return E_FAIL;
	}

	return _pLangBarItemSink->OnUpdate(TF_LBI_ICON | TF_LBI_STATUS);
}

STDAPI CLangBarItemButton::_GetIcon(HICON *phIcon, BOOL bNT62)
{
	size_t iconindex = 0;
	WORD iconid = 0;

	if(!_pTextService->_IsKeyboardDisabled() && _pTextService->_IsKeyboardOpen())
	{
		switch(_pTextService->inputmode)
		{
		case im_hiragana:
			iconindex = 1;
			break;
		case im_katakana:
			iconindex = 2;
			break;
		case im_katakana_ank:
			iconindex = 3;
			break;
		case im_jlatin:
			iconindex = 4;
			break;
		case im_ascii:
			iconindex = 5;
			break;
		default:
			break;
		}
	}

	if(bNT62)
	{
		if(iconindex < _countof(iconIDZ))
		{
			iconid = iconIDZ[iconindex];
		}
	}
	else
	{
		if(iconindex < _countof(iconIDX))
		{
			iconid = iconIDX[iconindex];
		}
	}

	//DPIを考慮
	HDC hdc = GetDC(NULL);
	int dpi = GetDeviceCaps(hdc, LOGPIXELSY);
	ReleaseDC(NULL, hdc);
	int size = MulDiv(16, dpi, 96);

	*phIcon = (HICON)LoadImageW(g_hInst, MAKEINTRESOURCEW(iconid), IMAGE_ICON, size, size, LR_SHARED);

	return (*phIcon != NULL) ? S_OK : E_FAIL;
}

BOOL CTextService::_InitLanguageBar()
{
	ITfLangBarItemMgr *pLangBarItemMgr;
	BOOL fRet = FALSE;
	BOOL fRetI = FALSE;

	_pLangBarItem = NULL;
	_pLangBarItemI = NULL;

	if(_pThreadMgr->QueryInterface(IID_PPV_ARGS(&pLangBarItemMgr)) == S_OK)
	{
		try
		{
			_pLangBarItem = new CLangBarItemButton(this, c_guidLangBarItemButton);
			if(pLangBarItemMgr->AddItem(_pLangBarItem) == S_OK)
			{
				fRet = TRUE;
			}
			else
			{
				SafeRelease(&_pLangBarItem);
			}
		}
		catch(...)
		{
		}

		if(IsWindowsVersion62OrLater())
		{
			try
			{
				_pLangBarItemI = new CLangBarItemButton(this, GUID_LBI_INPUTMODE);
				if(pLangBarItemMgr->AddItem(_pLangBarItemI) == S_OK)
				{
					fRetI = TRUE;
				}
				else
				{
					SafeRelease(&_pLangBarItemI);
				}
			}
			catch(...)
			{
			}
		}
		else
		{
			fRetI = TRUE;
		}

		SafeRelease(&pLangBarItemMgr);
	}

	return (fRet && fRetI);
}

void CTextService::_UninitLanguageBar()
{
	ITfLangBarItemMgr *pLangBarItemMgr;

	if(_pLangBarItem != NULL)
	{
		if(_pThreadMgr->QueryInterface(IID_PPV_ARGS(&pLangBarItemMgr)) == S_OK)
		{
			pLangBarItemMgr->RemoveItem(_pLangBarItem);
			SafeRelease(&pLangBarItemMgr);
		}
	}
	SafeRelease(&_pLangBarItem);

	if(_pLangBarItemI != NULL)
	{
		if(_pThreadMgr->QueryInterface(IID_PPV_ARGS(&pLangBarItemMgr)) == S_OK)
		{
			pLangBarItemMgr->RemoveItem(_pLangBarItemI);
			SafeRelease(&pLangBarItemMgr);
		}
	}
	SafeRelease(&_pLangBarItemI);
}

void CTextService::_UpdateLanguageBar(BOOL showinputmode)
{
	if(_pLangBarItem != NULL)
	{
		_pLangBarItem->_Update();
	}

	if(_pLangBarItemI != NULL)
	{
		_pLangBarItemI->_Update();
	}

	if(_ShowInputMode && showinputmode &&
		(_pCandidateList == NULL || !_pCandidateList->_IsShowCandidateWindow()))
	{
		_StartInputModeWindow(FALSE);
	}
}

void CTextService::_GetIcon(HICON *phIcon)
{
	if(_pLangBarItem != NULL)
	{
		_pLangBarItem->_GetIcon(phIcon, FALSE);
	}
}

class CInputModeEditSession : public CEditSessionBase
{
public:
	CInputModeEditSession(CTextService *pTextService, ITfContext *pContext, CInputModeWindow *pInputModeWindow) : CEditSessionBase(pTextService, pContext)
	{
		_pTextService = pTextService;
		_pContext = pContext;
		_pInputModeWindow = pInputModeWindow;
	}

	// ITfEditSession
	STDMETHODIMP DoEditSession(TfEditCookie ec)
	{
		HRESULT hr;

		_pTextService->_SetText(ec, _pContext, std::wstring(L""), -1, 0, FALSE);

		ITfContextView *pContextView;
		if(_pContext->GetActiveView(&pContextView) == S_OK)
		{
			try
			{
				CIMGetTextExtEditSession *pEditSession =
					new CIMGetTextExtEditSession(_pTextService, _pContext, pContextView, _pInputModeWindow);
				_pContext->RequestEditSession(_pTextService->_GetClientId(), pEditSession, TF_ES_SYNC | TF_ES_READ, &hr);
				SafeRelease(&pEditSession);
			}
			catch(...)
			{
			}

			SafeRelease(&pContextView);
		}

		return S_OK;
	}

private:
	CTextService *_pTextService;
	ITfContext *_pContext;
	CInputModeWindow *_pInputModeWindow;
};

void CTextService::_StartInputModeWindow(BOOL term)
{
	HRESULT hr = E_FAIL;

	switch(inputmode)
	{
	case im_hiragana:
	case im_katakana:
	case im_katakana_ank:
	case im_jlatin:
	case im_ascii:
		_EndInputModeWindow();

		ITfDocumentMgr *pDocumentMgr;
		if(_pThreadMgr->GetFocus(&pDocumentMgr) == S_OK && pDocumentMgr != NULL)
		{
			ITfContext *pContext;
			if(pDocumentMgr->GetTop(&pContext) == S_OK && pContext != NULL)
			{
				try
				{
					_pInputModeWindow = new CInputModeWindow();
					_pInputModeWindow->_term = term;

					if(_pInputModeWindow->_Create(this, pContext, FALSE, NULL))
					{
						try
						{
							CInputModeEditSession *pEditSession = new CInputModeEditSession(this, pContext, _pInputModeWindow);
							// Asynchronous
							pContext->RequestEditSession(_ClientId, pEditSession, TF_ES_ASYNC | TF_ES_READWRITE, &hr);
							SafeRelease(&pEditSession);
						}
						catch(...)
						{
						}

						if(hr != TF_S_ASYNC)
						{
							_EndInputModeWindow();
						}
					}
				}
				catch(...)
				{
				}

				SafeRelease(&pContext);
			}
			SafeRelease(&pDocumentMgr);
		}
		break;
	default:
		break;
	}
}

void CTextService::_EndInputModeWindow()
{
	if(_pInputModeWindow != NULL)
	{
		_pInputModeWindow->_Destroy();
	}
	SafeRelease(&_pInputModeWindow);
}
