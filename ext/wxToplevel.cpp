/*
 * wxToplevel.cpp
 *
 *  Created on: 05.02.2012
 *      Author: hanmac
 */

#include "wxWindow.hpp"
#include "wxBitmap.hpp"

VALUE rb_cWXTopLevel;

#define _self unwrap<wxTopLevelWindow*>(self)

namespace RubyWX {
namespace TopLevel {

macro_attr(Title,wxString)
macro_attr(Icon,wxIcon)
macro_attr(DefaultItem,wxWindow*)
macro_attr(TmpDefaultItem,wxWindow*)

void set_style_flags(VALUE hash,int& flags)
{

	set_hash_flag_option(hash,"stay_on_top",wxSTAY_ON_TOP,flags);
	set_hash_flag_option(hash,"iconize",wxICONIZE,flags);
	set_hash_flag_option(hash,"minimize",wxMINIMIZE,flags);
	set_hash_flag_option(hash,"maximize",wxMAXIMIZE,flags);
	set_hash_flag_option(hash,"close_box",wxCLOSE_BOX,flags);
	set_hash_flag_option(hash,"system_menu",wxSYSTEM_MENU,flags);
	set_hash_flag_option(hash,"minimize_box",wxMINIMIZE_BOX,flags);
	set_hash_flag_option(hash,"maximize_box",wxMAXIMIZE_BOX,flags);
	set_hash_flag_option(hash,"tiny_caption",wxTINY_CAPTION,flags);
	set_hash_flag_option(hash,"resize_border",wxRESIZE_BORDER,flags);

}

//APP_PROTECT(wxTopLevelWindow)

/*
 * call-seq:
 *   TopLevel.new(parent, name, [options])
 *   TopLevel.new(parent, [options])
 *
 * creates a new Control widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * title String Shown in the TitleBar of framed Window
 *   * icon WX::Bitmap Shown in the TitleBar of framed Window
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash, "title", &wxTopLevelWindow::SetTitle,_self);
		set_obj_option(hash, "icon", &wxTopLevelWindow::SetIcon,_self);
	}
	return self;
}


}
}


/* Document-attr: title
 * returns the title of the TopLevel window. Type is String
 */

/* Document-attr: icon
 * returns the icon of the TopLevel window. Type is WX::Bitmap
 */


DLL_LOCAL void Init_WXTopLevel(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
#endif

	using namespace RubyWX::TopLevel;
	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_undef_alloc_func(rb_cWXTopLevel);

	rb_define_method(rb_cWXTopLevel,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#if 0
	rb_define_attr(rb_cWXTopLevel, "title",1,1);
	rb_define_attr(rb_cWXTopLevel, "icon",1,1);

	rb_define_attr(rb_cWXTopLevel, "default_item",1,1);
	rb_define_attr(rb_cWXTopLevel, "tmp_default_item",1,1);

#endif

	rb_define_attr_method(rb_cWXTopLevel, "title",_getTitle,_setTitle);
	rb_define_attr_method(rb_cWXTopLevel, "icon",_getIcon,_setIcon);

	rb_define_attr_method(rb_cWXTopLevel, "default_item",_getDefaultItem,_setDefaultItem);
	rb_define_attr_method(rb_cWXTopLevel, "tmp_default_item",_getTmpDefaultItem,_setTmpDefaultItem);


	rb_define_const(rb_cWXTopLevel,"STAY_ON_TOP",INT2NUM(wxSTAY_ON_TOP));
	rb_define_const(rb_cWXTopLevel,"ICONIZE",INT2NUM(wxICONIZE));
	rb_define_const(rb_cWXTopLevel,"MINIMIZE",INT2NUM(wxMINIMIZE));
	rb_define_const(rb_cWXTopLevel,"MAXIMIZE",INT2NUM(wxMAXIMIZE));
	rb_define_const(rb_cWXTopLevel,"CLOSE_BOX",INT2NUM(wxCLOSE_BOX));

	rb_define_const(rb_cWXTopLevel,"SYSTEM_MENU",INT2NUM(wxSYSTEM_MENU));
	rb_define_const(rb_cWXTopLevel,"MINIMIZE_BOX",INT2NUM(wxMINIMIZE_BOX));
	rb_define_const(rb_cWXTopLevel,"MAXIMIZE_BOX",INT2NUM(wxMAXIMIZE_BOX));

	rb_define_const(rb_cWXTopLevel,"TINY_CAPTION",INT2NUM(wxTINY_CAPTION));
	rb_define_const(rb_cWXTopLevel,"RESIZE_BORDER",INT2NUM(wxRESIZE_BORDER));

	registerInfo<wxTopLevelWindow>(rb_cWXTopLevel);
}
