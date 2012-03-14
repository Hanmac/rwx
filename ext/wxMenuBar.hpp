/*
 * wxMenuBar.hpp
 *
 *  Created on: 24.02.2012
 *      Author: hanmac
 */

#ifndef WXMENUBAR_HPP_
#define WXMENUBAR_HPP_


#include "wxWindow.hpp"

extern VALUE rb_cWXMenuBar;
void Init_WXMenuBar(VALUE rb_mWX);

template <>
inline VALUE wrap< wxMenuBar >(wxMenuBar* window)
{
	return getEvtObj(window,rb_cWXMenuBar);
}


template <>
inline wxMenuBar* wrap< wxMenuBar* >(const VALUE &vwindow)
{
	return unwrapPtr<wxMenuBar>(vwindow, rb_cWXMenuBar);
}



#endif /* WXMENUBAR_HPP_ */
