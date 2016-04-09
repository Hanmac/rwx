/*
 * wxSTC.cpp
 *
 *  Created on: 01.04.2012
 *      Author: hanmac
 */

#include "wxSTC.hpp"
#include "wxColor.hpp"

#if wxUSE_TEXTCTRL
#include "wxTextArea.hpp"
#endif
#include "wxTextEntry.hpp"

#include <wx/tokenzr.h>

VALUE rb_cWXSTC;

#if wxUSE_STC
#define _self unwrap<wxStyledTextCtrl*>(self)


#define macro_stc_wrapper(name, type) \
DLL_LOCAL VALUE wrap##name(const int& val) \
{\
	return wrapenum(val, type);\
}\
DLL_LOCAL int unwrap##name(const VALUE& val) \
{\
	return unwrapenum(val, type);\
}

macro_stc_wrapper(Lexer, "WX::STC::Lex")
macro_stc_wrapper(WrapIndent, "WX::STC::WrapIndent")
macro_stc_wrapper(Weight, "WX::STC::Weight")
macro_stc_wrapper(SelMode, "WX::STC::SelMode")
macro_stc_wrapper(CaretStyle, "WX::STC::CaretStyle")

#define style_helper(name,T)\
	T get##name() { return m_ctrl->StyleGet##name(m_style);} \
	void set##name(T val) { m_ctrl->StyleSet##name(m_style,val); }



class RubyWXSTCStyle
{
public:
	RubyWXSTCStyle(wxStyledTextCtrl *ctrl, int style) :
		m_ctrl(ctrl), m_style(style)
	{}


	style_helper(Foreground,wxColor)
	style_helper(Background,wxColor)

	style_helper(Weight,int)

	style_helper(Bold,bool)
	style_helper(Italic,bool)
	style_helper(Underline,bool)

	style_helper(FaceName,wxString)

	style_helper(Visible,bool)
	style_helper(EOLFilled,bool)
private:
	wxStyledTextCtrl *m_ctrl;
	int m_style;
};

#define indic_helper(name,T)\
	T get##name() { return m_ctrl->IndicatorGet##name(m_indic);} \
	void set##name(T val) { m_ctrl->IndicatorSet##name(m_indic,val); }


class RubyWXSTCIndicator
{
public:
	RubyWXSTCIndicator(wxStyledTextCtrl *ctrl, int indic) :
		m_ctrl(ctrl), m_indic(indic)
	{}

	indic_helper(Style, int)
	indic_helper(Foreground, wxColor)
	indic_helper(Under, bool)
#ifdef HAVE_WXSTYLEDTEXTCTRL_INDICATORGETHOVERSTYLE
	indic_helper(HoverStyle, int)
	indic_helper(HoverForeground, wxColor)
#endif

private:
	wxStyledTextCtrl *m_ctrl;
	int m_indic;
};



namespace RubyWX {
namespace STC {

#ifdef HAVE_WXSTYLEDTEXTCTRL_GETLEXERLANGUAGE
singlereturn(GetLexerLanguage)
#endif

macro_attr(Text,wxString)
macro_attr(UseTabs,bool)

macro_attr(TabIndents,bool)
macro_attr(BackSpaceUnIndents,bool)

macro_attr(Indent,int)
macro_attr_with_func(Lexer,wrapLexer,unwrapLexer)

macro_attr(TwoPhaseDraw,bool)

macro_attr(CaretLineVisible,bool)
macro_attr(CaretForeground,wxColor)
macro_attr(CaretLineBackground,wxColor)

macro_attr(AdditionalCaretsVisible,bool)
macro_attr(AdditionalCaretForeground,wxColor)

macro_attr(SelAlpha,int)
macro_attr(AdditionalSelAlpha,int)
macro_attr(CaretLineBackAlpha,int)

macro_attr(SelEOLFilled,bool)

macro_attr(CaretWidth,int)
macro_attr(CaretPeriod,int)
macro_attr(WhitespaceSize,int)
macro_attr(WordChars,wxString)

macro_attr(UseHorizontalScrollBar,bool)
macro_attr(UseVerticalScrollBar,bool)

macro_attr(IndentationGuides,bool)
macro_attr(HighlightGuide,bool)

macro_attr(ReadOnly, bool)

macro_attr(Overtype,bool)

macro_attr(MultipleSelection,bool)
macro_attr(AdditionalSelectionTyping,bool)

macro_attr(AdditionalCaretsBlink,bool)

macro_attr_selection(MainSelection, GetSelections)

macro_attr_item_simple(SelectionNCaret, GetSelections, int)
macro_attr_item_simple(SelectionNAnchor, GetSelections, int)
macro_attr_item_simple(SelectionNCaretVirtualSpace, GetSelections, int)
macro_attr_item_simple(SelectionNAnchorVirtualSpace, GetSelections, int)
macro_attr_item_simple(SelectionNStart, GetSelections, int)
macro_attr_item_simple(SelectionNEnd, GetSelections, int)

macro_attr(Zoom,int)

macro_attr(SelectionStart,int)
macro_attr(SelectionEnd,int)

macro_attr(MarginLeft,int)
macro_attr(MarginRight,int)

macro_attr(MouseDwellTime,int)

macro_attr(ExtraAscent, int)
macro_attr(ExtraDescent, int)

macro_attr(ScrollWidth,int)
macro_attr(ScrollWidthTracking,bool)

macro_attr(WrapStartIndent,int)
macro_attr_with_func(WrapIndentMode,wrapWrapIndent,unwrapWrapIndent)

macro_attr_with_func(SelectionMode,wrapSelMode,unwrapSelMode)

#ifdef HAVE_WXSTYLEDTEXTCTRL_GETCARETLINEVISIBLEALWAYS
macro_attr(CaretLineVisibleAlways, bool)
#endif

macro_attr(TargetStart,int)
macro_attr(TargetEnd,int)


VALUE _getTargetRange(VALUE self)
{
	return rb_range_new(_getTargetStart(self), _getTargetEnd(self), false);
}

VALUE _setTargetRange(VALUE self, VALUE range)
{
	rb_check_frozen(self);

	VALUE begin, end;
	int excl;

	if(rb_range_values(range, &begin, &end, &excl)) {
#if HAVE_WXSTYLEDTEXTCTRL_SETTARGETRANGE
		_self->SetTargetRange(NUM2INT(begin), NUM2INT(end));
#else
		_self->SetTargetStart(NUM2INT(begin));
		_self->SetTargetEnd(NUM2INT(end));
#endif
	}
	return range;
}

#ifdef HAVE_WXSTYLEDTEXTCTRL_GETTARGETTEXT
macro_attr_func(TargetText,GetTargetText(), ReplaceTarget, wrap, unwrap<wxString>,true);
#else

DLL_LOCAL VALUE _getTargetText(VALUE self)
{
	return wrap(_self->GetTextRange(_self->GetTargetStart(), _self->GetTargetEnd()));
}

DLL_LOCAL VALUE _setTargetText(VALUE self,VALUE other)
{
	rb_check_frozen(self);
	_self->ReplaceTarget(unwrap<wxString>(other));
	return other;
}
#endif

APP_PROTECT(wxStyledTextCtrl)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id);
		}

	}
	
	_self->StyleClearAll();

	rb_call_super(argc,argv);

	return self;
}

DLL_LOCAL VALUE _test_style(VALUE self)
{
	_self->SetLexer(wxSTC_LEX_CPP);

	wxArrayString token = wxStringTokenize(_self->PropertyNames());

	for(wxArrayString::const_iterator it = token.begin(); it != token.end(); ++it)
	{
		rb_warn("%s: %d : %s", it->c_str().AsChar(), _self->PropertyType(*it), _self->DescribeProperty(*it).c_str().AsChar());
	}

	rb_warn("%s: %d", "xyz", _self->PropertyType("xyz"));

	return self;
}

DLL_LOCAL VALUE _UndoAction(VALUE self)
{
	rb_check_frozen(self);
	_self->BeginUndoAction();
	rb_yield(self);
	_self->EndUndoAction();
	return self;
}

DLL_LOCAL VALUE _Record(VALUE self)
{
	rb_check_frozen(self);
	_self->StartRecord();
	rb_yield(self);
	_self->StopRecord();
	return self;
}

#if !wxUSE_TEXTCTRL
DLL_LOCAL VALUE _load_file(VALUE self,VALUE file)
{
	if(!check_file_loadable(unwrap<wxString>(file)))
		return Qfalse;
	return wrap(_self->LoadFile(unwrap<wxString>(file)));
}

DLL_LOCAL VALUE _save_file(VALUE self,VALUE file)
{
	if(!check_file_saveable(unwrap<wxString>(file)))
		return Qfalse;

	return wrap(_self->SaveFile(unwrap<wxString>(file)));
}
#endif

}
}
#endif
DLL_LOCAL void Init_WXSTC(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXSTC,"caret_line_visible_always", 1, 1);

#endif

#if wxUSE_STC
	using namespace RubyWX::STC;
	rb_cWXSTC = rb_define_class_under(rb_mWX,"STC",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSTC,_alloc);

	rb_define_method(rb_cWXSTC,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#if wxUSE_TEXTCTRL
	rb_include_module(rb_cWXSTC,rb_mWXTextArea);

	rb_undef_method(rb_cWXSTC, "modified=");

	rb_define_attr_method_missing(rb_cWXSTC,"default_style");
#else
	rb_define_method(rb_cWXSTC,"load_file",RUBY_METHOD_FUNC(_load_file),1);
	rb_define_method(rb_cWXSTC,"save_file",RUBY_METHOD_FUNC(_save_file),1);
#endif
	rb_include_module(rb_cWXSTC,rb_mWXTextEntry);


	rb_define_attr_method(rb_cWXSTC,"text",_getText,_setText);
	rb_define_attr_method(rb_cWXSTC,"lexer",_getLexer,_setLexer);
	rb_define_attr_method(rb_cWXSTC,"use_tabs",_getUseTabs,_setUseTabs);

	rb_define_attr_method(rb_cWXSTC,"sel_alpha",_getSelAlpha,_setSelAlpha);
	rb_define_attr_method(rb_cWXSTC,"additional_sel_alpha",_getAdditionalSelAlpha,_setAdditionalSelAlpha);

	rb_define_attr_method(rb_cWXSTC,"caret_line_visible",_getCaretLineVisible,_setCaretLineVisible);
	rb_define_attr_method(rb_cWXSTC,"caret_foreground",_getCaretForeground,_setCaretForeground);
	rb_define_attr_method(rb_cWXSTC,"caret_line_background",_getCaretLineBackground,_setCaretLineBackground);

	rb_define_attr_method(rb_cWXSTC,"target_start",_getTargetStart,_setTargetStart);
	rb_define_attr_method(rb_cWXSTC,"target_end",_getTargetEnd,_setTargetEnd);
	rb_define_attr_method(rb_cWXSTC,"target_range",_getTargetRange,_setTargetRange);
	rb_define_attr_method(rb_cWXSTC,"target_text",_getTargetText,_setTargetText);

#ifdef HAVE_WXSTYLEDTEXTCTRL_GETCARETLINEVISIBLEALWAYS
	rb_define_attr_method(rb_cWXSTC,"caret_line_visible_always", _getCaretLineVisibleAlways, _setCaretLineVisibleAlways);
#else
	rb_define_attr_method_missing(rb_cWXSTC,"caret_line_visible_always");
#endif

#ifdef HAVE_WXSTYLEDTEXTCTRL_GETLEXERLANGUAGE
	rb_define_attr_method(rb_cWXSTC,"lexer_language", _GetLexerLanguage, NULL);
#else
	rb_define_attr_method_missing(rb_cWXSTC,"lexer_language", true, false);
#endif

	rb_define_method(rb_cWXSTC,"test_style",RUBY_METHOD_FUNC(_test_style),0);

	rb_define_method(rb_cWXSTC,"undo_action",RUBY_METHOD_FUNC(_UndoAction),0);


	rb_define_const(rb_cWXSTC,"Version",wrap(wxStyledTextCtrl::GetLibraryVersionInfo().GetVersionString()));

	registerEnum("WX::STC::Lex","WX::STC::Lex",wxSTC_LEX_NULL)
		->add(wxSTC_LEX_PYTHON,"python")
		->add(wxSTC_LEX_CPP,"cpp")
		->add(wxSTC_LEX_HTML,"html")
		->add(wxSTC_LEX_XML,"xml")
		->add(wxSTC_LEX_PERL,"perl")
		->add(wxSTC_LEX_SQL,"sql")
		->add(wxSTC_LEX_VB,"vb")
		->add(wxSTC_LEX_PROPERTIES,"properties")
		->add(wxSTC_LEX_ERRORLIST,"errorlist")
		->add(wxSTC_LEX_MAKEFILE,"makefile")
		->add(wxSTC_LEX_BATCH,"bath")
		->add(wxSTC_LEX_XCODE,"xcode")
		->add(wxSTC_LEX_LATEX,"latex")
		->add(wxSTC_LEX_LUA,"lua")
		->add(wxSTC_LEX_DIFF,"diff")
		->add(wxSTC_LEX_CONF,"conf")
		->add(wxSTC_LEX_PASCAL,"pascal")
		->add(wxSTC_LEX_AVE,"ave")
		->add(wxSTC_LEX_ADA,"ada")
		->add(wxSTC_LEX_LISP,"lisp")
		->add(wxSTC_LEX_RUBY,"ruby")
		->add(wxSTC_LEX_EIFFEL,"eiffel")
		->add(wxSTC_LEX_EIFFELKW,"eiffelkw")
		->add(wxSTC_LEX_TCL,"tcl")
		->add(wxSTC_LEX_NNCRONTAB,"nncrontab")
		->add(wxSTC_LEX_BULLANT,"bullant")
		->add(wxSTC_LEX_VBSCRIPT,"vbscript")
		->add(wxSTC_LEX_BAAN,"baan")
		->add(wxSTC_LEX_MATLAB,"matlab")
		->add(wxSTC_LEX_SCRIPTOL,"scriptol")
		->add(wxSTC_LEX_ASM,"asm")
		->add(wxSTC_LEX_CPPNOCASE,"cpp_nocase")
		->add(wxSTC_LEX_FORTRAN,"fortran")
		->add(wxSTC_LEX_F77,"f77")
		->add(wxSTC_LEX_CSS,"css")
		->add(wxSTC_LEX_POV,"pov")
		->add(wxSTC_LEX_LOUT,"lout")
		->add(wxSTC_LEX_ESCRIPT,"escript")
		->add(wxSTC_LEX_PS,"ps")
		->add(wxSTC_LEX_NSIS,"nsis")
		->add(wxSTC_LEX_MMIXAL,"mmixal")
		->add(wxSTC_LEX_CLW,"clw")
		->add(wxSTC_LEX_CLWNOCASE,"clw_nocase")
		->add(wxSTC_LEX_LOT,"lot")
		->add(wxSTC_LEX_YAML,"yaml")
		->add(wxSTC_LEX_TEX,"tex")
		->add(wxSTC_LEX_METAPOST,"metapost")
		->add(wxSTC_LEX_POWERBASIC,"power_basic")
		->add(wxSTC_LEX_FORTH,"forth")
		->add(wxSTC_LEX_ERLANG,"erlang")
		->add(wxSTC_LEX_OCTAVE,"octave")
		->add(wxSTC_LEX_MSSQL,"mssql")
		->add(wxSTC_LEX_VERILOG,"verilog")
		->add(wxSTC_LEX_KIX,"kix")
		->add(wxSTC_LEX_GUI4CLI,"gui4cli")
		->add(wxSTC_LEX_SPECMAN,"specman")
		->add(wxSTC_LEX_AU3,"au3")
		->add(wxSTC_LEX_APDL,"apdl")
		->add(wxSTC_LEX_BASH,"bash")
		->add(wxSTC_LEX_ASN1,"asn1")
		->add(wxSTC_LEX_VHDL,"vhdl")
		->add(wxSTC_LEX_CAML,"caml")
		->add(wxSTC_LEX_BLITZBASIC,"blitz_basic")
		->add(wxSTC_LEX_PUREBASIC,"pure_basic")
		->add(wxSTC_LEX_HASKELL,"haskell")
		->add(wxSTC_LEX_PHPSCRIPT,"php_script")
		->add(wxSTC_LEX_TADS3,"tads3")
		->add(wxSTC_LEX_REBOL,"rebol")
		->add(wxSTC_LEX_SMALLTALK,"smalltalk")
		->add(wxSTC_LEX_FLAGSHIP,"flagship")
		->add(wxSTC_LEX_CSOUND,"csound")
		->add(wxSTC_LEX_FREEBASIC,"free_basic")
		->add(wxSTC_LEX_INNOSETUP,"inno_setup")
		->add(wxSTC_LEX_OPAL,"opal")
		->add(wxSTC_LEX_SPICE,"spice")
		->add(wxSTC_LEX_D,"d")
		->add(wxSTC_LEX_CMAKE,"cmake")
		->add(wxSTC_LEX_GAP,"gap")
		->add(wxSTC_LEX_PLM,"plm")
		->add(wxSTC_LEX_PROGRESS,"progress")
		->add(wxSTC_LEX_ABAQUS,"abaqus")
		->add(wxSTC_LEX_ASYMPTOTE,"asymptote")
		->add(wxSTC_LEX_R,"r")
		->add(wxSTC_LEX_MAGIK,"magik")
		->add(wxSTC_LEX_POWERSHELL,"power_shell")
		->add(wxSTC_LEX_MYSQL,"mysql")
		->add(wxSTC_LEX_PO,"po")
		->add(wxSTC_LEX_TAL,"tal")
		->add(wxSTC_LEX_COBOL,"cobol")
		->add(wxSTC_LEX_TACL,"tacl")
		->add(wxSTC_LEX_SORCUS,"sorcus")
		->add(wxSTC_LEX_POWERPRO,"powerpro")
		->add(wxSTC_LEX_NIMROD,"nimrod")
		->add(wxSTC_LEX_SML,"sml")
		->add(wxSTC_LEX_MARKDOWN,"markdown")
		->add(wxSTC_LEX_TXT2TAGS,"txt2tags")
		->add(wxSTC_LEX_A68K,"a68k")
		->add(wxSTC_LEX_MODULA,"modula")
		->add(wxSTC_LEX_COFFEESCRIPT,"coffee_script")
		->add(wxSTC_LEX_TCMD,"tcmd")
		->add(wxSTC_LEX_AVS,"avs")
		->add(wxSTC_LEX_ECL,"ecl")
		->add(wxSTC_LEX_OSCRIPT,"oscript")
		->add(wxSTC_LEX_VISUALPROLOG,"visual_prolog")
#if HAVE_CONST_WXSTC_LEX_DMAP
		->add(wxSTC_LEX_LITERATEHASKELL,"lite_rate_haskell")
		->add(wxSTC_LEX_STTXT,"sttxt")
		->add(wxSTC_LEX_KVIRC,"kvirc")
		->add(wxSTC_LEX_RUST,"rust")
		->add(wxSTC_LEX_DMAP,"dmap")
#endif
#if HAVE_CONST_WXSTC_LEX_TEHEX
		->add(wxSTC_LEX_AS,"as")
		->add(wxSTC_LEX_DMIS,"dmis")
		->add(wxSTC_LEX_REGISTRY,"registry")
		->add(wxSTC_LEX_BIBTEX,"bibtex")
		->add(wxSTC_LEX_SREC,"srec")
		->add(wxSTC_LEX_IHEX,"ihex")
		->add(wxSTC_LEX_TEHEX,"tehex")
#endif
		->allow_array = false;


	registerEnum("WX::STC::WrapIndent","WX::STC::WrapIndent",wxSTC_WRAPINDENT_FIXED)
		->add(wxSTC_WRAPINDENT_FIXED,"fixed")
		->add(wxSTC_WRAPINDENT_SAME,"same")
		->add(wxSTC_WRAPINDENT_INDENT,"indent")
		->allow_array = false;

	registerEnum("WX::STC::SelMode","WX::STC::SelMode",wxSTC_SEL_STREAM)
		->add(wxSTC_SEL_STREAM,"stream")
		->add(wxSTC_SEL_RECTANGLE,"rectangle")
		->add(wxSTC_SEL_LINES,"lines")
		->add(wxSTC_SEL_THIN,"thin")
		->allow_array = false;

	registerEnum("WX::STC::CaretStyle","WX::STC::CaretStyle",wxSTC_CARETSTYLE_INVISIBLE)
		->add(wxSTC_CARETSTYLE_INVISIBLE,"invisible")
		->add(wxSTC_CARETSTYLE_LINE,"line")
		->add(wxSTC_CARETSTYLE_BLOCK,"block")
		->allow_array = false;

	registerInfo<wxStyledTextCtrl>(rb_cWXSTC);
#endif

}



