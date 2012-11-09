/*
 * wxBitmap.cpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */


#include "wxBitmap.hpp"
#include "wxDC.hpp"
#include <map>
#include <wx/artprov.h>

#define _self unwrap<wxBitmap*>(self)

VALUE rb_cWXBitmap;

typedef std::map<wxWindowID,wxArtID> WindowArt;
WindowArt windowArtHolder;
typedef std::map<ID,wxArtID> RubyArt;
RubyArt rubyArtHolder;

void registerArtID(const char * name,const wxArtID& artid,wxWindowID wid = wxID_NONE)
{
	if(wid != wxID_NONE)
		windowArtHolder.insert(std::make_pair(wid,artid));
	rubyArtHolder.insert(std::make_pair(rb_intern(name),artid));
}


template <>
VALUE wrap< wxBitmap >(wxBitmap *bitmap )
{
	if(bitmap)
		return Data_Wrap_Struct(rb_cWXBitmap, NULL, NULL, bitmap);
	return Qnil;
}

template <>
wxBitmap* unwrap< wxBitmap* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullBitmap;
	if(rb_obj_is_kind_of(vbitmap,rb_cWXBitmap))
		return unwrapPtr<wxBitmap>(vbitmap, rb_cWXBitmap);
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
	if(icon == &wxNullIcon)
		return Qnil;
	return wrap< wxBitmap >(icon);
}
template <>
VALUE wrap< wxIcon >(const wxIcon &icon )
{
	if(&icon == &wxNullIcon)
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

template <>
VALUE wrap< wxImageList >(wxImageList *imagelist )
{
	VALUE result = rb_ary_new();
	int count = imagelist->GetImageCount();
	for(int i = 0;i < count;++i)
		rb_ary_push(result,wrap(imagelist->GetBitmap(i)));
	return result;
}

template <>
wxImageList* unwrap< wxImageList* >(const VALUE &imagelist)
{
	wxImageList *result = new wxImageList;
	VALUE dup = rb_funcall(imagelist,rb_intern("to_a"),0);
	result->Create();
	size_t count = RARRAY_LEN(dup);
	for(size_t i = 0;i < count;++i)
		result->Add(unwrap<wxBitmap>(RARRAY_PTR(dup)[i]));
	return result;
}


namespace RubyWX {
namespace Bitmap {

macro_attr(Height,int)
macro_attr(Width,int)
macro_attr(Depth,int)

macro_attr(Mask,wxMask*)
#if wxUSE_PALETTE
//macro_attr(Palette,wxPalette)
#endif

VALUE _alloc(VALUE self) {
	return wrap(new wxBitmap);
}

VALUE _draw(VALUE self)
{
	wxDC *dc;
	wxMemoryDC *mdc = new wxMemoryDC;
	mdc->SelectObject(*_self);
#if wxUSE_GRAPHICS_CONTEXT
	dc = new wxGCDC(*mdc);
#else
	dc = &mdc;
#endif
	rb_yield(wrap(dc));
	mdc->SelectObject(wxNullBitmap);
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

VALUE _save_file(int argc,VALUE *argv,VALUE self)
{
	VALUE name;
	rb_scan_args(argc, argv, "10",&name);
	return wrap(_self->SaveFile(unwrap<wxString>(name),wxBITMAP_TYPE_PNG));
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
			WindowArt::iterator it = windowArtHolder.find(id);
			if(it == windowArtHolder.end())
				if(!disabled)
					rb_raise(rb_eArgError,"need an valid bitmap");
			return wxArtProvider::GetBitmap(it->second,type);
		}
	}else if(SYMBOL_P(vbitmap))
	{
		RubyArt::iterator it = rubyArtHolder.find(SYM2ID(vbitmap));
		if(it == rubyArtHolder.end())
			if(!disabled)
				rb_raise(rb_eArgError,"need an valid bitmap");
		return wxArtProvider::GetBitmap(it->second,type);
	}
	wxBitmap temp = wxArtProvider::GetBitmap(unwrap<wxString>(vbitmap),type);
	if(temp.IsOk())
		return temp;
	return *unwrap<wxBitmap*>(vbitmap);
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

	rb_define_method(rb_cWXBitmap,"save_file",RUBY_METHOD_FUNC(_save_file),-1);

	registerArtID("new",wxART_NEW,wxID_NEW);

	registerArtID("folder",wxART_FOLDER);

	registerArtID("open",wxART_FILE_OPEN,wxID_OPEN);
	registerArtID("save",wxART_FILE_SAVE,wxID_SAVE);
	registerArtID("save_as",wxART_FILE_SAVE_AS,wxID_SAVEAS);

	registerArtID("folder_open",wxART_FOLDER_OPEN);


	registerArtID("copy",wxART_COPY,wxID_COPY);
	registerArtID("cut",wxART_CUT,wxID_CUT);
	registerArtID("paste",wxART_PASTE,wxID_PASTE);

	registerArtID("undo",wxART_UNDO,wxID_UNDO);
	registerArtID("redo",wxART_REDO,wxID_REDO);

	registerArtID("print",wxART_PRINT,wxID_PRINT);

	registerArtID("delete",wxART_DELETE,wxID_DELETE);

	registerArtID("help",wxART_HELP,wxID_HELP);
	registerArtID("tip",wxART_TIP);


	registerArtID("close",wxART_CLOSE,wxID_CLOSE);
	registerArtID("quit",wxART_QUIT);

	registerArtID("find",wxART_FIND,wxID_FIND);
	registerArtID("replace",wxART_FIND_AND_REPLACE,wxID_FIND);


	registerArtID("floppy",wxART_FLOPPY,wxID_FLOPPY);
	registerArtID("cdrom",wxART_CDROM,wxID_CDROM);


}
