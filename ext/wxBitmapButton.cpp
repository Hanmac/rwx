/*
 * wxBitmapButton.cpp
 *
 *  Created on: 14.05.2012
 *      Author: hanmac
 */

#include "wxBitmapButton.hpp"

VALUE rb_cWXBitmapButton;
#if wxUSE_BMPBUTTON
#define _self unwrap<wxBitmapButton*>(self)

namespace RubyWX {
namespace BitmapButton {

APP_PROTECT(wxBitmapButton)

/*
 * call-seq:
 *   BitmapButton.new(parent, name, [options])
 *   BitmapButton.new(parent, [options])
 *
 * creates a new BitmapButton widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * bitmap WX::Bitmap
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxBitmap bitmap(wxNullBitmap);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			VALUE temp;
			set_hash_option(hash,"id",id,unwrapID);

			temp = rb_hash_aref(hash,RB_ID2SYM(rb_intern("bitmap")));
			bitmap = wrapBitmap(temp,id,WRAP_BITMAP_ID,wxART_BUTTON);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,bitmap);
		}
	}

	rb_call_super(argc,argv);
	return self;
}

}
}

#endif

DLL_LOCAL void Init_WXBitmapButton(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXControl);

	rb_cWXButton = rb_define_class_under(rb_mWX,"Button",rb_cWXAnyButton);
#endif

#if wxUSE_BMPBUTTON
	using namespace RubyWX::BitmapButton;
	rb_cWXBitmapButton = rb_define_class_under(rb_mWX,"BitmapButton",rb_cWXButton);
	rb_define_alloc_func(rb_cWXBitmapButton,_alloc);

	rb_define_method(rb_cWXBitmapButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxBitmapButton>(rb_cWXBitmapButton);
#endif
}

