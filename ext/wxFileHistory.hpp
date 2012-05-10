/*
 * wxFileHistory.hpp
 *
 *  Created on: 06.03.2012
 *      Author: hanmac
 */

#ifndef WXFILEHISTORY_HPP_
#define WXFILEHISTORY_HPP_

#include "main.hpp"

extern VALUE rb_cWXFileHistory;

void Init_WXFileHistory(VALUE rb_mWX);
#if wxUSE_FILE_HISTORY
#include <wx/filehistory.h>
#endif

#endif /* WXFILEHISTORY_HPP_ */
