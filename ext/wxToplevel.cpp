/*
 * wxToplevel.cpp
 *
 *  Created on: 05.02.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxBitmap.hpp"

VALUE rb_cWXTopLevel;

#define _self wrap<wxTopLevelWindow*>(self)

namespace RubyWX {
namespace TopLevel {

macro_attr(Title,wxString)
macro_attr(Icon,wxIcon)
macro_attr(DefaultItem,wxWindow*)
macro_attr(TmpDefaultItem,wxWindow*)


VALUE _alloc(VALUE self)
{
	return wrap(new wxTopLevelWindow,self);
}


VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("title")))))
			_self->SetTitle(wrap<wxString>(temp));

	}
	rb_call_super(argc,argv);
	return self;
}


}
}

void Init_WXTopLevel(VALUE rb_mWX)
{
	using namespace RubyWX::TopLevel;
	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_define_alloc_func(rb_cWXTopLevel,_alloc);

	rb_define_method(rb_cWXTopLevel,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXTopLevel, "title",_getTitle,_setTitle);
	rb_define_attr_method(rb_cWXTopLevel, "icon",_getIcon,_setIcon);


	rb_define_const(rb_cWXTopLevel,"STAY_ON_TOP",INT2NUM(wxSTAY_ON_TOP));
	rb_define_const(rb_cWXTopLevel,"ICONIZE",INT2NUM(wxICONIZE));
	rb_define_const(rb_cWXTopLevel,"MINIMIZE",INT2NUM(wxMINIMIZE));
	rb_define_const(rb_cWXTopLevel,"MAXIMIZE",INT2NUM(wxMAXIMIZE));
	rb_define_const(rb_cWXTopLevel,"CLOSE_BOX",INT2NUM(wxCLOSE_BOX));

	rb_define_const(rb_cWXTopLevel,"SYSTEM_MENU",INT2NUM(wxSYSTEM_MENU));
	rb_define_const(rb_cWXTopLevel,"MINIMIZE_BOX",INT2NUM(wxMINIMIZE_BOX));
	rb_define_const(rb_cWXTopLevel,"MAXIMIZE_BOX",INT2NUM(wxMAXIMIZE_BOX));

	rb_define_const(rb_cWXTopLevel,"TINY_CAPTION",INT2NUM(wxTINY_CAPTION));
	rb_define_const(rb_cWXTopLevel,"RESIZE_BORDER",INT2NUM(wxRESIZE_BORDER));

}
