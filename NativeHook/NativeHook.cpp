//==============================================================================
//                           NativeHook.cpp
//                             
// begin    : 2016/12/8
// describe : 本地钩子
//==============================================================================

#include <QSettings>
#include <QDebug>
#include <Windows.h>
#include "NativeHook.h"

#define HOOK_PASS_ON        0   // 继续传递钩子
#define HOOK_THROW_AWAY     1   // 丢弃钩子

#define TASKMANAGER "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
#define DISTASKMGR  "DisableTaskMgr"

HHOOK g_hkeyHook;   // handle for keyboard hook
bool g_bLockState;  // 屏蔽状态(system backdoor)

// Keyboard Hook process function
LRESULT CALLBACK keyHookProc(int nCode,WPARAM wParam,LPARAM lParam );

// function declaration
void HideTray(bool bState);
void DisableTaskMgr(bool bState);

//==============================================================================
//
//                            CNativeHook
//                             本地化钩子
//
//==============================================================================

CNativeHook::CNativeHook()
{
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
    g_hkeyHook =SetWindowsHookEx(WH_KEYBOARD_LL, keyHookProc, GetModuleHandle(NULL), 0);
}

void CNativeHook::CancelHook()
{
    UnhookWindowsHookEx(g_hkeyHook);
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
            if(GetAsyncKeyState(VK_CONTROL) & 0x8000
                || pKeyboardHook->flags & LLKHF_ALTDOWN
                )
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
        case VK_F4:
            if(pKeyboardHook->flags & LLKHF_ALTDOWN)
            {
                qDebug() << "Alt+F4";
                return HOOK_THROW_AWAY;
            }
        case VK_LWIN:
        case VK_RWIN:
            qDebug() << "LWIN/RWIN";
            return HOOK_THROW_AWAY;
        case VK_F8:
            if(GetAsyncKeyState(VK_CONTROL)&0x8000 && GetAsyncKeyState(VK_SHIFT)&0x8000)
            {
                g_bLockState = true;
            }
            return HOOK_PASS_ON;
            break;
        case VK_F11:
            if(GetAsyncKeyState(VK_CONTROL)&0x8000 && GetAsyncKeyState(VK_SHIFT)&0x8000)
            {
                if(g_bLockState)
                {
                    UnhookWindowsHookEx(g_hkeyHook);
                    DisableTaskMgr(false);
                    HideTray(false);
                }
                return HOOK_PASS_ON;
            }
            break;
        default:
            break;
        }
        g_bLockState = false;
    }
    return CallNextHookEx(g_hkeyHook, nCode, wParam, lParam);
}

// 禁用任务管理器
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

// 隐藏系统托盘
void HideTray(bool bState)
{
    return;
    HWND hTray = FindWindow(L"Shell_TrayWnd", NULL);    // 系统任务栏

    if(bState)
    {
        ShowWindow(hTray, SW_HIDE);
    }
    else
    {
        ShowWindow(hTray, SW_SHOW);
    }
}

