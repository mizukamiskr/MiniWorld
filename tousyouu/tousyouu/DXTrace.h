﻿
#ifndef DXTRACE_H
#define DXTRACE_H

#include "WinMin.h"
#include <Windows.h>


HRESULT WINAPI DXTraceW(_In_z_ const WCHAR* strFile, _In_ DWORD dwLine, _In_ HRESULT hr, _In_opt_ const WCHAR* strMsg, _In_ bool bPopMsgBox);


// ------------------------------
// HR宏
// ------------------------------
// Debug模式下的错误提醒与追踪
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
    {															\
        HRESULT hr = (x);										\
        if(FAILED(hr))											\
        {														\
            DXTraceW(__FILEW__, (DWORD)__LINE__, hr, L#x, true);\
        }														\
    }
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif 
#endif



#endif
