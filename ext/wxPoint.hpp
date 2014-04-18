/*
 * wxPoint.hpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#ifndef WXPOINT_HPP_
#define WXPOINT_HPP_

#include "main.hpp"

extern VALUE rb_cWXPoint;
DLL_LOCAL void Init_WXPoint(VALUE rb_mWX);


template <>
VALUE wrap< wxRealPoint >(wxRealPoint *point );

template <>
bool is_wrapable< wxRealPoint >(const VALUE &vpoint);

template <>
wxRealPoint unwrap< wxRealPoint >(const VALUE &vpoint);

template <>
VALUE wrap< wxPoint >(const wxPoint &point );

template <>
bool is_wrapable< wxPoint >(const VALUE &vpoint);

template <>
wxPoint unwrap< wxPoint >(const VALUE &vpoint);

template <>
wxPointList* unwrap< wxPointList* >(const VALUE &vpoint);


#endif /* WXPOINT_HPP_ */
