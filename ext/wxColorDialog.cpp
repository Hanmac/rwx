/*
 * wxColorDialog.cpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#include "wxColorDialog.hpp"
#include "wxColor.hpp"

VALUE rb_cWXColorDialog;
#if wxUSE_COLOURDLG
#define _self unwrap<wxColourDialog*>(self)

namespace RubyWX {
namespace ColourDialog {

APP_PROTECT(wxColourDialog)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	_self->Create(unwrap<wxWindow*>(parent));
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

VALUE _getColour(VALUE self)
{
	return wrap(_self->GetColourData().GetColour());
}

VALUE _setColour(VALUE self,VALUE val)
{
	_self->GetColourData().SetColour(unwrap<wxColour>(val));
	return val;
}


VALUE _getCustomColors(VALUE self)
{
	VALUE result = rb_ary_new();
	wxColourData &data = _self->GetColourData();
	for(size_t i = 0; i < wxColourData::NUM_CUSTOM; ++i)
		rb_ary_push(result,wrap(data.GetCustomColour(i)));
	return result;
}
VALUE _setCustomColors(VALUE self,VALUE val)
{
	VALUE dp = rb_funcall(val,rb_intern("to_a"),0);
	wxColourData &data = _self->GetColourData();
	for(size_t i = 0; i < wxColourData::NUM_CUSTOM && i < (size_t)RARRAY_LEN(dp); ++i)
		data.SetCustomColour(i,unwrap<wxColor>(RARRAY_PTR(dp)[i]));

	return val;
}


VALUE _getUserColor(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,caption;
	rb_scan_args(argc, argv, "11",&parent,&caption);
	wxColor col = wxGetColourFromUser(unwrap<wxWindow*>(parent),*wxBLACK,unwrap<wxString>(caption));
	return col.IsOk() ? wrap(col) : Qnil;
}

}
}

#endif

void Init_WXColorDialog(VALUE rb_mWX)
{
#if wxUSE_COLOURDLG
	using namespace RubyWX::ColourDialog;
	rb_cWXColorDialog = rb_define_class_under(rb_mWX,"ColorDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXColorDialog,_alloc);
	rb_define_method(rb_cWXColorDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXColorDialog,"color",_getColour,_setColour);
	rb_define_attr_method(rb_cWXColorDialog,"custom_colors",_getCustomColors,_setCustomColors);

	rb_define_module_function(rb_mWX,"color_dialog",RUBY_METHOD_FUNC(_getUserColor),-1);

	registerInfo<wxColourDialog>(rb_cWXColorDialog);
#endif
}
