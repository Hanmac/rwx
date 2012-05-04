/*
 * wxItemContainer.cpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */


#include "wxItemContainer.hpp"

VALUE rb_mWXItemContainer;
#if wxUSE_CONTROLS
#define _self wrap<wxItemContainer*>(self)

namespace RubyWX {
namespace ItemContainer {

macro_attr(Selection,int)
macro_attr(StringSelection,wxString)

singlefunc(Clear)
singlefunc(GetStrings)


VALUE _Append(VALUE self,VALUE items)
{
	_self->Append(wrap<wxArrayString>(items));
	return self;
}

VALUE _setItems(VALUE self,VALUE items)
{
	_self->Set(wrap<wxArrayString>(items));
	return items;
}

}
}
#endif

void Init_WXItemContainer(VALUE rb_mWX)
{
	using namespace RubyWX::ItemContainer;

#if wxUSE_CONTROLS
	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");

	rb_define_method(rb_mWXItemContainer,"clear",RUBY_METHOD_FUNC(_Clear),0);

	rb_define_method(rb_mWXItemContainer,"<<",RUBY_METHOD_FUNC(_Append),1);

	rb_define_attr_method(rb_mWXItemContainer,"items",_GetStrings,_setItems);
#endif
}

