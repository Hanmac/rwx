/*
 * wxItemContainer.cpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */


#include "wxItemContainer.hpp"

#include "wxListBox.hpp"
#include "wxChoice.hpp"
#include "wxComboBox.hpp"

VALUE rb_mWXItemContainer;
#if wxUSE_CONTROLS

//otherwise the casting is wrong
template <>
wxItemContainer* unwrap< wxItemContainer* >(const VALUE &obj)
{
	if(rb_obj_is_kind_of(obj,rb_cWXListBox))
		return unwrap<wxListBox*>(obj);
	if(rb_obj_is_kind_of(obj,rb_cWXChoice))
		return unwrap<wxChoice*>(obj);
	if(rb_obj_is_kind_of(obj,rb_cWXComboBox))
		return unwrap<wxComboBox*>(obj);
		
		return unwrapPtr<wxItemContainer>(obj,rb_mWXItemContainer);
 return NULL;
}

#define _self unwrap<wxItemContainer*>(self)

namespace RubyWX {
namespace ItemContainer {

macro_attr(Selection,int)
macro_attr(StringSelection,wxString)

singlefunc(Clear)

singlereturn(GetStrings)


VALUE _Append(VALUE self,VALUE items)
{
	
	_self->Append(unwrap<wxArrayString>(items));
	return self;
}

VALUE _setItems(VALUE self,VALUE items)
{
	_self->Set(unwrap<wxArrayString>(items));
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

	//registerType<wxItemContainer>(rb_mWXItemContainer);
#endif
}

