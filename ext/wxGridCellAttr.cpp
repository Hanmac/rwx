/*
 * wxGridCellAttr.cpp
 *
 *  Created on: 19.04.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"
#include "wxGridCellAttr.hpp"

#include "wxColor.hpp"
#include "wxFont.hpp"

VALUE rb_cWXGridCellAttr;

#if wxUSE_GRID

template <>
VALUE wrap< wxGridCellAttr >(wxGridCellAttr* window)
{
	return wrapTypedPtr(window,rb_cWXGridCellAttr);
}


#define _self unwrap<wxGridCellAttr*>(self)



namespace RubyWX {
namespace GridCellAttr {

macro_attr(TextColour,wxColour)
macro_attr(BackgroundColour,wxColour)
macro_attr(Font,wxFont)

APP_PROTECT(wxGridCellAttr)

}
}

#endif

DLL_LOCAL void Init_WXGridCellAttr(VALUE rb_mWX)
{

#if wxUSE_GRID
	using namespace RubyWX::GridCellAttr;
	rb_cWXGridCellAttr = rb_define_class_under(rb_mWX,"GridCellAttr",rb_cObject);
	rb_define_alloc_func(rb_cWXGridCellAttr,_alloc);

#if 0
	rb_define_attr(rb_cWXGridCellAttr,"text_color",1,1);
	rb_define_attr(rb_cWXGridCellAttr,"background_color",1,1);
	rb_define_attr(rb_cWXGridCellAttr,"font",1,1);
#endif

	//TODO make GridCellAttr copyable
	rb_undef_method(rb_cWXGridCellAttr,"initialize_copy");
	rb_undef_method(rb_cWXGridCellAttr,"_load");
	rb_undef_method(rb_cWXGridCellAttr,"_dump");

	rb_define_attr_method(rb_cWXGridCellAttr,"text_color",_getTextColour,_setTextColour);
	rb_define_attr_method(rb_cWXGridCellAttr,"background_color",_getBackgroundColour,_setBackgroundColour);
	rb_define_attr_method(rb_cWXGridCellAttr,"font",_getFont,_setFont);

	registerType<wxGridCellAttr>(rb_cWXGridCellAttr);
#endif
}
