/*
 * wxBitmap.cpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */


#include "wxBitmap.hpp"
#include "wxPalette.hpp"
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
VALUE wrap< wxBitmap >(const wxBitmap &vbitmap)
{
	if(!vbitmap.IsOk())
		return Qnil;
	return wrap(new wxBitmap(vbitmap));
}

template <>
VALUE wrap< wxBitmap >(wxBitmap *bitmap )
{
	if(!bitmap)
		return Qnil;
	if(!bitmap->IsOk())
		return Qnil;
	if(bitmap == &wxNullBitmap)
		return Qnil;
	return Data_Wrap_Struct(rb_cWXBitmap, NULL, NULL, bitmap);
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
	if(!icon)
		return Qnil;
	if(!icon->IsOk())
		return Qnil;
	if(icon == &wxNullIcon)
		return Qnil;
	return wrap< wxBitmap >(wxBitmap(*icon));
}
template <>
VALUE wrap< wxIcon >(const wxIcon &icon )
{
	if(!icon.IsOk())
		return Qnil;
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
	std::size_t count = RARRAY_LEN(dup);
	for(std::size_t i = 0;i < count;++i)
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
macro_attr(Palette,wxPalette)
#endif

DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrap(new wxBitmap);
}

DLL_LOCAL VALUE _draw(VALUE self)
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

	//TODO add a way to delete the DCs again
	return self;
}

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE x,y;
	rb_scan_args(argc, argv, "11",&x,&y);

	if(NIL_P(x))
	{
		_self->LoadFile(unwrap<wxString>(x),wxBITMAP_TYPE_ANY);
	}else
		_self->Create(NUM2INT(x),NUM2INT(y));

	return self;
}


DLL_LOCAL VALUE _initialize_copy(VALUE self,VALUE other)
{
	//TODO delete old data?
	DATA_PTR(self) = new wxBitmap(unwrap<wxBitmap>(other));
	return self;
}


#if wxUSE_IMAGE
DLL_LOCAL VALUE _to_image(VALUE self)
{
	return wrap(_self->ConvertToImage());
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
	return wrap(_self->ConvertToImage());
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
	//TODO delete old data?
	DATA_PTR(self) = new wxBitmap(unwrap<wxImage>(data));
	return self;
}

#endif

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



DLL_LOCAL void Init_WXBitmap(VALUE rb_mWX)
{
	wxBitmap::InitStandardHandlers();

	using namespace RubyWX::Bitmap;
	rb_cWXBitmap = rb_define_class_under(rb_mWX,"Bitmap",rb_cObject);
	rb_define_alloc_func(rb_cWXBitmap,_alloc);

	rb_define_method(rb_cWXBitmap,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXBitmap,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

#if wxUSE_IMAGE
	rb_define_method(rb_cWXBitmap,"to_image",RUBY_METHOD_FUNC(_to_image),0);

	rb_define_method(rb_cWXBitmap,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXBitmap,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);
#else
	rb_undef_method(rb_cWXBitmap,"_load");
	rb_undef_method(rb_cWXBitmap,"_dump");
#endif

#if wxUSE_PALETTE
	rb_define_attr_method(rb_cWXBitmap,"palette",_getPalette,_setPalette);
#endif

	rb_define_method(rb_cWXBitmap,"to_bitmap",RUBY_METHOD_FUNC(_to_bitmap),0);

	rb_define_method(rb_cWXBitmap,"save_file",RUBY_METHOD_FUNC(_save_file),-1);


	registerArtID("folder",wxART_FOLDER);

	registerArtID("open",wxART_FILE_OPEN,wxID_OPEN);
	registerArtID("save",wxART_FILE_SAVE,wxID_SAVE);
	registerArtID("save_as",wxART_FILE_SAVE_AS,wxID_SAVEAS);

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


	registerArtID("close",wxART_CLOSE,wxID_CLOSE);
	registerArtID("quit",wxART_QUIT);

	registerArtID("find",wxART_FIND,wxID_FIND);
	registerArtID("replace",wxART_FIND_AND_REPLACE,wxID_REPLACE);


	registerArtID("floppy",wxART_FLOPPY,wxID_FLOPPY);
	registerArtID("cdrom",wxART_CDROM,wxID_CDROM);


	registerArtID("question",wxART_QUESTION);
	registerArtID("warning",wxART_WARNING);
	registerArtID("information",wxART_INFORMATION);
	registerArtID("error",wxART_ERROR);

	registerArtID("help_side_panel",wxART_HELP_SIDE_PANEL);
	registerArtID("help_settings",wxART_HELP_SETTINGS);
	registerArtID("help_book",wxART_HELP_BOOK);
	registerArtID("help_folder",wxART_HELP_FOLDER);
	registerArtID("help_page",wxART_HELP_PAGE);

	registerArtID("go_back",wxART_GO_BACK);
	registerArtID("go_forward",wxART_GO_FORWARD);
	registerArtID("go_up",wxART_GO_UP);
	registerArtID("go_down",wxART_GO_DOWN);
	registerArtID("go_to_parent",wxART_GO_TO_PARENT);
	registerArtID("go_home",wxART_GO_HOME);
	registerArtID("goto_first",wxART_GOTO_FIRST);
	registerArtID("goto_last",wxART_GOTO_LAST);


}
