//==============================================================================
//							 NativeHook.cpp
//							   
// begin	: 2016/12/8
// describe	: 本地钩子
//==============================================================================

#include <QSettings>
#include <QDebug>
#include <Windows.h>
#include "NativeHook.h"

#define HOOK_PASS_ON		0	// 继续传递钩子
#define HOOK_THROW_AWAY		1	// 丢弃钩子

#define TASKMANAGER	"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
#define DISTASKMGR	"DisableTaskMgr"

HHOOK g_keyHook;	// 键盘钩子
HWND g_hTray;		// 系统任务栏

// 键盘钩子处理函数
LRESULT CALLBACK keyHookProc(int nCode,WPARAM wParam,LPARAM lParam );

// 隐藏任务栏
void HideTray(bool bState);
void DisableTaskMgr(bool bState);

//==============================================================================
//
//							  CNativeHook
//							   本地化钩子
//
//==============================================================================

CNativeHook::CNativeHook()
{
	g_hTray = FindWindow(L"Shell_TrayWnd",NULL);

	SetHook();
	HideTray(true);
	DisableTaskMgr(true);
}

CNativeHook::~CNativeHook()
{
	CancelHook();
	HideTray(false);
	DisableTaskMgr(false);
}

void CNativeHook::SetHook()
{
	//底层键盘钩子
	g_keyHook =SetWindowsHookEx( WH_KEYBOARD_LL, keyHookProc, GetModuleHandle(NULL), 0);
}

void CNativeHook::CancelHook()
{
	UnhookWindowsHookEx(g_keyHook);
}

//键盘钩子过程
LRESULT CALLBACK keyHookProc(int nCode, WPARAM wParam, LPARAM lParam )
{
	//在WH_KEYBOARD_LL模式下lParam 是指向KBDLLHOOKSTRUCT类型地址
	KBDLLHOOKSTRUCT* pKeyboardHook = (KBDLLHOOKSTRUCT *) lParam;
	//如果nCode等于HC_ACTION则处理该消息，如果小于0，则钩子子程就必须将该消息传递给 CallNextHookEx
	if(nCode == HC_ACTION)
	{
		switch(pKeyboardHook->vkCode)
		{
		case VK_ESCAPE:
			if(GetAsyncKeyState(VK_CONTROL) & 0x8000)
			{
				qDebug() << "Ctrl+Esc";
				return HOOK_THROW_AWAY;
			}
			break;
		case VK_TAB:
			if(pKeyboardHook->flags & LLKHF_ALTDOWN)
			{
				qDebug() << "Alt+Tab";
				return HOOK_THROW_AWAY;
			}
			break;
		case VK_LWIN:
		case VK_RWIN:
			qDebug() << "LWIN/RWIN";
			return HOOK_THROW_AWAY;
		case VK_F8:
			UnhookWindowsHookEx(g_keyHook);
			DisableTaskMgr(false);
			HideTray(false);
			break;
		default:
			break;
		}
	}
	return CallNextHookEx(g_keyHook, nCode, wParam, lParam);
}

void DisableTaskMgr(bool bState)
{
	return;

	QSettings objSettings(TASKMANAGER, QSettings::NativeFormat);
	if(bState)
	{
		objSettings.setValue(DISTASKMGR, "1");
	}
	else
	{
		objSettings.remove(DISTASKMGR);
	}
}

// 隐藏
void HideTray(bool bState)
{
	return;

	if(bState)
	{
		ShowWindow(g_hTray, SW_HIDE);
	}
	else
	{
		ShowWindow(g_hTray, SW_SHOW);
	}
}

