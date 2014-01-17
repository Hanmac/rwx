/*
 * wxPropertyGridManager.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */


#include "wxPropertyGridManager.hpp"
#include "wxPropertyGridInterface.hpp"
#include "wxPropertyGridPage.hpp"

VALUE rb_cWXPropertyGridManager;

#if wxUSE_PROPGRID
#define _self unwrap<wxPropertyGridManager*>(self)

namespace RubyWX {
namespace PropertyGridManager {

APP_PROTECT(wxPropertyGridManager)

singlereturn(GetCurrentPage)
singlereturn(GetGrid)
singlereturn(GetToolBar)

//need to define them again, stupid shadowing
singlefunc(Freeze)
singlefunc(Thaw)

singlereturn(IsFrozen)

/*
 * call-seq:
 *   PropertyGridManager.new(parent, name, [options])
 *   PropertyGridManager.new(parent, [options])
 *
 * creates a new PropertyGridManager widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * toolbar true/false Show toolbar for mode and page selection.
 *   * description true/false Show adjustable text box showing description
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;

	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(wxPGMAN_DEFAULT_STYLE);

		bool toolbar(false);
		bool description(false);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_option(hash,"toolbar",toolbar);
			set_hash_option(hash,"description",description);

			if(toolbar)
				style |= wxPG_TOOLBAR;
			if(description)
				style |= wxPG_DESCRIPTION;
		}

		_self->Create(unwrap<wxWindow*>(parent),id,wxDefaultPosition,wxDefaultSize,style);
		
	}

	rb_call_super(argc,argv);
	return self;
}


DLL_LOCAL VALUE _add_page(int argc,VALUE *argv,VALUE self)
{
	VALUE label,bitmap,page,hash;

	rb_scan_args(argc, argv, "13",&label,&bitmap,&page,&hash);

	wxPropertyGridPage* cpage = NULL;

	if(rb_obj_is_kind_of(page,rb_cClass) && rb_class_inherited(page,rb_cWXPropertyGridPage)) {
		VALUE argv2[] = {self, hash };
		cpage = unwrap<wxPropertyGridPage*>(rb_class_new_instance(2,argv2,page));
	} else {
		cpage = unwrap<wxPropertyGridPage*>(page);
	}

	return wrap(_self->AddPage(unwrap<wxString>(label),unwrap<wxBitmap>(bitmap),cpage));
}


DLL_LOCAL VALUE _insert_page(int argc,VALUE *argv,VALUE self)
{
	VALUE idx,label,bitmap,page,hash;

	rb_scan_args(argc, argv, "23",&idx,&label,&bitmap,&page,&hash);

	wxPropertyGridPage* cpage = NULL;

	if(rb_obj_is_kind_of(page,rb_cClass) && rb_class_inherited(page,rb_cWXPropertyGridPage)) {
		VALUE argv2[] = {self, hash };
		cpage = unwrap<wxPropertyGridPage*>(rb_class_new_instance(2,argv2,page));
	} else {
		cpage = unwrap<wxPropertyGridPage*>(page);
	}

	return wrap(_self->InsertPage(NUM2INT(idx),unwrap<wxString>(label),unwrap<wxBitmap>(bitmap),cpage));
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
	{
		rb_yield(wrap(_self->GetPage(i)));
	}
	return self;
}


}
}
#endif


/* Document-method: tool_bar
 * call-seq:
 *   tool_bar -> WX::ToolBar
 *
 * returns the tool bar of this WX::PropertyGridManager when available.
 * ===Return value
 * WX::ToolBar
 */

DLL_LOCAL void Init_WXPropertyGridManager(VALUE rb_mWX)
{
#if 0
	rb_cWXPanel = rb_define_class_under(rb_mWX,"Panel",rb_cWXWindow);
	rb_mWXPropertyGridInterface = rb_define_module_under(rb_mWX,"PropertyGridInterface");

#endif

#if wxUSE_PROPGRID
	using namespace RubyWX::PropertyGridManager;
	rb_cWXPropertyGridManager = rb_define_class_under(rb_mWX,"PropertyGridManager",rb_cWXPanel);
	rb_define_alloc_func(rb_cWXPropertyGridManager,_alloc);

	rb_include_module(rb_cWXPropertyGridManager,rb_mWXPropertyGridInterface);

	rb_define_method(rb_cWXPropertyGridManager,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXPropertyGridManager,"tool_bar",RUBY_METHOD_FUNC(_GetToolBar),0);

	rb_define_method(rb_cWXPropertyGridManager,"current_page",RUBY_METHOD_FUNC(_GetCurrentPage),0);

	rb_define_method(rb_cWXPropertyGridManager,"add_page",RUBY_METHOD_FUNC(_add_page),-1);
	rb_define_method(rb_cWXPropertyGridManager,"insert_page",RUBY_METHOD_FUNC(_insert_page),-1);
	rb_define_method(rb_cWXPropertyGridManager,"each_page",RUBY_METHOD_FUNC(_each),0);

	rb_define_const(rb_cWXPropertyGridManager,"DEFAULT_STYLE",INT2NUM(wxPGMAN_DEFAULT_STYLE));
	rb_define_const(rb_cWXPropertyGridManager,"TOOLBAR",INT2NUM(wxPG_TOOLBAR));
	rb_define_const(rb_cWXPropertyGridManager,"DESCRIPTION",INT2NUM(wxPG_DESCRIPTION));

	rb_define_const(rb_cWXPropertyGridManager,"EX_MODE_BUTTONS",INT2NUM(wxPG_EX_MODE_BUTTONS));

	registerInfo<wxPropertyGridManager>(rb_cWXPropertyGridManager);
#endif

}
