/*
 * wxRichTextCtrl.cpp
 *
 *  Created on: 22.06.2014
 *      Author: hanmac
 */

#include "wxRichTextCtrl.hpp"
#include "wxRichTextAttr.hpp"
#include "wxRichTextStyleSheet.hpp"

#include "wxTextArea.hpp"
#include "wxTextAttr.hpp"
#include "wxTextEntry.hpp"
#include "wxCursor.hpp"
#include "wxFont.hpp"
#include "wxColor.hpp"

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
	set_hash_flag_option(hash,"center_caret",wxRE_CENTRE_CARET,style);
}

macro_attr(Value,wxString)
macro_attr(Filename,wxString)

macro_attr(ContextMenu,wxMenu*)

macro_attr(TextCursor,wxCursor)
macro_attr(URLCursor,wxCursor)

macro_attr(Scale,double)
//macro_attr_bool2(ImagesEnabled,EnableImages)

macro_attr(BasicStyle,wxRichTextAttr)
macro_attr(StyleSheet,wxRichTextStyleSheet*)

macro_attr(CaretPosition,long)

singlereturn(IsSingleLine)
singlereturn(IsMultiLine)

singlereturn(BatchingUndo)
singlereturn(SuppressingUndo)

singlereturn_frozen(Newline)
singlereturn_frozen(LineBreak)

singlereturn_frozen(IsSelectionBold)
singlereturn_frozen(IsSelectionItalics)
singlereturn_frozen(IsSelectionUnderlined)

singlereturn_frozen(PopStyleSheet)

APP_PROTECT(wxRichTextCtrl)


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
		int style(wxRE_MULTILINE);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"style",style);

			set_style_flags(hash,style);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,value,
				wxDefaultPosition,wxDefaultSize,style
			);
		}
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(name,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash,"value", &wxRichTextCtrl::SetValue, _self);
	}

	return self;
}

#define block_func0(n) VALUE _block##n(VALUE self) \
{\
	_self->Begin##n();\
	rb_yield(self);\
	_self->End##n();\
	return self;\
}

#define block_func1(n,func1) VALUE _block##n(VALUE self,VALUE val) \
{\
	_self->Begin##n(func1(val));\
	rb_yield(self);\
	_self->End##n();\
	return self;\
}

#define block_bullet(n,func1,style) VALUE _block##n##Bullet(int argc,VALUE *argv,VALUE self)\
{\
	VALUE bulletObj, leftIndent, leftSubIndent, bulletStyle;\
	rb_scan_args(argc, argv, "31",&bulletObj, &leftIndent, &leftSubIndent, &bulletStyle);\
	_self->Begin##n##Bullet(\
		func1(bulletObj), RB_NUM2INT(leftIndent), RB_NUM2INT(leftSubIndent),\
		NIL_P(bulletStyle) ? style : unwrapenum<wxTextAttrBulletStyle>(bulletStyle)\
	);\
	rb_yield(self);\
	_self->End##n##Bullet();\
	return self;\
}

block_func0(Bold)
block_func0(Italic)
block_func0(Underline)

block_func1(FontSize,RB_NUM2INT)
block_func1(Font,unwrap<wxFont>)
block_func1(TextColour,unwrap<wxColor>)
block_func1(Alignment,unwrapenum<wxTextAttrAlignment>)

block_func1(RightIndent,RB_NUM2INT)
block_func1(LineSpacing,RB_NUM2INT)
block_func1(CharacterStyle,unwrap<wxString>)
block_func1(ParagraphStyle,unwrap<wxString>)

block_bullet(Numbered,RB_NUM2INT,wxTEXT_ATTR_BULLET_STYLE_ARABIC|wxTEXT_ATTR_BULLET_STYLE_PERIOD)
block_bullet(Symbol,unwrap<wxString>,wxTEXT_ATTR_BULLET_STYLE_SYMBOL)
block_bullet(Standard,unwrap<wxString>,wxTEXT_ATTR_BULLET_STYLE_STANDARD)

block_func1(BatchUndo,unwrap<wxString>)
block_func0(SuppressUndo)

namespace Event
{
#undef _self
#define _self unwrapTypedPtr<wxRichTextEvent>(self,rb_cWXRichTextCtrlEvent)

macro_attr(Position,int)

}

}
}
#endif


/* Document-attr: value
 * the value of the RichTextCtrl. String
 */
/* Document-attr: filename
 * the filename of the RichTextCtrl. String
 */
/* Document-attr: text_cursor
 * the text cursor of the RichTextCtrl. WX::Cursor
 */
/* Document-attr: url_cursor
 * the url cursor of the RichTextCtrl. WX::Cursor
 */
/* Document-attr: context_menu
 * the context menu of the RichTextCtrl. WX::Menu
 */
/* Document-attr: caret_position
 * the current caret position. Integer
 */
/* Document-attr: basic_style
 * the basic (overall) style. WX::RichTextAttr
 */
/* Document-attr: style_sheet
 * the style sheet associated with the control. WX::RichTextStyleSheet
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

/* Document-method: selection_bold?
 * call-seq:
 *   selection_bold? -> true/false
 *
 * Returns true if all of the selection, or the content at the caret position, is bold.
 * ===Return value
 * true/false
*/
/* Document-method: selection_italic?
 * call-seq:
 *   selection_italic? -> true/false
 *
 * Returns true if all of the selection, or the content at the caret position, is italic.
 * ===Return value
 * true/false
*/
/* Document-method: selection_underline?
 * call-seq:
 *   selection_underline? -> true/false
 *
 * Returns true if all of the selection, or the content at the caret position, is underline.
 * ===Return value
 * true/false
*/

/* Document-method: newline
 * call-seq:
 *   newline -> true/false
 *
 * Inserts a new paragraph at the current insertion point.
 * ===Return value
 * true/false
*/
/* Document-method: line_break
 * call-seq:
 *   line_break -> true/false
 *
 * A line break forces wrapping within a paragraph, and can be introduced by using this function.
 * ===Return value
 * true/false
*/

/* Document-method: batching_undo?
 * call-seq:
 *   batching_undo? -> true/false
 *
 * Returns true if undo commands are being batched.
 * ===Return value
 * true/false
*/
/* Document-method: suppressing_undo?
 * call-seq:
 *   suppressing_undo? -> true/false
 *
 * Returns true if undo history suppression is on.
 * ===Return value
 * true/false
*/

/* Document-method: block_bold
 * call-seq:
 *   block_bold { ... } -> self
 *
 * use bold while in the block
 * ===Return value
 * self
*/
/* Document-method: block_italic
 * call-seq:
 *   block_italic { ... } -> self
 *
 * use italic while in the block
 * ===Return value
 * self
*/
/* Document-method: block_underline
 * call-seq:
 *   block_underline { ... } -> self
 *
 * use underline while in the block
 * ===Return value
 * self
*/

/* Document-method: block_font_size
 * call-seq:
 *   block_font_size(size) { ... } -> self
 *
 * use given font size while in the block
 * ===Arguments
 * * size Integer
 * ===Return value
 * self
*/
/* Document-method: block_font
 * call-seq:
 *   block_font(font) { ... } -> self
 *
 * use given font while in the block
 * ===Arguments
 * * font WX::Font
 * ===Return value
 * self
*/
/* Document-method: block_text_color
 * call-seq:
 *   block_text_color(color) { ... } -> self
 *
 * use given text color while in the block
 * ===Arguments
 * * color WX::Color
 * ===Return value
 * self
*/

/* Document-method: block_character_style
 * call-seq:
 *   block_character_style(style) { ... } -> self
 *
 * use given character style while in the block
 * ===Arguments
 * * style String
 * ===Return value
 * self
*/
/* Document-method: block_paragraph_style
 * call-seq:
 *   block_paragraph_style(style) { ... } -> self
 *
 * use given paragraph style while in the block
 * ===Arguments
 * * style String
 * ===Return value
 * self
*/

/* Document-method: batch_undo
 * call-seq:
 *   batch_undo(name) { ... } -> self
 *
 * batching undo history for commands.
 * ===Arguments
 * * name String
 * ===Return value
 * self
*/
/* Document-method: suppress_undo
 * call-seq:
 *   suppress_undo { ... } -> self
 *
 * suppressing undo history for commands.
 * ===Return value
 * self
*/

/* Document-const: READONLY
 *   The text will not be user-editable.
 */
/* Document-const: MULTILINE
 *   The text control allows multiple lines. If this style is not specified, line break characters should not be used in the controls value.
 */
/* Document-const: CENTER_CARET
 *
 */
DLL_LOCAL void Init_WXRichTextCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_mWXTextArea = rb_define_module_under(rb_mWX,"TextArea");
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");
#endif

#if wxUSE_RICHTEXT
	using namespace RubyWX::RichTextCtrl;
	rb_cWXRichTextCtrl = rb_define_class_under(rb_mWX,"RichTextCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXRichTextCtrl,_alloc);

#if 0
	rb_define_attr(rb_cWXRichTextCtrl,"value",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"filename",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"text_cursor",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"url_cursor",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"context_menu",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"caret_position",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"basic_style",1,1);
	rb_define_attr(rb_cWXRichTextCtrl,"style_sheet",1,1);
#endif

	rb_include_module(rb_cWXRichTextCtrl,rb_mWXTextArea);
	rb_include_module(rb_cWXRichTextCtrl,rb_mWXTextEntry);

	rb_define_method(rb_cWXRichTextCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXRichTextCtrl,"single_line?",RUBY_METHOD_FUNC(_IsSingleLine),0);
	rb_define_method(rb_cWXRichTextCtrl,"multi_line?",RUBY_METHOD_FUNC(_IsMultiLine),0);

	rb_define_method(rb_cWXRichTextCtrl,"batching_undo?",RUBY_METHOD_FUNC(_BatchingUndo),0);
	rb_define_method(rb_cWXRichTextCtrl,"suppressing_undo?",RUBY_METHOD_FUNC(_SuppressingUndo),0);

	rb_define_method(rb_cWXRichTextCtrl,"newline",RUBY_METHOD_FUNC(_Newline),0);
	rb_define_method(rb_cWXRichTextCtrl,"line_break",RUBY_METHOD_FUNC(_LineBreak),0);

	rb_define_method(rb_cWXRichTextCtrl,"selection_bold?",RUBY_METHOD_FUNC(_IsSelectionBold),0);
	rb_define_method(rb_cWXRichTextCtrl,"selection_italic?",RUBY_METHOD_FUNC(_IsSelectionItalics),0);
	rb_define_method(rb_cWXRichTextCtrl,"selection_underline?",RUBY_METHOD_FUNC(_IsSelectionUnderlined),0);

	rb_define_method(rb_cWXRichTextCtrl,"block_bold",RUBY_METHOD_FUNC(_blockBold),0);
	rb_define_method(rb_cWXRichTextCtrl,"block_italic",RUBY_METHOD_FUNC(_blockItalic),0);
	rb_define_method(rb_cWXRichTextCtrl,"block_underline",RUBY_METHOD_FUNC(_blockUnderline),0);

	rb_define_method(rb_cWXRichTextCtrl,"block_font_size",RUBY_METHOD_FUNC(_blockFontSize),1);
	rb_define_method(rb_cWXRichTextCtrl,"block_font",RUBY_METHOD_FUNC(_blockFont),1);
	rb_define_method(rb_cWXRichTextCtrl,"block_text_color",RUBY_METHOD_FUNC(_blockTextColour),1);

	rb_define_method(rb_cWXRichTextCtrl,"block_character_style",RUBY_METHOD_FUNC(_blockCharacterStyle),1);
	rb_define_method(rb_cWXRichTextCtrl,"block_paragraph_style",RUBY_METHOD_FUNC(_blockParagraphStyle),1);

	rb_define_method(rb_cWXRichTextCtrl,"batch_undo",RUBY_METHOD_FUNC(_blockBatchUndo),1);
	rb_define_method(rb_cWXRichTextCtrl,"suppress_undo",RUBY_METHOD_FUNC(_blockSuppressUndo),0);

	rb_define_attr_method(rb_cWXRichTextCtrl,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXRichTextCtrl,"filename",_getFilename,_setFilename);
	rb_define_attr_method(rb_cWXRichTextCtrl,"text_cursor",_getTextCursor,_setTextCursor);
	rb_define_attr_method(rb_cWXRichTextCtrl,"url_cursor",_getURLCursor,_setURLCursor);
	rb_define_attr_method(rb_cWXRichTextCtrl,"context_menu",_getContextMenu,_setContextMenu);
	rb_define_attr_method(rb_cWXRichTextCtrl,"context_menu",_getCaretPosition,_setCaretPosition);
	rb_define_attr_method(rb_cWXRichTextCtrl,"basic_style",_getBasicStyle,_setBasicStyle);
	rb_define_attr_method(rb_cWXRichTextCtrl,"style_sheet",_getStyleSheet,_setStyleSheet);

	rb_define_const(rb_cWXRichTextCtrl,"READONLY",RB_INT2NUM(wxRE_READONLY));
	rb_define_const(rb_cWXRichTextCtrl,"MULTILINE",RB_INT2NUM(wxRE_MULTILINE));
	rb_define_const(rb_cWXRichTextCtrl,"CENTER_CARET",RB_INT2NUM(wxRE_CENTRE_CARET));

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
