/*
 * wxBitmap.cpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */


#include "wxBitmap.hpp"
#include "wxImage.hpp"
#include "wxDC.hpp"
#define _self wrap<wxBitmap*>(self)

VALUE rb_cWXBitmap;


namespace RubyWX {
namespace Bitmap {

macro_attr(Height,int)
macro_attr(Width,int)
macro_attr(Depth,int)

macro_attr(Mask,wxMask*)
#if wxUSE_PALETTE
macro_attr(Palette,wxPalette)
#endif

VALUE _alloc(VALUE self) {
	return wrap(new wxBitmap);
}

VALUE _draw(VALUE self)
{
	wxDC *dc;
	wxMemoryDC mdc;
	mdc.SelectObject(*_self);
#if wxUSE_GRAPHICS_CONTEXT
	wxGCDC gdc(mdc);
	dc = &gdc;
#else
	dc = &mdc;
#endif
	rb_yield(wrap(dc));
	mdc.SelectObject(wxNullBitmap);
	return self;
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE x,y;
	rb_scan_args(argc, argv, "11",&x,&y);

	_self->Create(NUM2INT(x),NUM2INT(y));

	return self;
}

#if wxUSE_IMAGE
VALUE _to_image(VALUE self)
{
	return wrap(_self->ConvertToImage());
}
#endif
VALUE _to_bitmap(VALUE self)
{
	return self;
}


}
}

wxBitmap wrapBitmap(const VALUE &vbitmap,wxWindowID id,bool disabled,const wxArtClient &type)
{
	if(NIL_P(vbitmap))
	{
		if(disabled)
			return wxNullBitmap;
		else
		{
			wxArtID aid;
			switch(id){
			case wxID_NEW:
				aid = wxART_NEW;
				break;
			case wxID_OPEN:
				aid = wxART_FILE_OPEN;
				break;
			case wxID_SAVE:
				aid = wxART_FILE_SAVE;
				break;
			case wxID_SAVEAS:
				aid = wxART_FILE_SAVE_AS;
				break;
			case wxID_COPY:
				aid = wxART_COPY;
				break;
			case wxID_CUT:
				aid = wxART_CUT;
				break;
			case wxID_PASTE:
				aid = wxART_PASTE;
				break;
			case wxID_UNDO:
				aid = wxART_UNDO;
				break;
			case wxID_REDO:
				aid = wxART_REDO;
				break;

			case wxID_PRINT:
				aid = wxART_PRINT;
				break;

			case wxID_DELETE:
				aid = wxART_DELETE;
				break;
			case wxID_HELP:
				aid = wxART_HELP;
				break;

			case wxID_CLOSE:
				aid = wxART_CLOSE;
				break;

			case wxID_FIND:
				aid = wxART_FIND;
				break;

			case wxID_REPLACE:
				aid = wxART_FIND_AND_REPLACE;
				break;

			default:
				if(!disabled)
					rb_raise(rb_eArgError,"need an valid bitmap");
				break;
			}
			return wxArtProvider::GetBitmap(aid,type);
		}
	}
	wxBitmap temp = wxArtProvider::GetBitmap(wrap<wxString>(vbitmap),type);
	if(temp.IsOk())
		return temp;
	return *wrap<wxBitmap*>(vbitmap);
}



void Init_WXBitmap(VALUE rb_mWX)
{
	wxBitmap::InitStandardHandlers();

	using namespace RubyWX::Bitmap;
	rb_cWXBitmap = rb_define_class_under(rb_mWX,"Bitmap",rb_cObject);
	rb_define_alloc_func(rb_cWXBitmap,_alloc);
#if wxUSE_IMAGE
	rb_define_method(rb_cWXBitmap,"to_image",RUBY_METHOD_FUNC(_to_image),0);
#endif
	rb_define_method(rb_cWXBitmap,"to_bitmap",RUBY_METHOD_FUNC(_to_bitmap),0);

}
