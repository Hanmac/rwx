/*
 * wxGridCellAttr.cpp
 *
 *  Created on: 19.04.2012
 *      Author: hanmac
 */

#include "wxGridCellAttr.hpp"

VALUE rb_cWXGridCellAttr;

#if wxUSE_GRID

namespace RubyWX {
namespace GridCellAttr {
//macro_attr(Path,wxString)


VALUE _alloc(VALUE self)
{
	return wrap(new wxGridCellAttr(),self);
}

}
}

#endif

void Init_WXGridCellAttr(VALUE rb_mWX)
{
#if wxUSE_GRID
	using namespace RubyWX::GridCellAttr;
	rb_cWXGridCellAttr = rb_define_class_under(rb_mWX,"GridCellAttr",rb_cObject);
	rb_define_alloc_func(rb_cWXGridCellAttr,_alloc);
#endif
}
