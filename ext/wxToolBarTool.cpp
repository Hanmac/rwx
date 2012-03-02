/*
 * wxToolBarTool.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */

#include "wxBitmap.hpp"

#include "wxWindow.hpp"
#include "wxToolBarTool.hpp"

#define _self wrap<wxToolBarToolBase*>(self)

VALUE rb_cWXToolBarTool;


namespace RubyWX {
namespace ToolBarTool {

macro_attr(NormalBitmap,wxBitmap)
macro_attr(DisabledBitmap,wxBitmap)

macro_attr(Label,wxString)
macro_attr(ShortHelp,wxString)
macro_attr(LongHelp,wxString)

#if wxUSE_MENUS
macro_attr(DropdownMenu,wxMenu*)
#endif

singlereturn(GetToolBar)
singlereturn(GetBitmap)


VALUE _getControl(VALUE self)
{
	if(_self->IsControl())
		return wrap(_self->GetControl());
	return Qnil;
}


VALUE _alloc(VALUE self)
{

	return wrap(new wxToolBarToolBase);
}

}
}


void Init_WXToolBarTool(VALUE rb_mWX)
{
	using namespace RubyWX::ToolBarTool;
	//rb_cWXMenu = rb_define_class_under(rb_mWX,"Menu",rb_cObject);
	rb_cWXToolBarTool = rb_define_class_under(rb_cWXToolBar,"Tool",rb_cObject);
	rb_define_alloc_func(rb_cWXToolBarTool,_alloc);

	rb_define_method(rb_cWXToolBarTool,"control",RUBY_METHOD_FUNC(_getControl),0);
}
