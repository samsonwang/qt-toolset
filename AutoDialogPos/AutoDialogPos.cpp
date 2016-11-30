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

#define SCREEEN_OFFSET		15		// 对话框与屏幕的最小边界位置
#define WIDGET_TITLE_OFFSET 40		// 对话框的标题栏估算高度

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
	int nPosY = (objScreenRect.height()-objWidgetSize.height())/2 + objScreenRect.y();

	pWidget->move(nPosX, nPosY);
	pWidget->show();
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
	int nPosX = (objScreenRect.width()-objWidgetSize.width())/2 + objScreenRect.x();
	int nPosY = (objScreenRect.height()-objWidgetSize.height())/2 + objScreenRect.y();


	// 开始根据鼠标的位置和控件的大小计算空间的显示坐标
	if(objCursorPoint.x()+nOffset+objWidgetSize.width()+SCREEEN_OFFSET < objScreenRect.x()+objScreenRect.width())
	{
		// 该对话框可以显示在右边
		nPosX = objCursorPoint.x()+nOffset;
	}
	else if(objCursorPoint.x()-nOffset-objWidgetSize.width()-SCREEEN_OFFSET > objScreenRect.x())
	{
		// 该对话框可以显示在左边
		nPosX = objCursorPoint.x()-nOffset-objWidgetSize.width();
	}
	else
	{
		// 该对话框太大了，显示在右边，但是会出现遮挡
		nPosX = objScreenRect.x() + (objScreenRect.width()-objWidgetSize.width()-SCREEEN_OFFSET);
	}

	// 计算y轴坐标
	if(objWidgetSize.height() > objScreenRect.height())
	{
		nPosY = SCREEEN_OFFSET;
	}
	else if(objCursorPoint.y()+objWidgetSize.height()/2 >= objScreenRect.height()-SCREEEN_OFFSET)
	{
		nPosY = objScreenRect.height() - SCREEEN_OFFSET - objWidgetSize.height() - WIDGET_TITLE_OFFSET;
	}
	else if(objCursorPoint.y()-objWidgetSize.height()/2 <= SCREEEN_OFFSET)
	{
		nPosY = SCREEEN_OFFSET;
	}
	else 
	{
		nPosY = objCursorPoint.y()-objWidgetSize.height()/2;
	}

	pWidget->move(nPosX, nPosY);
	pWidget->show();
}


CAutoDialogPos::CAutoDialogPos()
{

}

CAutoDialogPos::~CAutoDialogPos()
{

}
