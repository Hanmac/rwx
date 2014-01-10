/*
 * wxToolBook.cpp
 *
 *  Created on: 14.04.2012
 *      Author: hanmac
 */

#include "wxToolBook.hpp"
#include "wxBookCtrl.hpp"

VALUE rb_cWXToolbook;

#if wxUSE_TOOLBOOK
#define _self unwrap<wxToolbook*>(self)

namespace RubyWX {
namespace Toolbook {

APP_PROTECT(wxToolbook)

/*
 * call-seq:
 *   ToggleButton.new(parent, name, [options])
 *   ToggleButton.new(parent, [options])
 *
 * creates a new ToggleButton widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
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
DLL_LOCAL void Init_WXToolBookCtrl(VALUE rb_mWX)
{
#if wxUSE_TOOLBOOK
	using namespace RubyWX::Toolbook;
	rb_cWXToolbook = rb_define_class_under(rb_mWX,"ToolBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXToolbook,_alloc);

	rb_define_method(rb_cWXToolbook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxToolbook>(rb_cWXToolbook);

	registerEventType<wxBookCtrlEvent>("toolbook_page_changed",wxEVT_TOOLBOOK_PAGE_CHANGED);
	registerEventType<wxBookCtrlEvent>("toolbook_page_changing",wxEVT_TOOLBOOK_PAGE_CHANGING);
#endif

}






