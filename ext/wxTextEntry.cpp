/*
 * wxTextEntry.cpp
 *
 *  Created on: 30.12.2013
 *      Author: hanmac
 */

#include "wxTextEntry.hpp"
#include "wxTextCtrl.hpp"
#include "wxSearchCtrl.hpp"
#include "wxComboBox.hpp"
#include "wxPoint.hpp"

VALUE rb_mWXTextEntry;

template <>
wxTextEntryBase* unwrap< wxTextEntryBase* >(const VALUE &obj)
{
#if	wxUSE_TEXTCTRL
	if(rb_obj_is_kind_of(obj,rb_cWXTextCtrl))
		return unwrap<wxTextCtrl*>(obj);
#endif
#if	wxUSE_SEARCHCTRL
	if(rb_obj_is_kind_of(obj,rb_cWXSearchCtrl))
		return unwrap<wxSearchCtrl*>(obj);
#endif
#if wxUSE_COMBOBOX
	if(rb_obj_is_kind_of(obj,rb_cWXComboBox))
		return unwrap<wxComboBox*>(obj);
#endif
	rb_raise(rb_eTypeError,"Unknown wxTextEntryBase: %s",rb_obj_classname(obj));
return NULL;
}


#define _self unwrap<wxTextEntryBase*>(self)

namespace RubyWX {
namespace TextEntry {


singlefunc(Copy)
singlefunc(Cut)
singlefunc(Paste)

singlereturn(CanCopy)
singlereturn(CanCut)
singlereturn(CanPaste)

singlefunc(Undo)
singlefunc(Redo)

singlereturn(CanUndo)
singlereturn(CanRedo)

singlefunc(SelectAll)
singlefunc(SelectNone)
singlereturn(HasSelection)

macro_attr(Hint,wxString)

macro_attr_bool(Editable)

macro_attr(Margins,wxPoint)

}
}

DLL_LOCAL void Init_WXTextEntry(VALUE rb_mWX)
{

	using namespace RubyWX::TextEntry;
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");

	rb_define_method(rb_mWXTextEntry,"cut",RUBY_METHOD_FUNC(_Cut),0);
	rb_define_method(rb_mWXTextEntry,"copy",RUBY_METHOD_FUNC(_Copy),0);
	rb_define_method(rb_mWXTextEntry,"paste",RUBY_METHOD_FUNC(_Paste),0);

	rb_define_method(rb_mWXTextEntry,"can_cut?",RUBY_METHOD_FUNC(_CanCut),0);
	rb_define_method(rb_mWXTextEntry,"can_copy?",RUBY_METHOD_FUNC(_CanCopy),0);
	rb_define_method(rb_mWXTextEntry,"can_paste?",RUBY_METHOD_FUNC(_CanPaste),0);

	rb_define_method(rb_mWXTextEntry,"undo",RUBY_METHOD_FUNC(_Undo),0);
	rb_define_method(rb_mWXTextEntry,"redo",RUBY_METHOD_FUNC(_Redo),0);

	rb_define_method(rb_mWXTextEntry,"can_undo?",RUBY_METHOD_FUNC(_CanUndo),0);
	rb_define_method(rb_mWXTextEntry,"can_redo?",RUBY_METHOD_FUNC(_CanRedo),0);

}



