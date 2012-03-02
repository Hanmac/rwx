/*
 * wxToplevel.cpp
 *
 *  Created on: 05.02.2012
 *      Author: hanmac
 */

#include "wxWindow.hpp"

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

}
}

void Init_WXTopLevel(VALUE rb_mWX)
{
	using namespace RubyWX::TopLevel;
	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_define_alloc_func(rb_cWXTopLevel,_alloc);

	rb_define_attr_method(rb_cWXTopLevel, "title",_getTitle,_setTitle);
}
