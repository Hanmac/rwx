/*
 * wxPropertyGridInterface.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#include "wxPropertyGridInterface.hpp"
#include "wxPropertyGrid.hpp"
#include "wxProperty.hpp"
#include "wxPropertyGridPage.hpp"
#include "wxPropertyGridManager.hpp"

VALUE rb_mWXPropertyGridInterface;

#if wxUSE_PROPGRID
// i need this because if i try to access the interface directly it fails
template <>
wxPropertyGridInterface* unwrap< wxPropertyGridInterface* >(const VALUE &obj)
{
 if(rb_obj_is_kind_of(obj,rb_cWXPropertyGrid))
	 return unwrap<wxPropertyGrid*>(obj);
 if(rb_obj_is_kind_of(obj,rb_cWXPropertyGridPage))
	 return unwrap<wxPropertyGridPage*>(obj);
 if(rb_obj_is_kind_of(obj,rb_cWXPropertyGridManager))
	 return unwrap<wxPropertyGridManager*>(obj);

 return NULL;
}
template <>
wxPGPropArgCls unwrap< wxPGPropArgCls >(const VALUE &obj)
{
	if(rb_obj_is_kind_of(obj,rb_cWXProperty))
	{
		return unwrap<wxPGProperty*>(obj);
	}
	return unwrap<wxString>(obj);
}


#define _self unwrap<wxPropertyGridInterface*>(self)

namespace RubyWX {
namespace PropertyGridInterface {

singlefunc(Clear)
singlereturn(CollapseAll)
singlereturn(ExpandAll)
singlereturn(GetSelection)

DLL_LOCAL void _set_extra_style(wxWindow *wnd,VALUE hash)
{
	int exstyle = wnd->GetExtraStyle();

	set_hash_flag_option(hash,"mode_buttons",wxPG_EX_MODE_BUTTONS,exstyle);
	set_hash_flag_option(hash,"help_as_tooltips",wxPG_EX_HELP_AS_TOOLTIPS,exstyle);
	set_hash_flag_option(hash,"hide_page_buttons",wxPG_EX_HIDE_PAGE_BUTTONS,exstyle);
	set_hash_flag_option(hash,"multible_selection",wxPG_EX_MULTIPLE_SELECTION,exstyle);
	set_hash_flag_option(hash,"toolbar_separator",wxPG_EX_TOOLBAR_SEPARATOR,exstyle);

	wnd->SetExtraStyle(exstyle);
}

DLL_LOCAL VALUE _append(int argc,VALUE *argv,VALUE self)
{
	VALUE val,prop,hash;
	rb_scan_args(argc, argv, "11:",&val,&prop,&hash);

	wxPGProperty * cprop = unwrap<wxPGProperty*>(val);

	if(NIL_P(prop))
		cprop = _self->Append(cprop);
	else
		cprop = _self->AppendIn(unwrap<wxPGPropArgCls>(prop), cprop);

	VALUE result = wrap(cprop);
	
	if(rb_block_given_p())
		rb_yield(result);
	return result;
}

DLL_LOCAL VALUE _get_selections(VALUE self)
{
	VALUE ary = rb_ary_new();
	const wxArrayPGProperty& props = _self->GetSelectedProperties();
	for(wxArrayPGProperty::const_iterator it = props.begin(); it != props.end(); ++it)
	{
		rb_ary_push(ary,wrap(*it));
	}
	return ary;
}


}
}
#endif


/* Document-const: EX_MODE_BUTTONS
 * Shows alphabetic/categoric mode buttons from toolbar.
 */
/* Document-const: EX_HELP_AS_TOOLTIPS
 * Show property help strings as tool tips instead as text on the status bar.
 */
/* Document-const: EX_HIDE_PAGE_BUTTONS
 * Hides page selection buttons from toolbar.
 */
/* Document-const: EX_MULTIPLE_SELECTION
 * Allows multiple properties to be selected by user.
 */
/* Document-const: EX_TOOLBAR_SEPARATOR
 * Show a separator below the toolbar.
 */
DLL_LOCAL void Init_WXPropertyGridInterface(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::PropertyGridInterface;
	rb_mWXPropertyGridInterface = rb_define_module_under(rb_mWX,"PropertyGridInterface");

	rb_define_method(rb_mWXPropertyGridInterface,"append",RUBY_METHOD_FUNC(_append),-1);

	rb_define_method(rb_mWXPropertyGridInterface,"clear",RUBY_METHOD_FUNC(_Clear),0);

	rb_define_method(rb_mWXPropertyGridInterface,"collapse_all",RUBY_METHOD_FUNC(_CollapseAll),0);
	rb_define_method(rb_mWXPropertyGridInterface,"expand_all",RUBY_METHOD_FUNC(_ExpandAll),0);

	rb_define_method(rb_mWXPropertyGridInterface,"selection",RUBY_METHOD_FUNC(_GetSelection),0);

	rb_define_method(rb_mWXPropertyGridInterface,"selections",RUBY_METHOD_FUNC(_get_selections),0);

	rb_define_const(rb_mWXPropertyGridInterface,"EX_MODE_BUTTONS",RB_INT2NUM(wxPG_EX_MODE_BUTTONS));

	rb_define_const(rb_mWXPropertyGridInterface,"EX_HELP_AS_TOOLTIPS",RB_INT2NUM(wxPG_EX_HELP_AS_TOOLTIPS));
	rb_define_const(rb_mWXPropertyGridInterface,"EX_HIDE_PAGE_BUTTONS",RB_INT2NUM(wxPG_EX_HIDE_PAGE_BUTTONS));
	rb_define_const(rb_mWXPropertyGridInterface,"EX_MULTIPLE_SELECTION",RB_INT2NUM(wxPG_EX_MULTIPLE_SELECTION));
	rb_define_const(rb_mWXPropertyGridInterface,"EX_TOOLBAR_SEPARATOR",RB_INT2NUM(wxPG_EX_TOOLBAR_SEPARATOR));


	wxPropertyGrid::InitAllTypeHandlers();
#endif

}



