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

/*
 * call-seq:
 *   ColorDialog.new(parent, name, [options])
 *   ColorDialog.new(parent, [options])
 *
 * creates a new ColorDialog widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * * options: Hash with possible options to set
 *
*/
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


/*
 * call-seq:
 *   get_custom_color(pos) -> WX::Color or nil
 *
 * gets a custom color
 * ===Arguments
 * * pos position of the custom color. Integer
 * ===Return value
 * WX::Color or nil
 * === Exceptions
 * [IndexError]
 * * pos is greater than the possible count of custom colors.
*/
DLL_LOCAL VALUE _getCustomColor(VALUE self, VALUE idx)
{
	int i = NUM2INT(idx);

	wxColourData &data = _self->GetColourData();
	if(check_index(i, wxColourData::NUM_CUSTOM))
		return wrap(data.GetCustomColour(i));

	return Qnil;
}

/*
 * call-seq:
 *   set_custom_color(pos, color) -> self
 *
 * sets a custom color
 * ===Arguments
 * * pos position of the custom color. Integer
 * * color WX::Color
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the possible count of custom colors.
*/
DLL_LOCAL VALUE _setCustomColor(VALUE self, VALUE idx, VALUE col)
{
	int i = NUM2INT(idx);

	wxColourData &data = _self->GetColourData();
	if(check_index(i, wxColourData::NUM_CUSTOM))
		data.SetCustomColour(i,unwrap<wxColor>(col));

	return self;
}


/*
 * call-seq:
 *   color_dialog(parent, [caption], [color]) -> color
 *
 * shows a color dialog
 * ===Arguments
 * * parent of this window or nil
 * * caption String
 * * color WX::Color
 * ===Return value
 * color WX::Color
*/
DLL_LOCAL VALUE _getUserColor(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,caption, col;
	rb_scan_args(argc, argv, "12",&parent,&caption, &col);

	app_protected();

	wxColor def = *wxBLACK;

	if(!NIL_P(col))
		def = unwrap<wxColor>(col);

	return wrap(wxGetColourFromUser(unwrap<wxWindow*>(parent),def,unwrap<wxString>(caption)));

}

}
}

#endif

/* Document-attr: color
 * the selected color of the ColorDialog. WX::Color
 */
/* Document-attr: custom_colors
 * the custom colors of the ColorDialog. [WX::Color]
 */

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

#if 0
	rb_define_attr(rb_cWXColorDialog,"color",1,1);
	rb_define_attr(rb_cWXColorDialog,"custom_colors",1,1);
#endif

	rb_define_attr_method(rb_cWXColorDialog,"color",_getColour,_setColour);
	rb_define_attr_method(rb_cWXColorDialog,"custom_colors",_getCustomColors,_setCustomColors);

	rb_define_method(rb_cWXColorDialog,"get_custom_color",RUBY_METHOD_FUNC(_getCustomColor),1);
	rb_define_method(rb_cWXColorDialog,"set_custom_color",RUBY_METHOD_FUNC(_setCustomColor),2);

	rb_define_module_function(rb_mWX,"color_dialog",RUBY_METHOD_FUNC(_getUserColor),-1);

	registerInfo<wxColourDialog>(rb_cWXColorDialog);
#endif
}
