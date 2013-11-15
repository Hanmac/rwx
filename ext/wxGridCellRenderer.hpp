/*
 * wxGridCellRenderer.hpp
 *
 *  Created on: 19.04.2012
 *      Author: hanmac
 */

#ifndef WXGRIDCELLRENDERER_HPP_
#define WXGRIDCELLRENDERER_HPP_

#include "main.hpp"

extern VALUE rb_cWXGridCellRenderer;
void Init_WXGridCellRenderer(VALUE rb_mWX);

#if wxUSE_GRID
#include <wx/grid.h>

template <>
VALUE wrap< wxGridCellRenderer >(wxGridCellRenderer* window);


template <>
wxGridCellRenderer* unwrap<wxGridCellRenderer*>(const VALUE &arg);

#endif



#endif /* WXGRIDCELLRENDERER_HPP_ */
