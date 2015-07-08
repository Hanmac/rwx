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
macro_attr(ItemLabel,wxString)
macro_attr_with_func(Id,wrapID,unwrapID)
macro_attr_enum(Kind,wxItemKind)
macro_attr(SubMenu,wxMenu*)

macro_attr(Help,wxString)

macro_attr_bitmap(Bitmap, WRAP_BITMAP_RAISE, wxART_MENU)

/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self,VALUE other)
{
	_setItemLabel(self,_getItemLabel(other));
	_setId(self,_getId(other));
	_setKind(self,_getKind(other));
	_setSubMenu(self,_getSubMenu(other));
	_setHelp(self,_getHelp(other));
	_setBitmap(self,_getBitmap(other));
	return self;
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

	rb_ary_push(result,_getItemLabel(self));
	rb_ary_push(result,_getId(self));
	rb_ary_push(result,_getKind(self));
//	rb_ary_push(result,_getSubMenu(self));
	rb_ary_push(result,_getHelp(self));
#if wxUSE_IMAGE
	rb_ary_push(result,_getBitmap(self));
#endif
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
	_setItemLabel(self,RARRAY_AREF(data,0));
	_setId(self,RARRAY_AREF(data,1));
	_setKind(self,RARRAY_AREF(data,2));

	_setHelp(self,RARRAY_AREF(data,3));
#if wxUSE_IMAGE
	_setBitmap(self,RARRAY_AREF(data,4));
#endif
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
 * returns the Bitmap, currently can be copied with inititalize_copy but only stored with Marshal when wxImage is available
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

	rb_define_private_method(rb_cWXMenuItem,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),0);

	rb_define_attr_method(rb_cWXMenuItem,"menu",_getMenu,_setMenu);
	rb_define_attr_method(rb_cWXMenuItem,"label",_getItemLabel,_setItemLabel);
	rb_define_attr_method(rb_cWXMenuItem,"id",_getId,_setId);

	rb_define_attr_method(rb_cWXMenuItem,"sub_menu",_getSubMenu,_setSubMenu);

	rb_define_attr_method(rb_cWXMenuItem,"help",_getHelp,_setHelp);

	rb_define_attr_method(rb_cWXMenuItem,"bitmap",_getBitmap,_setBitmap);

	rb_define_method(rb_cWXMenuItem,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXMenuItem,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),-2);


	registerInfo<wxMenuItem>(rb_cWXMenuItem);

#endif
}
