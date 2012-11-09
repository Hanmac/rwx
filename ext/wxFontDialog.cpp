/*
 * wxFontDialog.cpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */

#include "wxFontDialog.hpp"
#include "wxFont.hpp"
#include "wxColor.hpp"

VALUE rb_cWXFontDialog;
#if wxUSE_FONTDLG
#define _self unwrap<wxFontDialog*>(self)

namespace RubyWX {
namespace FontDialog {

APP_PROTECT(wxFontDialog)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		_self->Create(unwrap<wxWindow*>(parent));
		_created = true;
	}

	rb_call_super(argc,argv);
	return self;
}

VALUE _getColour(VALUE self)
{
	return wrap(_self->GetFontData().GetColour());
}

VALUE _setColour(VALUE self,VALUE val)
{
	_self->GetFontData().SetColour(unwrap<wxColour>(val));
	return val;
}

VALUE _getInitialFont(VALUE self)
{
	return wrap(_self->GetFontData().GetInitialFont());
}

VALUE _setInitialFont(VALUE self,VALUE val)
{
	_self->GetFontData().SetInitialFont(unwrap<wxFont>(val));
	return val;
}

VALUE _getChosenFont(VALUE self)
{
	return wrap(_self->GetFontData().GetChosenFont());
}

VALUE _setChosenFont(VALUE self,VALUE val)
{
	_self->GetFontData().SetChosenFont(unwrap<wxFont>(val));
	return val;
}

VALUE _getUserFont(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,caption;
	rb_scan_args(argc, argv, "11",&parent,&caption);
	wxFont col = wxGetFontFromUser(unwrap<wxWindow*>(parent),wxNullFont,unwrap<wxString>(caption));
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

	rb_define_attr_method(rb_cWXFontDialog,"colour",_getColour,_setColour);

	rb_define_module_function(rb_mWX,"font_dialog",RUBY_METHOD_FUNC(_getUserFont),-1);

	registerInfo<wxFontDialog>(rb_cWXFontDialog);
#endif
}
