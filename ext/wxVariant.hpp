/*
 * wxVariant.hpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#ifndef WXVARIANT_HPP_
#define WXVARIANT_HPP_

#include "main.hpp"

template <>
VALUE wrap< wxVariant >(const wxVariant &window);
wxVariant unwrapVariant(VALUE obj,const wxString &type);

#endif /* WXVARIANT_HPP_ */
