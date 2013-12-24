/*
 * wxSizer.hpp
 *
 *  Created on: 20.03.2012
 *      Author: hanmac
 */

#ifndef WXSIZER_HPP_
#define WXSIZER_HPP_

#include "wxBoxSizer.hpp"

extern VALUE rb_cWXSizer;

void Init_WXSizer(VALUE rb_mWX);

template <>
wxSizerFlags unwrap< wxSizerFlags >(const VALUE &hash);

template <>
wxSizer* unwrap<wxSizer*>(const VALUE &arg);

#endif /* WXSIZER_HPP_ */
