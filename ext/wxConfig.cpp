/*
 * wxConfig.cpp
 *
 *  Created on: 06.03.2012
 *      Author: hanmac
 */

#include "wxConfig.hpp"

VALUE rb_cWXConfig;


void Init_WXConfig(VALUE rb_mWX)
{
#if wxUSE_CONFIG
	rb_cWXConfig = rb_define_class_under(rb_mWX,"Config",rb_cObject);
	registerType<wxConfig>(rb_cWXConfig);
#endif
}
