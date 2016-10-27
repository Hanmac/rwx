/*
 * wxBitmapToggleButton.cpp
 *
 *  Created on: 10.12.2013
 *      Author: hanmac
 */

#include "wxBitmapToggleButton.hpp"
#include "wxBitmap.hpp"

VALUE rb_cWXBitmapToggleButton;
#if wxUSE_TOGGLEBTN & defined(wxHAS_BITMAPTOGGLEBUTTON)
#define _self unwrap<wxBitmapToggleButton*>(self)

namespace RubyWX {
namespace BitmapToggleButton {

APP_PROTECT(wxBitmapToggleButton)


/*
 * call-seq:
 *   BitmapToggleButton.new(parent, name, [options])
 *   BitmapToggleButton.new(parent, [options])
 *
 * creates a new BitmapToggleButton widget.
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

DLL_LOCAL void Init_WXBitmapToggleButton(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_cWXToggleButton = rb_define_class_under(rb_mWX,"ToggleButton",rb_cWXAnyButton);
#endif
#if wxUSE_TOGGLEBTN & defined(wxHAS_BITMAPTOGGLEBUTTON)
	using namespace RubyWX::BitmapToggleButton;
	rb_cWXBitmapToggleButton = rb_define_class_under(rb_mWX,"BitmapToggleButton",rb_cWXToggleButton);
	rb_define_alloc_func(rb_cWXBitmapToggleButton,_alloc);

	rb_define_method(rb_cWXBitmapToggleButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxBitmapToggleButton>(rb_cWXBitmapToggleButton);

#endif
}
