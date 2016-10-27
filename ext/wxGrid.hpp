/*
 * wxGrid.hpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */

#ifndef WXGRID_HPP_
#define WXGRID_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXGrid;
void Init_WXGrid(VALUE rb_mWX);

#if wxUSE_GRID
#include <wx/grid.h>

template <>
VALUE wrap< wxGridCellCoords >(const wxGridCellCoords& coord);

template <>
VALUE wrap< wxGridCellCoordsArray >(const wxGridCellCoordsArray &st );

#endif

#endif /* WXGRID_HPP_ */
