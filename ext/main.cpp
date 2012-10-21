/*
 * main.cpp
 *
 *  Created on: 04.05.2012
 *      Author: hanmac
 */

#include "main.hpp"

infoholdertype infoklassholder;
typeholdertype typeklassholder;

VALUE wrapClass(const wxClassInfo * info)
{
	infoholdertype::iterator it = infoklassholder.find(info);
	if(it != infoklassholder.end())
		return it->second;
	if(const wxClassInfo *base = info->GetBaseClass1())
		return wrapClass(base);
	return Qnil;
}

VALUE wrapPtr(void *arg,VALUE klass)
{
	if(arg)
		return Data_Wrap_Struct(klass, 0, 0, arg);
	return Qnil;
}


template <>
bool unwrap< bool >(const VALUE &val )
{
	return RTEST(val);
}

template <>
VALUE wrap< bool >(const bool &st )
{
	return st ? Qtrue : Qfalse;
}

template <>
int unwrap< int >(const VALUE &val )
{
	return NUM2INT(val);
}

template <>
VALUE wrap< int >(const int &st )
{
	return INT2NUM(st);
}

template <>
unsigned int unwrap< unsigned int >(const VALUE &val )
{
	return NUM2UINT(val);
}

template <>
VALUE wrap< unsigned int >(const unsigned int &st )
{
	return UINT2NUM(st);
}


template <>
VALUE wrap< wxString >(const wxString &st )
{
#ifdef HAVE_RUBY_ENCODING_H
	return rb_enc_str_new(st.c_str(),strlen(st.c_str()),rb_utf8_encoding());
#else
	return rb_str_new2(st.c_str());
#endif
}
template <>
VALUE wrap< wxChar >(const wxChar &c )
{
	return wrap(wxString(c));
}

template <>
char* unwrap< char* >(const VALUE &val )
{
	if(NIL_P(val))
		return NULL;
	if (rb_obj_is_kind_of(val, rb_cString)){
		return RSTRING_PTR(val);
	}
	else
		return unwrap< char* >(rb_funcall(val,rb_intern("to_s"),0));
}

template <>
wxString unwrap< wxString >(const VALUE &val )
{
	if(NIL_P(val))
		return wxEmptyString;
	else
		return wxString(unwrap< char* >(val));
}
template <>
VALUE wrap< wxArrayString >(const wxArrayString &st )
{
	VALUE ary = rb_ary_new();
	for(wxArrayString::const_iterator it = st.begin(); it != st.end(); ++it)
		rb_ary_push(ary,wrap(*it));
	return ary;
}

template <>
VALUE wrap< wxArrayInt >(const wxArrayInt &st )
{
	VALUE ary = rb_ary_new();
	for(wxArrayInt::const_iterator it = st.begin(); it != st.end(); ++it)
		rb_ary_push(ary,wrap(*it));
	return ary;
}

template <>
wxArrayString unwrap< wxArrayString >(const VALUE &val )
{
	wxArrayString arry;
	if(NIL_P(val))
		return arry;
	else if(rb_respond_to(val,rb_intern("to_a")))	{
		VALUE dp = rb_funcall(val,rb_intern("to_a"),0);
		size_t length = RARRAY_LEN(dp);
		for(size_t i = 0; i < length; ++i)
			arry.Add(unwrap<wxString>(RARRAY_PTR(dp)[i]));
	}else{
		arry.Add(unwrap<wxString>(val));
	}
	return arry;
}

template <>
VALUE wrap< wxDateTime >(const wxDateTime &st )
{
	return rb_time_nano_new(st.GetTicks(),st.GetMillisecond()*1000);
}

template <>
wxDateTime unwrap< wxDateTime >(const VALUE &val )
{
	wxDateTime result;
	result.SetToCurrent();
	result.MakeTimezone(
		NUM2UINT(rb_funcall(val,rb_intern("gmt_offset"),0)) - 3600,
		RTEST(rb_funcall(val,rb_intern("dst?"),0))
	);

	result.Set(
		NUM2UINT(rb_funcall(val,rb_intern("day"),0)),
		(wxDateTime::Month)(NUM2UINT(rb_funcall(val,rb_intern("month"),0))-1),
		NUM2UINT(rb_funcall(val,rb_intern("year"),0)),
		NUM2UINT(rb_funcall(val,rb_intern("hour"),0)),
		NUM2UINT(rb_funcall(val,rb_intern("min"),0)),
		NUM2UINT(rb_funcall(val,rb_intern("sec"),0)),
		NUM2UINT(rb_funcall(val,rb_intern("usec"),0)) / 1000
	);
	return result;
}


