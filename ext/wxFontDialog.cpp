/*
 * wxFontDialog.cpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */

#include "wxFontDialog.hpp"
#include "wxFont.hpp"

VALUE rb_cWXFontDialog;
#if wxUSE_FONTDLG
#define _self wrap<wxFontDialog*>(self)

namespace RubyWX {
namespace FontDialog {

APP_PROTECT(wxFontDialog)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	_self->Create(wrap<wxWindow*>(parent));
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

macro_attr_pre(InitialFont,GetFontData,wxFont)
macro_attr_pre(ChosenFont,GetFontData,wxFont)

VALUE _getUserFont(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,caption;
	rb_scan_args(argc, argv, "11",&parent,&caption);
	wxFont col = wxGetFontFromUser(wrap<wxWindow*>(parent),wxNullFont,wrap<wxString>(caption));
	return col.IsOk() ? wrap(col) : Qnil;
}

}
}

#endif

void Init_WXFontDialog(VALUE rb_mWX)
{
#if wxUSE_FONTDLG
	using namespace RubyWX::FontDialog;
	rb_cWXFontDialog = rb_define_class_under(rb_mWX,"FontDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXFontDialog,_alloc);
	rb_define_method(rb_cWXFontDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFontDialog,"initial_font",_getInitialFont,_setInitialFont);
	rb_define_attr_method(rb_cWXFontDialog,"chosen_font",_getChosenFont,_setChosenFont);

	rb_define_module_function(rb_mWX,"font_dialog",RUBY_METHOD_FUNC(_getUserFont),-1);

	registerType<wxFontDialog>(rb_cWXFontDialog);
#endif
}
