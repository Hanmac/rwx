/*
 * wxSingleChoiceDialog.hpp
 *
 *  Created on: 08.12.2013
 *      Author: hanmac
 */

#ifndef WXMULTICHOICEDIALOG_HPP_
#define WXMULTICHOICEDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXMultiChoiceDialog;

void Init_WXMultiChoiceDialog(VALUE rb_mWX);
#if wxUSE_CHOICEDLG
#include <wx/choicdlg.h>
#endif
#endif /* WXMULTICHOICEDIALOG_HPP_ */
