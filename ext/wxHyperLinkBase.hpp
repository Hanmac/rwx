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


namespace RubyWX {
namespace HyperLinkBase {

DLL_LOCAL void _set_options(VALUE hash, wxWindowID &id, wxString &label, wxString &url, int &style);
DLL_LOCAL void _set_hash_options(VALUE hash, wxHyperlinkCtrlBase *self);

}
}

#endif
#endif /* WXHYPERLINKBASE_HPP_ */
