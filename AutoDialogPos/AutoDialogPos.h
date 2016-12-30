//==============================================================================
//                  AutoDialogPos.h
// 
// begin    : 2016-11-29
// describe : auto move dialog to proper position.
//==============================================================================

#ifndef AUTODIALOGPOS_H
#define AUTODIALOGPOS_H

class QWidget;

//==============================================================================
//
//                            CAutoDialogPos
//                          自动Qt调整对话框的位置
//
//==============================================================================

class CAutoDialogPos
{
public:
    // 自动将控件显示并移动到鼠标所在屏幕的中央, bResize表示该对话框在显示时会有自动大小适配
    static void AdjustPos(QWidget* pWidget, bool bResize=false);
    // 自动将控件显示并移动到合适的位置, nOffset表示该该设备图元的估算大小, bResize表示该对话框在显示时会有自动大小适配
    static void AdjustPos(QWidget* pWidget, int nOffset, bool bResize=false);
    
private:
    // 功能性函数接口，不需要实例化
    CAutoDialogPos();
public:
    ~CAutoDialogPos();
};

#endif  // AUTODIALOGPOS_H

