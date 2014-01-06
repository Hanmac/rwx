/*
 * wxPropertyGrid.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#include "wxPropertyGrid.hpp"
#include "wxPropertyGridInterface.hpp"

VALUE rb_cWXPropertyGrid;

#if wxUSE_PROPGRID
#define _self unwrap<wxPropertyGrid*>(self)

namespace RubyWX {
namespace PropertyGrid {

APP_PROTECT(wxPropertyGrid)

singlereturn(GetRoot)

#if wxUSE_STATUSBAR
singlereturn(GetStatusBar)
#endif

macro_attr(CaptionBackgroundColour,wxColour)
//macro_attr(CaptionForegroundColour,wxColour)
macro_attr(CellBackgroundColour,wxColour)
macro_attr(CellDisabledTextColour,wxColour)
macro_attr(CellTextColour,wxColour)

macro_attr(EmptySpaceColour,wxColour)
macro_attr(LineColour,wxColour)
macro_attr(MarginColour,wxColour)

macro_attr(SelectionBackgroundColour,wxColour)

singlereturn(GetCaptionForegroundColour)
singlereturn(GetSelectionForegroundColour)

singlereturn(GetCaptionFont)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	int style = wxPG_DEFAULT_STYLE;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,wxDefaultPosition,wxDefaultSize,style);
		
	}

	rb_call_super(argc,argv);
	return self;
}


}
}
#endif


/* Document-method: root
 * call-seq:
 *   root -> WX::Property
 *
 * returns the root property of this WX::PropertyGrid
 * ===Return value
 * WX::Property
 */

/* Document-method: status_bar
 * call-seq:
 *   status_bar -> WX::StatusBar
 *
 * returns the status bar of this WX::PropertyGrid when available.
 * ===Return value
 * WX::StatusBar
 */

/* Document-method: caption_font
 * call-seq:
 *   caption_font -> WX::Font
 *
 * returns the caption font of this WX::PropertyGrid.
 * ===Return value
 * WX::Font
 */

/* Document-attr: caption_background_color
 * the caption background color of the PropertyGrid. WX::Color
 */

/* Document-attr: selection_background_color
 * the selection background color of the PropertyGrid. WX::Color
 */

/* Document-attr: cell_background_color
 * the cell background color of the PropertyGrid. WX::Color
 */

/* Document-attr: cell_text_color
 * the cell text color of the PropertyGrid. WX::Color
 */

/* Document-attr: cell_disabled_text_color
 * the cell disabled text color of the PropertyGrid. WX::Color
 */

/* Document-attr: empty_space_color
 * the empty space color of the PropertyGrid. WX::Color
 */

/* Document-attr: line_color
 * the line color of the PropertyGrid. WX::Color
 */

/* Document-attr: margin_color
 * the margin color of the PropertyGrid. WX::Color
 */

DLL_LOCAL void Init_WXPropertyGrid(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXPropertyGrid,"caption_background_color",1,1);
	rb_define_attr(rb_cWXPropertyGrid,"selection_background_color",1,1);

	rb_define_attr(rb_cWXPropertyGrid,"cell_background_color",1,1);

	rb_define_attr(rb_cWXPropertyGrid,"cell_text_color",1,1);
	rb_define_attr(rb_cWXPropertyGrid,"cell_disabled_text_color",1,1);

	rb_define_attr(rb_cWXPropertyGrid,"empty_space_color",1,1);
	rb_define_attr(rb_cWXPropertyGrid,"line_color",1,1);
	rb_define_attr(rb_cWXPropertyGrid,"margin_color",1,1);

#endif
#if wxUSE_PROPGRID
	using namespace RubyWX::PropertyGrid;
	rb_cWXPropertyGrid = rb_define_class_under(rb_mWX,"PropertyGrid",rb_cWXControl);
	rb_define_alloc_func(rb_cWXPropertyGrid,_alloc);

	rb_include_module(rb_cWXPropertyGrid,rb_mWXPropertyGridInterface);

	rb_define_method(rb_cWXPropertyGrid,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXPropertyGrid,"root",RUBY_METHOD_FUNC(_GetRoot),0);
	rb_define_method(rb_cWXPropertyGrid,"status_bar",RUBY_METHOD_FUNC(_GetStatusBar),1);

	rb_define_method(rb_cWXPropertyGrid,"caption_font",RUBY_METHOD_FUNC(_GetCaptionFont),0);

	rb_define_attr_method(rb_cWXPropertyGrid,"caption_background_color",_getCaptionBackgroundColour,_setCaptionBackgroundColour);
	rb_define_attr_method(rb_cWXPropertyGrid,"selection_background_color",_getSelectionBackgroundColour,_setSelectionBackgroundColour);

	rb_define_attr_method(rb_cWXPropertyGrid,"cell_background_color",_getCellBackgroundColour,_setCellBackgroundColour);

	rb_define_attr_method(rb_cWXPropertyGrid,"cell_text_color",_getCellTextColour,_setCellTextColour);
	rb_define_attr_method(rb_cWXPropertyGrid,"cell_disabled_text_color",_getCellDisabledTextColour,_setCellDisabledTextColour);

	rb_define_attr_method(rb_cWXPropertyGrid,"empty_space_color",_getEmptySpaceColour,_setEmptySpaceColour);
	rb_define_attr_method(rb_cWXPropertyGrid,"line_color",_getLineColour,_setLineColour);
	rb_define_attr_method(rb_cWXPropertyGrid,"margin_color",_getMarginColour,_setMarginColour);

	rb_define_const(rb_cWXPropertyGrid,"DEFAULT_STYLE",INT2NUM(wxPG_DEFAULT_STYLE));

	rb_define_const(rb_cWXPropertyGrid,"EX_HELP_AS_TOOLTIPS",INT2NUM(wxPG_EX_HELP_AS_TOOLTIPS));



	registerInfo<wxPropertyGrid>(rb_cWXPropertyGrid);
#endif

}


