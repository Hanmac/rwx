/*
 * wxSize.hpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#ifndef WXSIZE_HPP_
#define WXSIZE_HPP_

#include "main.hpp"

extern VALUE rb_cWXSize;
void Init_WXSize(VALUE rb_mWX);

extern ID rwxID_width,rwxID_height;


template <>
VALUE wrap< wxSize >(wxSize *size );

template <>
bool is_wrapable< wxSize >(const VALUE &vsize);

template <>
wxSize unwrap< wxSize >(const VALUE &vsize);

bool check_negative_size(VALUE val, wxSize &size);
bool check_negative_size(const int &width, const int &height);
bool check_negative_size(const wxSize &size);

#endif /* WXSIZE_HPP_ */
