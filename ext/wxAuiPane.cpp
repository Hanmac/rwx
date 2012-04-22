/*
 * wxAuiPane.cpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */

#include "wxAuiPane.hpp"
#include "wxBitmap.hpp"

#define _self wrap<wxAuiPaneInfo*>(self)

VALUE rb_cWXAuiPane;


namespace RubyWX {
namespace AuiPane {

VALUE _alloc(VALUE self) {
	return wrap(new wxAuiPaneInfo);
}

VALUE _getCaption(VALUE self)
{
	return wrap(_self->caption);
}

VALUE _setCaption(VALUE self,VALUE val)
{
	_self->caption = wrap<wxString>(val);
	return val;
}

VALUE _getName(VALUE self)
{
	return wrap(_self->name);
}

VALUE _setName(VALUE self,VALUE val)
{
	_self->name = wrap<wxString>(val);
	return val;
}


VALUE _getIcon(VALUE self)
{
	return wrap(_self->icon);
}

VALUE _setIcon(VALUE self,VALUE val)
{
	_self->icon = wrap<wxBitmap>(val);
	return val;
}

VALUE _getDirection(VALUE self)
{
	return wrap<wxAuiManagerDock>(_self->dock_direction);
}

VALUE _setDirection(VALUE self,VALUE val)
{
	_self->dock_direction = wrap< wxAuiManagerDock >(val);
	return val;
}



VALUE _getFloatingPos(VALUE self)
{
	return wrap(_self->floating_pos);
}

VALUE _setFloatingPos(VALUE self,VALUE val)
{
	_self->floating_pos = wrap<wxPoint>(val);
	return val;
}


VALUE _getFloatingSize(VALUE self)
{
	return wrap(_self->floating_size);
}

VALUE _setFloatingSize(VALUE self,VALUE val)
{
	_self->floating_size = wrap<wxSize>(val);
	return val;
}

}
}

void Init_WXAuiPane(VALUE rb_mWX)
{
#if wxUSE_AUI
	using namespace RubyWX::AuiPane;
	rb_cWXAuiPane = rb_define_class_under(rb_mWX,"AuiPane",rb_cObject);
	rb_define_alloc_func(rb_cWXAuiPane,_alloc);
	//rb_define_method(rb_cWXAuiManager,"[]=",RUBY_METHOD_FUNC(_set),2);

	rb_define_attr_method(rb_cWXAuiPane,"caption",_getCaption,_setCaption);
	rb_define_attr_method(rb_cWXAuiPane,"name",_getName,_setName);
	rb_define_attr_method(rb_cWXAuiPane,"icon",_getIcon,_setIcon);
	rb_define_attr_method(rb_cWXAuiPane,"direction",_getDirection,_setDirection);

	rb_define_attr_method(rb_cWXAuiPane,"floating_pos",_getFloatingPos,_setFloatingPos);
	rb_define_attr_method(rb_cWXAuiPane,"floating_size",_getFloatingSize,_setFloatingSize);
#endif

}
