/*
 * wxFont.cpp
 *
 *  Created on: 28.02.2012
 *      Author: hanmac
 */

#include "wxFont.hpp"
#include "wxApp.hpp"
#include "wxSize.hpp"

#define _self unwrap<wxFont*>(self)

VALUE rb_cWXFont;

typedef std::map<wxFontEncoding,VALUE> encodingholdertype;
encodingholdertype encodingholder;


template <>
wxFont nullPtr<wxFont>(){ return wxNullFont;}

#ifdef HAVE_RUBY_ENCODING_H
template <>
VALUE wrapenum< wxFontEncoding >(const wxFontEncoding &enc )
{
	encodingholdertype::iterator it = encodingholder.find(enc);
	if(it != encodingholder.end())
		return it->second;
	return Qnil;
}

template <>
wxFontEncoding unwrapenum< wxFontEncoding >(const VALUE &venc)
{
	VALUE tmp = venc;
	if(rb_obj_is_kind_of(tmp,rb_cString))
	{
		tmp = rb_enc_from_encoding(rb_enc_find(unwrap<char*>(tmp)));
	}
	for(encodingholdertype::iterator it = encodingholder.begin();it != encodingholder.end();++it)
	{
		if(it->second == tmp)
			return it->first;
	}
	return wxFONTENCODING_DEFAULT;
}

void registerEnc(wxFontEncoding enc,const char* name)
{
	encodingholder[enc]=rb_enc_from_encoding(rb_enc_find(name));
}
#endif

namespace RubyWX {
namespace Font {

APP_PROTECT(wxFont)

macro_attr(PointSize,int)
macro_attr(PixelSize,wxSize)
macro_attr_enum(Family,wxFontFamily)
macro_attr_enum(Style,wxFontStyle)
macro_attr_enum(Weight,wxFontWeight)
#ifdef HAVE_RUBY_ENCODING_H
macro_attr_enum(Encoding,wxFontEncoding)
#endif
macro_attr(Underlined,bool)
macro_attr(Strikethrough,bool)

macro_attr(FaceName,wxString)

#define info_option(func,name,m) \
	if(!NIL_P(tmp = rb_hash_aref(hash,ID2SYM(rb_intern(#name)))))\
		val.func(m(tmp));

DLL_LOCAL wxFontInfo to_info(VALUE size,VALUE hash)
{
	wxFontInfo val;

	if(is_wrapable< wxSize >(size))
		val = wxFontInfo(unwrap<wxSize>(size));
	else
		val = wxFontInfo(NUM2INT(size));

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE tmp;
		info_option(Family,family,unwrapenum<wxFontFamily>)
		info_option(FaceName,face_name,unwrap<wxString>)

		info_option(Bold,bold,RTEST)
		info_option(Light,light,RTEST)

		info_option(Italic,italic,RTEST)
		info_option(Slant,slant,RTEST)

		info_option(AntiAliased,anti_aliased,RTEST)

		info_option(Underlined,underlined,RTEST)
		info_option(Strikethrough,strikethrough,RTEST)

#ifdef HAVE_RUBY_ENCODING_H
		info_option(Encoding,encoding,unwrapenum<wxFontEncoding>)
#endif

	}

	return val;

}

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{

	VALUE size,hash;
	rb_scan_args(argc, argv, "11",&size,&hash);
	(*_self) = wxFont(to_info(size,hash));

//	rb_call_super(argc,argv);
	return self;
}

singlereturn(Smaller)
singlereturn(Larger)

singlereturn(Bold)
singlereturn(Italic)

singlefunc(MakeSmaller)
singlefunc(MakeLarger)

singlefunc(MakeBold)
singlefunc(MakeItalic)

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
	VALUE ary = rb_ary_new();

	rb_ary_push(ary,_getPointSize(self));
	//rb_ary_push(ary,_getPixelSize(self));
	rb_ary_push(ary,_getFamily(self));
	rb_ary_push(ary,_getStyle(self));
	rb_ary_push(ary,_getWeight(self));

	rb_ary_push(ary,_getUnderlined(self));
	rb_ary_push(ary,_getStrikethrough(self));

	rb_ary_push(ary,_getFaceName(self));
#ifdef HAVE_RUBY_ENCODING_H
	rb_ary_push(ary,_getEncoding(self));
#endif
	return ary;
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
	VALUE *ptr = RARRAY_PTR(data);

	_self->Create(
		NUM2INT(ptr[0]),
		unwrapenum<wxFontFamily>(ptr[1]),
		unwrapenum<wxFontStyle>(ptr[2]),
		unwrapenum<wxFontWeight>(ptr[3]),
		RTEST(ptr[4]),
		unwrap<wxString>(ptr[6]),
#ifdef HAVE_RUBY_ENCODING_H
		unwrapenum<wxFontEncoding>(ptr[7])
#else
		wxFONTENCODING_DEFAULT
#endif
	);

	_setStrikethrough(self,ptr[5]);

	return self;
}


DLL_LOCAL VALUE _compare(VALUE self,VALUE other)
{
	return wrap((*_self) == unwrap<wxFont>(other));
}

DLL_LOCAL VALUE _to_s(VALUE self)
{
	return wrap(wxToString(*_self));
}

}
}

DLL_LOCAL void Init_WXFont(VALUE rb_mWX)
{
	using namespace RubyWX::Font;
	rb_cWXFont = rb_define_class_under(rb_mWX,"Font",rb_cObject);
	rb_define_alloc_func(rb_cWXFont,_alloc);

	rb_define_method(rb_cWXFont,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXFont,"smaller",RUBY_METHOD_FUNC(_Smaller),0);
	rb_define_method(rb_cWXFont,"larger",RUBY_METHOD_FUNC(_Larger),0);

	rb_define_method(rb_cWXFont,"to_bold",RUBY_METHOD_FUNC(_Bold),0);
	rb_define_method(rb_cWXFont,"to_italic",RUBY_METHOD_FUNC(_Italic),0);

	rb_define_method(rb_cWXFont,"smaller!",RUBY_METHOD_FUNC(_MakeSmaller),0);
	rb_define_method(rb_cWXFont,"larger!",RUBY_METHOD_FUNC(_MakeLarger),0);

	rb_define_method(rb_cWXFont,"to_bold!",RUBY_METHOD_FUNC(_MakeBold),0);
	rb_define_method(rb_cWXFont,"to_italic!",RUBY_METHOD_FUNC(_MakeItalic),0);


	rb_define_method(rb_cWXFont,"==",RUBY_METHOD_FUNC(_compare),1);

	rb_define_method(rb_cWXFont,"to_s",RUBY_METHOD_FUNC(_to_s),0);

	rb_define_attr_method(rb_cWXFont,"point_size",_getPointSize,_setPointSize);
	rb_define_attr_method(rb_cWXFont,"pixel_size",_getPixelSize,_setPixelSize);

	rb_define_attr_method(rb_cWXFont,"family",_getFamily,_setFamily);
	rb_define_attr_method(rb_cWXFont,"style",_getStyle,_setStyle);
	rb_define_attr_method(rb_cWXFont,"weight",_getWeight,_setWeight);
	rb_define_attr_method(rb_cWXFont,"face_name",_getFaceName,_setFaceName);
#ifdef HAVE_RUBY_ENCODING_H
	rb_define_attr_method(rb_cWXFont,"encoding",_getEncoding,_setEncoding);
#endif
	rb_define_method(rb_cWXFont,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXFont,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	registerInfo<wxFont>(rb_cWXFont);

	registerEnum<wxFontWeight>("WX::FontWeight",wxFONTWEIGHT_NORMAL)
		->add(wxFONTWEIGHT_NORMAL,"normal")
		->add(wxFONTWEIGHT_LIGHT,"light")
		->add(wxFONTWEIGHT_BOLD,"bold");
	registerEnum<wxFontStyle>("WX::FontStyle",wxFONTSTYLE_NORMAL)
		->add(wxFONTSTYLE_NORMAL,"normal")
		->add(wxFONTSTYLE_ITALIC,"italic")
		->add(wxFONTSTYLE_SLANT,"slant");
	registerEnum<wxFontFamily>("WX::FontFamily",wxFONTFAMILY_DEFAULT)
		->add(wxFONTFAMILY_DEFAULT,"default")
		->add(wxFONTFAMILY_DECORATIVE,"decorative")
		->add(wxFONTFAMILY_ROMAN,"roman")
		->add(wxFONTFAMILY_SCRIPT,"script")
		->add(wxFONTFAMILY_SWISS,"swiss")
		->add(wxFONTFAMILY_MODERN,"modern")
		->add(wxFONTFAMILY_TELETYPE,"teletype");

#ifdef HAVE_RUBY_ENCODING_H
	registerEnc(wxFONTENCODING_ISO8859_1,"ISO8859-1");
	registerEnc(wxFONTENCODING_ISO8859_2,"ISO8859-2");
	registerEnc(wxFONTENCODING_ISO8859_3,"ISO8859-3");
	registerEnc(wxFONTENCODING_ISO8859_4,"ISO8859-4");
	registerEnc(wxFONTENCODING_ISO8859_5,"ISO8859-5");
	registerEnc(wxFONTENCODING_ISO8859_6,"ISO8859-6");
	registerEnc(wxFONTENCODING_ISO8859_7,"ISO8859-7");
	registerEnc(wxFONTENCODING_ISO8859_8,"ISO8859-8");
	registerEnc(wxFONTENCODING_ISO8859_9,"ISO8859-9");
	registerEnc(wxFONTENCODING_ISO8859_10,"ISO8859-10");
	registerEnc(wxFONTENCODING_ISO8859_11,"ISO8859-11");
	registerEnc(wxFONTENCODING_ISO8859_12,"ISO8859-12");
	registerEnc(wxFONTENCODING_ISO8859_13,"ISO8859-13");
	registerEnc(wxFONTENCODING_ISO8859_14,"ISO8859-14");
	registerEnc(wxFONTENCODING_ISO8859_15,"ISO8859-15");

	registerEnc(wxFONTENCODING_CP437,"CP437");
	registerEnc(wxFONTENCODING_CP850,"CP850");
	registerEnc(wxFONTENCODING_CP855,"CP855");
	registerEnc(wxFONTENCODING_CP866,"CP866");

	registerEnc(wxFONTENCODING_CP874,"CP874");
	registerEnc(wxFONTENCODING_CP932,"CP932");
	registerEnc(wxFONTENCODING_CP936,"CP936");
	registerEnc(wxFONTENCODING_CP949,"CP949");
	registerEnc(wxFONTENCODING_CP950,"CP945");
	registerEnc(wxFONTENCODING_CP1250,"CP1250");
	registerEnc(wxFONTENCODING_CP1251,"CP1251");
	registerEnc(wxFONTENCODING_CP1252,"CP1252");
	registerEnc(wxFONTENCODING_CP1253,"CP1253");
	registerEnc(wxFONTENCODING_CP1254,"CP1254");
	registerEnc(wxFONTENCODING_CP1255,"CP1255");
	registerEnc(wxFONTENCODING_CP1256,"CP1256");
	registerEnc(wxFONTENCODING_CP1257,"CP1257");
	registerEnc(wxFONTENCODING_CP1258,"CP1258");
	registerEnc(wxFONTENCODING_CP1361,"CP1361");

	registerEnc(wxFONTENCODING_UTF7,"UTF-7");
	registerEnc(wxFONTENCODING_UTF8,"UTF-8");
	registerEnc(wxFONTENCODING_EUC_JP,"EUC-JP");

	registerEnc(wxFONTENCODING_UTF16LE,"UTF-16LE");
	registerEnc(wxFONTENCODING_UTF16BE,"UTF-16BE");
	registerEnc(wxFONTENCODING_UTF32LE,"UTF-32LE");
	registerEnc(wxFONTENCODING_UTF32BE,"UTF-32BE");
#endif
}
