/*
 * wxChoice.cpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */



#include "wxEvtHandler.hpp"
#include "wxItemContainer.hpp"

VALUE rb_cWXChoice;

#if wxUSE_CHOICE
#define _self unwrap<wxChoice*>(self)

namespace RubyWX {
namespace Choice {

APP_PROTECT(wxChoice)

/*
 * call-seq:
 *   ComboBox.new(parent, name, [options])
 *   ComboBox.new(parent, [options])
 *
 * creates a new ComboBox widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * items [String]
 *   * select Integer
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

	if(rb_obj_is_kind_of(name,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash)) {
		VALUE temp;
		set_option(items,,wxArrayString)
		set_option(select,Selection,int)
	}

	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXChoice(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");
#endif
#if wxUSE_CHOICE
	using namespace RubyWX::Choice;
	rb_cWXChoice = rb_define_class_under(rb_mWX,"Choice",rb_cWXControl);
	rb_define_alloc_func(rb_cWXChoice,_alloc);

	rb_define_method(rb_cWXChoice,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXChoice,rb_mWXItemContainer);

	registerInfo<wxChoice>(rb_cWXChoice);
#endif

}

