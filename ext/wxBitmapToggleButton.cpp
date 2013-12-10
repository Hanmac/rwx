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

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!_created) {
		wxWindowID id = wxID_ANY;
		wxBitmap bitmap = wxNullBitmap;

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			VALUE temp;
			if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("id")))))
				id = unwrapID(temp);

			temp = rb_hash_aref(hash,ID2SYM(rb_intern("bitmap")));
			bitmap = wrapBitmap(temp,id,true,wxART_BUTTON);
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

DLL_LOCAL void Init_WXBitmapToggleButton(VALUE rb_mWX)
{
#if wxUSE_TOGGLEBTN & defined(wxHAS_BITMAPTOGGLEBUTTON)
	using namespace RubyWX::BitmapToggleButton;
	rb_cWXBitmapToggleButton = rb_define_class_under(rb_mWX,"BitmapToggleButton",rb_cWXToggleButton);
	rb_define_alloc_func(rb_cWXBitmapToggleButton,_alloc);

	rb_define_method(rb_cWXBitmapToggleButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxBitmapToggleButton>(rb_cWXBitmapToggleButton);

#endif
}
