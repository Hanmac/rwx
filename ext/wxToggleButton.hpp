/*
 * wxToggleButton.hpp
 *
 *  Created on: 10.12.2013
 *      Author: hanmac
 */

#ifndef WXTOGGLEBUTTON_HPP_
#define WXTOGGLEBUTTON_HPP_

#include "wxAnyButton.hpp"

extern VALUE rb_cWXToggleButton;

void Init_WXToggleButton(VALUE rb_mWX);

#if wxUSE_TOGGLEBTN
#include <wx/tglbtn.h>
#endif

#endif /* WXTOGGLEBUTTON_HPP_ */
