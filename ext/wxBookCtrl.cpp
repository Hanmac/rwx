/*
 * wxBookCtrl.cpp
 *
 *  Created on: 26.03.2012
 *      Author: hanmac
 */



#include "wxEvtHandler.hpp"
#include "wxNotifyEvent.hpp"
#include "wxSizer.hpp"


#include "wxBookCtrl.hpp"
#include "wxChoiceBook.hpp"
#include "wxNoteBook.hpp"
#include "wxListBook.hpp"
#include "wxToolBook.hpp"
#include "wxAuiBook.hpp"
#include "wxTreeBook.hpp"


#include "wxBitmap.hpp"

VALUE rb_cWXBookCtrlBase,rb_cWXBookCtrlEvent;

#if wxUSE_BOOKCTRL

template <>
wxBookCtrlBase* unwrap<wxBookCtrlBase*>(const VALUE &arg)
{
	return unwrapTypedPtr<wxBookCtrlBase>(arg,rb_cWXBookCtrlBase);
}

#define _self unwrap<wxBookCtrlBase*>(self)

namespace RubyWX {
namespace BookCtrl {

macro_attr(ImageList,wxImageList*)
macro_attr(InternalBorder,unsigned int)
macro_attr(ControlMargin,int)
macro_attr_selection(Selection,GetPageCount)

macro_attr(FitToCurrentPage,bool)

singlereturn(IsVertical)
singlereturn(GetControlSizer)
singlereturn(GetCurrentPage)
singlereturn(GetControllerSize)


/*
 * call-seq:
 *   BookCtrl.new(parent, name, [options])
 *   BookCtrl.new(parent, [options])
 *
 * creates a new BookCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * image_list [WX::Bitmap]
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash, "image_list", &wxBookCtrlBase::SetImageList, _self);
	}

	return self;
}

/*
 * call-seq:
 *   page(pos) -> WX::Window
 *
 * returns the page with the given index.
 * ===Arguments
 * * pos is a Integer
 *
 * ===Return value
 * WX::Window
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
*/
DLL_LOCAL VALUE _page(VALUE self,VALUE idx)
{
	int cidx(NUM2INT(idx));

	if(check_index(cidx,_self->GetPageCount()))
		return wrap(_self->GetPage(cidx));
	return Qnil;
}

DLL_LOCAL VALUE _each_size(VALUE self)
{
	return UINT2NUM(_self->GetPageCount());
}


/*
 * call-seq:
 *   each_page -> Enumerator
 *   each_page { |child| } -> self
 *
 * iterates the pages in this book control.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_size));
	std::size_t count = _self->GetPageCount();
	for(std::size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->GetPage(i)));
	return self;
}

macro_attr_item(PageText,GetPageText, SetPageText, GetPageCount, wxString)

/*
 * call-seq:
 *   get_page_image(pos) -> Integer
 *
 * returns the image idx of the given page.
 * ===Arguments
 * * pos is a Integer
 *
 * ===Return value
 * Integer
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
*/
DLL_LOCAL VALUE _get_page_image(VALUE self,VALUE idx)
{
#if wxUSE_AUI
	//TODO should be fixed in wx
	if(rb_obj_is_kind_of(self,rb_cWXAuiNotebook))
		rb_raise(
			rb_eArgError,"get_page_image is not supported for %" PRIsVALUE,
			RB_CLASSNAME(rb_cWXAuiNotebook)
		);
#endif
	int cidx(NUM2INT(idx));
	if(check_index(cidx,_self->GetPageCount()))
		return INT2NUM(_self->GetPageImage(cidx));
	return Qnil;
}

/*
 * call-seq:
 *   set_page_image(pos,iid) -> self
 *
 * sets the image idx of the given page.
 * ===Arguments
 * * pos is a Integer
 * * iid Integer index of the image in the image_list
 *
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
 * * iid is greater than the count of images in the image_list
*/
DLL_LOCAL VALUE _set_page_image(VALUE self,VALUE idx,VALUE iid)
{
	rb_check_frozen(self);
	int cidx(NUM2INT(idx));

	if(check_index(cidx,_self->GetPageCount()))
	{
		int ciid(NUM2INT(iid));
		wxImageList *imglist = _self->GetImageList();
		if(imglist && check_index(ciid,imglist->GetImageCount()))
		{
			_self->SetPageImage(cidx,ciid);
		}
	}
	return self;
}



DLL_LOCAL bool check_imagelist(wxBookCtrlBase* self, VALUE imageid, int& iid)
{
	if(NIL_P(imageid))
		return true;

	iid = NUM2INT(imageid);
	wxImageList *imglist = self->GetImageList();
	if(imglist)
		return check_index(iid,imglist->GetImageCount());
	return true;
}

DLL_LOCAL bool check_window(VALUE self,VALUE hash, VALUE window, wxWindow*& w)
{

	if(rb_obj_is_kind_of(window,rb_cClass) && rb_class_inherited(window,rb_cWXWindow)) {
		VALUE argv2[] = {self, hash };
		w = unwrap<wxWindow*>(rb_class_new_instance(2,argv2,window));
		return true;
#if wxUSE_TREEBOOK
	}else if(nil_check(window,!rb_obj_is_kind_of(self,rb_cWXTreebook))) //TODO Tree Ctrl allows nil page, but i can't check that
#else
	}else if(nil_check(window))
#endif
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
 * * bitmap is a Integer and says the position of the bitmap in the image_list
 * ===Return value
 * true/false
 * === Exceptions
 * [IndexError]
 * * bitmap is greater than the list of bitmaps in the image_list
 * [TypeError]
 * * window is nil when the BookCtrl does not support it (currently only supported for TreeBookCtrl)
 * [ArgumentError]
 * * window does not have this BookCtrl as parent
 *
*/
DLL_LOCAL VALUE _addPage(int argc,VALUE *argv,VALUE self)
{
	VALUE window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	int iid = -1;
	rb_scan_args(argc, argv, "22:",&window,&text,&select,&imageid,&hash);

	rb_check_frozen(self);

	if(!NIL_P(select))
		sel = RTEST(select);

	check_imagelist(_self,imageid,iid);
	check_window(self,hash,window,w);

	return wrap(_self->AddPage(w,unwrap<wxString>(text),sel,iid));
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
 * * bitmap is a Integer and says the position of the bitmap in the image_list
 * ===Return value
 * true/false
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
 * * bitmap is greater than the list of bitmaps in the image_list
 * [TypeError]
 * * window is nil when the BookCtrl does not support it (currently only supported for TreeBookCtrl)
 * [ArgumentError]
 * * window does not have this BookCtrl as parent
 *
*/
DLL_LOCAL VALUE _insertPage(int argc,VALUE *argv,VALUE self)
{
	VALUE idx,window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	int iid = -1;
	rb_scan_args(argc, argv, "32:",&idx,&window,&text,&select,&imageid,&hash);

	rb_check_frozen(self);

	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetPageCount()+1))
	{
		if(!NIL_P(select))
			sel = RTEST(select);

		check_imagelist(_self,imageid,iid);
		check_window(self,hash,window,w);

		return wrap(_self->InsertPage(cidx,w,unwrap<wxString>(text),sel,iid));
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
 * * bitmap is a Integer and says the position of the bitmap in the image_list
 * ===Return value
 * true/false
 * === Exceptions
 * [IndexError]
 * * bitmap is greater than the list of bitmaps in the image_list
 * [TypeError]
 * * window is nil when the BookCtrl does not support it (currently only supported for TreeBookCtrl)
 * [ArgumentError]
 * * window does not have this BookCtrl as parent
 *
*/
DLL_LOCAL VALUE _prependPage(int argc,VALUE *argv,VALUE self)
{
	VALUE window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	int iid = -1;
	rb_scan_args(argc, argv, "22:",&window,&text,&select,&imageid,&hash);

	rb_check_frozen(self);

	if(!NIL_P(select))
		sel = RTEST(select);

	check_imagelist(_self,imageid,iid);
	check_window(self,hash,window,w);

	return wrap(_self->InsertPage(0,w,unwrap<wxString>(text),sel,iid));
}

/*
 * call-seq:
 *   delete_page(pos) -> WX::Window
 *
 * deletes and returns the page with the given index from the BookCtrl.
 * ===Arguments
 * * pos is a Integer
 *
 * ===Return value
 * WX::Window
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
*/
DLL_LOCAL VALUE _deletePage(VALUE self,VALUE idx)
{
	rb_check_frozen(self);
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetPageCount()))
	{
		wxWindow *w = _self->GetPage(cidx);
		if(_self->RemovePage(cidx))
			return wrap(w);
	}
	return Qnil;
}



singlefunc(DeleteAllPages)
singlefunc(AdvanceSelection)

/*
 * call-seq:
 *   prev_page -> self
 *
 * selects the previous page of the current selected.
 * ===Return value
 * self
*/
DLL_LOCAL VALUE _prev_page(VALUE self)
{
	rb_check_frozen(self);
	_self->AdvanceSelection(false);
	return self;
}

/* Document-method: get_page_text
 * call-seq:
 *   get_page_text(pos) -> String
 *
 * returns the text of the given page.
 * ===Arguments
 * * pos is a Integer
 *
 * ===Return value
 * String
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
*/

/* Document-method: set_page_text
 * call-seq:
 *   set_page_text(pos,text) -> self
 *
 * returns the text of the given page.
 * ===Arguments
 * * pos is a Integer
 * * text is a String
 *
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
*/

/* Document-method: delete_all_pages
 * call-seq:
 *   delete_all_pages -> self
 *
 * deletes all pages from the BookCtrl. Returns self.
 * ===Return value
 * self
*/

/* Document-method: next_page
 * call-seq:
 *   next_page -> self
 *
 * selects the next page of the current selected.
 * ===Return value
 * self
*/



/* Document-method: vertical?
 * call-seq:
 *   vertical? -> true/false
 *
 * returns true is vertical orientation is used.
 * ===Return value
 * true/false
*/

/* Document-method: current_page
 * call-seq:
 *   current_page -> WX::Window/nil
 *
 * returns the current selected page or nil if none is selected.
 * ===Return value
 * WX::Window/nil
*/


/* Document-method: control_sizer
 * call-seq:
 *   current_page -> WX::Sizer/nil
 *
 * returns the sizer containing the control, if any
 * ===Return value
 * WX::Sizer/nil
*/
/* Document-method: controller_size
 * call-seq:
 *   controller_size -> WX::Size
 *
 * return the size of the area needed to accommodate the controller
 * ===Return value
 * WX::Size
*/



/* Document-attr: image_list
 * [WX::Bitmap] array of bitmap images,
 * bitmap parameter for add, insert & prepend of pages must be smaller than size of array.
 */
/* Document-attr: selection
 * Integer/nil returns the index of the current selected page, or nil if none is selected.
 */
/* Document-attr: internal_border
 * Integer returns how many pixel the pages are depart of each other.
 */
/* Document-attr: control_margin
 * Integer returns the margin of the pages.
 */

/* Document-const: TOP
 * orient the book ctrl pages on top.
 */
/* Document-const: BOTTOM
 * orient the book ctrl pages on bottom.
 */
/* Document-const: LEFT
 * orient the book ctrl pages on left.
 */
/* Document-const: RIGHT
 * orient the book ctrl pages on right.
 */

//for event

/* Document-attr: old_selection
 * Integer/nil returns the index of the current selected page, or nil if none is selected.
 */

namespace Event {
#undef _self
#define _self unwrapTypedPtr<wxBookCtrlEvent>(self,rb_cWXBookCtrlEvent)
macro_attr(Selection,int)
macro_attr(OldSelection,int)
}

}
}
#endif
DLL_LOCAL void Init_WXBookCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");

	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
	rb_cWXCommandEvent = rb_define_class_under(rb_cWXEvent,"Command",rb_cWXEvent);
	rb_cWXNotifyEvent = rb_define_class_under(rb_cWXEvent,"Notify",rb_cWXCommandEvent);
#endif

#if wxUSE_BOOKCTRL
	using namespace RubyWX::BookCtrl;
	rb_cWXBookCtrlBase = rb_define_class_under(rb_mWX,"BookCtrl",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXBookCtrlBase);

	rb_cWXBookCtrlEvent = rb_define_class_under(rb_cWXEvent,"BookCtrl",rb_cWXNotifyEvent);

#if 0
	rb_define_attr(rb_cWXBookCtrlBase,"selection",1,1);
	rb_define_attr(rb_cWXBookCtrlBase,"image_list",1,1);
	rb_define_attr(rb_cWXBookCtrlBase,"intenal_border",1,1);
	rb_define_attr(rb_cWXBookCtrlBase,"control_margin",1,1);

	rb_define_attr(rb_cWXBookCtrlEvent,"selection",1,1);
	rb_define_attr(rb_cWXBookCtrlEvent,"old_selection",1,1);
#endif

	rb_define_method(rb_cWXBookCtrlBase,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXBookCtrlBase,"selection",_getSelection,_setSelection);
	rb_define_attr_method(rb_cWXBookCtrlBase,"image_list",_getImageList,_setImageList);
	rb_define_attr_method(rb_cWXBookCtrlBase,"intenal_border",_getInternalBorder,_setInternalBorder);
	rb_define_attr_method(rb_cWXBookCtrlBase,"control_margin",_getControlMargin,_setControlMargin);

	rb_define_method(rb_cWXBookCtrlBase,"add_page",RUBY_METHOD_FUNC(_addPage),-1);
	rb_define_method(rb_cWXBookCtrlBase,"insert_page",RUBY_METHOD_FUNC(_insertPage),-1);
	rb_define_method(rb_cWXBookCtrlBase,"prepend_page",RUBY_METHOD_FUNC(_prependPage),-1);

	rb_define_method(rb_cWXBookCtrlBase,"each_page",RUBY_METHOD_FUNC(_each),0);

	rb_define_method(rb_cWXBookCtrlBase,"page",RUBY_METHOD_FUNC(_page),1);

	rb_define_method(rb_cWXBookCtrlBase,"get_page_text",RUBY_METHOD_FUNC(_getPageText),1);
	rb_define_method(rb_cWXBookCtrlBase,"set_page_text",RUBY_METHOD_FUNC(_setPageText),2);

	rb_define_method(rb_cWXBookCtrlBase,"get_page_image",RUBY_METHOD_FUNC(_get_page_image),1);
	rb_define_method(rb_cWXBookCtrlBase,"set_page_image",RUBY_METHOD_FUNC(_set_page_image),2);

	rb_define_method(rb_cWXBookCtrlBase,"vertical?",RUBY_METHOD_FUNC(_IsVertical),0);

	rb_define_method(rb_cWXBookCtrlBase,"current_page",RUBY_METHOD_FUNC(_GetCurrentPage),0);
	rb_define_method(rb_cWXBookCtrlBase,"controller_size",RUBY_METHOD_FUNC(_GetControllerSize),0);
	rb_define_method(rb_cWXBookCtrlBase,"control_sizer",RUBY_METHOD_FUNC(_GetControlSizer),0);

	rb_define_method(rb_cWXBookCtrlBase,"delete_page",RUBY_METHOD_FUNC(_deletePage),1);
	rb_define_method(rb_cWXBookCtrlBase,"delete_all_pages",RUBY_METHOD_FUNC(_DeleteAllPages),0);

	rb_define_method(rb_cWXBookCtrlBase,"next_page",RUBY_METHOD_FUNC(_AdvanceSelection),0);
	rb_define_method(rb_cWXBookCtrlBase,"prev_page",RUBY_METHOD_FUNC(_prev_page),0);

	rb_define_const(rb_cWXBookCtrlBase,"TOP",INT2NUM(wxBK_TOP));
	rb_define_const(rb_cWXBookCtrlBase,"BOTTOM",INT2NUM(wxBK_BOTTOM));
	rb_define_const(rb_cWXBookCtrlBase,"LEFT",INT2NUM(wxBK_LEFT));
	rb_define_const(rb_cWXBookCtrlBase,"RIGHT",INT2NUM(wxBK_RIGHT));

	rb_define_attr_method(rb_cWXBookCtrlEvent,"selection",Event::_getSelection,Event::_setSelection);
	rb_define_attr_method(rb_cWXBookCtrlEvent,"old_selection",Event::_getOldSelection,Event::_setOldSelection);

	registerInfo<wxBookCtrlBase>(rb_cWXBookCtrlBase);
	registerInfo<wxBookCtrlEvent>(rb_cWXBookCtrlEvent);
#endif

}



