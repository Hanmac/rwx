/*
 * wxColorDialog.cpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#include "wxColorDialog.hpp"
#include "wxColor.hpp"

#include "wxApp.hpp"

VALUE rb_cWXColorDialog;
#if wxUSE_COLOURDLG
#define _self unwrap<wxColourDialog*>(self)

namespace RubyWX {
namespace ColourDialog {

APP_PROTECT(wxColourDialog)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString)){
		_self->Create(unwrap<wxWindow*>(parent));
	}
	
	rb_call_super(argc,argv);
	return self;
}

macro_attr_pre(Colour,wxColor,GetColourData)

DLL_LOCAL VALUE _getCustomColors(VALUE self)
{
	VALUE result = rb_ary_new();
	wxColourData &data = _self->GetColourData();
	for(int i = 0; i < wxColourData::NUM_CUSTOM; ++i)
		rb_ary_push(result,wrap(data.GetCustomColour(i)));
	return result;
}
DLL_LOCAL VALUE _setCustomColors(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	VALUE dp = rb_Array(val);
	wxColourData &data = _self->GetColourData();
	for(int i = 0; i < wxColourData::NUM_CUSTOM && i < (int)RARRAY_LEN(dp); ++i)
		data.SetCustomColour(i,unwrap<wxColor>(RARRAY_AREF(dp,i)));

	return val;
}


DLL_LOCAL VALUE _getUserColor(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,caption;
	rb_scan_args(argc, argv, "11",&parent,&caption);

	app_protected();

	wxColor col = wxGetColourFromUser(unwrap<wxWindow*>(parent),*wxBLACK,unwrap<wxString>(caption));
	return col.IsOk() ? wrap(col) : Qnil;
}

}
}

#endif

DLL_LOCAL void Init_WXColorDialog(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);

	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
#endif

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
