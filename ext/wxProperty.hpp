/*
 * wxProperty.hpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#ifndef WXPROPERTY_HPP_
#define WXPROPERTY_HPP_

#include "main.hpp"

extern VALUE rb_cWXProperty;
DLL_LOCAL void Init_WXProperty(VALUE rb_mWX);

template<typename T>
void registerProperty(VALUE mod,const char* name)
{
	VALUE klass = rb_define_class_under(mod,name,rb_cWXProperty);
	registerInfo<T>(klass);
}


#endif /* WXPROPERTY_HPP_ */
