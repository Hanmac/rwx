/*
 * wxAnyChoiceDialog.hpp
 *
 *  Created on: 08.12.2013
 *      Author: hanmac
 */

#ifndef WXSINGLECHOICEDIALOG_HPP_
#define WXSINGLECHOICEDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXSingleChoiceDialog;

void Init_WXSingleChoiceDialog(VALUE rb_mWX);
#if wxUSE_CHOICEDLG
#include <wx/choicdlg.h>
#endif
#endif /* WXSINGLECHOICEDIALOG_HPP_ */
