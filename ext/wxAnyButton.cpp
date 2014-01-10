/*
 * wxAnyButton.cpp
 *
 *  Created on: 13.02.2012
 *      Author: hanmac
 */


#include "wxAnyButton.hpp"

VALUE rb_cWXAnyButton;

#ifdef wxHAS_ANY_BUTTON

#define _self unwrap<wxAnyButton*>(self)

namespace RubyWX {
namespace AnyButton {

macro_attr(BitmapLabel,wxBitmap)
macro_attr(BitmapPressed,wxBitmap)
macro_attr(BitmapDisabled,wxBitmap)
macro_attr(BitmapCurrent,wxBitmap)
macro_attr(BitmapFocus,wxBitmap)

macro_attr(BitmapSelected,wxBitmap)
macro_attr(BitmapHover,wxBitmap)

macro_attr(BitmapMargins,wxSize)

singlereturn(GetBitmap)

APP_PROTECT(wxAnyButton)


/*
 * call-seq:
 *
 *
 * creates a new AnyButton widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * bitmap_label WX::Bitmap
 *   * bitmap_pressed WX::Bitmap
 *   * bitmap_disabled WX::Bitmap
 *   * bitmap_current WX::Bitmap
 *   * bitmap_focus WX::Bitmap
 *   * bitmap_selected WX::Bitmap
 *   * bitmap_hover WX::Bitmap
 *   * bitmap_margins WX::Size
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	rb_call_super(argc,argv);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(bitmap_label,BitmapLabel,wxBitmap)
		set_option(bitmap_pressed,BitmapPressed,wxBitmap)
		set_option(bitmap_disabled,BitmapDisabled,wxBitmap)
		set_option(bitmap_current,BitmapCurrent,wxBitmap)
		set_option(bitmap_focus,BitmapFocus,wxBitmap)
		set_option(bitmap_selected,BitmapSelected,wxBitmap)
		set_option(bitmap_hover,BitmapHover,wxBitmap)

		set_option(bitmap_margins,BitmapMargins,wxSize)
	}

	return self;
}

}
}

#endif

/* Document-attr: bitmap_label
 * the WX::Bitmap of the button in normal state
 */
/* Document-attr: bitmap_pressed
 * the WX::Bitmap of the button in pressed state
 */
/* Document-attr: bitmap_disabled
 * the WX::Bitmap of the button in disabled state
 */
/* Document-attr: bitmap_current
 * the WX::Bitmap of the button in current state
 */
/* Document-attr: bitmap_focus
 * the WX::Bitmap of the button in focus state
 */
/* Document-attr: bitmap_selected
 * the WX::Bitmap of the button in selected state
 */
/* Document-attr: bitmap_hover
 * the WX::Bitmap of the button in hover state
 */

DLL_LOCAL void Init_WXAnyButton(VALUE rb_mWX)
{
#if 0
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXAnyButton,"bitmap_label",1,1);
	rb_define_attr(rb_cWXAnyButton,"bitmap_pressed",1,1);
	rb_define_attr(rb_cWXAnyButton,"bitmap_disabled",1,1);
	rb_define_attr(rb_cWXAnyButton,"bitmap_current",1,1);
	rb_define_attr(rb_cWXAnyButton,"bitmap_focus",1,1);
	rb_define_attr(rb_cWXAnyButton,"bitmap_selected",1,1);
	rb_define_attr(rb_cWXAnyButton,"bitmap_hover",1,1);

#endif

#ifdef wxHAS_ANY_BUTTON
	using namespace RubyWX::AnyButton;
	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXAnyButton);

	rb_define_method(rb_cWXAnyButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXAnyButton,"bitmap_label",_getBitmapLabel,_setBitmapLabel);
	rb_define_attr_method(rb_cWXAnyButton,"bitmap_pressed",_getBitmapPressed,_setBitmapPressed);
	rb_define_attr_method(rb_cWXAnyButton,"bitmap_disabled",_getBitmapDisabled,_setBitmapDisabled);
	rb_define_attr_method(rb_cWXAnyButton,"bitmap_current",_getBitmapCurrent,_setBitmapCurrent);
	rb_define_attr_method(rb_cWXAnyButton,"bitmap_focus",_getBitmapFocus,_setBitmapFocus);
	rb_define_attr_method(rb_cWXAnyButton,"bitmap_selected",_getBitmapSelected,_setBitmapSelected);
	rb_define_attr_method(rb_cWXAnyButton,"bitmap_hover",_getBitmapHover,_setBitmapHover);

	registerInfo<wxAnyButton>(rb_cWXAnyButton);
#endif
}


