/*
 * wxMessageDialogBase.hpp
 *
 *  Created on: 25.11.2012
 *      Author: hanmac
 */

#ifndef WXMESSAGEDIALOGBASE_HPP_
#define WXMESSAGEDIALOGBASE_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXMessageDialogBase;

void Init_WXMessageDialogBase(VALUE rb_mWX);

#if wxUSE_MSGDLG
namespace RubyWX {
namespace MessageDialogBase {
	void _set_options(VALUE hash, wxString &message, wxString &caption, int &style);
}
}
#endif

#endif /* WXMESSAGEDIALOGBASE_HPP_ */
