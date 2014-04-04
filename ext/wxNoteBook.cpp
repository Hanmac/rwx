/*
 * wxNoteBook.cpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#include "wxNoteBook.hpp"
#include "wxBookCtrl.hpp"

VALUE rb_cWXNotebook;

#if wxUSE_NOTEBOOK
#define _self unwrap<wxNotebook*>(self)

namespace RubyWX {
namespace Notebook {

APP_PROTECT(wxNotebook)

/*
 * call-seq:
 *   NoteBookCtrl.new(parent, name, [options])
 *   NoteBookCtrl.new(parent, [options])
 *
 * creates a new NoteBookCtrl widget.
 * ===Arguments
 * * parent of this window or nil
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
DLL_LOCAL void Init_WXNoteBookCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");

	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXBookCtrlBase = rb_define_class_under(rb_mWX,"BookCtrl",rb_cWXControl);
#endif

#if wxUSE_NOTEBOOK
	using namespace RubyWX::Notebook;
	rb_cWXNotebook = rb_define_class_under(rb_mWX,"NoteBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXNotebook,_alloc);

	rb_define_method(rb_cWXNotebook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxNotebook>(rb_cWXNotebook);

	registerEventType<wxBookCtrlEvent>("notebook_page_changed",wxEVT_NOTEBOOK_PAGE_CHANGED);
	registerEventType<wxBookCtrlEvent>("notebook_page_changing",wxEVT_NOTEBOOK_PAGE_CHANGING);
#endif

}





