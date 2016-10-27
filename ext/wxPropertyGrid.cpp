/*
 * wxPropertyGrid.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#include "wxPropertyGrid.hpp"
#include "wxPropertyGridInterface.hpp"

#include "wxColor.hpp"
#include "wxFont.hpp"
#include "wxStatusBar.hpp"

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

/*
 * call-seq:
 *   PropertyGrid.new(parent, name, [options])
 *   PropertyGrid.new(parent, [options])
 *
 * creates a new PropertyGrid widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * caption_background_color WX::Color
 *   * selection_background_color WX::Color
 *   * cell_background_color WX::Color
 *   * cell_text_color WX::Color
 *   * cell_diabled_text_color WX::Color
 *   * empty_space_color WX::Color
 *   * line_color WX::Color
 *   * margin_color WX::Color
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;

	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(wxPG_DEFAULT_STYLE);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
		}
		
		if(nil_check(parent))
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,style
			);

	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash,"caption_background_color",&wxPropertyGrid::SetCaptionBackgroundColour,_self);
		set_obj_option(hash,"selection_background_color",&wxPropertyGrid::SetSelectionBackgroundColour,_self);

		set_obj_option(hash,"cell_background_color",&wxPropertyGrid::SetCellBackgroundColour,_self);
		set_obj_option(hash,"cell_text_color",&wxPropertyGrid::SetCellTextColour,_self);
		set_obj_option(hash,"cell_diabled_text_color",&wxPropertyGrid::SetCellDisabledTextColour,_self);

		set_obj_option(hash,"empty_space_color",&wxPropertyGrid::SetEmptySpaceColour,_self);
		set_obj_option(hash,"line_color",&wxPropertyGrid::SetLineColour,_self);
		set_obj_option(hash,"margin_color",&wxPropertyGrid::SetMarginColour,_self);

		PropertyGridInterface::_set_extra_style(_self,hash);
	}
	return self;
}


}
}
#endif

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

/* Document-method: caption_font
 * call-seq:
 *   caption_font -> WX::Font
 *
 * returns the caption font of this WX::PropertyGrid.
 * ===Return value
 * WX::Font
 */


/* Document-attr: root
 * the root property of this WX::PropertyGrid. WX::Property
 */
/* Document-attr: status_bar
 * the status bar of this WX::PropertyGrid when available. WX::StatusBar
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
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_mWXPropertyGridInterface = rb_define_module_under(rb_mWX,"PropertyGridInterface");

#endif
#if wxUSE_PROPGRID
	using namespace RubyWX::PropertyGrid;
	rb_cWXPropertyGrid = rb_define_class_under(rb_mWX,"PropertyGrid",rb_cWXControl);
	rb_define_alloc_func(rb_cWXPropertyGrid,_alloc);

#if 0
	rb_define_attr(rb_cWXPropertyGrid,"root",1,0);
	rb_define_attr(rb_cWXPropertyGrid,"status_bar",1,0);

	rb_define_attr(rb_cWXPropertyGrid,"caption_background_color",1,1);
	rb_define_attr(rb_cWXPropertyGrid,"selection_background_color",1,1);

	rb_define_attr(rb_cWXPropertyGrid,"cell_background_color",1,1);

	rb_define_attr(rb_cWXPropertyGrid,"cell_text_color",1,1);
	rb_define_attr(rb_cWXPropertyGrid,"cell_disabled_text_color",1,1);

	rb_define_attr(rb_cWXPropertyGrid,"empty_space_color",1,1);
	rb_define_attr(rb_cWXPropertyGrid,"line_color",1,1);
	rb_define_attr(rb_cWXPropertyGrid,"margin_color",1,1);

#endif

	rb_include_module(rb_cWXPropertyGrid,rb_mWXPropertyGridInterface);

	rb_define_method(rb_cWXPropertyGrid,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXPropertyGrid,"root",_GetRoot,0);

#if wxUSE_STATUSBAR
	rb_define_attr_method(rb_cWXPropertyGrid,"status_bar",_GetStatusBar,0);
#endif

	rb_define_method(rb_cWXPropertyGrid,"caption_font",RUBY_METHOD_FUNC(_GetCaptionFont),0);

	rb_define_attr_method(rb_cWXPropertyGrid,"caption_background_color",_getCaptionBackgroundColour,_setCaptionBackgroundColour);
	rb_define_attr_method(rb_cWXPropertyGrid,"selection_background_color",_getSelectionBackgroundColour,_setSelectionBackgroundColour);

	rb_define_attr_method(rb_cWXPropertyGrid,"cell_background_color",_getCellBackgroundColour,_setCellBackgroundColour);

	rb_define_attr_method(rb_cWXPropertyGrid,"cell_text_color",_getCellTextColour,_setCellTextColour);
	rb_define_attr_method(rb_cWXPropertyGrid,"cell_disabled_text_color",_getCellDisabledTextColour,_setCellDisabledTextColour);

	rb_define_attr_method(rb_cWXPropertyGrid,"empty_space_color",_getEmptySpaceColour,_setEmptySpaceColour);
	rb_define_attr_method(rb_cWXPropertyGrid,"line_color",_getLineColour,_setLineColour);
	rb_define_attr_method(rb_cWXPropertyGrid,"margin_color",_getMarginColour,_setMarginColour);

	rb_define_const(rb_cWXPropertyGrid,"DEFAULT_STYLE",RB_INT2NUM(wxPG_DEFAULT_STYLE));

	registerInfo<wxPropertyGrid>(rb_cWXPropertyGrid);
#endif

}


