/*
 * wxTreeBook.cpp
 *
 *  Created on: 26.01.2014
 *      Author: hanmac
 */

#include "wxTreeBook.hpp"
#include "wxBookCtrl.hpp"

VALUE rb_cWXTreebook;

#if wxUSE_TREEBOOK
#define _self unwrap<wxTreebook*>(self)

namespace RubyWX {
namespace Treebook {

APP_PROTECT(wxTreebook)

singlereturn(GetTreeCtrl)

/*
 * call-seq:
 *   TreeBookCtrl.new(parent, name, [options])
 *   TreeBookCtrl.new(parent, [options])
 *
 * creates a new TreeBookCtrl widget.
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
		int style(wxBK_DEFAULT);

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
 *   add_sub_page(window, text, [select], [bitmap]) -> true/false
 *   add_sub_page(WindowClass, text, [select], [bitmap],**options) [{|window| }] -> true/false
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
DLL_LOCAL VALUE _addSubPage(int argc,VALUE *argv,VALUE self)
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
	}else
	{
		window_parent_check(window,_self,w);
	}
	return wrap(_self->AddSubPage(w,unwrap<wxString>(text),sel,iid));
}


/*
 * call-seq:
 *   insert_sub_page(pos, window, text, [select], [bitmap]) -> true/false
 *   insert_sub_page(pos, WindowClass, text, [select], [bitmap],**options) [{|window| }] -> true/false
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
DLL_LOCAL VALUE _insertSubPage(int argc,VALUE *argv,VALUE self)
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
	}else {
		window_parent_check(window,_self,w);
	}
	return wrap(_self->InsertSubPage(NUM2INT(n),w,unwrap<wxString>(text),sel,iid));
}


/*
 * call-seq:
 *   prepend_sub_page(window, text, [select], [bitmap]) -> true/false
 *   prepend_sub_page(WindowClass, text, [select], [bitmap],**options) [{|window| }] -> true/false
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
DLL_LOCAL VALUE _prependSubPage(int argc,VALUE *argv,VALUE self)
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
	}else
	{
		window_parent_check(window,_self,w);
	}
	return wrap(_self->InsertSubPage(0,w,unwrap<wxString>(text),sel,iid));
}
}
}
#endif
DLL_LOCAL void Init_WXTreeBookCtrl(VALUE rb_mWX)
{
#if wxUSE_TREEBOOK
	using namespace RubyWX::Treebook;
	rb_cWXTreebook = rb_define_class_under(rb_mWX,"TreeBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXTreebook,_alloc);

	rb_define_method(rb_cWXTreebook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXTreebook,"add_sub_page",RUBY_METHOD_FUNC(_addSubPage),-1);
	rb_define_method(rb_cWXTreebook,"insert_sub_page",RUBY_METHOD_FUNC(_insertSubPage),-1);
	rb_define_method(rb_cWXTreebook,"prepend_sub_page",RUBY_METHOD_FUNC(_prependSubPage),-1);


	registerInfo<wxTreebook>(rb_cWXTreebook);

	registerEventType<wxBookCtrlEvent>("treebook_page_changed",wxEVT_TREEBOOK_PAGE_CHANGED);
	registerEventType<wxBookCtrlEvent>("treebook_page_changing",wxEVT_TREEBOOK_PAGE_CHANGING);
	registerEventType<wxBookCtrlEvent>("treebook_node_collapsed",wxEVT_TREEBOOK_NODE_COLLAPSED);
	registerEventType<wxBookCtrlEvent>("treebook_node_expanded",wxEVT_TREEBOOK_NODE_EXPANDED);
#endif

}





