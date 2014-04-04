/*
 * wxPropertyGridInterface.hpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#ifndef WXPROPERTYGRIDINTERFACE_HPP_
#define WXPROPERTYGRIDINTERFACE_HPP_

#include "main.hpp"

extern VALUE rb_mWXPropertyGridInterface;

DLL_LOCAL void Init_WXPropertyGridInterface(VALUE rb_mWX);

#if wxUSE_PROPGRID
#include <wx/propgrid/manager.h>

template <>
wxPGPropArgCls unwrap< wxPGPropArgCls >(const VALUE &obj);

namespace RubyWX {
namespace PropertyGridInterface {

DLL_LOCAL void _set_extra_style(wxWindow *wnd,VALUE hash);

}
}

#endif

#endif /* WXPROPERTYGRIDINTERFACE_HPP_ */
