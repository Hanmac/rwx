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
VALUE wrap< wxGridCellAttr >(wxGridCellAttr* window);

#endif



#endif /* WXGRIDCELLATTR_HPP_ */
