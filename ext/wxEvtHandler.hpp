/*
 * wxEvtHandler.hpp
 *
 *  Created on: 17.02.2012
 *      Author: hanmac
 */

#ifndef WXEVTHANDLER_HPP_
#define WXEVTHANDLER_HPP_


#include "wxMenu.hpp"
#include "wxApp.hpp"
#include "wxWindow.hpp"


extern VALUE rb_cWXEvtHandler;
void Init_WXEvtHandler(VALUE rb_mWX);


template <>
inline VALUE wrap< wxEvtHandler >(wxEvtHandler *color )
{
	return Data_Wrap_Struct(rb_cWXEvtHandler, NULL, free, color);
}

template <>
inline wxEvtHandler* wrap< wxEvtHandler* >(const VALUE &vcolor)
{
	return unwrapPtr<wxEvtHandler>(vcolor, rb_cWXEvtHandler);
}



#endif /* WXEVTHANDLER_HPP_ */
