/*
 * wxFileCtrlGeneric.hpp
 *
 *  Created on: 07.05.2012
 *      Author: hanmac
 */

#ifndef WXFILECTRLGENERIC_HPP_
#define WXFILECTRLGENERIC_HPP_

#include "wxFileCtrlBase.hpp"

extern VALUE rb_cWXFileCtrlGeneric;

void Init_WXFileCtrlGeneric(VALUE rb_mWX);
#if wxUSE_FILECTRL
#include <wx/generic/filectrlg.h>
#endif
#endif /* WXFILECTRLGENERIC_HPP_ */
