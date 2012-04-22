/*
 * wxNoteBook.cpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"

VALUE rb_cWXNotebook;

#if wxUSE_NOTEBOOK
#define _self wrap<wxNotebook*>(self)

namespace RubyWX {
namespace Notebook {

VALUE _alloc(VALUE self)
{
	return wrap(new wxNotebook(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!_created)
	{
		_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXNoteBookCtrl(VALUE rb_mWX)
{
#if wxUSE_NOTEBOOK
	using namespace RubyWX::Notebook;
	rb_cWXNotebook = rb_define_class_under(rb_mWX,"NoteBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXNotebook,_alloc);

	rb_define_method(rb_cWXNotebook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#endif

}





