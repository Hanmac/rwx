/*
 * wxGridCellRenderer.cpp
 *
 *  Created on: 18.04.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxGridCellAttr.hpp"
#include "wxRect.hpp"
#include "wxDC.hpp"

#define mRuby static_cast<RubyClientData*>(GetClientObject())->mRuby

VALUE rb_cWXGridCellRenderer;

#if wxUSE_GRID

template <>
VALUE wrap< wxGridCellRenderer >(wxGridCellRenderer* window)
{
	return wrapPtr(window,rb_cWXGridCellRenderer);
}

class RubyGridCellRenderer : public wxGridCellRenderer
{
public :
	void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)
	{
		wxGridCellRenderer::Draw(grid,attr,dc,rect,row,col,isSelected);
		rb_funcall(mRuby,rb_intern("draw"),7,wrap(&grid),wrap(&attr),wrap(&dc),wrap(rect),INT2NUM(row),INT2NUM(col),wrap(isSelected));
	}

	wxSize GetBestSize(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, int row, int col)
	{
		return unwrap<wxSize>(rb_funcall(mRuby,rb_intern("best_size"),5,wrap(&grid),wrap(&attr),wrap(&dc),INT2NUM(row),INT2NUM(col)));
	}
	wxGridCellRenderer *Clone() const
	{
		return new RubyGridCellRenderer;
	}
};


template <>
wxGridCellRenderer* unwrap<wxGridCellRenderer*>(const VALUE &arg)
{
	return unwrapPtr<wxGridCellRenderer>(arg,rb_cWXGridCellRenderer);
}


#define typeRenderer(type) \
VALUE rb_cWXGridCell##type##Renderer; \
class RubyGridCell##type##Renderer : public wxGridCell##type##Renderer \
{\
public :\
	void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)\
	{\
		VALUE self = mRuby;\
		if(!rubycall)\
		{\
			rubycall = true;\
			rb_funcall(self,rb_intern("draw"),7,wrap(&grid),wrap(&attr),wrap(&dc),wrap(rect),INT2NUM(row),INT2NUM(col),wrap(isSelected));\
			rubycall = false;\
		}else\
			wxGridCell##type##Renderer::Draw(grid,attr,dc,rect,row,col,isSelected);\
	}\
\
	wxSize GetBestSize(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, int row, int col)\
	{\
		VALUE self = mRuby;\
		if(!rubycall)\
		{\
			rubycall = true;\
			wxSize size(unwrap<wxSize>(rb_funcall(self,rb_intern("best_size"),7,wrap(&grid),wrap(&attr),wrap(&dc),INT2NUM(row),INT2NUM(col))));\
			rubycall = false;\
			return size;\
		}else\
			return wxGridCell##type##Renderer::GetBestSize(grid,attr,dc,row,col);\
\
	}\
private:\
bool rubycall;\
};

typeRenderer(String)
typeRenderer(AutoWrapString)

typeRenderer(Bool)
typeRenderer(Float)
typeRenderer(Number)
typeRenderer(Enum)

#if wxUSE_DATETIME
typeRenderer(DateTime)
#endif

namespace RubyWX {
namespace GridCellRenderer {
//macro_attr(Path,wxString)

APP_PROTECT(RubyGridCellRenderer);

}
}

#endif

DLL_LOCAL void Init_WXGridCellRenderer(VALUE rb_mWX)
{
#if wxUSE_GRID
	using namespace RubyWX::GridCellRenderer;
	rb_cWXGridCellRenderer = rb_define_class_under(rb_mWX,"GridCellRenderer",rb_cObject);
	rb_define_alloc_func(rb_cWXGridCellRenderer,_alloc);

	registerType<wxGridCellRenderer>(rb_cWXGridCellRenderer);
#endif
}


