/*
 * wxRadioBox.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxRadioBox.hpp"

VALUE rb_cWXRadioBox;

#if wxUSE_RADIOBOX

namespace RubyWX {
namespace RadioBox {
#define _self unwrap<wxRadioBox*>(self)

APP_PROTECT(wxRadioBox)

macro_attr(Selection,int)
macro_attr(StringSelection,wxString)

/*
 * call-seq:
 *   RadioBox.new(parent, name, [options])
 *   RadioBox.new(parent, [options])
 *
 * creates a new RadioBox widget.
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
		wxString label;
		wxArrayString choices;
		int style(wxRA_SPECIFY_COLS);
		int selection(-1);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"label",label);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"selection",selection);
		}
		_self->Create(
			unwrap<wxWindow*>(parent),id,label,
			wxDefaultPosition,wxDefaultSize,
			choices,0,style
		);
		
		_self->SetSelection(selection);

	}
	rb_call_super(argc,argv);
	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXRadioBox(VALUE rb_mWX)
{
#if 0
	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");
#endif
#if wxUSE_RADIOBOX
	using namespace RubyWX::RadioBox;
	rb_cWXRadioBox = rb_define_class_under(rb_mWX,"RadioBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXRadioBox,_alloc);

	rb_define_method(rb_cWXRadioBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerEventType("radiobox", wxEVT_RADIOBOX,rb_cWXCommandEvent);

	registerInfo<wxRadioBox>(rb_cWXRadioBox);
#endif

}


