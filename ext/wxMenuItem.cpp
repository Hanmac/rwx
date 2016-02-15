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

macro_attr_bitmap_func(Bitmap, WRAP_BITMAP_NULL, wxART_MENU, _self->GetKind() == wxITEM_NORMAL)

#define macro_menuitem_attr_get(name, get, gettool, con) \
DLL_LOCAL VALUE _get##name(VALUE self)\
{ \
	if(con) { \
		wxMenu *menu = _self->GetMenu();\
		int id = _self->GetId();\
		if(menu && menu->FindItem(id)) {\
			return wrap(menu->gettool(id));\
		}\
		return wrap(_self->get());\
	}else {\
		return Qnil;\
	}\
}\



#define macro_menuitem_attr_func(name, type, get, set, gettool, settool, con) \
macro_menuitem_attr_get(name, get, gettool, con) \
DLL_LOCAL VALUE _set##name(VALUE self,VALUE other)\
{\
	rb_check_frozen(self);\
	if(con) { \
		wxMenu *menu = _self->GetMenu();\
		if(menu) {\
			int id = _self->GetId();\
			if(menu->FindItem(id)) {\
				menu->settool(id, unwrap<type>(other));\
				return other;\
			}\
		}\
		_self->set(unwrap<type>(other));\
	}\
	return other;\
}

macro_menuitem_attr_func(Help, wxString, GetHelp, SetHelp, GetHelpString, SetHelpString, true)
macro_menuitem_attr_func(Enabled, bool, IsEnabled, Enable, IsEnabled, Enable, true)
macro_menuitem_attr_func(Checked, bool, IsChecked, Check, IsChecked, Check, _self->GetMenu() && _self->IsCheckable())

macro_menuitem_attr_get(ItemLabel, GetItemLabel, GetLabel, true)

DLL_LOCAL VALUE _setItemLabel(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxString cval(unwrap<wxString>(val));

	wxWindowID id = _self->GetId();

	if((!_self->IsSeparator() || !wxIsStockID(id)) && cval.empty()){
		rb_raise(rb_eArgError,"id '%" PRIsVALUE "' (%d) needs an text", RB_OBJ_STRING(_getId(self)), id);
	}

	wxMenu *menu = _self->GetMenu();

	if(menu && menu->FindItem(id)) {
		menu->SetLabel(id, cval);

	} else {
		_self->SetItemLabel(cval);
	}

	return val;
}

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE hash;
	rb_scan_args(argc, argv, "00:",&hash);

	wxMenu *parentMenu = NULL;
	int id = wxID_SEPARATOR;
	wxString text = wxEmptyString;
	wxString help = wxEmptyString;
	wxItemKind kind = wxITEM_NORMAL;
	wxMenu *subMenu = NULL;

	if(!NIL_P(hash))
	{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"text",text);
			set_hash_option(hash,"help",help);

			set_hash_option(hash,"kind",kind,unwrapenum<wxItemKind>);

			set_hash_option(hash,"help",help);
			set_hash_option(hash,"parent",parentMenu);
			set_hash_option(hash,"sub",subMenu);
	}

	RTYPEDDATA_DATA(self) = wxMenuItemBase::New(parentMenu,id, text, help, kind, subMenu);

	return self;
}

/* Document-method: initialize_copy
 * call-seq:
 *   initialize_copy(orig)
 *
 * Duplicate an object
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self,VALUE other)
{
	_self->SetKind( unwrap<wxMenuItem*>(other)->GetKind() );

	_setId(self,_getId(other));
	VALUE val = _getItemLabel(other);

	if(RSTRING_LEN(rb_String(val)) != 0)
		_setItemLabel(self,val);

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

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getId(self))));
	h = rb_hash_uint(h, _self->GetKind());
	h = rb_hash_uint32(h, rb_str_hash(_getItemLabel(self)));
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
	/*
	if(obj->self->IsCheckable() && cother->IsCheckable())
	{
		if(obj->self->IsChecked() != cother->IsChecked())
			return Qfalse;
	}
	//*/

	return Qtrue;
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

	rb_ary_push(result,INT2NUM(_self->GetKind()));
	rb_ary_push(result,_getId(self));
	rb_ary_push(result,_getItemLabel(self));
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

	_setKind(self,RARRAY_AREF(data,0));

	_setId(self,RARRAY_AREF(data,1));
	VALUE val = RARRAY_AREF(data,2);

	if(RSTRING_LEN(rb_String(val)) != 0)
		_setItemLabel(self,val);


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

	rb_define_attr(rb_cWXMenuItem,"kind",1,1);

	rb_define_attr(rb_cWXMenuItem,"menu",1,1);
	rb_define_attr(rb_cWXMenuItem,"label",1,1);
	rb_define_attr(rb_cWXMenuItem,"id",1,1);

	rb_define_attr(rb_cWXMenuItem,"sub_menu",1,1);

	rb_define_attr(rb_cWXMenuItem,"help",1,1);
	rb_define_attr(rb_cWXMenuItem,"enabled",1,1);
	rb_define_attr(rb_cWXMenuItem,"checked",1,1);

	rb_define_attr(rb_cWXMenuItem,"bitmap",1,1);

#endif

	rb_define_method(rb_cWXMenuItem,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_private_method(rb_cWXMenuItem,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXMenuItem,"kind",_getKind,_setKind);

	rb_define_attr_method(rb_cWXMenuItem,"menu",_getMenu,_setMenu);
	rb_define_attr_method(rb_cWXMenuItem,"label",_getItemLabel,_setItemLabel);
	rb_define_attr_method(rb_cWXMenuItem,"id",_getId,_setId);

	rb_define_attr_method(rb_cWXMenuItem,"sub_menu",_getSubMenu,_setSubMenu);

	rb_define_attr_method(rb_cWXMenuItem,"help",_getHelp,_setHelp);
	rb_define_attr_method(rb_cWXMenuItem,"enabled",_getEnabled,_setEnabled);
	rb_define_attr_method(rb_cWXMenuItem,"checked",_getChecked,_setChecked);

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
