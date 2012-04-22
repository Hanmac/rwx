/*
 * wxGrid.hpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */

#ifndef WXGRID_HPP_
#define WXGRID_HPP_

#include "main.hpp"

extern VALUE rb_cWXGrid;
void Init_WXGrid(VALUE rb_mWX);

#if wxUSE_GRID
#include <wx/grid.h>

template <>
inline VALUE wrap< wxGrid >(wxGrid* window)
{
	return wrap(window,rb_cWXGrid);
}


template <>
inline wxGrid* wrap< wxGrid* >(const VALUE &vwindow)
{
	return unwrapPtr<wxGrid>(vwindow, rb_cWXGrid);
}

template <>
inline VALUE wrap< wxGridCellCoords >(const wxGridCellCoords& coord)
{
	VALUE result = rb_ary_new();
	rb_ary_push(result,INT2NUM(coord.GetCol()));
	rb_ary_push(result,INT2NUM(coord.GetRow()));
	return result;
}


template <>
inline VALUE wrap< wxGridCellCoordsArray >(const wxGridCellCoordsArray &st )
{
	VALUE ary = rb_ary_new();

	for(size_t i = 0; i < st.Count() ; ++i)
	{
		rb_ary_push(ary,wrap(st.Item(i)));
	}
	return ary;
}

#endif

#endif /* WXGRID_HPP_ */
