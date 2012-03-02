/*
 * wxImage.cpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#include "wxSize.hpp"
#include "wxImage.hpp"
#include "wxColor.hpp"

#include "wxStream.hpp"

#include <wx/filename.h>

#define _self wrap<wxImage*>(self)

VALUE rb_cWXImage;

namespace RubyWX {
namespace Image {

VALUE _load(int argc,VALUE *argv,VALUE self)
{
	VALUE name,mime,nr;
	rb_scan_args(argc, argv, "12",&name,&mime,&nr);

	int err = 0;

	bool result;

	if(NIL_P(nr))
		nr = INT2NUM(-1);

#if wxUSE_STREAMS
	if(!rb_respond_to(name,rb_intern("read")))
	{
#endif
		wxFileName dir(wxPathOnly(wrap<wxString>(name)));
		dir.MakeAbsolute(wxGetCwd());

		wxFileName file(wrap<wxString>(name));
		file.MakeAbsolute(wxGetCwd());

		if(dir.Exists())
		{
			if(file.Exists()){
				if(!file.IsFileReadable())
					err = EACCES;
			}else
				err = ENOENT;
		}else
			err = ENOENT;

		if(err)
		{
			rb_syserr_fail(err,wrap< char* >(name));
			return Qfalse;
		}

		if(NIL_P(mime)){
			result = _self->LoadFile(file.GetFullPath());
		}else if(SYMBOL_P(mime) || FIXNUM_P(mime)){
			result = _self->LoadFile(file.GetFullPath(),wrap<wxBitmapType>(mime),NUM2INT(nr));
		}else
			result = _self->LoadFile(file.GetFullPath(),wrap<wxString>(mime),NUM2INT(nr));
#if wxUSE_STREAMS
	}else{
		RubyInputStream st(name);

		if(NIL_P(mime)){
			result = _self->LoadFile(st);
		}else if(SYMBOL_P(mime) || FIXNUM_P(mime)){
			result = _self->LoadFile(st,wrap<wxBitmapType>(mime),NUM2INT(nr));
		}else
			result = _self->LoadFile(st,wrap<wxString>(mime),NUM2INT(nr));
	}
#endif
	return wrap(result);
}

VALUE _alloc(VALUE self) {
	return wrap(new wxImage);
}


/*
 * call-seq:
 *   Image.new(path,[type,[id]])
 *   Image.new(width,height)
 *
 * creates a new Image Object.
*/
VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE width,height,arg1;
	rb_scan_args(argc, argv, "12",&width,&height,&arg1);
	if(rb_obj_is_kind_of(width, rb_cWXSize))
	{
		_self->Create(wrap<wxSize>(width));
	} else if(! rb_obj_is_kind_of(width, rb_cNumeric) || NIL_P(height) || !NIL_P(arg1))
		_load(argc,argv,self);
	else
		_self->Create(NUM2INT(width),NUM2INT(height));
	//_self->InitAlpha();
	return self;
}
/*
*/
VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_self->SetData(wrap<wxImage*>(other)->GetData());
	_self->SetAlpha(wrap<wxImage*>(other)->GetAlpha());
	return result;
}

VALUE _getHeight(VALUE self)
{
	if(_self->IsOk())
		return ULONG2NUM(_self->GetHeight());
	return INT2FIX(0);
}

VALUE _getWidth(VALUE self)
{
	if(_self->IsOk())
		return ULONG2NUM(_self->GetWidth());
	return INT2FIX(0);
}

VALUE _get(int argc,VALUE *argv,VALUE self)
{
	VALUE vx,vy;
	rb_scan_args(argc, argv, "20",&vx,&vy);
	if(_self->IsOk())
	{
		int x,y;
		x = NUM2UINT(vx);
		y = NUM2UINT(vy);

		if(y < 0 || x < 0 || x >= _self->GetWidth() || y >= _self->GetHeight())
			return Qnil;

		unsigned char red,green,blue,alpha;
		red = _self->GetRed(x,y);
		green = _self->GetGreen(x,y);
		blue = _self->GetBlue(x,y);
		if(_self->HasAlpha())
			alpha = _self->GetAlpha(x,y);
		else
			alpha = wxALPHA_OPAQUE;
		return wrap(new wxColor(red,green,blue,alpha));
	}else
		return Qnil;
}

VALUE _set(int argc,VALUE *argv,VALUE self)
{
	VALUE vx,vy,value;
	rb_scan_args(argc, argv, "30",&vx,&vy,&value);
	if(_self->IsOk())
	{
		int x,y;
		x = NUM2UINT(vx);
		y = NUM2UINT(vy);

		if(y < 0 || x < 0 || x >= _self->GetWidth() || y >= _self->GetHeight())
			return value;

		wxColor *c = wrap<wxColor*>(value);
		_self->SetRGB(x,y,c->Red(),c->Green(),c->Blue());
		if(_self->HasAlpha())
			_self->SetAlpha(x,y,c->Alpha());
	}

	return value;
}

VALUE _getMask(VALUE self)
{
	unsigned char r,g,b;
	if(_self->GetOrFindMaskColour(&r,&g,&b))
		return wrap(new wxColor(r,g,b));
	else
		return Qnil;
}

VALUE _setMask(VALUE self,VALUE val)
{
	if(NIL_P(val))
		_self->SetMask(false);
	else
	{
		wxColor* c = wrap<wxColor*>(val);
		_self->SetMaskColour(c->Red(),c->Green(),c->Blue());
	}
	return val;
}


VALUE _save(int argc,VALUE *argv,VALUE self)
{
	VALUE name,mime;
	rb_scan_args(argc, argv, "11",&name,&mime);
	if(!_self->IsOk())
		return Qfalse;
	int err = 0;

	wxFileName dir(wxPathOnly(wrap<wxString>(name)));
	dir.MakeAbsolute(wxGetCwd());

	wxFileName file(wrap<wxString>(name));
	file.MakeAbsolute(wxGetCwd());
	if(dir.Exists())
	{
		if(file.Exists() && !file.IsFileWritable())
			err = EACCES;
		else if(!dir.IsDirWritable())
			err = EACCES;
	}else
		err = ENOENT;

	if(err)
	{
		rb_syserr_fail(err,wrap< char* >(name));
		return Qfalse;
	}

	bool result = false;

	if(NIL_P(mime)){
		result = _self->SaveFile(file.GetFullPath());
	}else if(SYMBOL_P(mime) || FIXNUM_P(mime)){
//		if(wxImage::FindHandler(wrap<wxBitmapType>(mime)))
			result = _self->SaveFile(file.GetFullPath(),wrap<wxBitmapType>(mime));
//		else
//			rb_raise(rb_eArgError,"%s type not known",rb_id2name(SYM2ID(mime)));
	}else{
//	if(wxImage::FindHandlerMime(wrap<wxString>(mime)))
		result = _self->SaveFile(file.GetFullPath(),wrap<wxString>(mime));
//		else
//			rb_raise(rb_eArgError,"%s mime not known",wrap<wxString>(mime).c_str().AsChar());
	}
	return wrap(result);
}

}
}


void Init_WXImage(VALUE rb_mWX)
{
	wxInitAllImageHandlers();

	using namespace RubyWX::Image;
	rb_cWXImage = rb_define_class_under(rb_mWX,"Image",rb_cObject);
	rb_define_alloc_func(rb_cWXImage,_alloc);


	rb_define_method(rb_cWXImage,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXImage,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);


	rb_define_method(rb_cWXImage,"height",RUBY_METHOD_FUNC(_getHeight),0);
	rb_define_method(rb_cWXImage,"width",RUBY_METHOD_FUNC(_getWidth),0);

	rb_define_method(rb_cWXImage,"[]",RUBY_METHOD_FUNC(_get),-1);
	rb_define_method(rb_cWXImage,"[]=",RUBY_METHOD_FUNC(_set),-1);

	rb_define_attr_method(rb_cWXImage,"mask",_getMask,_setMask);

	rb_define_method(rb_cWXImage,"load",RUBY_METHOD_FUNC(_load),-1);
	rb_define_method(rb_cWXImage,"save",RUBY_METHOD_FUNC(_save),-1);
}


