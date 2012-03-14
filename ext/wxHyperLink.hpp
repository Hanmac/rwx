/*
 * wxHyperLink.hpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#ifndef WXHYPERLINK_HPP_
#define WXHYPERLINK_HPP_

#include "main.hpp"

extern VALUE rb_cWXHyperLink;

void Init_WXHyperLink(VALUE rb_mWX);

#if wxUSE_HYPERLINKCTRL
#include <wx/hyperlink.h>
template <>
inline VALUE wrap< wxHyperlinkCtrl >(wxHyperlinkCtrl* window)
{
	return getEvtObj(window,rb_cWXHyperLink);
}

template <>
inline wxHyperlinkCtrl* wrap< wxHyperlinkCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxHyperlinkCtrl>(vwindow, rb_cWXHyperLink);
}

#endif
#endif /* WXHYPERLINK_HPP_ */
