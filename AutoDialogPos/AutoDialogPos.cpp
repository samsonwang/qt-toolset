//==============================================================================
//							 AutoDialogPos.cpp
//							   
//begin	  : 2016/11/29
//describe: 自动调整对话框的位置
//==============================================================================
#include <cassert>
#include <QApplication>
#include <QDesktopWidget>
#include "AutoDialogPos.h"

#define SCREEN_OFFSET_H		25		// 对话框与屏幕左右边的最小边界位置
#define SCREEN_OFFSET_V		50		// 对话框与屏幕上下边的最小边界位置
#define WIDGET_TITLE_OFFSET 35		// 对话框的标题栏估算高度

//==============================================================================
//
//							  CAutoDialogPos
//							自动Qt调整对话框的位置
//
//==============================================================================

void CAutoDialogPos::AdjustPos(QWidget* pWidget, bool bResize)
{
	if(NULL == pWidget)
	{
		return;
	}

	// 获取鼠标所在屏幕的坐标
	QPoint objCursorPoint = pWidget->cursor().pos();

	// 获取鼠标所在屏幕的尺寸
	QDesktopWidget* pDesktop = QApplication::desktop();
	if(NULL == pDesktop)
	{
		return;
	}
	QRect objScreenRect = pDesktop->screenGeometry(objCursorPoint);
	
	// 如果界面会自动变化，那么界面在显示的时候其尺寸才是有效的
	if(bResize)
	{
		pWidget->show();
	}
	// 获取控件的尺寸
	QSize objWidgetSize = pWidget->size();

	// 计算最后的需要移动到的屏幕坐标
	int nPosX = (objScreenRect.width()-objWidgetSize.width())/2 + objScreenRect.x();
	int nPosY = (objScreenRect.height()-WIDGET_TITLE_OFFSET-objWidgetSize.height())/2 + objScreenRect.y();

	pWidget->move(nPosX, nPosY);
}

void CAutoDialogPos::AdjustPos(QWidget* pWidget, int nOffset, bool bResize)
{
	if(NULL == pWidget)
	{
		return;
	}

	// 获取鼠标所在屏幕的坐标
	QPoint objCursorPoint = pWidget->cursor().pos();

	// 获取鼠标所在屏幕的尺寸
	QDesktopWidget* pDesktop = QApplication::desktop();
	if(NULL == pDesktop)
	{
		return;
	}
	QRect objScreenRect = pDesktop->screenGeometry(objCursorPoint);

	if(bResize)
	{
		// 如果界面会自动变化，那么界面在显示的时候其尺寸才是有效的
		pWidget->show();
	}
	// 获取控件的尺寸
	QSize objWidgetSize = pWidget->size();

	// 计算最后的需要移动到的屏幕坐标
	int nPosX=0;
	int nPosY=0;

	// 开始根据鼠标的位置和控件的大小计算空间的显示坐标
	if(objCursorPoint.x()+nOffset+objWidgetSize.width()+SCREEN_OFFSET_H < objScreenRect.x()+objScreenRect.width())
	{
		// 该对话框可以显示在右边
		nPosX = objCursorPoint.x()+nOffset;
	}
	else if(objCursorPoint.x()-nOffset-objWidgetSize.width()-SCREEN_OFFSET_H > objScreenRect.x())
	{
		// 该对话框可以显示在左边
		nPosX = objCursorPoint.x()-nOffset-objWidgetSize.width();
	}
	else
	{
		// 该对话框太大了，显示在右边，但是会出现遮挡
		nPosX = objScreenRect.x() + (objScreenRect.width()-objWidgetSize.width()-SCREEN_OFFSET_H);
	}

	// 计算y轴坐标
	if(objWidgetSize.height() > objScreenRect.height())
	{
		// 该控件的高度大于屏幕能够显示的最大高度，这时对话框贴着顶端显示
		nPosY = SCREEN_OFFSET_V;
	}
	else if(objCursorPoint.y()+objWidgetSize.height()/2 >= objScreenRect.height()-SCREEN_OFFSET_V)
	{
		// 该窗口过大，显示超过屏幕底边界
		nPosY = objScreenRect.height() - SCREEN_OFFSET_V - objWidgetSize.height() - WIDGET_TITLE_OFFSET;
	}
	else if(objCursorPoint.y()-objWidgetSize.height()/2 <= SCREEN_OFFSET_V)
	{
		// 鼠标所在位置过高，显示超过屏幕顶边界
		nPosY = SCREEN_OFFSET_V;
	}
	else 
	{
		// 一般情况下该对话框中心会与鼠标所在位置同高
		nPosY = objCursorPoint.y()-objWidgetSize.height()/2;
	}

	pWidget->move(nPosX, nPosY);
}


CAutoDialogPos::CAutoDialogPos()
{

}

CAutoDialogPos::~CAutoDialogPos()
{

}
