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

#if wxUSE_TOOLBAR
singlereturn(GetToolBar)
#endif

singlereturn(GetPageCount)

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

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_flag_option(hash,"toolbar",wxPG_TOOLBAR,style);
			set_hash_flag_option(hash,"description",wxPG_DESCRIPTION,style);

		}

		if(nil_check(parent))
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,style
			);
		
	}

	rb_call_super(argc,argv);


	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		PropertyGridInterface::_set_extra_style(_self,hash);
	}
	return self;
}

wxPropertyGridPage* create_page(VALUE self,VALUE page,VALUE hash)
{
	if(NIL_P(page))
		return NULL;

	if(rb_obj_is_kind_of(page,rb_cClass) && RTEST(rb_class_inherited_p(page,rb_cWXPropertyGridPage))) {
		VALUE argv2[] = {self, hash };
		page = rb_class_new_instance(2,argv2,page);
	}

	return unwrap<wxPropertyGridPage*>(page);
}


/*
 * call-seq:
 *   add_page(label, [bitmap], [page]) -> WX::PropertyGridPage
 *   add_page(label, [bitmap], page, **options) [{|page| ... }] -> WX::PropertyGridPage
 *
 * adds a new page to the PropertyGridManager widget.
 *
 * ===Arguments
 * * label is the Label of the page. String
 * * bitmap is a WX::Bitmap
 * * page is a WX::PropertyGridPage, or a Class inherited from WX::PropertyGridPage
 * ===Return value
 * WX::PropertyGridPage
 *
*/
DLL_LOCAL VALUE _add_page(int argc,VALUE *argv,VALUE self)
{
	VALUE label,bitmap,page,hash;

	rb_scan_args(argc, argv, "13",&label,&bitmap,&page,&hash);

	rb_check_frozen(self);

	wxPropertyGridPage* cpage = create_page(self,page,hash);

	return wrap(_self->AddPage(unwrap<wxString>(label),unwrap<wxBitmap>(bitmap),cpage));
}

/*
 * call-seq:
 *   insert_page(pos, label, [bitmap], [page]) -> WX::PropertyGridPage
 *   insert_page(pos, label, [bitmap], page, **options) [{|page| ... }] -> WX::PropertyGridPage
 *
 * inserts a new page to the PropertyGridManager widget.
 *
 * ===Arguments
 * * pos is a Integer
 * * label is the Label of the page. String
 * * bitmap is a WX::Bitmap
 * * page is a WX::PropertyGridPage, or a Class inherited from WX::PropertyGridPage
 * ===Return value
 * WX::PropertyGridPage
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of pages
 *
*/
DLL_LOCAL VALUE _insert_page(int argc,VALUE *argv,VALUE self)
{
	VALUE idx,label,bitmap,page,hash;

	rb_scan_args(argc, argv, "23",&idx,&label,&bitmap,&page,&hash);

	rb_check_frozen(self);

	int cidx = RB_NUM2INT(idx);
	if(check_index(cidx,_self->GetPageCount()))
	{
		wxPropertyGridPage* cpage = create_page(self,page,hash);

		return wrap(_self->InsertPage(cidx,unwrap<wxString>(label),unwrap<wxBitmap>(bitmap),cpage));
	}
	return Qnil;
}


/*
 * call-seq:
 *   prepend_page(label, [bitmap], [page]) -> WX::PropertyGridPage
 *   prepend_page(label, [bitmap], page, **options) [{|page| ... }] -> WX::PropertyGridPage
 *
 * prepends a new page to the PropertyGridManager widget.
 *
 * ===Arguments
 * * label is the Label of the page. String
 * * bitmap is a WX::Bitmap
 * * page is a WX::PropertyGridPage, or a Class inherited from WX::PropertyGridPage
 * ===Return value
 * WX::PropertyGridPage
 *
*/
DLL_LOCAL VALUE _prepend_page(int argc,VALUE *argv,VALUE self)
{
	VALUE label,bitmap,page,hash;

	rb_scan_args(argc, argv, "13",&label,&bitmap,&page,&hash);

	rb_check_frozen(self);

	wxPropertyGridPage* cpage = create_page(self,page,hash);

	return wrap(_self->InsertPage(0,unwrap<wxString>(label),unwrap<wxBitmap>(bitmap),cpage));
}

/*
 * call-seq:
 *   each_page -> Enumerator
 *   each_page { | page| } -> self
 *
 * iterates the pages of this control.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_GetPageCount));
	for(std::size_t i = 0; i < _self->GetPageCount(); ++i)
	{
		rb_yield(wrap(_self->GetPage(i)));
	}
	return self;
}


}
}
#endif

/*
 * Document-class: WX::PropertyGridManager
 *
 * This class represents a PropertyGridManager with multiple pages.
*/

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */
/* Document-const: TOOLBAR
 * Show toolbar for mode and page selection.
 */
/* Document-const: DESCRIPTION
 * Show adjustable text box showing description.
 */

/* Document-attr: tool_bar
 * the WX::ToolBar of this WX::PropertyGridManager when available.
 */
/* Document-attr: grid
 * the WX::PropertyGrid of this WX::PropertyGridManager.
 */
/* Document-attr: current_page
 * the current WX::PropertyGridPage of this WX::PropertyGridManager.
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

#if 0
	rb_define_attr(rb_cWXPropertyGridManager,"tool_bar",1,0);
	rb_define_attr(rb_cWXPropertyGridManager,"grid",1,0);
	rb_define_attr(rb_cWXPropertyGridManager,"current_page",1,0);
#endif

	rb_include_module(rb_cWXPropertyGridManager,rb_mWXPropertyGridInterface);

	rb_define_method(rb_cWXPropertyGridManager,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
#if wxUSE_TOOLBAR
	rb_define_attr_method(rb_cWXPropertyGridManager,"tool_bar",_GetToolBar,0);
#else
	rb_define_attr_method_missing(rb_cWXPropertyGridManager,"tool_bar", true, false);
#endif
	rb_define_attr_method(rb_cWXPropertyGridManager,"grid",_GetGrid,0);
	rb_define_attr_method(rb_cWXPropertyGridManager,"current_page",_GetCurrentPage,0);

	rb_define_method(rb_cWXPropertyGridManager,"add_page",RUBY_METHOD_FUNC(_add_page),-1);
	rb_define_method(rb_cWXPropertyGridManager,"insert_page",RUBY_METHOD_FUNC(_insert_page),-1);
	rb_define_method(rb_cWXPropertyGridManager,"each_page",RUBY_METHOD_FUNC(_each),0);

	rb_define_const(rb_cWXPropertyGridManager,"DEFAULT_STYLE",RB_INT2NUM(wxPGMAN_DEFAULT_STYLE));
	rb_define_const(rb_cWXPropertyGridManager,"TOOLBAR",RB_INT2NUM(wxPG_TOOLBAR));
	rb_define_const(rb_cWXPropertyGridManager,"DESCRIPTION",RB_INT2NUM(wxPG_DESCRIPTION));

	registerInfo<wxPropertyGridManager>(rb_cWXPropertyGridManager);
#endif

}
