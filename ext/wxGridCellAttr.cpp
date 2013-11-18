/*
 * wxGridCellAttr.cpp
 *
 *  Created on: 19.04.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"
#include "wxGridCellAttr.hpp"

VALUE rb_cWXGridCellAttr;

#if wxUSE_GRID

template <>
VALUE wrap< wxGridCellAttr >(wxGridCellAttr* window)
{
	return wrapPtr(window,rb_cWXGridCellAttr);
}


namespace RubyWX {
namespace GridCellAttr {
//macro_attr(Path,wxString)


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

	registerType<wxGridCellAttr>(rb_cWXGridCellAttr);
#endif
}
