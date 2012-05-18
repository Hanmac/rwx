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

#include <typeinfo>
#include <map>
#include <string>

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#endif

#if wxUSE_XRC
#include <wx/xrc/xmlres.h>
#endif

#if wxUSE_PROPGRID
#include <wx/propgrid/property.h>
#endif


template< class T > struct remove_pointer                    {typedef T type;};
template< class T > struct remove_pointer<T*>                {typedef T type;};
template< class T > struct remove_pointer<T* const>          {typedef T type;};
template< class T > struct remove_pointer<T* volatile>       {typedef T type;};
template< class T > struct remove_pointer<T* const volatile> {typedef T type;};

template< typename T >
struct is_pointer{
  static const bool value = false;
};

template< typename T >
struct is_pointer< T* >{
  static const bool value = true;
};

//typedef std::map<std::string,VALUE> klassholdertype;
typedef std::map<const wxClassInfo*,VALUE> infoholdertype;
extern infoholdertype infoklassholder;
typedef std::map<std::string,VALUE> typeholdertype;
extern typeholdertype typeklassholder;



VALUE wrap(void *arg,VALUE klass);
VALUE wrap(wxEvtHandler *handler,VALUE klass);
VALUE wrap(wxClientDataContainer *sizer,VALUE klass);
VALUE wrap(wxSizer *sizer,VALUE klass);

#if wxUSE_PROPGRID
VALUE wrap(wxPGProperty *sizer,VALUE klass);
#endif


template <typename T>
void registerType(VALUE klass)
{
	typeklassholder[typeid(T*).name()]=klass;
}

template <typename T>
void registerInfo(VALUE klass)
{
	infoklassholder[wxCLASSINFO(T)]=klass;
	typeklassholder[typeid(T*).name()]=klass;
}

VALUE wrapClass(const wxClassInfo * info);

template <typename T>
VALUE wrap(T *arg)
{
	if(!arg)
		return Qnil;
	wxClassInfo *info = arg->GetClassInfo();
	VALUE klass = wrapClass(info);
	if(!NIL_P(klass))
	{
		return wrap(arg,klass);
	}
	rb_warn("%s type unknown",wxString(info->GetClassName()).c_str().AsChar());
	return Qnil;
}

template <typename T>
T* unwrapPtr(const VALUE &obj,const VALUE &klass)
{
	if(NIL_P(obj))
		return NULL;

	if(rb_obj_is_instance_of(obj,rb_cClass) && rb_class_inherited(obj,klass)) {
		return unwrapPtr<T>(rb_class_new_instance(0,NULL,obj),klass);
	}else if (rb_obj_is_kind_of(obj, klass)){
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

template <typename T>
struct WrapReturn
{
	WrapReturn(T *val) : mValue(val) {};
	WrapReturn(T &val) : mValue(&val) {};

	T *mValue;

	operator T*() {return mValue;};
	operator T() {return *mValue;};

};

template <typename T>
T nullPtr(){
	return (T)NULL;
}


template <typename T>
T wrap(const VALUE &arg)
{
	if(NIL_P(arg))
		return nullPtr<T>();
	typedef typename remove_pointer<T>::type rtype;

	typeholdertype::iterator it = typeklassholder.find(typeid(rtype*).name());
	if(it != typeklassholder.end())
	{
		return WrapReturn<rtype>(unwrapPtr<rtype>(arg,it->second));
	}
	rb_raise(rb_eTypeError,"%s type unknown",typeid(rtype*).name());
	return nullPtr<T>();
}

template <class T>
VALUE wrap(const T &arg){
	return wrap(new T(arg));
};

template <typename T>
bool is_wrapable(const VALUE &arg);


template <>
bool wrap< bool >(const VALUE &val );
template <>
VALUE wrap< bool >(const bool &st );
template <>
int wrap< int >(const VALUE &val );
template <>
VALUE wrap< int >(const int &st );

template <>
unsigned int wrap< unsigned int >(const VALUE &val );
template <>
VALUE wrap< unsigned int >(const unsigned int &st );


template <>
VALUE wrap< wxString >(const wxString &st );

template <>
VALUE wrap< wxChar >(const wxChar &c );

template <>
char* wrap< char* >(const VALUE &val );

template <>
wxString wrap< wxString >(const VALUE &val );

template <>
VALUE wrap< wxArrayString >(const wxArrayString &st );

template <>
VALUE wrap< wxArrayInt >(const wxArrayInt &st );

template <>
wxArrayString wrap< wxArrayString >(const VALUE &val );

template <>
VALUE wrap< wxDateTime >(const wxDateTime &st );

template <>
wxDateTime wrap< wxDateTime >(const VALUE &val );

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


DLL_LOCAL void rb_define_attr_method(VALUE klass,std::string name,VALUE(get)(VALUE),VALUE(set)(VALUE,VALUE));

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
