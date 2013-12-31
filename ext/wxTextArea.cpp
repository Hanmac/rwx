/*
 * wxTextArea.cpp
 *
 *  Created on: 30.12.2013
 *      Author: hanmac
 */

#include "wxTextArea.hpp"
#include "wxTextAttr.hpp"
#include "wxTextCtrl.hpp"
#include "wxSearchCtrl.hpp"

VALUE rb_mWXTextArea;

#if wxUSE_TEXTCTRL

template <>
wxTextAreaBase* unwrap< wxTextAreaBase* >(const VALUE &obj)
{
	if(rb_obj_is_kind_of(obj,rb_cWXTextCtrl))
		return unwrap<wxTextCtrl*>(obj);
#if	wxUSE_SEARCHCTRL
	if(rb_obj_is_kind_of(obj,rb_cWXSearchCtrl))
		return unwrap<wxSearchCtrl*>(obj);
#endif
	rb_raise(rb_eTypeError,"Unknown wxTextAreaBase: %s",rb_obj_classname(obj));
return NULL;
}

#define _self unwrap<wxTextAreaBase*>(self)

namespace RubyWX {
namespace TextArea {

macro_attr_bool(Modified)

macro_attr(DefaultStyle,wxTextAttr)

VALUE _each_line_size(VALUE self)
{
	return INT2NUM(_self->GetNumberOfLines());
}

VALUE _each_line(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,_each_line_size);

	int count = _self->GetNumberOfLines();
	for(int i = 0; i < count; ++i)
	{
		rb_yield(wrap(_self->GetLineText(i)));
	}

	return self;
}

VALUE _load_file(VALUE self,VALUE file)
{
	//TODO add file.exist & permission check

	return wrap(_self->LoadFile(unwrap<wxString>(file)));
}

VALUE _save_file(VALUE self,VALUE file)
{
	//TODO add file.exist & permission check

	return wrap(_self->SaveFile(unwrap<wxString>(file)));
}


}
}

#endif

DLL_LOCAL void Init_WXTextArea(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_mWXTextArea,"modified",1,1);
	rb_define_attr(rb_mWXTextArea,"default_style",1,1);
#endif

#if wxUSE_TEXTCTRL
	using namespace RubyWX::TextArea;
	rb_mWXTextArea = rb_define_module_under(rb_mWX,"TextArea");
	rb_define_method(rb_mWXTextArea,"each_line",RUBY_METHOD_FUNC(_each_line),0);

	rb_define_attr_method(rb_mWXTextArea,"modified",_getModified,_setModified);
	rb_define_attr_method(rb_mWXTextArea,"default_style",_getDefaultStyle,_setDefaultStyle);
#endif

}



