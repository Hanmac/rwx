/*
 * wxSearchCtrl.cpp
 *
 *  Created on: 14.05.2012
 *      Author: hanmac
 */

#include "wxSearchCtrl.hpp"
#include "wxMenu.hpp"
#include "wxBitmap.hpp"
#include "wxBitmapButton.hpp"
#include "wxTextCtrl.hpp"
#include "wxTextArea.hpp"
#include "wxTextEntry.hpp"

VALUE rb_cWXSearchCtrl;

#if wxUSE_SEARCHCTRL
#define _self unwrap<wxSearchCtrl*>(self)

namespace RubyWX {
namespace SearchCtrl {

APP_PROTECT(wxSearchCtrl)

macro_attr(Value,wxString)

#if wxUSE_MENUS
macro_attr(Menu,wxMenu*)
#endif

singlereturn(IsSearchButtonVisible)
singlereturn(IsCancelButtonVisible)

DLL_LOCAL VALUE _setSearchButtonVisible(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->ShowSearchButton(RTEST(val));
	return self;
}

DLL_LOCAL VALUE _setCancelButtonVisible(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->ShowCancelButton(RTEST(val));
	return self;
}

/*
 * call-seq:
 *   SearchCtrl.new(parent, [options])
 *
 * creates a new SearchCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * menu WX::Menu
 *   * search true/false
 *   * cancel true/false
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;

		bool search(_self->IsSearchButtonVisible());
		bool cancel(_self->IsCancelButtonVisible());

		set_option(value,Value,wxString)
#if wxUSE_MENUS
		set_option(menu,Menu,wxMenu*)
#endif

		set_hash_option(hash,"search",search);
		set_hash_option(hash,"cancel",cancel);

		_self->ShowSearchButton(search);
		_self->ShowCancelButton(cancel);
	}
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXSearchCtrl(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_mWXTextArea = rb_define_module_under(rb_mWX,"TextArea");
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");

	rb_define_attr(rb_cWXSearchCtrl,"value",1,1);
	rb_define_attr(rb_cWXSearchCtrl,"menu",1,1);

	rb_define_attr(rb_cWXSearchCtrl,"search_button_visible",1,1);
	rb_define_attr(rb_cWXSearchCtrl,"cancel_button_visible",1,1);

#endif

#if wxUSE_SEARCHCTRL
	using namespace RubyWX::SearchCtrl;
	rb_cWXSearchCtrl = rb_define_class_under(rb_mWX,"SearchCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSearchCtrl,_alloc);

	rb_define_method(rb_cWXSearchCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXSearchCtrl,rb_mWXTextArea);
	rb_include_module(rb_cWXSearchCtrl,rb_mWXTextEntry);

	rb_define_attr_method(rb_cWXSearchCtrl,"value",_getValue,_setValue);
#if wxUSE_MENUS
	rb_define_attr_method(rb_cWXSearchCtrl,"menu",_getMenu,_setMenu);
#endif

	rb_define_attr_method(rb_cWXSearchCtrl,"search_button_visible",_IsSearchButtonVisible,_setSearchButtonVisible);
	rb_define_attr_method(rb_cWXSearchCtrl,"cancel_button_visible",_IsCancelButtonVisible,_setCancelButtonVisible);

	registerInfo<wxSearchCtrl>(rb_cWXSearchCtrl);

	registerEventType("searchctrl_cancel_btn",wxEVT_SEARCHCTRL_CANCEL_BTN);
	registerEventType("searchctrl_cancel_btn",wxEVT_SEARCHCTRL_SEARCH_BTN);


#endif

}
