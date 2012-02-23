/*
 * wxSize.cpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#include "wxSize.hpp"
VALUE rb_cWXSize;


void Init_WXSize(VALUE rb_mWX)
{
	rb_cWXSize = rb_define_class_under(rb_mWX,"Size",rb_cObject);
}
