/*
 * wxChoiceBook.cpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */


#include "wxBookCtrl.hpp"
#include "wxChoiceBook.hpp"

VALUE rb_cWXChoicebook;

#if wxUSE_CHOICEBOOK
#define _self unwrap<wxChoicebook*>(self)

namespace RubyWX {
namespace Choicebook {

APP_PROTECT(wxChoicebook)

/*
 * call-seq:
 *   ChoiceBookCtrl.new(parent, [options])
 *   ChoiceBookCtrl.new(parent, name, [options])
 *
 * creates a new ChoiceBookCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;

	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,wxDefaultPosition,wxDefaultSize,style);
		
	}

	rb_call_super(argc,argv);

	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXChoiceBookCtrl(VALUE rb_mWX)
{
#if wxUSE_CHOICEBOOK
	using namespace RubyWX::Choicebook;
	rb_cWXChoicebook = rb_define_class_under(rb_mWX,"ChoiceBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXChoicebook,_alloc);

	rb_define_method(rb_cWXChoicebook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxChoicebook>(rb_cWXChoicebook);

	registerEventType<wxBookCtrlEvent>("choicebook_page_changed",wxEVT_CHOICEBOOK_PAGE_CHANGED);
	registerEventType<wxBookCtrlEvent>("choicebook_page_changing",wxEVT_CHOICEBOOK_PAGE_CHANGING);

#endif

}


