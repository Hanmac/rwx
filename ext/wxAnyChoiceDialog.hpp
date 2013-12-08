/*
 * wxAnyChoiceDialog.hpp
 *
 *  Created on: 08.12.2013
 *      Author: hanmac
 */

#ifndef WXANYCHOICEDIALOG_HPP_
#define WXANYCHOICEDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXAnyChoiceDialog;

void Init_WXAnyChoiceDialog(VALUE rb_mWX);
#if wxUSE_CHOICEDLG
#include <wx/choicdlg.h>
#endif
#endif /* WXANYCHOICEDIALOG_HPP_ */
