/*
 * wxHyperLinkGeneric.hpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#ifndef WXHYPERLINKGENERIC_HPP_
#define WXHYPERLINKGENERIC_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXHyperLinkGeneric;

void Init_WXHyperLinkGeneric(VALUE rb_mWX);

#if wxUSE_HYPERLINKCTRL
#include <wx/hyperlink.h>
#include <wx/generic/hyperlink.h>
#endif
#endif /* WXHYPERLINKGENERIC_HPP_ */
