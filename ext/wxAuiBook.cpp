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

/*
 * call-seq:
 *   AuiNoteBookCtrl.new(parent, name, [options])
 *   AuiNoteBookCtrl.new(parent, [options])
 *
 * creates a new AuiNoteBookCtrl widget.
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
		int style(wxAUI_NB_DEFAULT_STYLE);

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


/*
 * call-seq:
 *   add_page(window, text, [select], [bitmap]) -> true/false
 *   add_page(WindowClass, text, [select], [bitmap],**options) [{|window| }] -> true/false
 *
 * adds a new page to the BookCtrl widget.
 *
 * ===Arguments
 * * window is a WX::Window instance
 * * text is the Label of the page. String
 * * select is true/false and says if the new page should be selected
 * * bitmap is a Integer and says the position of the bitmap in the image_list or a WX::Bitmap
 * ===Return value
 * true/false
 *
*/
DLL_LOCAL VALUE _addPage(int argc,VALUE *argv,VALUE self)
{
	VALUE window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;

	rb_scan_args(argc, argv, "22:",&window,&text,&select,&imageid,&hash);

	if(!NIL_P(select))
		sel = RTEST(select);

	if(rb_obj_is_kind_of(window,rb_cClass) && rb_class_inherited(window,rb_cWXWindow)) {
		VALUE argv2[] = {self, hash };
		w = unwrap<wxWindow*>(rb_class_new_instance(2,argv2,window));
	}else
	{
		w = unwrap<wxWindow*>(window);
//		if(w->GetParent() != _self)
//			rb_raise(rb_eArgError, "%s has wrong parent.",unwrap<char*>(window));
	}

	if(NIL_P(imageid) || rb_obj_is_kind_of(imageid,rb_cInteger))
	{
		int iid = -1;

		if(!NIL_P(imageid))
			iid = NUM2INT(imageid);

		return wrap(_self->AddPage(w,unwrap<wxString>(text),sel,iid));
	}else{
		return wrap(_self->AddPage(w,unwrap<wxString>(text),sel,unwrap<wxBitmap>(imageid)));
	}
}


/*
 * call-seq:
 *   insert_page(pos, window, text, [select], [bitmap]) -> true/false
 *   insert_page(pos, WindowClass, text, [select], [bitmap],**options) [{|window| }] -> true/false
 *
 * inserts a new page to the BookCtrl widget into the given position.
 *
 * ===Arguments
 * * pos is a Integer
 * * window is a WX::Window instance
 * * text is the Label of the page. String
 * * select is true/false and says if the new page should be selected
 * * bitmap is a Integer and says the position of the bitmap in the image_list, or a WX::Bitmap
 * ===Return value
 * true/false
 *
*/
DLL_LOCAL VALUE _insertPage(int argc,VALUE *argv,VALUE self)
{
	VALUE n,window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	rb_scan_args(argc, argv, "32:",&n,&window,&text,&select,&imageid,&hash);

	if(!NIL_P(select))
		sel = RTEST(select);

	if(rb_obj_is_kind_of(window,rb_cClass) && rb_class_inherited(window,rb_cWXWindow)) {
		VALUE argv2[] = {self, hash };
		w = unwrap<wxWindow*>(rb_class_new_instance(2,argv2,window));
	}else {
		w = unwrap<wxWindow*>(window);
	//		if(w->GetParent() != _self)
	//			rb_raise(rb_eArgError, "%s has wrong parent.",unwrap<char*>(window));
	}

	if(NIL_P(imageid) || rb_obj_is_kind_of(imageid,rb_cInteger))
	{
		int iid = -1;

		if(!NIL_P(imageid))
			iid = NUM2INT(imageid);

		return wrap(_self->InsertPage(NUM2INT(n),w,unwrap<wxString>(text),sel,iid));
	}else{
		return wrap(_self->InsertPage(NUM2INT(n),w,unwrap<wxString>(text),sel,unwrap<wxBitmap>(imageid)));
	}

}


/*
 * call-seq:
 *   prepend_page(window, text, [select], [bitmap]) -> true/false
 *   prepend_page(WindowClass, text, [select], [bitmap],**options) [{|window| }] -> true/false
 *
 * prepends a new page to the BookCtrl widget.
 *
 * ===Arguments
 * * window is a WX::Window instance
 * * text is the Label of the page. String
 * * select is true/false and says if the new page should be selected
 * * bitmap is a Integer and says the position of the bitmap in the image_list or a WX::Bitmap
 * ===Return value
 * true/false
 *
*/
DLL_LOCAL VALUE _prependPage(int argc,VALUE *argv,VALUE self)
{
	VALUE window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	rb_scan_args(argc, argv, "22:",&window,&text,&select,&imageid,&hash);

	if(!NIL_P(select))
		sel = RTEST(select);

	if(rb_obj_is_kind_of(window,rb_cClass) && rb_class_inherited(window,rb_cWXWindow)) {
		VALUE argv2[] = {self, hash };
		w = unwrap<wxWindow*>(rb_class_new_instance(2,argv2,window));
	}else
	{
		w = unwrap<wxWindow*>(window);
//		if(w->GetParent() != _self)
//			rb_raise(rb_eArgError, "%s has wrong parent.",unwrap<char*>(window));
	}

	if(NIL_P(imageid) || rb_obj_is_kind_of(imageid,rb_cInteger))
	{
		int iid = -1;

		if(!NIL_P(imageid))
			iid = NUM2INT(imageid);

		return wrap(_self->InsertPage(0,w,unwrap<wxString>(text),sel,iid));
	}else{
		return wrap(_self->InsertPage(0,w,unwrap<wxString>(text),sel,unwrap<wxBitmap>(imageid)));
	}

}


}
}
#endif
DLL_LOCAL void Init_WXAuiNoteBookCtrl(VALUE rb_mWX)
{
#if wxUSE_AUI
	using namespace RubyWX::AuiNotebook;
	rb_cWXAuiNotebook = rb_define_class_under(rb_mWX,"AuiNoteBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXAuiNotebook,_alloc);

	rb_define_method(rb_cWXAuiNotebook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXAuiNotebook,"add_page",RUBY_METHOD_FUNC(_addPage),-1);
	rb_define_method(rb_cWXAuiNotebook,"insert_page",RUBY_METHOD_FUNC(_insertPage),-1);
	rb_define_method(rb_cWXAuiNotebook,"prepend_page",RUBY_METHOD_FUNC(_prependPage),-1);

//	rb_define_method(rb_cWXAuiNotebook,"each_page",RUBY_METHOD_FUNC(_each),0);
//	rb_define_method(rb_cWXAuiNotebook,"page",RUBY_METHOD_FUNC(_page),1);

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

