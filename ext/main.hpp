/*
 * main.hpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <cerrno>

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif




#include <wx/wx.h>
#include <ruby.h>

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#endif

#if wxUSE_XRC
#include <wx/xrc/xmlres.h>
#endif

template <typename T>
VALUE wrap(T *arg);

/*
template <typename T>
*/
inline VALUE wrap(void *arg,VALUE klass)
{
	return Data_Wrap_Struct(klass, 0, 0, arg);
}
//*/

template <typename T>
T wrap(const VALUE &arg);

template <typename T>
VALUE wrap(const T &arg){
	return wrap(new T(arg));
};

template <typename T>
bool is_wrapable(const VALUE &arg);


template <>
inline bool wrap< bool >(const VALUE &val )
{
	return RTEST(val);
}

template <>
inline VALUE wrap< bool >(const bool &st )
{
	return st ? Qtrue : Qfalse;
}

template <>
inline int wrap< int >(const VALUE &val )
{
	return NUM2INT(val);
}

template <>
inline VALUE wrap< int >(const int &st )
{
	return INT2NUM(st);
}


template <>
inline VALUE wrap< wxString >(const wxString &st )
{
#ifdef HAVE_RUBY_ENCODING_H
	return rb_enc_str_new(st.c_str(),strlen(st.c_str()),rb_utf8_encoding());
#else
	return rb_str_new2(st.c_str());
#endif
}
template <>
inline VALUE wrap< wxChar >(const wxChar &c )
{
	return wrap(wxString(c));
}

template <>
inline char* wrap< char* >(const VALUE &val )
{
	if(NIL_P(val))
		return NULL;
	if (rb_obj_is_kind_of(val, rb_cString)){
		return RSTRING_PTR(val);
	}
	else
		return wrap< char* >(rb_funcall(val,rb_intern("to_s"),0));
}

template <>
inline wxString wrap< wxString >(const VALUE &val )
{
	if(NIL_P(val))
		return wxEmptyString;
	else
		return wxString(wrap< char* >(val));
}
template <>
inline VALUE wrap< wxArrayString >(const wxArrayString &st )
{
	VALUE ary = rb_ary_new();
	for(wxArrayString::const_iterator it = st.begin(); it != st.end(); ++it)
		rb_ary_push(ary,wrap(*it));
	return ary;
}

template <>
inline VALUE wrap< wxArrayInt >(const wxArrayInt &st )
{
	VALUE ary = rb_ary_new();
	for(wxArrayInt::const_iterator it = st.begin(); it != st.end(); ++it)
		rb_ary_push(ary,wrap(*it));
	return ary;
}

template <>
inline wxArrayString wrap< wxArrayString >(const VALUE &val )
{
	wxArrayString arry;
	if(NIL_P(val))
		return arry;
	else if(rb_respond_to(val,rb_intern("to_a")))	{
		VALUE dp = rb_funcall(val,rb_intern("to_a"),0);
		size_t length = RARRAY_LEN(dp);
		for(size_t i = 0; i < length; ++i)
			arry.Add(wrap<wxString>(RARRAY_PTR(dp)[i]));
	}else{
		arry.Add(wrap<wxString>(val));
	}
	return arry;
}

template <>
inline VALUE wrap< wxDateTime >(const wxDateTime &st )
{
	return rb_time_nano_new(st.GetTicks(),st.GetMillisecond()*1000);
}

template <>
inline wxDateTime wrap< wxDateTime >(const VALUE &val )
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

#define macro_attr_with_func(attr,get,set) \
DLL_LOCAL VALUE _get##attr(VALUE self)\
{return get(_self->Get##attr());}\
\
DLL_LOCAL VALUE _set##attr(VALUE self,VALUE other)\
{\
	_self->Set##attr(set(other));\
	return other;\
}

#define macro_attr_pre_with_func(attr,pre,get,set) \
DLL_LOCAL VALUE _get##attr(VALUE self)\
{return get(_self->pre().Get##attr());}\
\
DLL_LOCAL VALUE _set##attr(VALUE self,VALUE other)\
{\
	_self->pre().Set##attr(set(other));\
	return other;\
}


#define macro_attr(attr,type) macro_attr_with_func(attr,wrap,wrap<type>)
#define macro_attr_pre(attr,pre,type) macro_attr_pre_with_func(attr,pre,wrap,wrap<type>)

template <typename T>
T* unwrapPtr(const VALUE &obj,const VALUE &klass)
{
	if(NIL_P(obj))
		return NULL;
	if (rb_obj_is_kind_of(obj, klass)){
		T *temp;
		Data_Get_Struct( obj, T, temp);
		return temp;
	}else{
		rb_raise(rb_eTypeError,
				"Expected %s got %s!",
				rb_class2name(klass),
				rb_obj_classname(obj)
		);
		return NULL;
	}

}

void rb_define_attr_method(VALUE klass,std::string name,VALUE(get)(VALUE),VALUE(set)(VALUE,VALUE));

VALUE wrap(wxEvtHandler *handler,VALUE klass);
VALUE wrap(wxClientDataContainer *sizer,VALUE klass);
VALUE wrap(wxSizer *sizer,VALUE klass);

VALUE wrapVariant(const wxVariant& var);
wxVariant unwrapVariant(VALUE obj,const wxString &type);

#define singlefunc(func) \
DLL_LOCAL VALUE _##func(VALUE self)\
{\
	_self->func();\
	return self;\
}


#define singlereturn(func) \
DLL_LOCAL VALUE _##func(VALUE self)\
{\
	return wrap(_self->func());\
}

#endif /* MAIN_HPP_ */
