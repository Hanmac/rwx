/*
 * wxComboBox.hpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */

#ifndef WXCOMBOBOX_HPP_
#define WXCOMBOBOX_HPP_

#include "main.hpp"

extern VALUE rb_cWXComboBox;

void Init_WXComboBox(VALUE rb_mWX);

#if wxUSE_COMBOBOX
#include <wx/combobox.h>
#endif


#endif /* WXCOMBOBOX_HPP_ */
