/*
 * wxOwnerDrawnComboBox.hpp
 *
 *  Created on: 14.10.2014
 *      Author: hanmac
 */

#ifndef WXOWNERDRAWNCOMBOBOX_HPP_
#define WXOWNERDRAWNCOMBOBOX_HPP_

#include "main.hpp"

extern VALUE rb_cWXOwnerDrawnComboBox;

void Init_WXOwnerDrawnComboBox(VALUE rb_mWX);

#if wxUSE_ODCOMBOBOX
#include <wx/odcombo.h>
#endif


#endif /* WXOWNERDRAWNCOMBOBOX_HPP_ */
