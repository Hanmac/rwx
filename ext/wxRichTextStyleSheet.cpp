/*
 * wxRichTextStyleSheet.cpp
 *
 *  Created on: 03.08.2014
 *      Author: hanmac
 */

#include "wxRichTextStyleSheet.hpp"
#include "wxRichTextStyle.hpp"

VALUE rb_cWXRichTextStyleSheet;

#if wxUSE_RICHTEXT
#define _self unwrap<wxRichTextStyleSheet*>(self)

namespace RubyWX {
namespace RichTextStyleSheet {

APP_PROTECT(wxRichTextStyleSheet)

macro_attr(Name,wxString)
macro_attr(Description,wxString)

macro_attr(NextSheet,wxRichTextStyleSheet*)
macro_attr(PreviousSheet,wxRichTextStyleSheet*)

singlefunc(DeleteStyles)
singlefunc(Unlink)

#define macro_style_type(n,t) DLL_LOCAL VALUE _each_##n##_style_size(VALUE self)\
{\
	return RB_UINT2NUM(_self->Get##t##StyleCount());\
}\
DLL_LOCAL VALUE _each_##n##_style(VALUE self)\
{\
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_##n##_style_size));\
	for(std::size_t i = 0; i < _self->Get##t##StyleCount(); ++i)\
		rb_yield(wrap(_self->Get##t##Style(i)));\
	return self;\
}\
DLL_LOCAL VALUE _add_##n##_style(VALUE self)\
{\
	wxRichText##t##StyleDefinition *def = new wxRichText##t##StyleDefinition;\
	rb_yield(wrap(def));\
	return wrap(_self->Add##t##Style(def));\
}\
DLL_LOCAL VALUE _find_##n##_style(VALUE self,VALUE name)\
{\
	return wrap(_self->Find##t##Style(unwrap<wxString>(name)));\
}\
DLL_LOCAL VALUE _remove_##n##_style(VALUE self,VALUE obj)\
{\
	wxRichTextStyleDefinition *def;\
	if(rb_obj_is_kind_of(obj,rb_cString))\
		def = _self->Find##t##Style(unwrap<wxString>(obj));\
	else\
		def = unwrap<wxRichText##t##StyleDefinition*>(obj);\
	if(def)\
		return wrap(_self->Remove##t##Style(def));\
	return Qnil;\
}\
DLL_LOCAL VALUE _reject_##n##_style(VALUE self)\
{\
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_##n##_style_size));\
	for(std::size_t i = 0; i < _self->Get##t##StyleCount(); ++i) {\
		wxRichText##t##StyleDefinition *def = _self->Get##t##Style(i);\
		if(RTEST(rb_yield(wrap(def)))) {\
			_self->Remove##t##Style((wxRichTextStyleDefinition*)def);\
			--i;\
		}\
	}\
	return self;\
}\
DLL_LOCAL VALUE _keepif_##n##_style(VALUE self)\
{\
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_##n##_style_size));\
	for(std::size_t i = 0; i < _self->Get##t##StyleCount(); ++i) {\
		wxRichText##t##StyleDefinition *def = _self->Get##t##Style(i);\
		if(!RTEST(rb_yield(wrap(def)))) {\
			_self->Remove##t##Style((wxRichTextStyleDefinition*)def);\
			--i;\
		}\
	}\
	return self;\
}

macro_style_type(character,Character)
macro_style_type(paragraph,Paragraph)
macro_style_type(list,List)
macro_style_type(box,Box)


/*
 * call-seq:
 *   add_style(style) -> true/false
 *
 * adds a new  style.
 * ===Arguments
 * * style Style
 * ===Return value
 * true/false
 *
 */
DLL_LOCAL VALUE _add_style(VALUE self,VALUE obj)
{
	return wrap(_self->AddStyle(unwrap<wxRichTextStyleDefinition*>(obj)));
}

/*
 * call-seq:
 *   find_style(name) -> style or nil
 *
 * find a style by the given name.
 * ===Arguments
 * * name String
 * ===Return value
 * style or nil
 *
 */
DLL_LOCAL VALUE _find_style(VALUE self,VALUE name)
{
	return wrap(_self->FindStyle(unwrap<wxString>(name)));
}

/*
 * call-seq:
 *   remove_style(name) -> true/false or nil
 *
 * removes a style by the given name.
 * ===Arguments
 * * name String/Style
 * ===Return value
 * true/false or nil
 *
 */
DLL_LOCAL VALUE _remove_style(VALUE self,VALUE obj)
{
	wxRichTextStyleDefinition *def;
	if(rb_obj_is_kind_of(obj,rb_cString))
		def = _self->FindStyle(unwrap<wxString>(obj));
	else
		def = unwrap<wxRichTextStyleDefinition*>(obj);
	if(def)
		return wrap(_self->RemoveStyle(def));
	return Qnil;
}


/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_self->Copy(*unwrap<wxRichTextStyleSheet*>(other));
	return result;
}

}
}
#endif


/* Document-attr: name
 * the name of the RichTextStyleSheet. String
 */
/* Document-attr: description
 * the description of the RichTextStyleSheet. String
 */
/* Document-attr: next_sheet
 * the next sheet. WX::RichTextStyleSheet
 */
/* Document-attr: previous_sheet
 * the previous sheet. WX::RichTextStyleSheet
 */

/* Document-method: each_character_style
 * call-seq:
 *   each_character_style -> Enumerator
 *   each_character_style { |style| } -> self
 *
 * iterates of the character styles.
 * ===Return value
 * self
 *
 */
/* Document-method: each_paragraph_style
 * call-seq:
 *   each_paragraph_style -> Enumerator
 *   each_paragraph_style { |style| } -> self
 *
 * iterates of the paragraph styles.
 * ===Return value
 * self
 *
 */
/* Document-method: each_list_style
 * call-seq:
 *   each_list_style -> Enumerator
 *   each_list_style { |style| } -> self
 *
 * iterates of the list styles.
 * ===Return value
 * self
 *
 */
/* Document-method: each_box_style
 * call-seq:
 *   each_box_style -> Enumerator
 *   each_box_style { |style| } -> self
 *
 * iterates of the character styles.
 * ===Return value
 * self
 *
 */

/* Document-method: add_character_style
 * call-seq:
 *   add_character_style { |style| } -> true/false
 *
 * adds a new character style.
 * ===Return value
 * true/false
 *
 */
/* Document-method: add_paragraph_style
 * call-seq:
 *   add_paragraph_style { |style| } -> true/false
 *
 * adds a new paragraph style.
 * ===Return value
 * true/false
 *
 */
/* Document-method: add_list_style
 * call-seq:
 *   add_list_style { |style| } -> true/false
 *
 * adds a new list style.
 * ===Return value
 * true/false
 *
 */
/* Document-method: add_box_style
 * call-seq:
 *   add_box_style { |style| } -> true/false
 *
 * adds a new box style.
 * ===Return value
 * true/false
 *
 */

/* Document-method: find_character_style
 * call-seq:
 *   find_character_style(name) -> style or nil
 *
 * find a character style by the given name.
 * ===Arguments
 * * name String
 * ===Return value
 * style or nil
 *
 */
/* Document-method: find_paragraph_style
 * call-seq:
 *   find_paragraph_style(name) -> style or nil
 *
 * find a paragraph style by the given name.
 * ===Arguments
 * * name String
 * ===Return value
 * style or nil
 *
 */
/* Document-method: find_list_style
 * call-seq:
 *   find_list_style(name) -> style or nil
 *
 * find a list style by the given name.
 * ===Arguments
 * * name String
 * ===Return value
 * style or nil
 *
 */
/* Document-method: find_box_style
 * call-seq:
 *   find_box_style(name) -> style or nil
 *
 * find a box style by the given name.
 * ===Arguments
 * * name String
 * ===Return value
 * style or nil
 *
 */

/* Document-method: remove_character_style
 * call-seq:
 *   remove_character_style(name) -> true/false or nil
 *
 * removes a character style by the given name.
 * ===Arguments
 * * name String/Style
 * ===Return value
 * true/false or nil
 *
 */
/* Document-method: remove_paragraph_style
 * call-seq:
 *   remove_paragraph_style(name) -> true/false or nil
 *
 * removes a paragraph style by the given name.
 * ===Arguments
 * * name String/Style
 * ===Return value
 * true/false or nil
 *
 */
/* Document-method: remove_list_style
 * call-seq:
 *   remove_list_style(name) -> true/false or nil
 *
 * removes a list style by the given name.
 * ===Arguments
 * * name String/Style
 * ===Return value
 * true/false or nil
 *
 */
/* Document-method: remove_box_style
 * call-seq:
 *   remove_box_style(name) -> true/false or nil
 *
 * removes a box style by the given name.
 * ===Arguments
 * * name String/Style
 * ===Return value
 * true/false or nil
 *
 */

/* Document-method: reject_character_style
 * call-seq:
 *   reject_character_style -> Enumerator
 *   reject_character_style { |style| } -> self
 *
 * iterates of the character styles,
 * removes this styles where the block does return true.
 * ===Return value
 * self
 *
 */
/* Document-method: reject_paragraph_style
 * call-seq:
 *   reject_paragraph_style -> Enumerator
 *   reject_paragraph_style { |style| } -> self
 *
 * iterates of the paragraph styles,
 * removes this styles where the block does return true.
 * ===Return value
 * self
 *
 */
/* Document-method: reject_list_style
 * call-seq:
 *   reject_list_style -> Enumerator
 *   reject_list_style { |style| } -> self
 *
 * iterates of the list styles,
 * removes this styles where the block does return true.
 * ===Return value
 * self
 *
 */
/* Document-method: reject_box_style
 * call-seq:
 *   reject_box_style -> Enumerator
 *   reject_box_style { |style| } -> self
 *
 * iterates of the box styles,
 * removes this styles where the block does return true.
 * ===Return value
 * self
 *
 */

/* Document-method: keep_if_character_style
 * call-seq:
 *   keep_if_character_style -> Enumerator
 *   keep_if_character_style { |style| } -> self
 *
 * iterates of the character styles,
 * removes this styles where the block does return false.
 * ===Return value
 * self
 *
 */
/* Document-method: keep_if_paragraph_style
 * call-seq:
 *   keep_if_paragraph_style -> Enumerator
 *   keep_if_paragraph_style { |style| } -> self
 *
 * iterates of the paragraph styles,
 * removes this styles where the block does return false.
 * ===Return value
 * self
 *
 */
/* Document-method: keep_if_list_style
 * call-seq:
 *   keep_if_list_style -> Enumerator
 *   keep_if_list_style { |style| } -> self
 *
 * iterates of the list styles,
 * removes this styles where the block does return false.
 * ===Return value
 * self
 *
 */
/* Document-method: keep_if_box_style
 * call-seq:
 *   keep_if_box_style -> Enumerator
 *   keep_if_box_style { |style| } -> self
 *
 * iterates of the box styles,
 * removes this styles where the block does return false.
 * ===Return value
 * self
 *
 */

DLL_LOCAL void Init_WXRichTextStyleSheet(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
#endif

#if wxUSE_RICHTEXT
	using namespace RubyWX::RichTextStyleSheet;
	rb_cWXRichTextStyleSheet = rb_define_class_under(rb_mWX,"RichTextStyleSheet",rb_cObject);
	rb_define_alloc_func(rb_cWXRichTextStyleSheet,_alloc);

#if 0
	rb_define_attr(rb_cWXRichTextStyleSheet,"name",1,1);
	rb_define_attr(rb_cWXRichTextStyleSheet,"description",1,1);
	rb_define_attr(rb_cWXRichTextStyleSheet,"next_sheet",1,1);
	rb_define_attr(rb_cWXRichTextStyleSheet,"previous_sheet",1,1);
#endif

	rb_define_private_method(rb_cWXRichTextStyleSheet,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXRichTextStyleSheet,"name",_getName,_setName);
	rb_define_attr_method(rb_cWXRichTextStyleSheet,"description",_getDescription,_setDescription);
	rb_define_attr_method(rb_cWXRichTextStyleSheet,"next_sheet",_getNextSheet,_setNextSheet);
	rb_define_attr_method(rb_cWXRichTextStyleSheet,"previous_sheet",_getPreviousSheet,_setPreviousSheet);

	rb_define_method(rb_cWXRichTextStyleSheet,"each_character_style",RUBY_METHOD_FUNC(_each_character_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"each_paragraph_style",RUBY_METHOD_FUNC(_each_paragraph_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"each_list_style",RUBY_METHOD_FUNC(_each_list_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"each_box_style",RUBY_METHOD_FUNC(_each_box_style),0);

	rb_define_method(rb_cWXRichTextStyleSheet,"add_character_style",RUBY_METHOD_FUNC(_add_character_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"add_paragraph_style",RUBY_METHOD_FUNC(_add_paragraph_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"add_list_style",RUBY_METHOD_FUNC(_add_list_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"add_box_style",RUBY_METHOD_FUNC(_add_box_style),0);

	rb_define_method(rb_cWXRichTextStyleSheet,"find_character_style",RUBY_METHOD_FUNC(_find_character_style),1);
	rb_define_method(rb_cWXRichTextStyleSheet,"find_paragraph_style",RUBY_METHOD_FUNC(_find_paragraph_style),1);
	rb_define_method(rb_cWXRichTextStyleSheet,"find_list_style",RUBY_METHOD_FUNC(_find_list_style),1);
	rb_define_method(rb_cWXRichTextStyleSheet,"find_box_style",RUBY_METHOD_FUNC(_find_box_style),1);

	rb_define_method(rb_cWXRichTextStyleSheet,"remove_character_style",RUBY_METHOD_FUNC(_remove_character_style),1);
	rb_define_method(rb_cWXRichTextStyleSheet,"remove_paragraph_style",RUBY_METHOD_FUNC(_remove_paragraph_style),1);
	rb_define_method(rb_cWXRichTextStyleSheet,"remove_list_style",RUBY_METHOD_FUNC(_remove_list_style),1);
	rb_define_method(rb_cWXRichTextStyleSheet,"remove_box_style",RUBY_METHOD_FUNC(_remove_box_style),1);

	rb_define_method(rb_cWXRichTextStyleSheet,"reject_character_style",RUBY_METHOD_FUNC(_reject_character_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"reject_paragraph_style",RUBY_METHOD_FUNC(_reject_paragraph_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"reject_list_style",RUBY_METHOD_FUNC(_reject_list_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"reject_box_style",RUBY_METHOD_FUNC(_reject_box_style),0);

	rb_define_method(rb_cWXRichTextStyleSheet,"keep_if_character_style",RUBY_METHOD_FUNC(_keepif_character_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"keep_if_paragraph_style",RUBY_METHOD_FUNC(_keepif_paragraph_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"keep_if_list_style",RUBY_METHOD_FUNC(_keepif_list_style),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"keep_if_box_style",RUBY_METHOD_FUNC(_keepif_box_style),0);

	rb_define_method(rb_cWXRichTextStyleSheet,"add_style",RUBY_METHOD_FUNC(_add_style),1);
	rb_define_method(rb_cWXRichTextStyleSheet,"find_style",RUBY_METHOD_FUNC(_find_style),1);
	rb_define_method(rb_cWXRichTextStyleSheet,"remove_style",RUBY_METHOD_FUNC(_remove_style),1);

	rb_define_method(rb_cWXRichTextStyleSheet,"clear",RUBY_METHOD_FUNC(_DeleteStyles),0);
	rb_define_method(rb_cWXRichTextStyleSheet,"unlink",RUBY_METHOD_FUNC(_Unlink),0);

	registerInfo<wxRichTextStyleSheet>(rb_cWXRichTextStyleSheet);
#endif

}
