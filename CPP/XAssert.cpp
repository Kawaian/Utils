// Copyright(c) 2017 Matyas Yves Constans. All rights reserved.
// Info: Win32 implementation of an assert dialog
#include "XAssert.hpp"

#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
// #include <stdlib.h> // abort

// -- Enable visual styles
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// -- Win32 stuff, based on cryenigne's assert
#define IDD_DIALOG_ASSERT               101

#define IDC_ASSERT_STATIC_TEXT          0
#define IDC_ASSERT_EDIT_LINE            1000
#define IDC_ASSERT_EDIT_FILE            1001
#define IDC_ASSERT_EDIT_CONDITION       1002
#define IDC_ASSERT_EDIT_REASON          1003
#define IDC_ASSERT_BUTTON_CONTINUE      1004
#define IDC_ASSERT_BUTTON_STOP          1005
#define IDC_ASSERT_BUTTON_BREAK         1006

#define DLG_ASSERT_TITLE                L"Assertion Failed"
#define DLG_ASSERT_FONT                 L"Consolas"
#define DLG_ASSERT_ITEM_TEXT_0          L"Continue"
#define DLG_ASSERT_ITEM_TEXT_1          L"Stop"
#define DLG_ASSERT_ITEM_TEXT_2          L"Info"
#define DLG_ASSERT_ITEM_TEXT_3          L""
#define DLG_ASSERT_ITEM_TEXT_4          L"Line"
#define DLG_ASSERT_ITEM_TEXT_5          L""
#define DLG_ASSERT_ITEM_TEXT_6          L"File"
#define DLG_ASSERT_ITEM_TEXT_7          L""
#define DLG_ASSERT_ITEM_TEXT_8          L"Condition"
#define DLG_ASSERT_ITEM_TEXT_9          L"failed!"
#define DLG_ASSERT_ITEM_TEXT_10         L""
#define DLG_ASSERT_ITEM_TEXT_11         L"Reason"
#define DLG_ASSERT_ITEM_TEXT_12         L"Break"

#define DLG_ASSERT_ITEM_COUNT           13
#define CREATE_DIALOGITEM(TEXT, NAME) 	::DialogItem<sizeof(TEXT) / 2> NAME;

namespace {

    template <int TitleSize>
    struct DialogItem
    {
        DWORD style;
        DWORD dwExtStyle;

        short  x;
        short  y;
        short  cx;
        short  cy;

        WORD        id;
        WORD        ch;
        WORD        c;
        WCHAR       title[TitleSize];
        
        // padding
        WORD        dummy;
    };

    struct DialogData
    {
        DLGTEMPLATE dlt;
        WORD        menu;
        WORD        wndClass;

        WCHAR       title[sizeof(DLG_ASSERT_TITLE) / 2];
        WORD        pointSize;
        WCHAR       font[sizeof(DLG_ASSERT_FONT) / 2];

        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_0,   i0)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_1,   i1)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_12,  i12)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_2,   i2)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_3,   i3)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_4,   i4)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_5,   i5)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_6,   i6)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_7,   i7)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_8,   i8)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_9,   i9)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_10,  i10)
        CREATE_DIALOGITEM(DLG_ASSERT_ITEM_TEXT_11,  i11)
    };

    static DialogData g_dialogResource =
    {
        { DS_SETFOREGROUND | DS_MODALFRAME | DS_3DLOOK | DS_SETFONT | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0, DLG_ASSERT_ITEM_COUNT, 0, 0, 380, 134 },
        0, 0, DLG_ASSERT_TITLE, 8, DLG_ASSERT_FONT,
        
        // -- Items
        { BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 0, 7,   113, 50, 14, IDC_ASSERT_BUTTON_CONTINUE, 0xFFFF, 0x0080, DLG_ASSERT_ITEM_TEXT_0, 0 },
        { BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 0, 60, 113, 50, 14, IDC_ASSERT_BUTTON_STOP, 0xFFFF, 0x0080, DLG_ASSERT_ITEM_TEXT_1, 0 },
        { BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 0, 110, 113, 50, 14, IDC_ASSERT_BUTTON_BREAK, 0xFFFF, 0x0080, DLG_ASSERT_ITEM_TEXT_12, 0 },
        
        { BS_GROUPBOX | WS_CHILD | WS_VISIBLE, 0, 7, 7, 366, 100, IDC_ASSERT_STATIC_TEXT, 0xFFFF, 0x0080, DLG_ASSERT_ITEM_TEXT_2, 0 },
        
        { ES_LEFT | ES_AUTOHSCROLL | ES_READONLY | WS_BORDER | WS_CHILD | WS_VISIBLE, 0, 60, 48, 25, 13, IDC_ASSERT_EDIT_LINE, 0xFFFF, 0x0081, DLG_ASSERT_ITEM_TEXT_3, 0 },
        { WS_CHILD | WS_VISIBLE, 0, 14, 50, 16, 8, IDC_ASSERT_STATIC_TEXT, 0xFFFF, 0x0082, DLG_ASSERT_ITEM_TEXT_4, 0},

        { ES_LEFT | ES_AUTOHSCROLL | ES_READONLY | WS_BORDER | WS_CHILD | WS_VISIBLE, 0, 60, 32, 270, 13, IDC_ASSERT_EDIT_FILE, 0xFFFF, 0x0081, DLG_ASSERT_ITEM_TEXT_5, 0 },
        { WS_CHILD | WS_VISIBLE, 0, 14, 34, 16, 8, IDC_ASSERT_STATIC_TEXT, 0xFFFF, 0x0082, DLG_ASSERT_ITEM_TEXT_6, 0 },

        { ES_LEFT | ES_AUTOHSCROLL | ES_READONLY | WS_BORDER | WS_CHILD | WS_VISIBLE, 0, 60, 16, 270, 13, IDC_ASSERT_EDIT_CONDITION, 0xFFFF, 0x0081, DLG_ASSERT_ITEM_TEXT_7, 0 },
        { WS_CHILD | WS_VISIBLE, 0, 13, 18, 36, 8, IDC_ASSERT_STATIC_TEXT, 0xFFFF, 0x0082, DLG_ASSERT_ITEM_TEXT_8, 0 },
        { WS_CHILD | WS_VISIBLE, 0, 335, 19, 27, 8, IDC_ASSERT_STATIC_TEXT, 0xFFFF, 0x0082, DLG_ASSERT_ITEM_TEXT_9, 0 },

        { ES_LEFT | ES_AUTOHSCROLL | ES_READONLY | WS_BORDER | WS_CHILD | WS_VISIBLE, 0, 60, 64, 270, 13, IDC_ASSERT_EDIT_REASON, 0xFFFF, 0x0081, DLG_ASSERT_ITEM_TEXT_10, 0 },
        { WS_CHILD | WS_VISIBLE, 0, 14, 69, 26, 8, IDC_ASSERT_STATIC_TEXT, 0xFFFF, 0x0082, DLG_ASSERT_ITEM_TEXT_11, 0 }
    };

    struct AssertInfo
    {
        const char *reason;
        const char *condition_str;
        const char *filename;
        const char *line;
        xassert::Option option;
    };

    static INT_PTR CALLBACK AssertDlgProc(HWND hdlg, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        static thread_local AssertInfo *assertInfo = nullptr;

        switch (msg)
        {
        case WM_INITDIALOG:
        {
            assertInfo = (AssertInfo*)lparam;

            SetWindowText(GetDlgItem(hdlg, IDC_ASSERT_EDIT_CONDITION), assertInfo->condition_str);
            SetWindowText(GetDlgItem(hdlg, IDC_ASSERT_EDIT_FILE), assertInfo->filename);
            SetWindowText(GetDlgItem(hdlg, IDC_ASSERT_EDIT_LINE), assertInfo->line);
            SetWindowText(GetDlgItem(hdlg, IDC_ASSERT_EDIT_REASON), assertInfo->reason);

        } break;

        case WM_COMMAND:
        {
            switch (LOWORD(wparam))
            {
            case IDC_ASSERT_BUTTON_CONTINUE:
                assertInfo->option = xassert::Option::Continue;
                EndDialog(hdlg, 0);
                break;
            case IDC_ASSERT_BUTTON_STOP:
                assertInfo->option = xassert::Option::Stop;
                EndDialog(hdlg, 0);
                break;
            case IDC_ASSERT_BUTTON_BREAK:
                assertInfo->option = xassert::Option::Break;
                EndDialog(hdlg, 0);
                break;
            }
        } break;

        case WM_DESTROY:
        {
        } break;

        case WM_CLOSE:
        {
            EndDialog(hdlg, 0);
        } break;

        default:
            return FALSE;
        }
        return TRUE;
    }
}

namespace xassert {

    namespace detail {
        thread_local char g_assertDebugBreak;
    }

    void Assert(const char* msg, const char* condition_str, const char *filename, const char *line)
    {
        detail::g_assertDebugBreak = false;
        AssertInfo assertInfo { msg, condition_str, filename, line, Option::Stop };

        MessageBeep(MB_ICONERROR);
        DialogBoxIndirectParam(GetModuleHandle(NULL), (DLGTEMPLATE*)&g_dialogResource, GetDesktopWindow(), AssertDlgProc, (LPARAM)&assertInfo);
        
        switch (assertInfo.option)
        {
        case Option::Continue:
            return; // Just continue execution
        case Option::Stop:
            ExitProcess(0); // Stop execution
        case Option::Break:
            detail::g_assertDebugBreak = true; // Create a breakpoint
            return;
        default:
            break;
        }
    }

} // namespace xassert