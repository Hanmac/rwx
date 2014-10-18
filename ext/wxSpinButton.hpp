/*
 * wxSpinButton.hpp
 *
 *  Created on: 22.03.2012
 *      Author: hanmac
 */

#ifndef WXSPINBUTTON_HPP_
#define WXSPINBUTTON_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXSpinButton, rb_cWXSpinEvent;

void Init_WXSpinButton(VALUE rb_mWX);
#if wxUSE_SPINBTN
#include <wx/spinbutt.h>
#endif

#endif /* WXSPINBUTTON_HPP_ */
