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
	return unwrapPtr<wxBookCtrlBase>(arg,rb_cWXBookCtrlBase);
}

#define _self unwrap<wxBookCtrlBase*>(self)

namespace RubyWX {
namespace BookCtrl {

macro_attr(ImageList,wxImageList*)
macro_attr(InternalBorder,unsigned int)
macro_attr(ControlMargin,int)
macro_attr(Selection,int)

macro_attr(FitToCurrentPage,bool)

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
		VALUE temp;
		set_option(image_list,ImageList,wxImageList*)
	}

	return self;
}

DLL_LOCAL VALUE _page(VALUE self,VALUE i)
{
	return wrap(_self->GetPage(NUM2UINT(i)));
}

DLL_LOCAL VALUE _each_size(VALUE self)
{
	return UINT2NUM(_self->GetPageCount());
}


DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,_each_size);
	std::size_t count = _self->GetPageCount();
	for(std::size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->GetPage(i)));
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
 * * bitmap is a Integer and says the position of the bitmap in the image_list
 * ===Return value
 * true/false
 *
*/
DLL_LOCAL VALUE _addPage(int argc,VALUE *argv,VALUE self)
{
	VALUE window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	int iid = -1;
	rb_scan_args(argc, argv, "22:",&window,&text,&select,&imageid,&hash);

	if(!NIL_P(select))
		sel = RTEST(select);

	if(!NIL_P(imageid))
		iid = NUM2INT(imageid);

	if(rb_obj_is_kind_of(window,rb_cClass) && rb_class_inherited(window,rb_cWXWindow)) {
		VALUE argv2[] = {self, hash };
		w = unwrap<wxWindow*>(rb_class_new_instance(2,argv2,window));
	}else if(nil_check(window,!rb_obj_is_kind_of(window,rb_cWXTreebook))) //TODO Tree Ctrl allows nil page, but i can't check that
	{
		window_parent_check(window,_self,w);
	}
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
 *
*/
DLL_LOCAL VALUE _insertPage(int argc,VALUE *argv,VALUE self)
{
	VALUE n,window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	int iid = -1;
	rb_scan_args(argc, argv, "32:",&n,&window,&text,&select,&imageid,&hash);

	if(!NIL_P(select))
		sel = RTEST(select);

	if(!NIL_P(imageid))
		iid = NUM2INT(imageid);

	if(rb_obj_is_kind_of(window,rb_cClass) && rb_class_inherited(window,rb_cWXWindow)) {
		VALUE argv2[] = {self, hash };
		w = unwrap<wxWindow*>(rb_class_new_instance(2,argv2,window));
	}else if(nil_check(window,!rb_obj_is_kind_of(window,rb_cWXTreebook))) //TODO Tree Ctrl allows nil page, but i can't check that
	{
		window_parent_check(window,_self,w);
	}
	return wrap(_self->InsertPage(NUM2INT(n),w,unwrap<wxString>(text),sel,iid));
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
 *
*/
DLL_LOCAL VALUE _prependPage(int argc,VALUE *argv,VALUE self)
{
	VALUE window,text,select,imageid,hash;
	wxWindow *w = NULL;
	bool sel = false;
	int iid = -1;
	rb_scan_args(argc, argv, "22:",&window,&text,&select,&imageid,&hash);

	if(!NIL_P(select))
		sel = RTEST(select);

	if(!NIL_P(imageid))
		iid = NUM2INT(imageid);

	if(rb_obj_is_kind_of(window,rb_cClass) && rb_class_inherited(window,rb_cWXWindow)) {
		VALUE argv2[] = {self, hash };
		w = unwrap<wxWindow*>(rb_class_new_instance(2,argv2,window));
	}else if(nil_check(window,!rb_obj_is_kind_of(window,rb_cWXTreebook))) //TODO Tree Ctrl allows nil page, but i can't check that
	{
		window_parent_check(window,_self,w);
	}
	return wrap(_self->InsertPage(0,w,unwrap<wxString>(text),sel,iid));
}


DLL_LOCAL VALUE _deletePage(VALUE self,VALUE idx)
{
	std::size_t cidx = NUM2UINT(idx);
	if(cidx >= _self->GetPageCount())
		return Qnil;
	wxWindow *w = _self->GetPage(cidx);
	if(_self->RemovePage(cidx))
		return wrap(w);
	return Qfalse;
}



singlefunc(DeleteAllPages)


namespace Event {
#undef _self
#define _self unwrapPtr<wxBookCtrlEvent>(self,rb_cWXBookCtrlEvent)
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

	rb_define_attr(rb_cWXBookCtrlBase,"selection",1,1);
	rb_define_attr(rb_cWXBookCtrlBase,"image_list",1,1);

	rb_define_attr(rb_cWXBookCtrlEvent,"selection",1,1);
	rb_define_attr(rb_cWXBookCtrlEvent,"old_selection",1,1);
#endif

#if wxUSE_BOOKCTRL
	using namespace RubyWX::BookCtrl;
	rb_cWXBookCtrlBase = rb_define_class_under(rb_mWX,"BookCtrl",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXBookCtrlBase);

	rb_define_method(rb_cWXBookCtrlBase,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXBookCtrlBase,"selection",_getSelection,_setSelection);
	rb_define_attr_method(rb_cWXBookCtrlBase,"image_list",_getImageList,_setImageList);

	rb_define_method(rb_cWXBookCtrlBase,"add_page",RUBY_METHOD_FUNC(_addPage),-1);
	rb_define_method(rb_cWXBookCtrlBase,"insert_page",RUBY_METHOD_FUNC(_insertPage),-1);
	rb_define_method(rb_cWXBookCtrlBase,"prepend_page",RUBY_METHOD_FUNC(_prependPage),-1);

	rb_define_method(rb_cWXBookCtrlBase,"each_page",RUBY_METHOD_FUNC(_each),0);

	rb_define_method(rb_cWXBookCtrlBase,"page",RUBY_METHOD_FUNC(_page),1);

	rb_define_method(rb_cWXBookCtrlBase,"current_page",RUBY_METHOD_FUNC(_GetCurrentPage),0);

	rb_define_method(rb_cWXBookCtrlBase,"delete_page",RUBY_METHOD_FUNC(_deletePage),1);
	rb_define_method(rb_cWXBookCtrlBase,"delete_all_pages",RUBY_METHOD_FUNC(_DeleteAllPages),0);

	rb_cWXBookCtrlEvent = rb_define_class_under(rb_cWXEvent,"BookCtrl",rb_cWXNotifyEvent);

	rb_define_attr_method(rb_cWXBookCtrlEvent,"selection",Event::_getSelection,Event::_setSelection);
	rb_define_attr_method(rb_cWXBookCtrlEvent,"old_selection",Event::_getOldSelection,Event::_setOldSelection);

	registerInfo<wxBookCtrlBase>(rb_cWXBookCtrlBase);
	registerInfo<wxBookCtrlEvent>(rb_cWXBookCtrlEvent);
#endif

}



