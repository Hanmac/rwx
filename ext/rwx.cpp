/*
 * rwx.cpp
 *
 *  Created on: 07.02.2012
 *      Author: hanmac
 */

#include "wxError.hpp"

#include "wxSize.hpp"
#include "wxColor.hpp"

#include "wxImage.hpp"
#include "wxBitmap.hpp"

#include "wxEvent.hpp"
#include "wxEvtHandler.hpp"

#include "wxMenu.hpp"
#include "wxMenuItem.hpp"

#include "wxApp.hpp"
#include "wxWindow.hpp"

VALUE rb_mWX;


extern "C" void Init_rwx()
{
	//wxInitialize();


	rb_mWX = rb_define_module("WX");

	Init_WXError(rb_mWX);

	Init_WXColor(rb_mWX);

	Init_WXImage(rb_mWX);
	Init_WXBitmap(rb_mWX);

	Init_WXSize(rb_mWX);

	Init_WXEvent(rb_mWX);

	Init_WXEvtHandler(rb_mWX);

	Init_WXMenu(rb_mWX);
	Init_WXMenuItem(rb_mWX);

	Init_WXApp(rb_mWX);

	Init_WXWindow(rb_mWX);
	Init_WXTopLevel(rb_mWX);
	Init_WXFrame(rb_mWX);
	Init_WXDialog(rb_mWX);
}
