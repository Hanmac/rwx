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

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!_created)
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXNoteBookCtrl(VALUE rb_mWX)
{
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





