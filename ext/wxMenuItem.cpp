/*
 * wxMenuItem.cpp
 *
 *  Created on: 18.02.2012
 *      Author: hanmac
 */

#include "wxWindow.hpp"
#include "wxApp.hpp"
#include "wxMenu.hpp"
#include "wxMenuItem.hpp"
#include "wxBitmap.hpp"



VALUE rb_cWXMenuItem;

#if wxUSE_MENUS

#define _self unwrap<wxMenuItem*>(self)

namespace RubyWX {
namespace MenuItem {

APP_PROTECT(wxMenuItem)

macro_attr(Menu,wxMenu*)
macro_attr_with_func(Id,wrapID,unwrapID)
macro_attr_enum(Kind,wxItemKind)
macro_attr(SubMenu,wxMenu*)

macro_attr(Help,wxString)

macro_attr_bitmap_func(Bitmap, WRAP_BITMAP_NULL, wxART_MENU, _self->GetKind() == wxITEM_NORMAL)

singlereturn(GetItemLabel)

DLL_LOCAL VALUE _setItemLabel(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxString cval(unwrap<wxString>(val));

	if((!_self->IsSeparator() || !wxIsStockID(_self->GetId())) && cval.empty()){
		rb_raise(rb_eArgError,"id '%" PRIsVALUE "' (%d) needs an text", RB_OBJ_STRING(_getId(self)), _self->GetId());
	}
	_self->SetItemLabel(cval);

	return val;
}

/* Document-method: initialize_copy
 * call-seq:
 *   initialize_copy(orig)
 *
 * Duplicate an object
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self,VALUE other)
{
	_setId(self,_getId(other));
	_setItemLabel(self,_GetItemLabel(other));
	_self->SetKind( unwrap<wxMenuItem*>(other)->GetKind() );
	_setSubMenu(self,_getSubMenu(other));
	_setHelp(self,_getHelp(other));
	if(_self->GetKind() == wxITEM_NORMAL)
		_setBitmap(self,_getBitmap(other));
	return self;
}


/*
 * call-seq:
 *   hash -> Fixnum
 *
 * Generates a Fixnum hash value for this object.
 *
 *
 */
DLL_LOCAL VALUE _getHash(VALUE self)
{
	st_index_t h = rb_hash_start(0);

	h = rb_hash_uint32(h, rb_str_hash(_GetItemLabel(self)));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getId(self))));
	h = rb_hash_uint(h, _self->GetKind());
	h = rb_hash_uint(h, rb_str_hash(_getHelp(self)));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getBitmap(self))));

	h = rb_hash_end(h);
	return LONG2FIX(h);
}

struct equal_obj {
	wxMenuItem* self;
	VALUE other;
};

VALUE _equal_block(equal_obj *obj)
{
	wxMenuItem *cother = unwrap<wxMenuItem*>(obj->other);

	if(obj->self->GetId() != cother->GetId())
		return Qfalse;

	if(obj->self->GetKind() != cother->GetKind())
		return Qfalse;

	if(obj->self->GetItemLabel() != cother->GetItemLabel())
		return Qfalse;

	if(obj->self->GetHelp() != cother->GetHelp())
		return Qfalse;

	if(obj->self->IsEnabled() != cother->IsEnabled())
		return Qfalse;
	
	if(obj->self->IsCheckable())
	{
		if(obj->self->IsChecked() != cother->IsChecked())
			return Qfalse;
	}

	return Qtrue;
}

VALUE _equal_rescue(VALUE val)
{
	return Qfalse;
}

/*
 * call-seq:
 *   == item -> bool
 *
 * compares two menu items.
 *
 *
 */
DLL_LOCAL VALUE _equal(VALUE self, VALUE other)
{
	equal_obj obj;
	obj.self = _self;
	obj.other = other;

	return rb_rescue(
		RUBY_METHOD_FUNC(_equal_block),(VALUE)&obj,
		RUBY_METHOD_FUNC(_equal_rescue),Qnil
	);
}

/*
 * call-seq:
 *   marshal_dump -> Array
 *
 * Provides marshalling support for use by the Marshal library.
 * ===Return value
 * Array
 */
DLL_LOCAL VALUE _marshal_dump(VALUE self)
{
	VALUE result = rb_ary_new();

	rb_ary_push(result,_getId(self));
	rb_ary_push(result,_GetItemLabel(self));
	rb_ary_push(result,INT2NUM(_self->GetKind()));
//	rb_ary_push(result,_getSubMenu(self));
	rb_ary_push(result,_getHelp(self));
	rb_ary_push(result,_getBitmap(self));
	return result;
}

/*
 * call-seq:
 *   marshal_load(array) -> nil
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self,VALUE data)
{
	data = rb_Array(data);

	_setId(self,RARRAY_AREF(data,0));
	_setItemLabel(self,RARRAY_AREF(data,1));
	_setKind(self,RARRAY_AREF(data,2));

	_setHelp(self,RARRAY_AREF(data,3));

	if(_self->GetKind() == wxITEM_NORMAL)
		_setBitmap(self,RARRAY_AREF(data,4));

	return self;
}


}
}

#endif

/*
 * Document-class: WX::Menu::Item
 *
 * This class represents an Item inside a WX::Menu
*/

/* Document-attr: menu
 * returns the parent menu, this attribute are not copied with inititalize_copy or stored with Marshal
 */
/* Document-attr: label
 * returns the label of the menu item. Type is String
 */
/* Document-attr: id
 * returns the Id of the menu item. Type can be Symbol,if pre- or user defined, or Integer if auto generated.
 */
/* Document-attr: sub_menu
 * returns the sub_menu, currently can be copied with inititalize_copy but not stored with Marshal
 */
/* Document-attr: help
 * returns the help string of the menu item. Type is String
 */
/* Document-attr: bitmap
 * returns the Bitmap
 */

DLL_LOCAL void Init_WXMenuItem(VALUE rb_mWX)
{
#if 0
	rb_cWXMenu = rb_define_class_under(rb_mWX,"Menu",rb_cObject);
#endif

#if wxUSE_MENUS
	using namespace RubyWX::MenuItem;
	//
	rb_cWXMenuItem = rb_define_class_under(rb_cWXMenu,"Item",rb_cObject);
	rb_define_alloc_func(rb_cWXMenuItem,_alloc);

#if 0
	rb_define_attr(rb_cWXMenuItem,"menu",1,1);
	rb_define_attr(rb_cWXMenuItem,"label",1,1);
	rb_define_attr(rb_cWXMenuItem,"id",1,1);

	rb_define_attr(rb_cWXMenuItem,"sub_menu",1,1);

	rb_define_attr(rb_cWXMenuItem,"help",1,1);

	rb_define_attr(rb_cWXMenuItem,"bitmap",1,1);

#endif

	rb_define_private_method(rb_cWXMenuItem,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXMenuItem,"menu",_getMenu,_setMenu);
	rb_define_attr_method(rb_cWXMenuItem,"label",_GetItemLabel,_setItemLabel);
	rb_define_attr_method(rb_cWXMenuItem,"id",_getId,_setId);

	rb_define_attr_method(rb_cWXMenuItem,"sub_menu",_getSubMenu,_setSubMenu);

	rb_define_attr_method(rb_cWXMenuItem,"help",_getHelp,_setHelp);

	rb_define_attr_method(rb_cWXMenuItem,"bitmap",_getBitmap,_setBitmap);

	rb_define_method(rb_cWXMenuItem,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXMenuItem,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	rb_define_method(rb_cWXMenuItem,"hash",RUBY_METHOD_FUNC(_getHash),0);

	rb_define_method(rb_cWXMenuItem,"==",RUBY_METHOD_FUNC(_equal),1);
	rb_define_alias(rb_cWXMenuItem,"eql?","==");

	registerInfo<wxMenuItem>(rb_cWXMenuItem);

	registerEnum<wxItemKind>("wxItemKind", Qnil)
		->add(wxITEM_SEPARATOR, "separator")
		->add(wxITEM_NORMAL, "normal")
		->add(wxITEM_CHECK, "check")
		->add(wxITEM_RADIO, "radio")
		->add(wxITEM_DROPDOWN, "dropdown");

#endif
}
