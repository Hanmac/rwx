/*
 * wxRichTextCtrl.cpp
 *
 *  Created on: 22.06.2014
 *      Author: hanmac
 */

#include "wxRichTextCtrl.hpp"

#include "wxTextArea.hpp"
#include "wxTextEntry.hpp"
#include "wxCursor.hpp"

#include "wxEvtHandler.hpp"
#include "wxNotifyEvent.hpp"

VALUE rb_cWXRichTextCtrl, rb_cWXRichTextCtrlEvent;

#if wxUSE_RICHTEXT
#define _self unwrap<wxRichTextCtrl*>(self)

namespace RubyWX {
namespace RichTextCtrl {

void set_style_flags(VALUE hash,int& style)
{
	set_hash_flag_option(hash,"readonly",wxRE_READONLY,style);
	set_hash_flag_option(hash,"multiline",wxRE_MULTILINE,style);
}


macro_attr(Value,wxString)
macro_attr(Filename,wxString)

macro_attr(ContextMenu,wxMenu*)

macro_attr(TextCursor,wxCursor)
macro_attr(URLCursor,wxCursor)

singlereturn(IsSingleLine)
singlereturn(IsMultiLine)

APP_PROTECT(wxTextCtrl)


/*
 * call-seq:
 *   RichTextCtrl.new(parent, name, [options])
 *   RichTextCtrl.new(parent, [options])
 *
 * creates a new RichTextCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * value String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent, name, hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		wxString value(wxEmptyString);
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"style",style);

			set_style_flags(hash,style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,value,wxDefaultPosition,wxDefaultSize,style);
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(name,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(value,Value,wxString)
	}

	return self;
}

namespace Event
{
#undef _self
#define _self unwrapPtr<wxRichTextEvent>(self,rb_cWXRichTextCtrlEvent)

macro_attr(Position,int)

}

}
}
#endif


/* Document-attr: value
 * the value of the RichTextCtrl. String
 */

/* Document-method: single_line?
 * call-seq:
 *   single_line? -> true/false
 *
 * Returns true if this is a single line edit control and false otherwise.
 * ===Return value
 * true/false
*/
/* Document-method: multi_line?
 * call-seq:
 *   multi_line? -> true/false
 *
 * Returns true if this is a multi line edit control and false otherwise.
 * ===Return value
 * true/false
*/

/* Document-const: READONLY
 *   The text will not be user-editable.
 */
/* Document-const: MULTILINE
 *   The text control allows multiple lines. If this style is not specified, line break characters should not be used in the controls value.
 */
DLL_LOCAL void Init_WXRichTextCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_mWXTextArea = rb_define_module_under(rb_mWX,"TextArea");
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");

	rb_define_attr(rb_cWXRichTextCtrl,"value",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"filename",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"text_cursor",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"url_cursor",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"context_menu",1,1);
#endif

#if wxUSE_RICHTEXT
	using namespace RubyWX::RichTextCtrl;
	rb_cWXRichTextCtrl = rb_define_class_under(rb_mWX,"RichTextCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXRichTextCtrl,_alloc);

	rb_include_module(rb_cWXRichTextCtrl,rb_mWXTextArea);
	rb_include_module(rb_cWXRichTextCtrl,rb_mWXTextEntry);

	rb_define_method(rb_cWXRichTextCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXRichTextCtrl,"single_line?",RUBY_METHOD_FUNC(_IsSingleLine),0);
	rb_define_method(rb_cWXRichTextCtrl,"multi_line?",RUBY_METHOD_FUNC(_IsMultiLine),0);

	rb_define_attr_method(rb_cWXRichTextCtrl,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXRichTextCtrl,"filename",_getFilename,_setFilename);
	rb_define_attr_method(rb_cWXRichTextCtrl,"text_cursor",_getTextCursor,_setTextCursor);
	rb_define_attr_method(rb_cWXRichTextCtrl,"url_cursor",_getURLCursor,_setURLCursor);
	rb_define_attr_method(rb_cWXRichTextCtrl,"context_menu",_getContextMenu,_setContextMenu);

	rb_define_const(rb_cWXRichTextCtrl,"READONLY",INT2NUM(wxRE_READONLY));
	rb_define_const(rb_cWXRichTextCtrl,"MULTILINE",INT2NUM(wxRE_MULTILINE));

	rb_cWXRichTextCtrlEvent = rb_define_class_under(rb_cWXEvent,"RichTextCtrl",rb_cWXNotifyEvent);

	registerEventType("richtext_left_click",wxEVT_RICHTEXT_LEFT_CLICK,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_right_click",wxEVT_RICHTEXT_RIGHT_CLICK,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_middle_click",wxEVT_RICHTEXT_MIDDLE_CLICK,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_left_dclick",wxEVT_RICHTEXT_LEFT_DCLICK,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_return",wxEVT_RICHTEXT_RETURN,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_character",wxEVT_RICHTEXT_CHARACTER,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_delete",wxEVT_RICHTEXT_DELETE,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_stylesheet_changing",wxEVT_RICHTEXT_STYLESHEET_CHANGING,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_stylesheet_changed",wxEVT_RICHTEXT_STYLESHEET_CHANGED,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_stylesheet_replacing",wxEVT_RICHTEXT_STYLESHEET_REPLACING,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_stylesheet_replaced",wxEVT_RICHTEXT_STYLESHEET_REPLACED,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_content_inserted",wxEVT_RICHTEXT_CONTENT_INSERTED,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_content_deleted",wxEVT_RICHTEXT_CONTENT_DELETED,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_style_changed",wxEVT_RICHTEXT_STYLE_CHANGED,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_properties_changed",wxEVT_RICHTEXT_PROPERTIES_CHANGED,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_selection_changed",wxEVT_RICHTEXT_SELECTION_CHANGED,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_buffer_reset",wxEVT_RICHTEXT_BUFFER_RESET,rb_cWXRichTextCtrlEvent);
	registerEventType("richtext_focus_object_changed",wxEVT_RICHTEXT_FOCUS_OBJECT_CHANGED,rb_cWXRichTextCtrlEvent);

	registerInfo<wxRichTextCtrl>(rb_cWXRichTextCtrl);
#endif

}
