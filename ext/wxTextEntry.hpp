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


namespace RubyWX {
namespace TextEntry {

DLL_LOCAL VALUE _getSelection(VALUE self);
DLL_LOCAL VALUE _setSelection(VALUE self, VALUE range);

DLL_LOCAL VALUE _GetStringSelection(VALUE self);
DLL_LOCAL VALUE _setStringSelection(VALUE self, VALUE val);

}
}

#endif /* WXTEXTENTRY_HPP_ */
