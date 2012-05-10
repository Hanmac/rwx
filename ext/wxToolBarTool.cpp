/*
 * wxToolBarTool.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */

#include "wxBitmap.hpp"

#include "wxToolBarBase.hpp"
#include "wxToolBarTool.hpp"

#define _self wrap<wxToolBarToolBase*>(self)

VALUE rb_cWXToolBarTool;


namespace RubyWX {
namespace ToolBarTool {

macro_attr(Label,wxString)
macro_attr(ShortHelp,wxString)
macro_attr(LongHelp,wxString)

#if wxUSE_MENUS
macro_attr(DropdownMenu,wxMenu*)
#endif

singlereturn(GetToolBar)
singlereturn(GetBitmap)
singlereturn(GetNormalBitmap)
singlereturn(GetDisabledBitmap)

VALUE _SetNormalBitmap(VALUE self,VALUE val)
{
	_self->SetNormalBitmap(wrapBitmap(val,_self->GetId(),false,wxART_TOOLBAR));
	return val;
}

VALUE _SetDisabledBitmap(VALUE self,VALUE val)
{
	_self->SetDisabledBitmap(wrapBitmap(val,_self->GetId(),true,wxART_TOOLBAR));
	return val;
}


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
	rb_cWXToolBarTool = rb_define_class_under(rb_cWXToolBarBase,"Tool",rb_cObject);
	rb_define_alloc_func(rb_cWXToolBarTool,_alloc);

	rb_define_method(rb_cWXToolBarTool,"control",RUBY_METHOD_FUNC(_getControl),0);

	rb_define_method(rb_cWXToolBarTool,"bitmap",RUBY_METHOD_FUNC(_GetBitmap),0);

	rb_define_attr_method(rb_cWXToolBarTool,"normal_bitmap",_GetNormalBitmap,_SetNormalBitmap);
	rb_define_attr_method(rb_cWXToolBarTool,"disabled_bitmap",_GetDisabledBitmap,_SetDisabledBitmap);

	rb_define_attr_method(rb_cWXToolBarTool,"label",_getLabel,_setLabel);
	rb_define_attr_method(rb_cWXToolBarTool,"short_help",_getShortHelp,_setShortHelp);
	rb_define_attr_method(rb_cWXToolBarTool,"long_help",_getLongHelp,_setLongHelp);

#if wxUSE_MENUS
	rb_define_attr_method(rb_cWXToolBarTool,"dropdown_menu",_getDropdownMenu,_setDropdownMenu);
#endif

	registerType<wxToolBarToolBase>(rb_cWXToolBarTool);
}
