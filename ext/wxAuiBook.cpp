/*
 * wxAuiBook.cpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */


#include "wxBookCtrl.hpp"

VALUE rb_cWXAuiNotebook;

#if wxUSE_AUI
#define _self unwrap<wxAuiNotebook*>(self)

namespace RubyWX {
namespace AuiNotebook {

APP_PROTECT(wxAuiNotebook)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,
		wxDefaultPosition,wxDefaultSize,wxAUI_NB_DEFAULT_STYLE
	);
	_created = true;

	rb_call_super(argc,argv);
	return self;
}

VALUE _page(VALUE self,VALUE i)
{
	return wrap(_self->GetPage(NUM2UINT(i)));
}

VALUE _each(VALUE self)
{
	RETURN_ENUMERATOR(self,0,NULL);
	size_t count = _self->GetPageCount();
	for(size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->GetPage(i)));
	return self;
}

}
}
#endif
void Init_WXAuiNoteBookCtrl(VALUE rb_mWX)
{
#if wxUSE_AUI
	using namespace RubyWX::AuiNotebook;
	rb_cWXAuiNotebook = rb_define_class_under(rb_mWX,"AuiNoteBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXAuiNotebook,_alloc);

	rb_define_method(rb_cWXAuiNotebook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXAuiNotebook,"each_page",RUBY_METHOD_FUNC(_each),0);
	rb_define_method(rb_cWXAuiNotebook,"page",RUBY_METHOD_FUNC(_page),1);

	registerInfo<wxAuiNotebook>(rb_cWXAuiNotebook);

	registerEventType("auinotebook_page_close", wxEVT_AUINOTEBOOK_PAGE_CLOSE,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_page_changed", wxEVT_AUINOTEBOOK_PAGE_CHANGED,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_page_changing", wxEVT_AUINOTEBOOK_PAGE_CHANGING,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_page_closed", wxEVT_AUINOTEBOOK_PAGE_CLOSED,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_button", wxEVT_AUINOTEBOOK_BUTTON,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_begin_drag", wxEVT_AUINOTEBOOK_BEGIN_DRAG,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_end_drag", wxEVT_AUINOTEBOOK_END_DRAG,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_drag_motion", wxEVT_AUINOTEBOOK_DRAG_MOTION,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_allow_dnd", wxEVT_AUINOTEBOOK_ALLOW_DND,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_tab_middle_down", wxEVT_AUINOTEBOOK_TAB_MIDDLE_DOWN,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_tab_middle_up", wxEVT_AUINOTEBOOK_TAB_MIDDLE_UP,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_tab_right_down", wxEVT_AUINOTEBOOK_TAB_RIGHT_DOWN,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_tab_right_up", wxEVT_AUINOTEBOOK_TAB_RIGHT_UP,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_drag_done", wxEVT_AUINOTEBOOK_DRAG_DONE,rb_cWXBookCtrlEvent);
	registerEventType("auinotebook_bg_dclick", wxEVT_AUINOTEBOOK_BG_DCLICK,rb_cWXBookCtrlEvent);


#endif

}

