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
#if wxUSE_LISTBOX
	if(rb_obj_is_kind_of(obj,rb_cWXListBox))
		return unwrap<wxListBox*>(obj);
#endif
#if wxUSE_CHOICE
	if(rb_obj_is_kind_of(obj,rb_cWXChoice))
		return unwrap<wxChoice*>(obj);
#endif
#if wxUSE_COMBOBOX
	if(rb_obj_is_kind_of(obj,rb_cWXComboBox))
		return unwrap<wxComboBox*>(obj);
#endif
		return unwrapPtr<wxItemContainer>(obj,rb_mWXItemContainer);
 return NULL;
}

#define _self unwrap<wxItemContainer*>(self)

namespace RubyWX {
namespace ItemContainer {

macro_attr_selection(Selection,GetCount)
macro_attr(StringSelection,wxString)

singlefunc(Clear)

singlereturn(GetCount)
singlereturn(IsSorted)
singlereturn(GetStrings)

DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_GetCount));
	std::size_t count = _self->GetCount();
	for(std::size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->GetString(i)));
	return self;
}


/*
 * call-seq:
 *   append(items) -> self
 *
 * adds new items to item container.
 * ===Arguments
 * * string items added to item container [String]
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _Append(VALUE self,VALUE items)
{
	rb_check_frozen(self);
	_self->Append(unwrap<wxArrayString>(items));
	return self;
}

/*
 * call-seq:
 *   items(pos,items) -> self
 *
 * insert new items to item container at the given position.
 * ===Arguments
 * * pos where the item should be added.
 * * string items added to item container [String]
 * ===Return value
 * * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _Insert(VALUE self,VALUE idx,VALUE items)
{
	rb_check_frozen(self);

	if(!_self->IsSorted())
	{
		int cidx = NUM2INT(idx);
		if(check_index(cidx,_self->GetCount()+1))
			_self->Insert(unwrap<wxArrayString>(items),cidx);
	}else
		_self->Append(unwrap<wxArrayString>(items));

	return self;
}

/*
 * call-seq:
 *   prepend(items) -> self
 *
 * prepends new items to item container.
 * ===Arguments
 * * string items prepend to item container [String]
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _Prepend(VALUE self,VALUE items)
{
	rb_check_frozen(self);

	if(!_self->IsSorted())
		_self->Insert(unwrap<wxArrayString>(items),0);
	else
		_self->Append(unwrap<wxArrayString>(items));

	return self;
}


DLL_LOCAL VALUE _Delete(VALUE self,VALUE idx)
{
	rb_check_frozen(self);

	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		_self->Delete(cidx);
	return self;
}

DLL_LOCAL VALUE _setItems(VALUE self,VALUE items)
{
	rb_check_frozen(self);
	_self->Set(unwrap<wxArrayString>(items));
	return items;
}


/*
 * call-seq:
 *   get_item_string(pos) -> String
 *
 * returns the String of the item at the given position.
 * ===Arguments
 * * pos of the item. Integer
 * ===Return value
 * String
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _getItemString(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		return wrap(_self->GetString(cidx));
	return Qnil;
}

/*
 * call-seq:
 *   set_item_string(pos,text) -> self
 *
 * sets the String of the item at the given position.
 * ===Arguments
 * * pos of the item. Integer
 * * text of the item. String
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _setItemString(VALUE self,VALUE idx,VALUE val)
{
	rb_check_frozen(self);

	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		_self->SetString(cidx,unwrap<wxString>(val));

	return self;
}


}
}
#endif

DLL_LOCAL void Init_WXItemContainer(VALUE rb_mWX)
{

#if 0
	rb_define_attr(rb_mWXItemContainer,"selection",1,1);
	rb_define_attr(rb_mWXItemContainer,"string_selection",1,1);

	rb_define_attr(rb_mWXItemContainer,"items",1,1);
#endif

#if wxUSE_CONTROLS
	using namespace RubyWX::ItemContainer;

	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");

	rb_define_method(rb_mWXItemContainer,"clear",RUBY_METHOD_FUNC(_Clear),0);

	rb_define_method(rb_mWXItemContainer,"each_item",RUBY_METHOD_FUNC(_each),0);

	rb_define_method(rb_mWXItemContainer,"append",RUBY_METHOD_FUNC(_Append),1);
	rb_define_alias(rb_mWXItemContainer,"<<","append");

	rb_define_method(rb_mWXItemContainer,"insert",RUBY_METHOD_FUNC(_Insert),2);

	rb_define_method(rb_mWXItemContainer,"prepend",RUBY_METHOD_FUNC(_Prepend),1);

	rb_define_method(rb_mWXItemContainer,"delete_item",RUBY_METHOD_FUNC(_Delete),1);

	rb_define_method(rb_mWXItemContainer,"get_item_string",RUBY_METHOD_FUNC(_getItemString),1);
	rb_define_method(rb_mWXItemContainer,"set_item_string",RUBY_METHOD_FUNC(_setItemString),2);

	rb_define_method(rb_mWXItemContainer,"item_count",RUBY_METHOD_FUNC(_GetCount),0);
	rb_define_method(rb_mWXItemContainer,"sorted?",RUBY_METHOD_FUNC(_IsSorted),0);

	rb_define_attr_method(rb_mWXItemContainer,"selection",_getSelection,_setSelection);
	rb_define_attr_method(rb_mWXItemContainer,"string_selection",_getStringSelection,_setStringSelection);

	rb_define_attr_method(rb_mWXItemContainer,"items",_GetStrings,_setItems);

	//registerType<wxItemContainer>(rb_mWXItemContainer);
#endif
}

