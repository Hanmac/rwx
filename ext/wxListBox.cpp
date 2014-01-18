/*
 * wxListBox.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxListBox.hpp"
#include "wxItemContainer.hpp"

VALUE rb_cWXListBox;

#if wxUSE_LISTBOX

namespace RubyWX {
namespace ListBox {
#define _self unwrap<wxListBox*>(self)

APP_PROTECT(wxListBox)

/*
 * call-seq:
 *   ListBox.new(parent, name, [options])
 *   ListBox.new(parent, [options])
 *
 * creates a new ListBox widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxArrayString choices;
		int style(0);
		int selection(-1);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"selection",selection);
		}
		_self->Create(
			unwrap<wxWindow*>(parent),id,
			wxDefaultPosition,wxDefaultSize,
			choices,style
		);
		
		_self->SetSelection(selection);

	}
	rb_call_super(argc,argv);
	return self;
}

DLL_LOCAL VALUE _getSelections(VALUE self)
{
	wxArrayInt data;
	_self->GetSelections(data);
	return wrap(data);
}

}
}

#endif
DLL_LOCAL void Init_WXListBox(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");
#endif
#if wxUSE_LISTBOX
	using namespace RubyWX::ListBox;
	rb_cWXListBox = rb_define_class_under(rb_mWX,"ListBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXListBox,_alloc);

	rb_define_method(rb_cWXListBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXListBox,rb_mWXItemContainer);

	rb_define_method(rb_cWXListBox,"selections",RUBY_METHOD_FUNC(_getSelections),0);

	registerEventType("listbox", wxEVT_LISTBOX,rb_cWXCommandEvent);
	registerEventType("listbox_dclick",  wxEVT_LISTBOX_DCLICK,rb_cWXCommandEvent);

	registerInfo<wxListBox>(rb_cWXListBox);
#endif

}


