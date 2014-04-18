/*
 * wxAuiBook.cpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */


#include "wxBookCtrl.hpp"
#include "wxBitmap.hpp"

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

			set_hash_flag_option(hash,"tap_split",wxAUI_NB_TAB_SPLIT,style);
			set_hash_flag_option(hash,"tap_move",wxAUI_NB_TAB_MOVE,style);
			set_hash_flag_option(hash,"tap_external_move",wxAUI_NB_TAB_EXTERNAL_MOVE,style);
			set_hash_flag_option(hash,"tap_fixed_with",wxAUI_NB_TAB_FIXED_WIDTH,style);
			set_hash_flag_option(hash,"scroll_buttons",wxAUI_NB_SCROLL_BUTTONS,style);
			set_hash_flag_option(hash,"windowlist_button",wxAUI_NB_WINDOWLIST_BUTTON,style);
			set_hash_flag_option(hash,"close_button",wxAUI_NB_CLOSE_BUTTON,style);
			set_hash_flag_option(hash,"close_on_active_tab",wxAUI_NB_CLOSE_ON_ACTIVE_TAB,style);
			set_hash_flag_option(hash,"close_on_all_tabs",wxAUI_NB_CLOSE_ON_ALL_TABS,style);
			set_hash_flag_option(hash,"middle_click_close",wxAUI_NB_MIDDLE_CLICK_CLOSE,style);

		}

		_self->Create(unwrap<wxWindow*>(parent),id,wxDefaultPosition,wxDefaultSize,style);

	}

	rb_call_super(argc,argv);
	return self;
}


/*
 * call-seq:
 *   get_page_bitmap(pos) -> WX::Bitmap
 *
 * returns the bitmap of the given page.
 * ===Arguments
 * * pos is a Integer
 *
 * ===Return value
 * WX::Bitmap
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
*/
DLL_LOCAL VALUE _get_page_bitmap(VALUE self,VALUE idx)
{
	int cidx(NUM2INT(idx));
	if(check_index(cidx,_self->GetPageCount()))
		return wrap(_self->GetPageBitmap(cidx));
	return Qnil;
}

/*
 * call-seq:
 *   set_page_bitmap(pos,bitmap) -> self
 *
 * sets the bitmap of the given page.
 * ===Arguments
 * * pos is a Integer
 * * bitmap is a WX::Bitmap
 *
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
*/
DLL_LOCAL VALUE _set_page_bitmap(VALUE self,VALUE idx,VALUE str)
{
	rb_check_frozen(self);
	int cidx(NUM2INT(idx));
	if(check_index(cidx,_self->GetPageCount()))
		_self->SetPageBitmap(cidx,unwrap<wxBitmap>(str));
	return self;
}

/*
 * call-seq:
 *   get_page_tooltip(pos) -> String
 *
 * returns the tool tip of the given page.
 * ===Arguments
 * * pos is a Integer
 *
 * ===Return value
 * String
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
*/
DLL_LOCAL VALUE _get_page_tooltip(VALUE self,VALUE idx)
{
	int cidx(NUM2INT(idx));
	if(check_index(cidx,_self->GetPageCount()))
		return wrap(_self->GetPageToolTip(cidx));
	return Qnil;
}

/*
 * call-seq:
 *   set_page_tooltip(pos,tooltip) -> self
 *
 * sets the tool tip of the given page.
 * ===Arguments
 * * pos is a Integer
 * * tooltip is a String
 *
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
*/
DLL_LOCAL VALUE _set_page_tooltip(VALUE self,VALUE idx,VALUE str)
{
	rb_check_frozen(self);
	int cidx(NUM2INT(idx));
	if(check_index(cidx,_self->GetPageCount()))
		_self->SetPageToolTip(cidx,unwrap<wxString>(str));
	return self;
}


DLL_LOCAL bool check_imagelist(wxAuiNotebook* self, VALUE imageid, int& iid)
{
	if(NIL_P(imageid))
		return true;
	if(rb_obj_is_kind_of(imageid,rb_cInteger))
	{
		iid = NUM2INT(imageid);
		wxImageList *imglist = self->GetImageList();
		if(imglist)
			return check_index(iid,imglist->GetImageCount());
	}
	return false;
}


DLL_LOCAL bool check_window(VALUE self,VALUE hash, VALUE window, wxWindow*& w)
{

	if(rb_obj_is_kind_of(window,rb_cClass) && rb_class_inherited(window,rb_cWXWindow)) {
		VALUE argv2[] = {self, hash };
		w = unwrap<wxWindow*>(rb_class_new_instance(2,argv2,window));
		return true;
	}else if(nil_check(window))
	{
		return window_parent_check(window,_self,w);
	}
	return false;
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
 * === Exceptions
 * [IndexError]
 * * bitmap is Integer and greater than the list of bitmaps in the image_list
 * [TypeError]
 * * window is nil
 * [ArgumentError]
 * * window does not have this BookCtrl as parent
 *
*/
DLL_LOCAL VALUE _addPage(int argc,VALUE *argv,VALUE self)
{
	VALUE window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;

	rb_scan_args(argc, argv, "22:",&window,&text,&select,&imageid,&hash);

	rb_check_frozen(self);

	if(!NIL_P(select))
		sel = RTEST(select);

	check_window(self,hash,window,w);

	int iid = -1;

	if(check_imagelist(_self,imageid,iid))
	{
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
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
 * * bitmap is Integer and greater than the list of bitmaps in the image_list
 * [TypeError]
 * * window is nil
 * [ArgumentError]
 * * window does not have this BookCtrl as parent
 *
*/
DLL_LOCAL VALUE _insertPage(int argc,VALUE *argv,VALUE self)
{
	VALUE idx,window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	rb_scan_args(argc, argv, "32:",&idx,&window,&text,&select,&imageid,&hash);

	rb_check_frozen(self);

	if(!NIL_P(select))
		sel = RTEST(select);

	check_window(self,hash,window,w);

	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetPageCount()+1))
	{
		int iid = -1;

		if(check_imagelist(_self,imageid,iid))
		{
			return wrap(_self->InsertPage(cidx,w,unwrap<wxString>(text),sel,iid));
		}else{
			return wrap(_self->InsertPage(cidx,w,unwrap<wxString>(text),sel,unwrap<wxBitmap>(imageid)));
		}
	}
	return Qnil;
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
 * === Exceptions
 * [IndexError]
 * * bitmap is Integer and greater than the list of bitmaps in the image_list
 * [TypeError]
 * * window is nil
 * [ArgumentError]
 * * window does not have this BookCtrl as parent
 *
*/
DLL_LOCAL VALUE _prependPage(int argc,VALUE *argv,VALUE self)
{
	VALUE window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	rb_scan_args(argc, argv, "22:",&window,&text,&select,&imageid,&hash);

	rb_check_frozen(self);

	if(!NIL_P(select))
		sel = RTEST(select);

	check_window(self,hash,window,w);

	int iid = -1;

	if(check_imagelist(_self,imageid,iid))
	{
		return wrap(_self->InsertPage(0,w,unwrap<wxString>(text),sel,iid));
	}else{
		return wrap(_self->InsertPage(0,w,unwrap<wxString>(text),sel,unwrap<wxBitmap>(imageid)));
	}

}


}
}
#endif


/* Document-const: TAB_SPLIT
 *  Allows the tab control to be split by dragging a tab.
 */
/* Document-const: TAB_MOVE
 *  Allows a tab to be moved horizontally by dragging.
 */
/* Document-const: TAB_EXTERNAL_MOVE
 *  Allows a tab to be moved to another tab control.
 */
/* Document-const: TAB_FIXED_WIDTH
 *  With this style, all tabs have the same width.
 */

/* Document-const: SCROLLBUTTONS
 *  With this style, left and right scroll buttons are displayed.
 */
/* Document-const: WINDOWLIST_BUTTON
 *  With this style, a drop-down list of windows is available.
 */
/* Document-const: CLOSE_BUTTON
 *  With this style, a close button is available on the tab bar.
 */
/* Document-const: CLOSE_ON_ACTIVE_TAB
 *  With this style, the close button is visible on the active tab.
 */
/* Document-const: CLOSE_ON_ALL_TABS
 *  With this style, the close button is visible on all tabs.
 */
/* Document-const: MIDDLE_CLICK_CLOSE
 *  With this style, middle click on a tab closes the tab.
 */

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

DLL_LOCAL void Init_WXAuiNoteBookCtrl(VALUE rb_mWX)
{

#if 0
	rb_mWX = rb_define_module("WX");
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");

	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXBookCtrlBase = rb_define_class_under(rb_mWX,"BookCtrl",rb_cWXControl);
#endif

#if wxUSE_AUI
	using namespace RubyWX::AuiNotebook;
	rb_cWXAuiNotebook = rb_define_class_under(rb_mWX,"AuiNoteBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXAuiNotebook,_alloc);

	rb_define_method(rb_cWXAuiNotebook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXAuiNotebook,"add_page",RUBY_METHOD_FUNC(_addPage),-1);
	rb_define_method(rb_cWXAuiNotebook,"insert_page",RUBY_METHOD_FUNC(_insertPage),-1);
	rb_define_method(rb_cWXAuiNotebook,"prepend_page",RUBY_METHOD_FUNC(_prependPage),-1);

	rb_define_method(rb_cWXAuiNotebook,"get_page_tooltip",RUBY_METHOD_FUNC(_get_page_tooltip),1);
	rb_define_method(rb_cWXAuiNotebook,"set_page_tooltip",RUBY_METHOD_FUNC(_set_page_tooltip),2);

	rb_define_method(rb_cWXAuiNotebook,"get_page_bitmap",RUBY_METHOD_FUNC(_get_page_bitmap),1);
	rb_define_method(rb_cWXAuiNotebook,"set_page_bitmap",RUBY_METHOD_FUNC(_set_page_bitmap),2);

//	rb_define_method(rb_cWXAuiNotebook,"each_page",RUBY_METHOD_FUNC(_each),0);
//	rb_define_method(rb_cWXAuiNotebook,"page",RUBY_METHOD_FUNC(_page),1);

	rb_define_const(rb_cWXAuiNotebook,"TAB_SPLIT",INT2NUM(wxAUI_NB_TAB_SPLIT));
	rb_define_const(rb_cWXAuiNotebook,"TAB_MOVE",INT2NUM(wxAUI_NB_TAB_MOVE));
	rb_define_const(rb_cWXAuiNotebook,"TAB_EXTERNAL_MOVE",INT2NUM(wxAUI_NB_TAB_EXTERNAL_MOVE));
	rb_define_const(rb_cWXAuiNotebook,"TAB_FIXED_WIDTH",INT2NUM(wxAUI_NB_TAB_FIXED_WIDTH));
	rb_define_const(rb_cWXAuiNotebook,"SCROLLBUTTONS",INT2NUM(wxAUI_NB_SCROLL_BUTTONS));
	rb_define_const(rb_cWXAuiNotebook,"WINDOWLIST_BUTTON",INT2NUM(wxAUI_NB_WINDOWLIST_BUTTON));
	rb_define_const(rb_cWXAuiNotebook,"CLOSE_BUTTON",INT2NUM(wxAUI_NB_CLOSE_BUTTON));
	rb_define_const(rb_cWXAuiNotebook,"CLOSE_ON_ACTIVE_TAB",INT2NUM(wxAUI_NB_CLOSE_ON_ACTIVE_TAB));
	rb_define_const(rb_cWXAuiNotebook,"CLOSE_ON_ALL_TABS",INT2NUM(wxAUI_NB_CLOSE_ON_ALL_TABS));
	rb_define_const(rb_cWXAuiNotebook,"MIDDLE_CLICK_CLOSE",INT2NUM(wxAUI_NB_MIDDLE_CLICK_CLOSE));
	rb_define_const(rb_cWXAuiNotebook,"DEFAULT_STYLE",INT2NUM(wxAUI_NB_DEFAULT_STYLE));

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

