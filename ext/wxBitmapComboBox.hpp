/*
 * wxBitmapComboBox.hpp
 *
 *  Created on: 21.06.2014
 *      Author: hanmac
 */

#ifndef WXBITMAPCOMBOBOX_HPP_
#define WXBITMAPCOMBOBOX_HPP_

#include "main.hpp"

extern VALUE rb_cWXBitmapComboBox;

void Init_WXBitmapComboBox(VALUE rb_mWX);

#if wxUSE_BITMAPCOMBOBOX
#include <wx/bmpcbox.h>
#endif


#endif /* WXBITMAPCOMBOBOX_HPP_ */
