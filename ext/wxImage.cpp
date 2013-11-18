/*
 * wxImage.cpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#include "wxSize.hpp"
#include "wxRect.hpp"

#include "wxImage.hpp"
#include "wxBitmap.hpp"
#include "wxColor.hpp"

#include "wxStream.hpp"

#include <wx/filename.h>

#if wxUSE_IMAGE

#define _self unwrap<wxImage*>(self)

VALUE rb_cWXImage;

template <>
wxImage* unwrap< wxImage* >(const VALUE &vimage)
{
	if(rb_obj_is_kind_of(vimage,rb_cWXImage))
		return unwrapPtr<wxImage>(vimage, rb_cWXImage);
	if(is_wrapable<wxSize>(vimage))
		return new wxImage(unwrap<wxSize>(vimage));
	return unwrapPtr<wxImage>(rb_class_new_instance(1,const_cast<VALUE*>(&vimage),rb_cWXImage), rb_cWXImage);
}


template <>
wxImage unwrap< wxImage >(const VALUE &vimage)
{
	return *unwrap<wxImage*>(vimage);
}


namespace RubyWX {
namespace Image {

DLL_LOCAL VALUE _load(int argc,VALUE *argv,VALUE self)
{
	VALUE name,mime,nr;
	rb_scan_args(argc, argv, "12",&name,&mime,&nr);

	errno = 0;

	bool result;

	if(NIL_P(nr))
		nr = INT2NUM(-1);

#if wxUSE_STREAMS
	if(!rb_respond_to(name,rb_intern("read")))
	{
#endif
		wxFileName dir(wxPathOnly(unwrap<wxString>(name)));
		dir.MakeAbsolute(wxGetCwd());

		wxFileName file(unwrap<wxString>(name));
		file.MakeAbsolute(wxGetCwd());

		if(dir.DirExists())
		{
			if(file.FileExists()){
				if(!file.IsFileReadable())
					errno = EACCES;
			}else
				errno = ENOENT;
		}else
			errno = ENOENT;

		if(errno)
		{
			rb_sys_fail(unwrap< char* >(name));
			return Qfalse;
		}

		if(NIL_P(mime)){
			result = _self->LoadFile(file.GetFullPath());
		}else if(SYMBOL_P(mime) || FIXNUM_P(mime)){
//			result = _self->LoadFile(file.GetFullPath(),unwrap<wxBitmapType>(mime),NUM2INT(nr));
		}else
			result = _self->LoadFile(file.GetFullPath(),unwrap<wxString>(mime),NUM2INT(nr));
#if wxUSE_STREAMS
	}else{
		RubyInputStream st(name);

		if(NIL_P(mime)){
			result = _self->LoadFile(st);
		}else if(SYMBOL_P(mime) || FIXNUM_P(mime)){
//			result = _self->LoadFile(st,unwrap<wxBitmapType>(mime),NUM2INT(nr));
		}else
			result = _self->LoadFile(st,unwrap<wxString>(mime),NUM2INT(nr));
	}
#endif
	return wrap(result);
}

DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrap(new wxImage);
}


/*
 * call-seq:
 *   Image.new(path,[type,[id]])
 *   Image.new(width,height)
 *
 * creates a new Image Object.
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE width,height,arg1;
	rb_scan_args(argc, argv, "12",&width,&height,&arg1);
	if(rb_obj_is_kind_of(width, rb_cWXSize))
	{
		_self->Create(unwrap<wxSize>(width));
	} else if(! rb_obj_is_kind_of(width, rb_cNumeric) || NIL_P(height) || !NIL_P(arg1))
		_load(argc,argv,self);
	else
		_self->Create(NUM2INT(width),NUM2INT(height));
	//_self->InitAlpha();
	return self;
}
/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_self->SetData(unwrap<wxImage*>(other)->GetData());
	_self->SetAlpha(unwrap<wxImage*>(other)->GetAlpha());
	return result;
}

DLL_LOCAL VALUE _getHeight(VALUE self)
{
	if(_self->IsOk())
		return ULONG2NUM(_self->GetHeight());
	return INT2FIX(0);
}

DLL_LOCAL VALUE _getWidth(VALUE self)
{
	if(_self->IsOk())
		return ULONG2NUM(_self->GetWidth());
	return INT2FIX(0);
}

DLL_LOCAL VALUE _get(int argc,VALUE *argv,VALUE self)
{
	VALUE vx,vy;
	rb_scan_args(argc, argv, "11",&vx,&vy);
	if(_self->IsOk())
	{
		if(NIL_P(vy))
		{
			return wrap(_self->GetSubImage(unwrap<wxRect>(vx)));
		}

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

DLL_LOCAL VALUE _set(int argc,VALUE *argv,VALUE self)
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

		wxColor c(unwrap<wxColor>(value));
		_self->SetRGB(x,y,c.Red(),c.Green(),c.Blue());
		if(_self->HasAlpha())
			_self->SetAlpha(x,y,c.Alpha());
	}

	return value;
}

DLL_LOCAL VALUE _scale(VALUE self,VALUE x_scale,VALUE y_scale)
{
	wxImage result(*_self);

	double x,y;
	x = NUM2DBL(x_scale);
	y = NUM2DBL(y_scale);

	if(x < 0)
	{
		x *= -1;
		result = result.Mirror(false);
	}
	if(y < 0)
	{
		y *= -1;
		result = result.Mirror();
	}

	result = result.Scale(result.GetWidth() * x,result.GetHeight() * y);
	return wrap(result);
}

DLL_LOCAL VALUE _mal(VALUE self,VALUE obj)
{


	wxImage *result = new wxImage(_self->GetWidth(),_self->GetHeight());
	//result->SetData(_self->GetData(),true);
	//result->SetAlpha(_self->GetAlpha());
	result->Paste(*_self,0,0);
	if(rb_obj_is_kind_of(obj,rb_cNumeric))
	{
		double d = NUM2DBL(obj);
		for(int x = 0; x < result->GetWidth();++x)
			for(int y = 0; y < result->GetHeight();++y)
			{
				unsigned char alpha;
				if(result->HasAlpha())
					alpha = result->GetAlpha(x,y);
				else
				{
					result->InitAlpha();
					alpha = wxALPHA_OPAQUE;
				}
				result->SetAlpha(x,y,d * alpha);
			}

	}else
	{
		wxColor c(unwrap<wxColor>(obj));
		for(int x = 0; x < result->GetWidth();++x)
			for(int y = 0; y < result->GetHeight();++y)
			{
				unsigned char red,green,blue,alpha;
				red = result->GetRed(x,y);
				green = result->GetGreen(x,y);
				blue = result->GetBlue(x,y);

				if(result->HasAlpha())
					alpha = result->GetAlpha(x,y);
				else
					alpha = wxALPHA_OPAQUE;
				result->SetRGB(x,y,
						red * c.Red() / 256,
						green * c.Green() / 256,
						blue * c.Blue() / 256);
				result->SetAlpha(x,y,alpha * c.Alpha() / wxALPHA_OPAQUE);
			}

	}
	return wrap(result);
}

DLL_LOCAL VALUE _getMask(VALUE self)
{
	unsigned char r,g,b;
	if(_self->GetOrFindMaskColour(&r,&g,&b))
		return wrap(new wxColor(r,g,b));
	else
		return Qnil;
}

DLL_LOCAL VALUE _setMask(VALUE self,VALUE val)
{
	if(NIL_P(val))
		_self->SetMask(false);
	else
	{
		wxColor c(wrap<wxColor>(val));
		_self->SetMaskColour(c.Red(),c.Green(),c.Blue());
	}
	return val;
}


DLL_LOCAL VALUE _save(int argc,VALUE *argv,VALUE self)
{
	VALUE name,mime;
	rb_scan_args(argc, argv, "11",&name,&mime);
	if(!_self->IsOk())
		return Qfalse;
	errno = 0;

	wxFileName dir(wxPathOnly(unwrap<wxString>(name)));
	dir.MakeAbsolute(wxGetCwd());

	wxFileName file(unwrap<wxString>(name));
	file.MakeAbsolute(wxGetCwd());
	if(dir.DirExists())
	{
		if(file.FileExists() && !file.IsFileWritable())
			errno = EACCES;
		else if(!dir.IsDirWritable())
			errno = EACCES;
	}else
		errno = ENOENT;

	if(errno)
	{
		rb_sys_fail(unwrap< char* >(name));
		return Qfalse;
	}

	bool result = false;

	if(NIL_P(mime)){
		result = _self->SaveFile(file.GetFullPath());
	}else if(SYMBOL_P(mime) || FIXNUM_P(mime)){
//		if(wxImage::FindHandler(unwrap<wxBitmapType>(mime)))
//			result = _self->SaveFile(file.GetFullPath(),unwrap<wxBitmapType>(mime));
//		else
//			rb_raise(rb_eArgError,"%s type not known",rb_id2name(SYM2ID(mime)));
	}else{
//	if(wxImage::FindHandlerMime(unwrap<wxString>(mime)))
		result = _self->SaveFile(file.GetFullPath(),unwrap<wxString>(mime));
//		else
//			rb_raise(rb_eArgError,"%s mime not known",unwrap<wxString>(mime).c_str().AsChar());
	}
	return wrap(result);
}


DLL_LOCAL VALUE _to_image(VALUE self)
{
	return self;
}
DLL_LOCAL VALUE _to_bitmap(VALUE self)
{
	return wrap(unwrap<wxBitmap*>(self));
}

}
}

#endif

DLL_LOCAL void Init_WXImage(VALUE rb_mWX)
{
#if wxUSE_IMAGE

	wxInitAllImageHandlers();

	using namespace RubyWX::Image;
	rb_cWXImage = rb_define_class_under(rb_mWX,"Image",rb_cObject);
	rb_define_alloc_func(rb_cWXImage,_alloc);


	rb_define_method(rb_cWXImage,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXImage,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);


	rb_define_method(rb_cWXImage,"height",RUBY_METHOD_FUNC(_getHeight),0);
	rb_define_method(rb_cWXImage,"width",RUBY_METHOD_FUNC(_getWidth),0);

	rb_define_method(rb_cWXImage,"to_image",RUBY_METHOD_FUNC(_to_image),0);
	rb_define_method(rb_cWXImage,"to_bitmap",RUBY_METHOD_FUNC(_to_bitmap),0);


	rb_define_method(rb_cWXImage,"*",RUBY_METHOD_FUNC(_mal),1);
	rb_define_method(rb_cWXImage,"scale",RUBY_METHOD_FUNC(_scale),2);

	rb_define_method(rb_cWXImage,"[]",RUBY_METHOD_FUNC(_get),-1);
	rb_define_method(rb_cWXImage,"[]=",RUBY_METHOD_FUNC(_set),-1);

	rb_define_attr_method(rb_cWXImage,"mask",_getMask,_setMask);

	rb_define_method(rb_cWXImage,"load",RUBY_METHOD_FUNC(_load),-1);
	rb_define_method(rb_cWXImage,"save",RUBY_METHOD_FUNC(_save),-1);

	registerInfo<wxImage>(rb_cWXImage);
#endif

}


