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
 *   BitmapButton.new(parent, [options])
 *
 * creates a new BitmapButton widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 * * *bitmap WX::Bitmap
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!_created) {
		wxWindowID id(wxID_ANY);
		wxBitmap bitmap(wxNullBitmap);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			VALUE temp;
			set_hash_option(hash,"id",id,unwrapID);

			temp = rb_hash_aref(hash,ID2SYM(rb_intern("bitmap")));
			bitmap = wrapBitmap(temp,id,WRAP_BITMAP_ID,wxART_BUTTON);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,bitmap);
		_created = true;
	}

	rb_call_super(argc,argv);
	return self;
}

}
}

#endif

DLL_LOCAL void Init_WXBitmapButton(VALUE rb_mWX)
{
#if wxUSE_BMPBUTTON
	using namespace RubyWX::BitmapButton;
	rb_cWXBitmapButton = rb_define_class_under(rb_mWX,"BitmapButton",rb_cWXButton);
	rb_define_alloc_func(rb_cWXBitmapButton,_alloc);

	rb_define_method(rb_cWXBitmapButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxBitmapButton>(rb_cWXBitmapButton);
#endif
}

