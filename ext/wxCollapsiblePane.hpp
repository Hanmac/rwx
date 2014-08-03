/*
 * wxCollapsiblePane.hpp
 *
 *  Created on: 19.07.2014
 *      Author: hanmac
 */

#ifndef WXCOLLAPSIBLEPANE_HPP_
#define WXCOLLAPSIBLEPANE_HPP_


#include "wxWindow.hpp"

extern VALUE rb_cWXCollapsiblePane;
DLL_LOCAL void Init_WXCollapsiblePane(VALUE rb_mWX);

#if wxUSE_COLLPANE
#include <wx/collpane.h>
#endif

#endif /* WXCOLLAPSIBLEPANE_HPP_ */
