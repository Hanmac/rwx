/*
 * wxGridCellAttr.hpp
 *
 *  Created on: 19.04.2012
 *      Author: hanmac
 */

#ifndef WXGRIDCELLATTR_HPP_
#define WXGRIDCELLATTR_HPP_

#include "main.hpp"

extern VALUE rb_cWXGridCellAttr;
void Init_WXGridCellAttr(VALUE rb_mWX);

#if wxUSE_GRID
#include <wx/grid.h>

template <>
inline VALUE wrap< wxGridCellAttr >(wxGridCellAttr* window)
{
	return wrap(window,rb_cWXGridCellAttr);
}


template <>
inline wxGridCellAttr* wrap< wxGridCellAttr* >(const VALUE &vwindow)
{
	return unwrapPtr<wxGridCellAttr>(vwindow, rb_cWXGridCellAttr);
}

#endif



#endif /* WXGRIDCELLATTR_HPP_ */
