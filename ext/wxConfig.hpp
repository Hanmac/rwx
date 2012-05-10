/*
 * wxConfig.hpp
 *
 *  Created on: 06.03.2012
 *      Author: hanmac
 */

#ifndef WXCONFIG_HPP_
#define WXCONFIG_HPP_

#include "main.hpp"
extern VALUE rb_cWXConfig;
void Init_WXConfig(VALUE rb_mWX);
#if wxUSE_CONFIG
#include <wx/config.h>
#endif
#endif /* WXCONFIG_HPP_ */
