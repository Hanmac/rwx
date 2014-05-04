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

singlereturn(IsSingleLine)
singlereturn(IsMultiLine)

macro_attr_bool2(SearchButtonVisible,ShowSearchButton)
macro_attr_bool2(CancelButtonVisible,ShowCancelButton)

/*
 * call-seq:
 *   SearchCtrl.new(parent, name, [options])
 *   SearchCtrl.new(parent, [options])
 *
 * creates a new SearchCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * value String
 *   * menu WX::Menu
 *   * search true/false
 *   * cancel true/false
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxString value(wxEmptyString);

		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);

			set_hash_option(hash,"style",style);

			TextCtrl::set_style_flags(hash,style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,value,
			wxDefaultPosition,wxDefaultSize,style
		);
		
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;

		bool search;
		bool cancel;

		set_option(value,Value,wxString)
#if wxUSE_MENUS
		set_option(menu,Menu,wxMenu*)
#endif

		if(set_hash_option(hash,"search",search))
			_self->ShowSearchButton(search);

		if(set_hash_option(hash,"cancel",cancel))
			_self->ShowCancelButton(cancel);
	}
	return self;
}

}
}
#endif


/* Document-attr: value
 * the value of the SearchCtrl. String
 */

/* Document-attr: menu
 * the menu of the SearchCtrl. WX::Menu
 */

/* Document-attr: search_button_visible
 * shows if the search button is visible. true/false
 */
/* Document-attr: cancel_button_visible
 * shows if the cancel button is visible. true/false
 */

DLL_LOCAL void Init_WXSearchCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

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

	rb_define_method(rb_cWXSearchCtrl,"single_line?",RUBY_METHOD_FUNC(_IsSingleLine),0);
	rb_define_method(rb_cWXSearchCtrl,"multi_line?",RUBY_METHOD_FUNC(_IsMultiLine),0);


	rb_define_attr_method(rb_cWXSearchCtrl,"value",_getValue,_setValue);
#if wxUSE_MENUS
	rb_define_attr_method(rb_cWXSearchCtrl,"menu",_getMenu,_setMenu);
#endif

	rb_define_attr_method(rb_cWXSearchCtrl,"search_button_visible",_getSearchButtonVisible,_setSearchButtonVisible);
	rb_define_attr_method(rb_cWXSearchCtrl,"cancel_button_visible",_getCancelButtonVisible,_setCancelButtonVisible);

	registerInfo<wxSearchCtrl>(rb_cWXSearchCtrl);

	registerEventType("searchctrl_cancel_btn",wxEVT_SEARCHCTRL_CANCEL_BTN);
	registerEventType("searchctrl_cancel_btn",wxEVT_SEARCHCTRL_SEARCH_BTN);


#endif

}
