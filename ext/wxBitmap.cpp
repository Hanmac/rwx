/*
 * wxBitmap.cpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"
#include "wxBitmap.hpp"
#include "wxColor.hpp"
#include "wxPalette.hpp"
#include "wxDC.hpp"
#include <map>
#include <wx/artprov.h>

#include <wx/colour.h>

#if !wxUSE_IMAGE
#include <wx/rawbmp.h>
#endif

#define _self unwrap<wxBitmap*>(self)

VALUE rb_cWXBitmap, rb_cWXMask;

typedef std::map<wxWindowID,wxArtID> WindowArt;
WindowArt windowArtHolder;
typedef std::map<ID,wxArtID> RubyArt;
RubyArt rubyArtHolder;

void registerWindowArtID(wxWindowID wid, const wxArtID& artid)
{
	windowArtHolder.insert(std::make_pair(wid,artid));
}

void registerArtID(const char * name,const wxArtID& artid,wxWindowID wid = wxID_NONE)
{
	if(wid != wxID_NONE)
		registerWindowArtID(wid, artid);
	rubyArtHolder.insert(std::make_pair(rb_intern(name),artid));
}


template <>
VALUE wrap< wxBitmap >(const wxBitmap &vbitmap)
{
	if(!vbitmap.IsOk())
		return Qnil;
	return wrap(new wxBitmap(vbitmap));
}

template <>
VALUE wrap< wxBitmap >(wxBitmap *bitmap )
{
	if(!bitmap || !bitmap->IsOk() || bitmap == &wxNullBitmap)
		return Qnil;
	return wrapTypedPtr(bitmap,rb_cWXBitmap);
}

template <>
wxBitmap* unwrap< wxBitmap* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullBitmap;
	if(rb_obj_is_kind_of(vbitmap,rb_cWXBitmap))
		return unwrapTypedPtr<wxBitmap>(vbitmap, rb_cWXBitmap);
#if wxUSE_IMAGE
	if(rb_obj_is_kind_of(vbitmap,rb_cWXImage))
		return new wxBitmap(unwrap<wxImage>(vbitmap));
#endif
	if(is_wrapable<wxSize>(vbitmap))
		return new wxBitmap(unwrap<wxSize>(vbitmap));
	else
		return new wxBitmap(unwrap<wxString>(vbitmap),wxBITMAP_TYPE_ANY);
}

template <>
wxBitmap unwrap< wxBitmap >(const VALUE &vbitmap)
{
	return *unwrap<wxBitmap*>(vbitmap);
}

template <>
wxBitmap& unwrap< wxBitmap& >(const VALUE &vbitmap)
{
	return *unwrap< wxBitmap* >(vbitmap);
}


template <>
VALUE wrap< wxIcon >(wxIcon *icon )
{
	if(!icon || !icon->IsOk() || icon == &wxNullIcon)
		return Qnil;
	return wrap< wxBitmap >(wxBitmap(*icon));
}
template <>
VALUE wrap< wxIcon >(const wxIcon &icon )
{
	if(!icon.IsOk() || &icon == &wxNullIcon)
		return Qnil;
	return wrap< wxBitmap >(new wxBitmap(icon));
}

template <>
wxIcon* unwrap< wxIcon* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullIcon;
	wxIcon *icon = new wxIcon();
	icon->CopyFromBitmap(unwrap<wxBitmap>(vbitmap));
	return icon;
}

template <>
wxIcon unwrap< wxIcon >(const VALUE &vbitmap)
{
	return *unwrap<wxIcon*>(vbitmap);
}


namespace RubyWX {
namespace Bitmap {

macro_attr(Height,int)
macro_attr(Width,int)
macro_attr(Depth,int)

singlereturn(GetMask)

singlereturn(HasAlpha)

#if wxUSE_PALETTE
macro_attr(Palette,wxPalette)
#endif

DLL_LOCAL VALUE _setMask(VALUE self, VALUE val) {
	if(rb_obj_is_kind_of(val, rb_cWXMask))
		_self->SetMask(unwrap<wxMask*>(val));
	else if(is_wrapable<wxColor>(val))
		_self->SetMask(new wxMask(*_self, unwrap<wxColor>(val)));
#if wxUSE_PALETTE
	else if(FIXNUM_P(val))
		_self->SetMask(new wxMask(*_self, NUM2INT(val)));
#endif
	else
		_self->SetMask(new wxMask(unwrap<wxBitmap>(val)));
	return val;
}

DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrapTypedPtr(new wxBitmap, self);
}

DLL_LOCAL VALUE _draw(VALUE self)
{
	app_protected();
	rb_check_frozen(self);

	wxDC *dc;
	wxMemoryDC *mdc = new wxMemoryDC(*_self);
#if wxUSE_GRAPHICS_CONTEXT
	dc = new wxGCDC(*mdc);
#else
	dc = mdc;
#endif
	rb_yield(wrap(dc));
	mdc->SelectObject(wxNullBitmap);

	//TODO add a way to delete the DCs again
	return self;
}

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE x,y, depth;
	rb_scan_args(argc, argv, "12",&x,&y, &depth);

	if(NIL_P(x)){
		_self->LoadFile(unwrap<wxString>(x),wxBITMAP_TYPE_ANY);
	}else {
#if wxUSE_IMAGE
		(*_self) = wxBitmap(wxImage(NUM2INT(x),NUM2INT(y)));
#else
		_self->Create(NUM2INT(x),NUM2INT(y), NIL_P(depth) ? wxBITMAP_SCREEN_DEPTH : NUM2INT(depth));

		if(_self->HasAlpha()) {

			wxAlphaPixelData data(*_self);

			wxAlphaPixelData::Iterator p(data);

			for ( int cy = 0; cy < data.GetHeight(); ++cy )
			{
				wxAlphaPixelData::Iterator rowStart = p;

				for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p )
				{
					p.Red() = p.Green() = p.Blue() = p.Alpha() = 0;
				}

				p = rowStart;
				p.OffsetY(data, 1);
			}
		}else {
			wxNativePixelData data(*_self);

			wxNativePixelData::Iterator p(data);

			for ( int cy = 0; cy < data.GetHeight(); ++cy )
			{
				wxNativePixelData::Iterator rowStart = p;

				for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p )
				{
					p.Red() = p.Green() = p.Blue() = 0;
				}

				p = rowStart;
				p.OffsetY(data, 1);
			}

		}
#endif
	}
	return self;
}


DLL_LOCAL VALUE _initialize_copy(VALUE self,VALUE other)
{
	wxBitmap cbitmap = unwrap<wxBitmap>(other);
	(*_self) = cbitmap.GetSubBitmap(wxRect(0, 0, cbitmap.GetWidth(), cbitmap.GetHeight()));
	return self;
}


#if wxUSE_IMAGE
DLL_LOCAL VALUE _to_image(VALUE self)
{
	return wrap(_self->ConvertToImage());
}
#endif


/*
 * call-seq:
 *   hash -> Fixnum
 *
 * Generates a Fixnum hash value for this object.
 *
 *
 */
DLL_LOCAL VALUE _getHash(VALUE self)
{
	st_index_t h = rb_hash_start(0);

#if wxUSE_IMAGE
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_to_image(self))));
#else
	h = rb_hash_uint(h, _self->GetWidth());
	h = rb_hash_uint(h, _self->GetHeight());
	h = rb_hash_uint(h, _self->GetDepth());

	if(_self->HasAlpha()) {
		wxAlphaPixelData data(*_self);

		wxAlphaPixelData::Iterator p(data);

		for ( int cy = 0; cy < data.GetHeight(); ++cy )
		{
			wxAlphaPixelData::Iterator rowStart = p;

			for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p )
			{
				h = rb_hash_uint32(h, p.Red() | (p.Green() << 8) | (p.Blue() << 16) | (p.Alpha() << 24) );
			}

			p = rowStart;
			p.OffsetY(data, 1);
		}
	} else {
		wxNativePixelData data(*_self);

		wxNativePixelData::Iterator p(data);

		for ( int cy = 0; cy < data.GetHeight(); ++cy )
		{
			wxNativePixelData::Iterator rowStart = p;

			for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p )
			{
				h = rb_hash_uint32(h, p.Red() | (p.Green() << 8) | (p.Blue() << 16) );
			}

			p = rowStart;
			p.OffsetY(data, 1);
		}

	}
#endif

	h = rb_hash_end(h);
	return LONG2FIX(h);
}

/*
 * call-seq:
 *   marshal_dump -> Array
 *
 * Provides marshalling support for use by the Marshal library.
 * ===Return value
 * Array
 */
DLL_LOCAL VALUE _marshal_dump(VALUE self)
{
	//TODO: turn the result into something that is compatible when image is missing

	VALUE result = rb_ary_new();
#if wxUSE_IMAGE
	rb_ary_push(result,wrap(_self->ConvertToImage()));
#else
	rb_ary_push(result, _getWidth(self));
	rb_ary_push(result, _getHeight(self));
	rb_ary_push(result, _getDepth(self));

	if(_self->HasAlpha()) {
		wxColourBase::ChannelType color_data[_self->GetHeight() * _self->GetWidth() * 3];
		wxColourBase::ChannelType alpha_data[_self->GetHeight() * _self->GetWidth()];

		wxAlphaPixelData data(*_self);

		wxAlphaPixelData::Iterator p(data);

		for ( int cy = 0; cy < data.GetHeight(); ++cy )
		{
			wxAlphaPixelData::Iterator rowStart = p;

			for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p )
			{
				color_data[(cy * data.GetWidth() + cx) * 3] = p.Red();
				color_data[(cy * data.GetWidth() + cx) * 3 + 1] = p.Green();
				color_data[(cy * data.GetWidth() + cx) * 3 + 2] = p.Blue();

				alpha_data[cy * data.GetWidth() + cx] = p.Alpha();

			}

			p = rowStart;
			p.OffsetY(data, 1);
		}
		rb_ary_push(result, rb_str_new((const char*)color_data, _self->GetHeight() * _self->GetWidth() * 3));
		rb_ary_push(result, rb_str_new((const char*)alpha_data, _self->GetHeight() * _self->GetWidth()));

	} else {
		wxColourBase::ChannelType color_data[_self->GetHeight() * _self->GetWidth() * 3];

		wxNativePixelData data(*_self);

		wxNativePixelData::Iterator p(data);

		for ( int cy = 0; cy < data.GetHeight(); ++cy )
		{
			wxNativePixelData::Iterator rowStart = p;

			for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p )
			{
				color_data[(cy * data.GetWidth() + cx) * 3] = p.Red();
				color_data[(cy * data.GetWidth() + cx) * 3 + 1] = p.Green();
				color_data[(cy * data.GetWidth() + cx) * 3 + 2] = p.Blue();
			}

			p = rowStart;
			p.OffsetY(data, 1);
		}
		rb_ary_push(result, rb_str_new((const char*)color_data, _self->GetHeight() * _self->GetWidth() * 3));
		rb_ary_push(result, Qnil);

	}
#endif

	return result;
}

/*
 * call-seq:
 *   marshal_load(array) -> nil
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self,VALUE data)
{
	data = rb_Array(data);
	//TODO delete old data?
#if wxUSE_IMAGE
	(*_self) = wxBitmap(unwrap<wxImage>(RARRAY_AREF(data,0)));
#else
	_self->Create(
		NUM2INT(RARRAY_AREF(data,0)),
		NUM2INT(RARRAY_AREF(data,1)),
		NUM2INT(RARRAY_AREF(data,2))
	);


	if(_self->HasAlpha()) {
		VALUE val = RARRAY_AREF(data,3);
		wxColourBase::ChannelType *color_data = (wxColourBase::ChannelType*)StringValuePtr(val);

		val = RARRAY_AREF(data,4);
		wxColourBase::ChannelType *alpha_data = (wxColourBase::ChannelType*)StringValuePtr(val);

		wxAlphaPixelData data(*_self);

		wxAlphaPixelData::Iterator p(data);

		for ( int cy = 0; cy < data.GetHeight(); ++cy )
		{
			wxAlphaPixelData::Iterator rowStart = p;

			for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p )
			{
				p.Red() = color_data[(cy * data.GetWidth() + cx) * 3];
				p.Green() = color_data[(cy * data.GetWidth() + cx) * 3 + 1];
				p.Blue() = color_data[(cy * data.GetWidth() + cx) * 3 + 2];

				p.Alpha() = alpha_data[cy * data.GetWidth() + cx];

			}

			p = rowStart;
			p.OffsetY(data, 1);
		}

	} else {
		VALUE val = RARRAY_AREF(data,3);
		wxColourBase::ChannelType *color_data = (wxColourBase::ChannelType*)StringValuePtr(val);

		wxNativePixelData data(*_self);

		wxNativePixelData::Iterator p(data);

		for ( int cy = 0; cy < data.GetHeight(); ++cy )
		{
			wxNativePixelData::Iterator rowStart = p;

			for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p )
			{
				p.Red() = color_data[(cy * data.GetWidth() + cx) * 3];
				p.Green() = color_data[(cy * data.GetWidth() + cx) * 3 + 1];
				p.Blue() = color_data[(cy * data.GetWidth() + cx) * 3 + 2];
			}

			p = rowStart;
			p.OffsetY(data, 1);
		}
	}
#endif
	return self;
}

DLL_LOCAL VALUE _to_bitmap(VALUE self)
{
	return self;
}

DLL_LOCAL VALUE _save_file(int argc,VALUE *argv,VALUE self)
{
	VALUE name;
	rb_scan_args(argc, argv, "10",&name);
	return wrap(_self->SaveFile(unwrap<wxString>(name),wxBITMAP_TYPE_PNG));
}


struct equal_obj {
	wxBitmap* self;
	VALUE other;
};

VALUE _equal_block(equal_obj *obj)
{
	wxBitmap cbitmap = unwrap<wxBitmap>(obj->other);
	if(obj->self->IsSameAs(cbitmap))
		return Qtrue;
#if wxUSE_IMAGE
	if(RubyWX::Image::check_equal(
		obj->self->ConvertToImage(), cbitmap.ConvertToImage()
	))
		return Qtrue;
#else
	//compare using PixelData

	if(obj->self->GetWidth() != cbitmap.GetWidth())
		return Qfalse;
	if(obj->self->GetHeight() != cbitmap.GetHeight())
		return Qfalse;
	if(obj->self->GetDepth() != cbitmap.GetDepth())
		return Qfalse;

	if(obj->self->HasAlpha()) {
		wxAlphaPixelData data(*obj->self);
		wxAlphaPixelData cdata(cbitmap);

		wxAlphaPixelData::Iterator p(data);
		wxAlphaPixelData::Iterator cp(cdata);

		for ( int cy = 0; cy < data.GetHeight(); ++cy )
		{
			wxAlphaPixelData::Iterator rowStart = p;
			wxAlphaPixelData::Iterator crowStart = cp;

			for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p, ++cp )
			{
				if(p.Red() != cp.Red()) {
					return Qfalse;
				}
				if(p.Green() != cp.Green()) {
					return Qfalse;
				}
				if(p.Blue() != cp.Blue()) {
					return Qfalse;
				}
				if(p.Alpha() != cp.Alpha()) {
					return Qfalse;
				}
			}

			p = rowStart;
			cp = crowStart;
			p.OffsetY(data, 1);
			cp.OffsetY(cdata, 1);
		}

	} else {
		wxNativePixelData data(*obj->self);
		wxNativePixelData cdata(cbitmap);

		wxNativePixelData::Iterator p(data);
		wxNativePixelData::Iterator cp(cdata);

		for ( int cy = 0; cy < data.GetHeight(); ++cy )
		{
			wxNativePixelData::Iterator rowStart = p;
			wxNativePixelData::Iterator crowStart = cp;

			for ( int cx = 0; cx < data.GetWidth(); ++cx, ++p, ++cp )
			{
				if(p.Red() != cp.Red()) {
					return Qfalse;
				}
				if(p.Green() != cp.Green()) {
					return Qfalse;
				}
				if(p.Blue() != cp.Blue()) {
					return Qfalse;
				}
			}

			p = rowStart;
			cp = crowStart;
			p.OffsetY(data, 1);
			cp.OffsetY(cdata, 1);
		}

	}
	return Qtrue;
#endif
	//TODO: add check with wxPIxelData if Image is not awailable

	return Qfalse;

}

VALUE _equal_rescue(VALUE val)
{
	return Qfalse;
}

/*
 * call-seq:
 *   == brush -> bool
 *
 * compares two brush.
 *
 *
 */
DLL_LOCAL VALUE _equal(VALUE self, VALUE other)
{
	equal_obj obj;
	obj.self = _self;
	obj.other = other;

	return rb_rescue(
		RUBY_METHOD_FUNC(_equal_block),(VALUE)&obj,
		RUBY_METHOD_FUNC(_equal_rescue),Qnil
	);
}

}
}

wxBitmap wrapBitmap(const VALUE &vbitmap,wxWindowID id,wrapBitmapType type,const wxArtClient &client)
{
	if(NIL_P(vbitmap))
	{
		if(type == WRAP_BITMAP_NULL)
			return wxNullBitmap;
		else
		{
			WindowArt::iterator it = windowArtHolder.find(id);
			if(it == windowArtHolder.end())
			{
				if(type == WRAP_BITMAP_RAISE)
					rb_raise(rb_eArgError,"need an valid bitmap");
				return wxNullBitmap;
			}
			return wxArtProvider::GetBitmap(it->second,client);
		}
	}else if(SYMBOL_P(vbitmap))
	{
		RubyArt::iterator it = rubyArtHolder.find(SYM2ID(vbitmap));
		if(it == rubyArtHolder.end())
		{
			if(type == WRAP_BITMAP_RAISE)
				rb_raise(rb_eArgError,"need an valid bitmap");
			return wxNullBitmap;
		}
		return wxArtProvider::GetBitmap(it->second,client);
	}
	wxBitmap temp = wxArtProvider::GetBitmap(unwrap<wxString>(vbitmap),client);
	if(temp.IsOk())
		return temp;
	return *unwrap<wxBitmap*>(vbitmap);
}


/* Document-attr: width
* returns the width of the Bitmap. Integer
*/
/* Document-attr: height
* returns the height of the Bitmap. Integer
*/
/* Document-attr: depth
* returns the depth of the Bitmap. Integer
*/
/* Document-attr: mask
* returns the mask color of the Bitmap. WX::Mask
*/
/* Document-attr: palette
* returns the color palette of the Bitmap. WX::Palette
*/

DLL_LOCAL void Init_WXBitmap(VALUE rb_mWX)
{
	wxBitmap::InitStandardHandlers();

	using namespace RubyWX::Bitmap;
	rb_cWXBitmap = rb_define_class_under(rb_mWX,"Bitmap",rb_cObject);
	rb_define_alloc_func(rb_cWXBitmap,_alloc);
	
#if 0
	rb_define_attr(rb_cWXBitmap,"width",1,1);
	rb_define_attr(rb_cWXBitmap,"height",1,1);
	rb_define_attr(rb_cWXBitmap,"depth",1,1);
	rb_define_attr(rb_cWXBitmap,"mask",1,1);
	rb_define_attr(rb_cWXBitmap,"palette",1,1);
#endif

	rb_define_method(rb_cWXBitmap,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXBitmap,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXBitmap,"height",_getHeight,_setHeight);
	rb_define_attr_method(rb_cWXBitmap,"width",_getWidth,_setWidth);
	rb_define_attr_method(rb_cWXBitmap,"depth",_getDepth,_setDepth);

	rb_define_attr_method(rb_cWXBitmap,"mask",_GetMask,_setMask);

#if wxUSE_IMAGE
	rb_define_method(rb_cWXBitmap,"to_image",RUBY_METHOD_FUNC(_to_image),0);
#endif

	rb_define_method(rb_cWXBitmap,"alpha?",RUBY_METHOD_FUNC(_HasAlpha),0);

	rb_define_method(rb_cWXBitmap,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXBitmap,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	rb_define_method(rb_cWXBitmap,"hash",RUBY_METHOD_FUNC(_getHash),0);

	rb_define_method(rb_cWXBitmap,"==",RUBY_METHOD_FUNC(_equal),1);
	rb_define_alias(rb_cWXBitmap,"eql?","==");

#if wxUSE_PALETTE
	rb_define_attr_method(rb_cWXBitmap,"palette",_getPalette,_setPalette);
#else
	rb_define_attr_method_missing(rb_cWXBitmap,"palette");
#endif

	rb_define_method(rb_cWXBitmap,"draw",RUBY_METHOD_FUNC(_draw),0);

	rb_define_method(rb_cWXBitmap,"to_bitmap",RUBY_METHOD_FUNC(_to_bitmap),0);

	rb_define_method(rb_cWXBitmap,"save_file",RUBY_METHOD_FUNC(_save_file),-1);

	registerInfo<wxBitmap>(rb_cWXBitmap);

	rb_cWXMask = rb_define_class_under(rb_mWX,"Mask",rb_cObject);
	rb_undef_alloc_func(rb_cWXMask);
	rb_undef_method(rb_cWXMask,"initialize_copy");
	rb_undef_method(rb_cWXMask,"_load");
	rb_undef_method(rb_cWXMask,"_dump");

	registerInfo<wxMask>(rb_cWXMask);

	registerArtID("open",wxART_FILE_OPEN,wxID_OPEN);
	registerArtID("save",wxART_FILE_SAVE,wxID_SAVE);
	registerArtID("save_as",wxART_FILE_SAVE_AS,wxID_SAVEAS);

	registerArtID("folder",wxART_FOLDER);
	registerArtID("folder_open",wxART_FOLDER_OPEN);


	registerArtID("copy",wxART_COPY,wxID_COPY);
	registerArtID("cut",wxART_CUT,wxID_CUT);
	registerArtID("paste",wxART_PASTE,wxID_PASTE);
	registerArtID("delete",wxART_DELETE,wxID_DELETE);
	registerArtID("new",wxART_NEW,wxID_NEW);

	registerArtID("undo",wxART_UNDO,wxID_UNDO);
	registerArtID("redo",wxART_REDO,wxID_REDO);

	registerArtID("plus",wxART_PLUS);
	registerArtID("minus",wxART_MINUS);

	registerArtID("print",wxART_PRINT,wxID_PRINT);

	registerArtID("help",wxART_HELP,wxID_HELP);
	registerArtID("tip",wxART_TIP);


	registerArtID("close",wxART_CLOSE, wxID_CLOSE);
	registerArtID("quit",wxART_QUIT, wxID_EXIT);

	registerArtID("find",wxART_FIND,wxID_FIND);
	registerArtID("replace",wxART_FIND_AND_REPLACE,wxID_REPLACE);

	registerArtID("floppy",wxART_FLOPPY,wxID_FLOPPY);
	registerArtID("cdrom",wxART_CDROM,wxID_CDROM);

	registerArtID("error",wxART_ERROR);
	registerArtID("question",wxART_QUESTION);
	registerArtID("warning",wxART_WARNING);
	registerArtID("information",wxART_INFORMATION);
	registerArtID("missing_image",wxART_MISSING_IMAGE);

	registerArtID("help_side_panel",wxART_HELP_SIDE_PANEL);
	registerArtID("help_settings",wxART_HELP_SETTINGS);
	registerArtID("help_book",wxART_HELP_BOOK);
	registerArtID("help_folder",wxART_HELP_FOLDER);
	registerArtID("help_page",wxART_HELP_PAGE);

	registerArtID("go_back",wxART_GO_BACK, wxID_BACKWARD);
	registerArtID("go_forward",wxART_GO_FORWARD, wxID_FORWARD);
	registerArtID("go_up",wxART_GO_UP, wxID_UP);
	registerArtID("go_down",wxART_GO_DOWN, wxID_DOWN);
	registerArtID("go_to_parent",wxART_GO_TO_PARENT);
	registerArtID("go_home",wxART_GO_HOME, wxID_HOME);
	registerArtID("goto_first",wxART_GOTO_FIRST, wxID_FIRST);
	registerArtID("goto_last",wxART_GOTO_LAST, wxID_LAST);


}
