/*
 * wxItemContainer.hpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */

#ifndef WXITEMCONTAINER_HPP_
#define WXITEMCONTAINER_HPP_

#include "main.hpp"

extern VALUE rb_mWXItemContainer;

void Init_WXItemContainer(VALUE rb_mWX);

#if wxUSE_CONTROLS

namespace RubyWX {
namespace ItemContainer {

DLL_LOCAL VALUE _setItems(VALUE self, VALUE other);

DLL_LOCAL VALUE _getSelection(VALUE self);
DLL_LOCAL VALUE _setSelection(VALUE self, VALUE other);

DLL_LOCAL VALUE _getStringSelection(VALUE self);
DLL_LOCAL VALUE _setStringSelection(VALUE self, VALUE other);

}
}
#endif

#endif /* WXITEMCONTAINER_HPP_ */
