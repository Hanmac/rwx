/*
 * wxtextEntry.hpp
 *
 *  Created on: 28.04.2012
 *      Author: hanmac
 */

#ifndef WXTEXTENTRY_HPP_
#define WXTEXTENTRY_HPP_

#include "main.hpp"

extern VALUE rb_mWXTextEntry;

void Init_WXTextEntry(VALUE rb_mWX);

#include <wx/textentry.h>

#endif /* WXTEXTENTRY_HPP_ */
