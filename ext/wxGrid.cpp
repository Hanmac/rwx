/*
 * wxGrid.cpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */


#include "wxControl.hpp"
#include "wxGrid.hpp"
#include "wxGridTable.hpp"
#include "wxGridCellRenderer.hpp"
#include "wxGridCellAttr.hpp"

#include "wxColor.hpp"
#include "wxFont.hpp"

VALUE rb_cWXGrid, rb_cWXGridEvent;
#if wxUSE_GRID

#define _self unwrap<wxGrid*>(self)

namespace RubyWX {
namespace Grid {

macro_attr(Table,wxGridTableBase*)
macro_attr(GridLineColour,wxColour)
macro_attr(LabelBackgroundColour,wxColour)
macro_attr(LabelTextColour,wxColour)
macro_attr(LabelFont,wxFont)

macro_attr(CellHighlightColour,wxColour)
macro_attr(CellHighlightPenWidth,int)
macro_attr(CellHighlightROPenWidth,int)

macro_attr(RowLabelSize,int)
macro_attr(ColLabelSize,int)

macro_attr(DefaultRowSize,int)
macro_attr(DefaultColSize,int)

macro_attr(ColMinimalAcceptableWidth,int)
macro_attr(RowMinimalAcceptableHeight,int)


//macro_attr(SelectionMode,wxGrid::wxGridSelectionModes)

macro_attr(SelectionBackground,wxColour)
macro_attr(SelectionForeground,wxColour)

macro_attr(DefaultCellBackgroundColour,wxColour)
macro_attr(DefaultCellTextColour,wxColour)
macro_attr(DefaultCellFont,wxFont)
macro_attr(DefaultCellOverflow,bool)

macro_attr(DefaultRenderer,wxGridCellRenderer*)
//macro_attr(DefaultEditor,wxGridCellEditor*)

singlereturn(GetSelectedCells)
singlereturn(GetSelectedRows)
singlereturn(GetSelectedCols)

singlereturn(GetSelectionBlockTopLeft);
singlereturn(GetSelectionBlockBottomRight);

singlereturn(IsSelection)

singlefunc(AutoSize)

APP_PROTECT(wxGrid)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash,name;
	rb_scan_args(argc, argv, "12",&parent,&name,&hash);

	if(!_created) {
#if wxUSE_XRC
		if(!loadxrc(_self,name,unwrap<wxWindow*>(parent)))
#endif
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_self->SetMargins(0,0);
		
	}
	if(rb_obj_is_kind_of(name,rb_cString)){
		VALUE args[] = {parent,hash};

		rb_call_super(2,args);
	}else {
		rb_call_super(argc,argv);
	}

	return self;
}
DLL_LOCAL VALUE _getRowSize(VALUE self,VALUE row)
{
	return INT2NUM(	_self->GetRowSize(NUM2INT(row)));
}

DLL_LOCAL VALUE _getColSize(VALUE self,VALUE row)
{
	return INT2NUM(	_self->GetColSize(NUM2INT(row)));
}

DLL_LOCAL VALUE _getCellSize(VALUE self,VALUE x,VALUE y)
{
	return wrap(_self->GetCellSize(wxGridCellCoords(NUM2INT(x),NUM2INT(y))));
}




DLL_LOCAL VALUE _getEditable(VALUE self)
{
	return wrap(_self->IsEditable());
}

DLL_LOCAL VALUE _setEditable(VALUE self,VALUE val)
{
	_self->EnableEditing(RTEST(val));
	return val;
}


}
}

#endif

DLL_LOCAL void Init_WXGrid(VALUE rb_mWX)
{
#if wxUSE_GRID
	using namespace RubyWX::Grid;
	rb_cWXGrid = rb_define_class_under(rb_mWX,"Grid",rb_cWXPanel);
	rb_define_alloc_func(rb_cWXGrid,_alloc);

	rb_define_method(rb_cWXGrid,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXGrid,"table",_getTable,_setTable);
	rb_define_attr_method(rb_cWXGrid,"line_color",_getGridLineColour,_setGridLineColour);
	rb_define_attr_method(rb_cWXGrid,"label_background_color",_getLabelBackgroundColour,_setLabelBackgroundColour);
	rb_define_attr_method(rb_cWXGrid,"label_text_color",_getLabelTextColour,_setLabelTextColour);
	rb_define_attr_method(rb_cWXGrid,"label_font",_getLabelFont,_setLabelFont);

	rb_define_attr_method(rb_cWXGrid,"row_label_size",_getRowLabelSize,_setRowLabelSize);
	rb_define_attr_method(rb_cWXGrid,"col_label_size",_getColLabelSize,_setColLabelSize);

	rb_define_attr_method(rb_cWXGrid,"default_row_size",_getDefaultRowSize,_setDefaultRowSize);
	rb_define_attr_method(rb_cWXGrid,"default_col_size",_getDefaultColSize,_setDefaultColSize);


	rb_define_attr_method(rb_cWXGrid,"highlight_color",_getCellHighlightColour,_setCellHighlightColour);
	rb_define_attr_method(rb_cWXGrid,"highlight_penwidth",_getCellHighlightPenWidth,_setCellHighlightPenWidth);
	rb_define_attr_method(rb_cWXGrid,"highlight_ropen_width",_getCellHighlightROPenWidth,_setCellHighlightROPenWidth);

	rb_define_attr_method(rb_cWXGrid,"default_renderer",_getDefaultRenderer,_setDefaultRenderer);

	rb_define_attr_method(rb_cWXGrid,"minimal_width",_getColMinimalAcceptableWidth,_setColMinimalAcceptableWidth);
	rb_define_attr_method(rb_cWXGrid,"minimal_height",_getRowMinimalAcceptableHeight,_setRowMinimalAcceptableHeight);


	rb_define_attr_method(rb_cWXGrid,"editable",_getEditable,_setEditable);

	rb_define_method(rb_cWXGrid,"autosize",RUBY_METHOD_FUNC(_AutoSize),0);
	rb_define_method(rb_cWXGrid,"cellsize",RUBY_METHOD_FUNC(_getCellSize),2);

	rb_define_method(rb_cWXGrid,"row_size",RUBY_METHOD_FUNC(_getRowSize),1);
	rb_define_method(rb_cWXGrid,"col_size",RUBY_METHOD_FUNC(_getColSize),1);



	rb_define_method(rb_cWXGrid,"selection?",RUBY_METHOD_FUNC(_IsSelection),0);

	rb_define_method(rb_cWXGrid,"selected_cells",RUBY_METHOD_FUNC(_GetSelectedCells),0);
	rb_define_method(rb_cWXGrid,"selected_rows",RUBY_METHOD_FUNC(_GetSelectedRows),0);
	rb_define_method(rb_cWXGrid,"selected_cols",RUBY_METHOD_FUNC(_GetSelectedCols),0);

	rb_define_method(rb_cWXGrid,"selected_topleft",RUBY_METHOD_FUNC(_GetSelectionBlockTopLeft),0);
	rb_define_method(rb_cWXGrid,"selected_bottomright",RUBY_METHOD_FUNC(_GetSelectionBlockBottomRight),0);


	rb_cWXGridEvent = rb_define_class_under(rb_cWXEvent,"Grid",rb_cWXEvent);

	registerEventType("grid_cell_left_click", wxEVT_GRID_CELL_LEFT_CLICK, rb_cWXGridEvent );
	registerEventType("grid_cell_right_click", wxEVT_GRID_CELL_RIGHT_CLICK, rb_cWXGridEvent );
	registerEventType("grid_cell_left_dclick", wxEVT_GRID_CELL_LEFT_DCLICK, rb_cWXGridEvent );
	registerEventType("grid_cell_right_dclick", wxEVT_GRID_CELL_RIGHT_DCLICK, rb_cWXGridEvent );
	registerEventType("grid_label_left_click", wxEVT_GRID_LABEL_LEFT_CLICK, rb_cWXGridEvent );
	registerEventType("grid_label_right_click", wxEVT_GRID_LABEL_RIGHT_CLICK, rb_cWXGridEvent );
	registerEventType("grid_label_left_dclick", wxEVT_GRID_LABEL_LEFT_DCLICK, rb_cWXGridEvent );
	registerEventType("grid_label_right_dclick", wxEVT_GRID_LABEL_RIGHT_DCLICK, rb_cWXGridEvent );
	//registerEventType("", wxEVT_GRID_ROW_SIZE, wxGridSizeEvent );
	//registerEventType("", wxEVT_GRID_COL_SIZE, wxGridSizeEvent );
	//registerEventType("", wxEVT_GRID_RANGE_SELECT, wxGridRangeSelectEvent );
	registerEventType("grid_cell_changing", wxEVT_GRID_CELL_CHANGING, rb_cWXGridEvent );
	registerEventType("grid_cell_changed", wxEVT_GRID_CELL_CHANGED, rb_cWXGridEvent );
	registerEventType("grid_select_cell", wxEVT_GRID_SELECT_CELL, rb_cWXGridEvent );
	registerEventType("grid_editor_shown", wxEVT_GRID_EDITOR_SHOWN, rb_cWXGridEvent );
	registerEventType("grid_editor_hidden", wxEVT_GRID_EDITOR_HIDDEN, rb_cWXGridEvent );
	//registerEventType("", wxEVT_GRID_EDITOR_CREATED, wxGridEditorCreatedEvent );
	registerEventType("grid_cell_begin_drag", wxEVT_GRID_CELL_BEGIN_DRAG, rb_cWXGridEvent );
	registerEventType("grid_col_move", wxEVT_GRID_COL_MOVE, rb_cWXGridEvent );
	registerEventType("grid_col_sort", wxEVT_GRID_COL_SORT, rb_cWXGridEvent );

	registerInfo<wxGrid>(rb_cWXGrid);

#endif
}



