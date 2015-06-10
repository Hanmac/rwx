/*
 * wxHyperLinkBase.hpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#ifndef WXHYPERLINKBASE_HPP_
#define WXHYPERLINKBASE_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXHyperLinkBase;

void Init_WXHyperLinkBase(VALUE rb_mWX);

#if wxUSE_HYPERLINKCTRL
#include <wx/hyperlink.h>
#endif
#endif /* WXHYPERLINKBASE_HPP_ */
