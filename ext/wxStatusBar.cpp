/*
 * wxStatusBar.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */


#include "wxWindow.hpp"


VALUE rb_cWXStatusBar;

#if wxUSE_STATUSBAR
#define _self unwrap<wxStatusBar*>(self)

namespace RubyWX {
namespace StatusBar {

APP_PROTECT(wxStatusBar)


macro_attr(StatusText,wxString)
macro_attr(FieldsCount,int)


VALUE _getStatusText2(VALUE self,VALUE num)
{
	if(NUM2INT(num) < _self->GetFieldsCount())
		return wrap(_self->GetStatusText(NUM2INT(num)));
	return Qnil;
}

VALUE _setStatusText2(VALUE self,VALUE str,VALUE num)
{
	if(NUM2INT(num) < _self->GetFieldsCount())
		_self->SetStatusText(unwrap<wxString>(str),NUM2INT(num));
	//const_cast<wxStatusBarPane&>(_self->GetField(NUM2INT(num))).SetText(unwrap<wxString>(str));
	//_self->Update();
	return Qnil;
}


}
}
#endif
DLL_LOCAL void Init_WXStatusBar(VALUE rb_mWX)
{
#if wxUSE_STATUSBAR
	using namespace RubyWX::StatusBar;
	rb_cWXStatusBar = rb_define_class_under(rb_mWX,"StatusBar",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStatusBar,_alloc);

	rb_define_attr_method(rb_cWXStatusBar,"status_text",_getStatusText,_setStatusText);
	rb_define_attr_method(rb_cWXStatusBar,"fields_count",_getFieldsCount,_setFieldsCount);

	rb_define_method(rb_cWXStatusBar,"get_status_text",RUBY_METHOD_FUNC(_getStatusText2),1);
	rb_define_method(rb_cWXStatusBar,"set_status_text",RUBY_METHOD_FUNC(_setStatusText2),2);

	registerInfo<wxStatusBar>(rb_cWXStatusBar);
#endif

}




