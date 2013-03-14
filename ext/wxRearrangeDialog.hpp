/*
 * wxRearrangeDialog.hpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#ifndef WXREARRANGEDIALOG_HPP_
#define WXREARRANGEDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXRearrangeDialog;

void Init_WXRearrangeDialog(VALUE rb_mWX);
#if wxUSE_REARRANGECTRL
#include <wx/rearrangectrl.h>
#endif

#endif /* WXREARRANGEDIALOG_HPP_ */
