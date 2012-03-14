/*
 * rwx.cpp
 *
 *  Created on: 07.02.2012
 *      Author: hanmac
 */

#include "wxError.hpp"

#include "wxSize.hpp"
#include "wxColor.hpp"
#include "wxFont.hpp"

#include "wxCursor.hpp"

#include "wxImage.hpp"
#include "wxBitmap.hpp"

#include "wxEvent.hpp"
#include "wxEvtHandler.hpp"

#include "wxMenu.hpp"
#include "wxMenuItem.hpp"

#include "wxToolBarTool.hpp"

#include "wxApp.hpp"
#include "wxWindow.hpp"

#include "wxAboutDlg.hpp"
#include "wxStartUpTip.hpp"

#include "wxFileHistory.hpp"
#include "wxConfig.hpp"

VALUE rb_mWX;


extern "C" void Init_rwx()
{
	//wxInitialize();


	rb_mWX = rb_define_module("WX");

	Init_WXError(rb_mWX);

	Init_WXColor(rb_mWX);
	Init_WXFont(rb_mWX);

	Init_WXImage(rb_mWX);
	Init_WXBitmap(rb_mWX);

	Init_WXCursor(rb_mWX);

	Init_WXSize(rb_mWX);

	Init_WXEvent(rb_mWX);

	Init_WXEvtHandler(rb_mWX);
	Init_WXTimer(rb_mWX);

	Init_WXMenu(rb_mWX);
		Init_WXMenuItem(rb_mWX);

	Init_WXApp(rb_mWX);


	Init_WXWindow(rb_mWX);
	Init_WXTopLevel(rb_mWX);
	Init_WXFrame(rb_mWX);

	Init_WXDialog(rb_mWX);
	Init_WXColorDialog(rb_mWX);
	Init_WXFontDialog(rb_mWX);
	Init_WXFileDialog(rb_mWX);
	Init_WXDirDialog(rb_mWX);
	Init_WXProgressDialog(rb_mWX);

	Init_WXMenuBar(rb_mWX);

	Init_WXControl(rb_mWX);
	Init_WXToolBar(rb_mWX);
	Init_WXToolBarTool(rb_mWX);

	Init_WXTimePicker(rb_mWX);
	Init_WXColorPicker(rb_mWX);
	Init_WXFontPicker(rb_mWX);
	Init_WXFilePicker(rb_mWX);

	Init_WXHyperLink(rb_mWX);
	Init_WXStaticBitmap(rb_mWX);

	Init_WXTreeCtrl(rb_mWX);

	Init_WXRadioButton(rb_mWX);
	Init_WXStatusBar(rb_mWX);

	Init_WXGauge(rb_mWX);

	Init_WXAboutDlg(rb_mWX);
	Init_ShowTip(rb_mWX);

	Init_WXFileHistory(rb_mWX);
	Init_WXConfig(rb_mWX);
}
