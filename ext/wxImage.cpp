/*
 * wxImage.cpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"
#include "wxSize.hpp"
#include "wxRect.hpp"
#include "wxPoint.hpp"

#include "wxImage.hpp"
#include "wxBitmap.hpp"
#include "wxDC.hpp"
#include "wxColor.hpp"

#include "wxStream.hpp"

#if wxUSE_PALETTE
#include "wxPalette.hpp"
#endif

#if wxUSE_IMAGE

#define _self unwrap<wxImage*>(self)

VALUE rb_cWXImage;

template <>
wxImage* unwrap< wxImage* >(const VALUE &vimage)
{
	if(rb_obj_is_kind_of(vimage,rb_cWXImage))
		return unwrapTypedPtr<wxImage>(vimage, rb_cWXImage);
	if(is_wrapable<wxSize>(vimage))
		return new wxImage(unwrap<wxSize>(vimage));
	return unwrapTypedPtr<wxImage>(rb_class_new_instance(1,const_cast<VALUE*>(&vimage),rb_cWXImage), rb_cWXImage);
}


template <>
wxImage unwrap< wxImage >(const VALUE &vimage)
{
	return *unwrap<wxImage*>(vimage);
}


namespace RubyWX {
namespace Image {

#if wxUSE_PALETTE
macro_attr(Palette,wxPalette)
#endif

DLL_LOCAL VALUE _load(int argc,VALUE *argv,VALUE self)
{
	VALUE name,mime,nr;
	rb_scan_args(argc, argv, "12",&name,&mime,&nr);

	bool result;

	if(NIL_P(nr))
		nr = INT2NUM(-1);

#if wxUSE_STREAMS
	if(!rb_respond_to(name,rb_intern("read")))
	{
#endif
		if(!check_file_loadable(unwrap<wxString>(name)))
			return Qfalse;

		wxFileName file(unwrap<wxString>(name));
		file.MakeAbsolute(wxGetCwd());

		if(NIL_P(mime)){
			result = _self->LoadFile(file.GetFullPath(), wxBITMAP_TYPE_ANY, NUM2INT(nr));
		}else if(SYMBOL_P(mime) || FIXNUM_P(mime)){
//			result = _self->LoadFile(file.GetFullPath(),unwrap<wxBitmapType>(mime),NUM2INT(nr));
		}else
			result = _self->LoadFile(file.GetFullPath(),unwrap<wxString>(mime),NUM2INT(nr));
#if wxUSE_STREAMS
	}else{
		RubyInputStream st(name);

		if(NIL_P(mime)){
			result = _self->LoadFile(st, wxBITMAP_TYPE_ANY, NUM2INT(nr));
		}else if(SYMBOL_P(mime) || FIXNUM_P(mime)){
//			result = _self->LoadFile(st,unwrap<wxBitmapType>(mime),NUM2INT(nr));
		}else
			result = _self->LoadFile(st,unwrap<wxString>(mime),NUM2INT(nr));
	}
#endif
	return wrap(result);
}

DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrapTypedPtr(new wxImage, self);
}


/*
 * call-seq:
 *   Image.new(path,[type,[id]])
 *   Image.new(width,height)
 *
 * creates a new Image Object.
 * === Exceptions
 * [ArgumentError]
 * * if height or width are negative or zero
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
	else {
		int cwidth = NUM2INT(width);
		int cheight = NUM2INT(height);

		if(check_negative_size(cwidth, cheight)) {
			_self->Create(cwidth, cheight);
		}
	}
	//_self->InitAlpha();
	return self;
}

singlereturn_if(GetHeight, _self->IsOk())
singlereturn_if(GetWidth, _self->IsOk())
singlereturn_if(GetSize, _self->IsOk())

DLL_LOCAL VALUE _getData(VALUE self)
{
	if(_self->IsOk())
	{
		//if(_self->GetType() == wxBITMAP_TYPE_INVALID)
		//	return Qnil;
		char* data = (char*)_self->GetData();
		if(data)
			return rb_str_new(data,_self->GetHeight() * _self->GetWidth() * 3);
	}
	return Qnil;
}

DLL_LOCAL VALUE _getAlpha(VALUE self)
{
	if(_self->IsOk() && _self->HasAlpha())
	{
		return rb_str_new((char*) _self->GetAlpha(),_self->GetHeight() * _self->GetWidth());
	}
	return Qnil;
}


bool check_inside(int x, int y, const wxSize& size)
{
	return x >= 0 && y >= 0 && x < size.GetWidth() && y < size.GetHeight();
}

/*
 * call-seq:
 *   image.sub_image(rect) -> WX::Image or nil
 *
 * return a sub image of the given place
 * ===Arguments
 * * rect is a WX::Rect
 * ===Return value
 * WX::Image or nil
 * === Exceptions
 * [ArgumentError]
 * * rect does have negative size
 * * rect does not fit into the Size of the Image
*/
DLL_LOCAL VALUE _getSubImage(VALUE self, VALUE vrect)
{
	if(_self->IsOk())
	{
		wxRect rect;
		wxSize size;

		if(!check_negative_size(vrect, size))
			return Qnil;

		if(!check_contain_rect(_GetSize(self), _self->GetSize(), vrect, rect))
			return Qnil;

		return wrap(_self->GetSubImage(rect));
	}
	return Qnil;
}

/*
 * call-seq:
 *   image[rect] -> WX::Image or nil
 *   image[x,y] -> WX::Color or nil
 *
 * if giving a WX::Rect, return a sub image of the given place
 * if giving x and y, return the color of the given position or nil when out of range
 * ===Arguments
 * * x and y are Integer
 * * rect is a WX::Rect
 * ===Return value
 * WX::Image, WX::Color or nil
 * === Exceptions
 * [ArgumentError]
 * * rect does have negative size
 * * rect does not fit into the Size of the Image
*/
DLL_LOCAL VALUE _get(int argc,VALUE *argv,VALUE self)
{
	VALUE vx,vy;
	rb_scan_args(argc, argv, "11",&vx,&vy);
	if(_self->IsOk())
	{
		if(NIL_P(vy))
			return _getSubImage(self, vy);

		int x,y;
		x = NUM2UINT(vx);
		y = NUM2UINT(vy);

		if(!check_inside(x,y, _self->GetSize()))
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

void set_at_pos(int x, int y, wxImage* self, VALUE val)
{
	if(!check_inside(x, y, self->GetSize()))
		return;

	if(rb_obj_is_kind_of(val,rb_cWXImage) || rb_obj_is_kind_of(val,rb_cWXBitmap))
	{
		self->Paste(unwrap<wxImage>(val),x,y);
	} else {
		wxColor c(unwrap<wxColor>(val));
		self->SetRGB(x,y,c.Red(),c.Green(),c.Blue());
		if(self->HasAlpha())
			self->SetAlpha(x,y,c.Alpha());
	}
}

/*
 * call-seq:
 *   image[x,y]= WX::Color or WX::Image or WX::Bitmap
 *   image[pos]= WX::Color or WX::Image or WX::Bitmap
 *   image[rect]= WX::Color
 *
 * if giving x and y or pos, and as value a color, sets the color at the given position
 * if giving x and y or pos, and an image or bitmap does paste it at the given position
 * if giving a WX::Rect, fill the color at the place with the given one
 * ===Arguments
 * * x and y are Integer
 * * pos is a WX::Point
 * * rect is a WX::Rect
 *
 * === Exceptions
 * [ArgumentError]
 * * rect does not fit into the Size of the Image
 */
DLL_LOCAL VALUE _set(int argc,VALUE *argv,VALUE self)
{
	VALUE vx,vy,value;
	rb_scan_args(argc, argv, "21",&vx,&vy,&value);

	rb_check_frozen(self);

	if(_self->IsOk())
	{
		if(NIL_P(value)) {
			if(is_wrapable<wxRect>(vx)) {
				wxColor c(unwrap<wxColor>(vy));
				wxSize size(_self->GetSize());
				wxRect vrect;

				if(check_contain_rect(_GetSize(self), size, vx, vrect))
				{
					_self->SetRGB(vrect,c.Red(),c.Green(),c.Blue());
					if(_self->HasAlpha()) {
						for(int i = vrect.x; i <= vrect.GetRight(); ++i)
							for(int j = vrect.y; j <= vrect.GetBottom(); ++j)
								if(check_inside(i, j, size))
									_self->SetAlpha(i,j,c.Alpha());
					}
				}
			} else {
				wxPoint vpoint(unwrap<wxPoint>(vx));
				set_at_pos(vpoint.x, vpoint.y, _self, vy);
			}
		} else {
			set_at_pos(NUM2UINT(vx), NUM2UINT(vy), _self, value);
		}
	}

	return NIL_P(value) ? value : vy;
}

/*
 * call-seq:
 *   image.resize(size, pos [, color]) -> WX::Image
 *   image.resize(rect [, color]) -> WX::Image
 *
 * returns a new resized image, if color is given use this one to fill new space
 * ===Arguments
 * * size WX::Size
 * * pos WX::Point
 * * rect WX::Rect
 * * color WX::Color
 * ===Return value
 * WX::Image
 *
 * === Exceptions
 * [ArgumentError]
 * * size is invalid
*/
DLL_LOCAL VALUE _resize(int argc,VALUE *argv,VALUE self)
{
	VALUE size, pos, color;
	rb_scan_args(argc, argv, "12",&size,&pos, &color);

	wxSize csize;
	if(!check_negative_size(size,csize))
		return Qnil;

	wxPoint cpos;

	if(is_wrapable<wxColor>(pos))
	{
		std::swap(color,pos);
	}

	if(NIL_P(pos) && is_wrapable<wxPoint>(size))
		cpos = unwrap<wxPoint>(size);
	else
		cpos = unwrap<wxPoint>(pos);

	if(NIL_P(color))
		return wrap(_self->Size(csize, cpos));
	else {
		wxColor c(unwrap<wxColor>(size));

		return wrap(_self->Size(csize, cpos, c.Red(), c.Green(), c.Blue()));
	}
}

/*
 * call-seq:
 *   image.resize!(size, pos [, color]) -> self
 *   image.resize!(rect [, color]) -> self
 *
 * returns this image resized, if color is given use this one to fill new space
 * ===Arguments
 * * size WX::Size
 * * pos WX::Point
 * * rect WX::Rect
 * * color WX::Color
 * ===Return value
 * self
 * === Exceptions
 * [ArgumentError]
 * * size is invalid
*/
DLL_LOCAL VALUE _resize_self(int argc,VALUE *argv,VALUE self)
{
	VALUE size, pos, color;
	rb_scan_args(argc, argv, "12",&size,&pos, &color);

	rb_check_frozen(self);

	wxSize csize;
	if(!check_negative_size(size,csize))
		return self;

	wxPoint cpos;

	if(is_wrapable<wxColor>(pos))
	{
		std::swap(color,pos);
	}

	if(NIL_P(pos) && is_wrapable<wxPoint>(size))
		cpos = unwrap<wxPoint>(size);
	else
		cpos = unwrap<wxPoint>(pos);

	if(NIL_P(color))
		_self->Resize(csize, cpos);
	else {
		wxColor c(unwrap<wxColor>(size));

		_self->Resize(csize, cpos, c.Red(), c.Green(), c.Blue());
	}
	return self;
}


/*
 * call-seq:
 *   image.scale(size) -> WX::Image
 *   image.scale(x_ratio, y_ratio) -> WX::Image
 *
 * returns a new scaled image
 * ===Arguments
 * * size WX::Size
 * * x_ratio and y_ratio are Float
 * ===Return value
 * WX::Image
 * === Exceptions
 * [ArgumentError]
 * * size is invalid or x_ratio or y_ratio is zero
*/
DLL_LOCAL VALUE _scale(int argc,VALUE *argv,VALUE self)
{
	VALUE x_scale, y_scale;
	rb_scan_args(argc, argv, "11",&x_scale,&y_scale);

	if(NIL_P(y_scale))
	{
		wxSize size;
		if(!check_negative_size(x_scale,size))
			return Qnil;

		return wrap(_self->Scale(size.GetWidth(),size.GetHeight()));
	}else {

		double x,y;
		x = NUM2DBL(x_scale);
		y = NUM2DBL(y_scale);

		if(x == 0 || y == 0)
		{
			if(!check_negative_size(x, y))
				return Qnil;
		}

		wxImage result(*_self);

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

		result.Rescale(result.GetWidth() * x,result.GetHeight() * y);
		return wrap(result);
	}
}


/*
 * call-seq:
 *   image.scale!(size) -> self
 *   image.scale!(x_ratio, y_ratio) -> self
 *
 * scaled this image, return self
 * ===Arguments
 * * size WX::Size
 * * x_ratio and y_ratio are Float
 * ===Return value
 * self
 * === Exceptions
 * [ArgumentError]
 * * size is invalid or x_ratio or y_ratio is zero
*/
DLL_LOCAL VALUE _scale_self(int argc,VALUE *argv,VALUE self)
{
	VALUE x_scale, y_scale;
	rb_scan_args(argc, argv, "11",&x_scale,&y_scale);

	rb_check_frozen(self);

	if(NIL_P(y_scale))
	{
		wxSize size;
		if(check_negative_size(x_scale,size))
			_self->Rescale(size.GetWidth(),size.GetHeight());
	}else {
		double x,y;
		x = NUM2DBL(x_scale);
		y = NUM2DBL(y_scale);

		if(x == 0 || y == 0)
		{
			if(!check_negative_size(x, y))
				return Qnil;
		}

		if(x < 0)
		{
			x *= -1;
			*_self = _self->Mirror(false);
		}
		if(y < 0)
		{
			y *= -1;
			*_self = _self->Mirror();
		}

		_self->Rescale(_self->GetWidth() * x,_self->GetHeight() * y);
	}
	return self;
}

/*
 * call-seq:
 *   image.replace_color(color1, color2) -> WX::Image
 *
 * replaces color1 with color2, return new image
 * ===Arguments
 * * color1 and color2 are WX::Color
 * ===Return value
 * WX::Image
*/
DLL_LOCAL VALUE _replace_color(VALUE self, VALUE color1, VALUE color2)
{
	wxColor c1(unwrap<wxColor>(color1));
	wxColor c2(unwrap<wxColor>(color2));
	wxImage result(*_self);
	result.Replace(c1.Red(),c1.Green(),c1.Blue(),c2.Red(),c2.Green(),c2.Blue());
	return wrap(result);
}


/*
 * call-seq:
 *   image.replace_color!(color1, color2) -> self
 *
 * replaces color1 with color2, return self
 * ===Arguments
 * * color1 and color2 are WX::Color
 * ===Return value
 * self
*/
DLL_LOCAL VALUE _replace_color_self(VALUE self, VALUE color1, VALUE color2)
{
	rb_check_frozen(self);
	wxColor c1(unwrap<wxColor>(color1));
	wxColor c2(unwrap<wxColor>(color2));
	_self->Replace(c1.Red(),c1.Green(),c1.Blue(),c2.Red(),c2.Green(),c2.Blue());
	return self;
}

bool range_check(double val, double min, double max)
{
	bool result = val >= min && val <= max;
	if(!result)
		rb_raise(rb_eRangeError,"%f is out of range (%f..%f)", val, min, max);
	return result;
}


/*
 * call-seq:
 *   image.rotate_hue(angle) -> WX::Image
 *
 * rotates the hue, where -1.0 is -360 degrees and 1.0 is 360 degrees, return new image
 * ===Arguments
 * * angle is Float
 * ===Return value
 * WX::Image
 * === Exceptions
 * [RangeError]
 * * angle is outside of -1..1
*/
DLL_LOCAL VALUE _rotate_hue(VALUE self, VALUE angle)
{
	double val = NUM2DBL(angle);
	if(range_check(val, -1.0, 1.0)){
		wxImage result(*_self);
		result.RotateHue(val);
		wrap(result);
	}
	return Qnil;
}

/*
 * call-seq:
 *   image.rotate_hue!(angle) -> self
 *
 * rotates the hue, where -1.0 is -360 degrees and 1.0 is 360 degrees, return self
 * ===Arguments
 * * angle is Float
 * ===Return value
 * self
 * === Exceptions
 * [RangeError]
 * * angle is outside of -1..1
*/
DLL_LOCAL VALUE _rotate_hue_self(VALUE self, VALUE angle)
{
	rb_check_frozen(self);
	double val = NUM2DBL(angle);
	if(range_check(val, -1.0, 1.0))
		_self->RotateHue(val);

	return self;
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
	rb_check_frozen(self);
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

	if(!check_file_saveable(unwrap<wxString>(name)))
		return Qfalse;

	wxFileName file(unwrap<wxString>(name));
	file.MakeAbsolute(wxGetCwd());

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

/*
 * call-seq:
 *   image.blur(radius) -> WX::Image
 *
 * blur the image according to the specified pixel radius
 * ===Arguments
 * * radius Integer
 * ===Return value
 * WX::Image
*/
DLL_LOCAL VALUE _Blur(VALUE self, VALUE radius)
{
	return wrap(_self->Blur(NUM2INT(radius)));
}

/*
 * call-seq:
 *   image.blur!(radius) -> self
 *
 * blur the image according to the specified pixel radius
 * ===Arguments
 * * radius Integer
 * ===Return value
 * self
*/
DLL_LOCAL VALUE _Blur_self(VALUE self, VALUE radius)
{
	rb_check_frozen(self);
	_self->Paste(_self->Blur(NUM2INT(radius)), 0, 0);
	return self;
}

/*
 * call-seq:
 *   image.blur_horizontal(radius) -> WX::Image
 *
 * blur the image according to the specified pixel radius
 * ===Arguments
 * * radius Integer
 * ===Return value
 * WX::Image
*/
DLL_LOCAL VALUE _BlurHorizontal(VALUE self, VALUE radius)
{
	return wrap(_self->BlurHorizontal(NUM2INT(radius)));
}

/*
 * call-seq:
 *   image.blur_horizontal!(radius) -> self
 *
 * blur the image according to the specified pixel radius
 * ===Arguments
 * * radius Integer
 * ===Return value
 * self
*/
DLL_LOCAL VALUE _BlurHorizontal_self(VALUE self, VALUE radius)
{
	rb_check_frozen(self);
	_self->Paste(_self->BlurHorizontal(NUM2INT(radius)), 0, 0);
	return self;
}

/*
 * call-seq:
 *   image.blur_vertical(radius) -> WX::Image
 *
 * blur the image according to the specified pixel radius
 * ===Arguments
 * * radius Integer
 * ===Return value
 * WX::Image
*/
DLL_LOCAL VALUE _BlurVertical(VALUE self, VALUE radius)
{
	return wrap(_self->BlurVertical(NUM2INT(radius)));
}

/*
 * call-seq:
 *   image.blur_vertical(radius) -> WX::Image
 *
 * blur the image according to the specified pixel radius
 * ===Arguments
 * * radius Integer
 * ===Return value
 * WX::Image
*/
DLL_LOCAL VALUE _BlurVertical_self(VALUE self, VALUE radius)
{
	rb_check_frozen(self);
	_self->Paste(_self->BlurVertical(NUM2INT(radius)), 0, 0);
	return self;
}

/*
 * call-seq:
 *   image.convert_to_disabled([brightness = 255]) -> WX::Image
 *
 * Convert to disabled (dimmed) image.
 * ===Arguments
 * * brightness Fixnum
 * ===Return value
 * WX::Image
*/
DLL_LOCAL VALUE _ConvertToDisabled(int argc,VALUE *argv,VALUE self)
{
	VALUE brightness;
	rb_scan_args(argc, argv, "01",&brightness);
	return wrap(_self->ConvertToDisabled(NUM2CHR(brightness)));
}

/*
 * call-seq:
 *   image.convert_to_disabled([brightness = 255]) -> WX::Image
 *
 * Convert to disabled (dimmed) image.
 * ===Arguments
 * * brightness Fixnum
 * ===Return value
 * WX::Image
*/
DLL_LOCAL VALUE _ConvertToDisabled_self(int argc,VALUE *argv,VALUE self)
{
	VALUE brightness;
	rb_scan_args(argc, argv, "01",&brightness);
	rb_check_frozen(self);
	_self->Paste(_self->ConvertToDisabled(NUM2CHR(brightness)), 0, 0);
	return self;
}

/*
 * call-seq:
 *   image.convert_to_mono(color) -> WX::Image
 *
 * returns a new image where color is turned white
 * and all other colors are turned black.
 * ===Arguments
 * * color WX::Color
 * ===Return value
 * WX::Image
*/
DLL_LOCAL VALUE _ConvertToMono(VALUE self, VALUE color)
{
	wxColor c(unwrap<wxColor>(color));
	return wrap(_self->ConvertToMono(c.Red(), c.Green(), c.Blue()));
}

/*
 * call-seq:
 *   image.convert_to_mono!(color) -> self
 *
 * returns a new image where color is turned white
 * and all other colors are turned black.
 * ===Arguments
 * * color WX::Color
 * ===Return value
 * self
*/
DLL_LOCAL VALUE _ConvertToMono_self(VALUE self, VALUE color)
{
	rb_check_frozen(self);
	wxColor c(unwrap<wxColor>(color));
	_self->Paste(_self->ConvertToMono(c.Red(), c.Green(), c.Blue()), 0, 0);
	return self;
}
/*
 * call-seq:
 *   draw { | dc | } -> self
 *
 * create a DC for drawing in the image.
 *
 */
DLL_LOCAL VALUE _draw(VALUE self)
{
	app_protected();
	rb_check_frozen(self);

	wxDC *dc;

#if wxUSE_GRAPHICS_CONTEXT
	dc = new wxGCDC(wxGraphicsContext::Create(*_self));
#else
	wxBitmap bit(*_self);
	wxMemoryDC *mdc = new wxMemoryDC(bit);
	dc = mdc;
#endif
	rb_yield(wrap(dc));

#if !wxUSE_GRAPHICS_CONTEXT
	(*_self) = bit.ConvertToImage();
	mdc->SelectObject(wxNullBitmap);
#endif
	//TODO add a way to delete the DCs again
	return self;
}


/*
 * call-seq:
 *   to_image -> WX::Image
 *
 * return self
 *
 */
DLL_LOCAL VALUE _to_image(VALUE self)
{
	return self;
}

/*
 * call-seq:
 *   to_bitmap -> WX::Bitmap
 *
 * convert to WX::Bitmap
 *
 * ===Arguments
 * * opts: Hash with possible options to set:
 *   * depth Integer
 *   * scale Float
 * === Exceptions
 * [ArgumentError]
 * * if scale is negative or zero
 */
DLL_LOCAL VALUE _to_bitmap(int argc,VALUE *argv,VALUE self)
{
	VALUE opts;
	rb_scan_args(argc, argv, "0:",&opts);

	int cdepth(wxBITMAP_SCREEN_DEPTH);
	double cscale(1.0);

	if(rb_obj_is_kind_of(opts, rb_cHash)) {
		set_hash_option(opts, "depth", cdepth);
		if(set_hash_option(opts, "scale", cscale)) {
			if(cscale <= 0.0) {
				rb_raise(rb_eArgError, "scale cant be negative or zero");
				return Qnil;
			}
		}
	}

	return wrap(new wxBitmap(*_self, cdepth, cscale));
}

/* Document-method: initialize_copy
 * call-seq:
 *   initialize_copy(orig)
 *
 * Duplicate an object
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	wxImage* cother = unwrap<wxImage*>(other);

	_self->SetData(cother->GetData(),cother->GetWidth(),cother->GetHeight());
	if(cother->HasAlpha())
		_self->SetAlpha(cother->GetAlpha());

	_setMask(self,_getMask(other));

#if wxUSE_PALETTE
	_setPalette(self,_getPalette(other));
#endif
	return result;
}

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

	h = rb_hash_uint(h, _self->GetWidth());
	h = rb_hash_uint(h, _self->GetHeight());

	if(_self->IsOk())
	{
		//if(_self->GetType() == wxBITMAP_TYPE_INVALID)
		//	return Qnil;
		char* data = (char*)_self->GetData();
		if(data)
			h = rb_hash_uint(h, rb_str_hash(rb_str_new(data,_self->GetHeight() * _self->GetWidth() * 3)));

		if(_self->HasAlpha())
		{
			h = rb_hash_uint(h, rb_str_hash(rb_str_new((char*) _self->GetAlpha(),_self->GetHeight() * _self->GetWidth())));
		}

		unsigned char r,g,b;
		if(_self->GetOrFindMaskColour(&r,&g,&b))
		{
			h = rb_hash_uint(h, r);
			h = rb_hash_uint(h, g);
			h = rb_hash_uint(h, b);
		}
	}

	h = rb_hash_end(h);
	return LONG2FIX(h);
}

bool check_equal(const wxImage &self, const wxImage &cother)
{
	if(self.GetHeight() != cother.GetHeight()){
		return false;
	}
	if(self.GetWidth() != cother.GetWidth()) {
		return false;
	}
	if(strcmp((char*)self.GetData(), (char*)cother.GetData()) != 0){
		return false;
	}
	if(self.HasAlpha()) {
		if(!cother.HasAlpha())
			return false;
		if(strcmp((char*)self.GetAlpha(), (char*)cother.GetAlpha()) != 0){
			return false;
		}
	} else {
		return !cother.HasAlpha();
	}

	return true;
}

struct equal_obj {
	wxImage* self;
	VALUE other;
};

VALUE _equal_block(equal_obj *obj)
{
	return wrap(check_equal(*obj->self, unwrap<wxImage>(obj->other)));
}

/*
 * call-seq:
 *   == image -> bool
 *
 * compares two image.
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
	VALUE result = rb_ary_new();

	rb_ary_push(result,_GetWidth(self));
	rb_ary_push(result,_GetHeight(self));
	rb_ary_push(result,_getData(self));
	rb_ary_push(result,_getAlpha(self));
	rb_ary_push(result,_getMask(self));
#if wxUSE_PALETTE
	rb_ary_push(result,_getPalette(self));
#else
	rb_ary_push(result,Qnil);
#endif
	return result;
}

/*
 * call-seq:
 *   marshal_load(array) -> self
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self,VALUE data)
{
	data = rb_Array(data);

	VALUE tmp = RARRAY_AREF(data,3);
	unsigned char* alpha = NULL;
	if(!NIL_P(tmp))
		alpha = (unsigned char*)StringValuePtr(tmp);

	VALUE val = RARRAY_AREF(data,2);
	_self->Create(
		NUM2UINT(RARRAY_AREF(data,0)),NUM2UINT(RARRAY_AREF(data,1)),
		(unsigned char*)StringValuePtr(val),alpha);

	_setMask(self,RARRAY_AREF(data,4));

#if wxUSE_PALETTE
	_setPalette(self,RARRAY_AREF(data,5));
#endif

	return self;
}

}
}

#endif


/* Document-attr: width
 * returns the width of the Image. Integer
 */
/* Document-attr: height
 * returns the height of the Image. Integer
 */
/* Document-attr: size
 * returns the size of the Image. WX::Size
 */

/* Document-attr: data
 * returns the data of the Image. String
 */
/* Document-attr: alpha
 * returns the alpha of the Image. String
 */

/* Document-attr: mask
 * returns the mask color of the Image. WX::Color
 */
/* Document-attr: palette
 * returns the color palette of the Image. WX::Palette
 */

DLL_LOCAL void Init_WXImage(VALUE rb_mWX)
{
#if wxUSE_IMAGE

	wxInitAllImageHandlers();

	using namespace RubyWX::Image;
	rb_cWXImage = rb_define_class_under(rb_mWX,"Image",rb_cObject);
	rb_define_alloc_func(rb_cWXImage,_alloc);

#if 0
	rb_define_attr(rb_cWXImage,"width",1,0);
	rb_define_attr(rb_cWXImage,"height",1,0);
	rb_define_attr(rb_cWXImage,"size",1,0);
	rb_define_attr(rb_cWXImage,"data",1,0);
	rb_define_attr(rb_cWXImage,"alpha",1,0);

	rb_define_attr(rb_cWXImage,"mask",1,1);
	rb_define_attr(rb_cWXImage,"palette",1,1);
#endif

	rb_define_method(rb_cWXImage,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXImage,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXImage,"height",_GetHeight,NULL);
	rb_define_attr_method(rb_cWXImage,"width",_GetWidth,NULL);
	rb_define_attr_method(rb_cWXImage,"size",_GetSize,NULL);
	rb_define_attr_method(rb_cWXImage,"data",_getData,NULL);
	rb_define_attr_method(rb_cWXImage,"alpha",_getAlpha,NULL);

	rb_define_method(rb_cWXImage,"draw",RUBY_METHOD_FUNC(_draw),0);

	rb_define_method(rb_cWXImage,"to_image",RUBY_METHOD_FUNC(_to_image),0);
	rb_define_method(rb_cWXImage,"sub_image",RUBY_METHOD_FUNC(_getSubImage),1);
	rb_define_method(rb_cWXImage,"to_bitmap",RUBY_METHOD_FUNC(_to_bitmap),-1);

	rb_define_method(rb_cWXImage,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXImage,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	rb_define_method(rb_cWXImage,"hash",RUBY_METHOD_FUNC(_getHash),0);

	rb_define_method(rb_cWXImage,"==",RUBY_METHOD_FUNC(_equal),1);
	rb_define_alias(rb_cWXImage,"eql?","==");

	rb_define_attr_method(rb_cWXImage,"mask",_getMask,_setMask);
#if wxUSE_PALETTE
	rb_define_attr_method(rb_cWXImage,"palette",_getPalette,_setPalette);
#endif

	rb_define_method(rb_cWXImage,"*",RUBY_METHOD_FUNC(_mal),1);

	rb_define_method(rb_cWXImage,"scale",RUBY_METHOD_FUNC(_scale),-1);
	rb_define_method(rb_cWXImage,"scale!",RUBY_METHOD_FUNC(_scale_self),-1);

	rb_define_method(rb_cWXImage,"resize",RUBY_METHOD_FUNC(_resize),-1);
	rb_define_method(rb_cWXImage,"resize!",RUBY_METHOD_FUNC(_resize_self),-1);

	rb_define_method(rb_cWXImage,"replace_color",RUBY_METHOD_FUNC(_replace_color),2);
	rb_define_method(rb_cWXImage,"replace_color!",RUBY_METHOD_FUNC(_replace_color_self),2);

	rb_define_method(rb_cWXImage,"rotate_hue",RUBY_METHOD_FUNC(_rotate_hue),1);
	rb_define_method(rb_cWXImage,"rotate_hue!",RUBY_METHOD_FUNC(_rotate_hue_self),1);

	rb_define_method(rb_cWXImage,"blur",RUBY_METHOD_FUNC(_Blur),1);
	rb_define_method(rb_cWXImage,"blur_horizontal",RUBY_METHOD_FUNC(_BlurHorizontal),1);
	rb_define_method(rb_cWXImage,"blur_vertical",RUBY_METHOD_FUNC(_BlurVertical),1);

	rb_define_method(rb_cWXImage,"blur!",RUBY_METHOD_FUNC(_Blur_self),1);
	rb_define_method(rb_cWXImage,"blur_horizontal!",RUBY_METHOD_FUNC(_BlurHorizontal_self),1);
	rb_define_method(rb_cWXImage,"blur_vertical!",RUBY_METHOD_FUNC(_BlurVertical_self),1);

	rb_define_method(rb_cWXImage,"convert_to_disabled",RUBY_METHOD_FUNC(_ConvertToDisabled),-1);
	rb_define_method(rb_cWXImage,"convert_to_mono",RUBY_METHOD_FUNC(_ConvertToMono),1);

	rb_define_method(rb_cWXImage,"convert_to_disabled!",RUBY_METHOD_FUNC(_ConvertToDisabled_self),-1);
	rb_define_method(rb_cWXImage,"convert_to_mono!",RUBY_METHOD_FUNC(_ConvertToMono_self),1);

	rb_define_method(rb_cWXImage,"[]",RUBY_METHOD_FUNC(_get),-1);
	rb_define_method(rb_cWXImage,"[]=",RUBY_METHOD_FUNC(_set),-1);

	rb_define_method(rb_cWXImage,"load",RUBY_METHOD_FUNC(_load),-1);
	rb_define_method(rb_cWXImage,"save",RUBY_METHOD_FUNC(_save),-1);

	registerInfo<wxImage>(rb_cWXImage, true);
#endif

}


