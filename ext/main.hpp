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

//need to include setup first because main header does it wrong
#include <wx/setup.h>
#include <wx/wx.h>
#include <ruby.h>

#include <typeinfo>
#include <map>
#include <string>
#include <string.h>


#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#endif

#if wxUSE_XRC
#include <wx/xrc/xmlres.h>
#endif

#if wxUSE_PROPGRID
#include <wx/propgrid/property.h>
#endif

#if wxUSE_GRID
#include <wx/grid.h>
#endif

#include "extconf.h"


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

extern VALUE global_holder;
void rwx_refobject(VALUE object);
void rwx_unrefobject(VALUE object);

VALUE wrapPtr(void *arg,VALUE klass);
VALUE wrapPtr(wxObject *object,VALUE klass);
VALUE wrapPtr(wxEvtHandler *handler,VALUE klass);

VALUE wrapPtr(wxClientDataContainer *sizer,VALUE klass);
VALUE wrapPtr(wxSizer *sizer,VALUE klass);

#if wxUSE_PROPGRID
VALUE wrapPtr(wxPGProperty *sizer,VALUE klass);
#endif
#if wxUSE_GRID
VALUE wrapPtr(wxGridTableBase *sizer,VALUE klass);
VALUE wrapPtr(wxGridCellAttr *sizer,VALUE klass);
#endif


struct enumtype
{
	std::string name;
	typedef std::map<int,ID> value_type;
	value_type values;

	int defaults;

	enumtype* add(int enumo,const char* sym)
	{
		values.insert(std::make_pair(enumo,rb_intern(sym)));
		return this;
	}
};
//typedef std::map<int,ID > enumtype;
typedef std::map<std::string,enumtype* > enumregistertype;

extern enumregistertype enumregister;


template <typename T>
DLL_LOCAL enumtype* registerEnum(const char* name,int def = 0)
{
	enumtype *type = new enumtype;
	enumregister.insert(std::make_pair(std::string(typeid(T).name()),type));
	type->name = std::string(name);
	type->defaults = def;
	return type;
}


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
		return wrapPtr(arg,klass);
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
T unwrap(const VALUE &arg)
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



template <typename T>
VALUE wrapenum(const T &arg){
	enumtype::value_type &enummap = enumregister[std::string(typeid(T).name())]->values;
	enumtype::value_type::iterator it = enummap.find((int)arg);
	if(it != enummap.end())
		return ID2SYM(it->second);
	bool found = false;

	VALUE result = rb_ary_new();
	for(it = enummap.begin();it != enummap.end();++it)
	{
		if((arg & it->first) != 0)
		{
			found = true;
			rb_ary_push(result,ID2SYM(it->second));
		}
	}
	return found ? result : Qnil;
}
template <typename T>
VALUE wrapenum(int arg){
	return wrapenum((T)arg);
}

template <typename T>
T unwrapenum(const VALUE &arg){
	enumregistertype::iterator it = enumregister.find(typeid(T).name());
	if(it != enumregister.end())
	{
		if(NIL_P(arg))
			return (T)it->second->defaults;
		else if(SYMBOL_P(arg))
		{
			ID id = SYM2ID(arg);

			for(enumtype::value_type::iterator it2 = it->second->values.begin();
					it2 != it->second->values.end();
					++it2)
			{
				if(it2->second == id)
					return (T)it2->first;
			}
			rb_raise(rb_eTypeError,"%s is not a %s-Enum.",rb_id2name(id),it->second->name.c_str());
		}else if(rb_obj_is_kind_of(arg,rb_cArray))
		{
			int result = 0;
			size_t count = RARRAY_LEN(arg);
			for(size_t i = 0; i < count; ++i)
				result = result | unwrapenum<T>(RARRAY_PTR(arg)[i]);
		}else
			return (T)NUM2INT(arg);
	}
	return (T)0;
}


template <>
bool unwrap< bool >(const VALUE &val );
template <>
VALUE wrap< bool >(const bool &st );
template <>
int unwrap< int >(const VALUE &val );
template <>
VALUE wrap< int >(const int &st );

template <>
unsigned int unwrap< unsigned int >(const VALUE &val );
template <>
VALUE wrap< unsigned int >(const unsigned int &st );

template <>
long unwrap< long >(const VALUE &val );
template <>
VALUE wrap< long >(const long &st );

template <>
unsigned long unwrap< unsigned long >(const VALUE &val );
template <>
VALUE wrap< unsigned long >(const unsigned long &st );

template <>
VALUE wrap< wxString >(const wxString &st );

template <>
VALUE wrap< wxChar >(const wxChar &c );

template <>
char* unwrap< char* >(const VALUE &val );

template <>
wxString unwrap< wxString >(const VALUE &val );

template <>
VALUE wrap< wxArrayString >(const wxArrayString &st );

template <>
VALUE wrap< wxArrayInt >(const wxArrayInt &st );

template <>
wxArrayString unwrap< wxArrayString >(const VALUE &val );
template <>
wxArrayInt unwrap< wxArrayInt >(const VALUE &val );

template <>
VALUE wrap< wxDateTime >(const wxDateTime &st );

template <>
wxDateTime unwrap< wxDateTime >(const VALUE &val );

DLL_LOCAL int unwrap_iconflag(const VALUE& val,int mask = wxICON_MASK);
DLL_LOCAL int unwrap_buttonflag(const VALUE& val);

DLL_LOCAL bool check_file_loadable(const wxString& path);
DLL_LOCAL bool check_file_saveable(const wxString& path);

template <typename T>
DLL_LOCAL bool window_parent_check(VALUE window, wxWindow* parent, T* &w)
{
	w = unwrap<T*>(window);
	if(w && w->GetParent() != parent)
	{
		rb_raise(rb_eArgError, "%s has wrong parent.",unwrap<char*>(window));
		return false;
	}
	return true;
}

bool nil_check(VALUE window,const char* type,bool raise = true);
bool nil_check(VALUE window,bool raise = true);

bool check_index(int &cidx,const std::size_t &count);

#define macro_attr_func(attr,funcget,funcset,wrapget,wrapset,con) \
DLL_LOCAL VALUE _get##attr(VALUE self)\
{ \
	if(con)\
		return wrapget(_self->funcget);\
	return Qnil;\
}\
\
DLL_LOCAL VALUE _set##attr(VALUE self,VALUE other)\
{\
	rb_check_frozen(self);\
	if(con)\
		_self->funcset(wrapset(other));\
	return other;\
}


template <typename T>
DLL_LOCAL bool set_hash_option(VALUE hash,const char* name,T& val,T func(const VALUE&) = unwrap<T> )
{
	VALUE temp;
	if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern(name)))))
	{
		val = func(temp);
		return true;
	}
	return false;

}

DLL_LOCAL void set_hash_flag_option(VALUE hash,const char* name,const int& flag,int& val);

#define macro_attr(attr,type) macro_attr_func(attr,Get##attr(),Set##attr,wrap,unwrap<type>,true)
#define macro_attr_enum(attr,type) macro_attr_func(attr,Get##attr(),Set##attr,wrapenum<type>,unwrapenum<type>,true)
#define macro_attr_with_func(attr,getf,setf) macro_attr_func(attr,Get##attr(),Set##attr,getf,setf,true)

#define macro_attr_con(attr,type,con) macro_attr_func(attr,Get##attr(),Set##attr,wrap,unwrap<type>,con)
#define macro_attr_enum_con(attr,type,con) macro_attr_func(attr,Get##attr(),Set##attr,wrapenum<type>,unwrapenum<type>,_self->con())

#define macro_attr_pre(attr,type,pre) macro_attr_func(attr,pre().Get##attr(),pre().Set##attr,wrap,unwrap<type>,true)

#define macro_attr_bool(attr) macro_attr_func(attr,Is##attr(),Set##attr,wrap,RTEST,true)
#define macro_attr_bool2(attr,attr2) macro_attr_func(attr,Is##attr(),attr2,wrap,RTEST,true)
#define macro_attr_bool_con(attr,con) macro_attr_func(attr,Is##attr(),Set##attr,wrap,RTEST,_self->con())

//*/
#define macro_attr_prop(attr,type) macro_attr_func(_##attr,attr,attr = ,wrap,unwrap<type>,true)
#define macro_attr_prop_enum(attr,type) macro_attr_func(_##attr,attr,attr = ,wrapenum<type>,unwrapenum<type>,true)
#define macro_attr_prop_with_func(attr,getf,setf) macro_attr_func(_##attr,attr,attr = ,getf,setf,true)


/*
 * special macro for select attributes that manages wxNOT_FOUND
 */
#define macro_attr_selection(attr,count) \
DLL_LOCAL VALUE _get##attr(VALUE self)\
{ \
	int val = _self->Get##attr();\
	return val == wxNOT_FOUND ? Qnil : UINT2NUM(val);\
}\
\
DLL_LOCAL VALUE _set##attr(VALUE self,VALUE other)\
{\
	rb_check_frozen(self);\
	if(NIL_P(other))\
		_self->Set##attr(wxNOT_FOUND);\
	int cother = NUM2INT(other);\
	if(check_index(cother,_self->count()))\
		_self->Set##attr(cother);\
	return other;\
}


DLL_LOCAL void rb_define_attr_method(VALUE klass,std::string name,VALUE(get)(VALUE),VALUE(set)(VALUE,VALUE));

#define singlefunc(func) \
DLL_LOCAL VALUE _##func(VALUE self)\
{\
	rb_check_frozen(self);\
	_self->func();\
	return self;\
}


#define singlereturn(func) \
DLL_LOCAL VALUE _##func(VALUE self)\
{\
	return wrap(_self->func());\
}

#define singlereturn_frozen(func) \
DLL_LOCAL VALUE _##func(VALUE self)\
{\
	rb_check_frozen(self);\
	return wrap(_self->func());\
}

#define set_option_func(name,cname,func) \
	if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern(#name)))))\
		_self->Set##cname(func(temp));

#define set_option(name,cname,type) set_option_func(name,cname,unwrap<type>)

#endif /* MAIN_HPP_ */
