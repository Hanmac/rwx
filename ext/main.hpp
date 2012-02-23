/*
 * main.hpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <cerrno>

#include <wx/wx.h>
#include <ruby.h>

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#endif


template <typename T>
VALUE wrap(T *arg);

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
inline VALUE wrap< wxString >(const wxString &st )
{
#ifdef HAVE_RUBY_ENCODING_H
	return rb_enc_str_new(st.c_str(),st.capacity(),rb_utf8_encoding());
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



#define macro_attr(attr,type) \
VALUE _get##attr(VALUE self)\
{return wrap(_self->Get##attr());}\
\
VALUE _set##attr(VALUE self,VALUE other)\
{\
	_self->Set##attr(wrap<type>(other));\
	return other;\
}


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

inline void rb_define_attr_method(VALUE klass,std::string name,VALUE(get)(VALUE),VALUE(set)(VALUE,VALUE))
{
	rb_define_method(klass,name.c_str(),RUBY_METHOD_FUNC(get),0);
	rb_define_method(klass,(name + "=").c_str(),RUBY_METHOD_FUNC(set),1);
}




#define singlefunc(func) \
VALUE _##func(VALUE self)\
{\
	_self->func();\
	return self;\
}


#define singlereturn(func) \
VALUE _##func(VALUE self)\
{\
	return wrap(_self->func());\
}

#endif /* MAIN_HPP_ */
