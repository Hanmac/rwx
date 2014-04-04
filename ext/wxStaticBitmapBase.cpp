/*
 * wxStaticBitmapBase.cpp
 *
 *  Created on: 12.05.2012
 *      Author: hanmac
 */


#include "wxApp.hpp"
#include "wxStaticBitmap.hpp"
#include "wxBitmap.hpp"

VALUE rb_cWXStaticBitmapBase;

#if wxUSE_STATBMP
#define _self unwrapPtr<wxStaticBitmapBase>(self,rb_cWXStaticBitmapBase)

namespace RubyWX {
namespace StaticBitmapBase {

macro_attr(Bitmap,wxBitmap)

}
}
#endif
DLL_LOCAL void Init_WXStaticBitmapBase(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXStaticBitmapBase,"bitmap",1,1);
#endif
#if wxUSE_STATBMP
	using namespace RubyWX::StaticBitmapBase;
	rb_cWXStaticBitmapBase = rb_define_class_under(rb_mWX,"StaticBitmapBase",rb_cWXControl);

	rb_define_attr_method(rb_cWXStaticBitmapBase,"bitmap",_getBitmap,_setBitmap);

#endif

}



