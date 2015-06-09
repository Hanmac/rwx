/*
 * wxHyperLink.hpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#ifndef WXHYPERLINK_HPP_
#define WXHYPERLINK_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXHyperLink;

void Init_WXHyperLink(VALUE rb_mWX);

#if wxUSE_HYPERLINKCTRL
#include <wx/hyperlink.h>
#endif
#endif /* WXHYPERLINK_HPP_ */
