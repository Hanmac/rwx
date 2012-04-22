/*
 * wxPanel.hpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#ifndef WXPANEL_HPP_
#define WXPANEL_HPP_


#include "wxGrid.hpp"
#include "wxWizardPage.hpp"

extern VALUE rb_cWXPanel;
void Init_WXPanel(VALUE rb_mWX);

template <>
inline VALUE wrap< wxPanel >(wxPanel* window)
{
	return wrap(window,rb_cWXPanel);
}


template <>
inline wxPanel* wrap< wxPanel* >(const VALUE &vwindow)
{
	return unwrapPtr<wxPanel>(vwindow, rb_cWXPanel);
}

#endif /* WXPANEL_HPP_ */
