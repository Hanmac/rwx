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

#include "extconf.h"

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

#ifndef RARRAY_AREF
#ifdef RARRAY_CONST_PTR
#define RARRAY_AREF(a,i) RARRAY_CONST_PTR(a)[i]
#else
#define RARRAY_AREF(a,i) RARRAY_PTR(a)[i]
#endif

#endif


template< class T > struct remove_pointer                    {typedef T type;};
template< class T > struct remove_pointer<T*>                {typedef T type;};
template< class T > struct remove_pointer<T* const>          {typedef T type;};
template< class T > struct remove_pointer<T* volatile>       {typedef T type;};
template< class T > struct remove_pointer<T* const volatile> {typedef T type;};

//typedef std::map<std::string,VALUE> klassholdertype;
typedef std::map<const wxClassInfo*,VALUE> infoholdertype;
extern infoholdertype infoklassholder;
typedef std::map<std::string,VALUE> typeholdertype;
extern typeholdertype typeklassholder;

typedef std::map<VALUE, rb_data_type_t> datatypeholdertype;
extern datatypeholdertype datatypeholder;
extern datatypeholdertype datatypeholder_const;

extern VALUE global_holder;
void rwx_refobject(VALUE object);
bool rwx_unrefobject(VALUE object);

VALUE wrapTypedPtr(void *arg,VALUE klass, bool allowNull = false);
VALUE wrapTypedPtr(wxObject *object,VALUE klass);
VALUE wrapTypedPtr(wxEvtHandler *handler,VALUE klass);

VALUE wrapTypedPtr(wxClientDataContainer *sizer,VALUE klass);
VALUE wrapTypedPtr(wxSizer *sizer,VALUE klass);

#if wxUSE_PROPGRID
VALUE wrapTypedPtr(wxPGProperty *sizer,VALUE klass);
#endif
#if wxUSE_GRID
VALUE wrapTypedPtr(wxGridTableBase *sizer,VALUE klass);
VALUE wrapTypedPtr(wxGridCellAttr *sizer,VALUE klass);
#endif


struct enumtype
{
	std::string name;
	typedef std::multimap<int,ID> value_type;
	value_type values;

	bool allow_array;
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


DLL_LOCAL enumtype* registerEnum(const std::string& name,const std::string& type ,int def = 0);


template <typename T>
DLL_LOCAL enumtype* registerEnum(const char* name,int def = 0)
{
	return registerEnum(name,typeid(T).name(),def);
}


template <typename T>
DLL_LOCAL size_t type_size_of(const void* data)
{
	return data ? sizeof(T) : 0;
}

DLL_LOCAL void registerDataType(VALUE klass);
DLL_LOCAL void registerDataType(VALUE klass, RUBY_DATA_FUNC freefunc, size_t (*sizefunc)(const void *) = NULL);

template <typename T>
DLL_LOCAL void registerType(VALUE klass, bool bfree = false)
{
	typeklassholder[typeid(T*).name()]=klass;
	if(bfree)
		registerDataType(klass, RUBY_TYPED_DEFAULT_FREE, type_size_of<T>);
	else
		registerDataType(klass, RUBY_TYPED_NEVER_FREE, type_size_of<T>);
}

template <typename T>
DLL_LOCAL void registerInfo(VALUE klass, bool bfree = false)
{
	infoklassholder[wxCLASSINFO(T)]=klass;
	registerType<T>(klass, bfree);
}

DLL_LOCAL VALUE wrapClass(const wxClassInfo * info);

DLL_LOCAL rb_data_type_t* unwrapDataType(const VALUE& klass);

template <typename T>
VALUE wrap(T *arg)
{
	if(!arg)
		return Qnil;
	wxClassInfo *info = arg->GetClassInfo();
	VALUE klass = wrapClass(info);
	if(!NIL_P(klass))
	{
		return wrapTypedPtr(arg,klass);
	}
	rb_warn("%s type unknown",wxString(info->GetClassName()).c_str().AsChar());
	return Qnil;
}

template <typename T>
VALUE wrap(const T *arg)
{
	VALUE result = rb_obj_freeze(wrap(const_cast<T*>(arg)));
	RTYPEDDATA_TYPE(result) = &datatypeholder_const[CLASS_OF(result)];
	return result;
}


DLL_LOCAL void* unwrapTypedPtr(const VALUE &obj, rb_data_type_t* rbdata);

template <typename T>
T* unwrapTypedPtr(const VALUE &obj,const VALUE &klass, rb_data_type_t* rbdata = NULL)
{
	if(NIL_P(obj))
		return NULL;

	if(rb_obj_is_instance_of(obj,rb_cClass) && rb_class_inherited(obj,klass)) {
		return unwrapTypedPtr<T>(rb_class_new_instance(0,NULL,obj), klass, rbdata);
	}else if (rb_obj_is_kind_of(obj, klass)){
		return (T*)unwrapTypedPtr(obj, rbdata ? rbdata : unwrapDataType(klass));
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
T unwrap(const VALUE &arg)
{
	if(NIL_P(arg))
		return NULL;
	typedef typename remove_pointer<T>::type rtype;

	typeholdertype::iterator it = typeklassholder.find(typeid(rtype*).name());
	if(it != typeklassholder.end())
	{
		return unwrapTypedPtr<rtype>(arg,it->second);
	}
	rb_raise(rb_eTypeError,"%s type unknown",typeid(rtype*).name());
	return NULL;
}

template <class T>
VALUE wrap(const T &arg){
	return wrap(new T(arg));
};

template <typename T>
bool is_wrapable(const VALUE &arg);


VALUE wrapenum(const int &arg, const std::string& name);


template <typename T>
VALUE wrapenum(const T &arg){
	return wrapenum(arg,typeid(T).name());
}

template <typename T>
VALUE wrapenum(int arg){
	return wrapenum((T)arg);
}

int unwrapenum(const VALUE &arg, const std::string& name);

template <typename T>
T unwrapenum(const VALUE &arg){
	return (T)unwrapenum(arg,typeid(T).name());
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
double unwrap< double >(const VALUE &val );
template <>
VALUE wrap< double >(const double &st );


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
bool nil_check(VALUE window,VALUE klass ,bool raise = true);
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

template <typename C, typename T>
DLL_LOCAL bool set_hash_option(VALUE hash,const char* name,void (C::*set)(const T&), C& obj,T func(const VALUE&) = unwrap<T> )
{
	T val;
	bool result = set_hash_option(hash,name,val,func);
	if(result) {
		(obj.*set)(val);
	}
	return result;

}
template <typename C, typename T>
DLL_LOCAL bool set_hash_option(VALUE hash,const char* name,void (C::*set)(T), C& obj,T func(const VALUE&) = unwrap<T> )
{
	T val;
	bool result = set_hash_option(hash,name,val,func);
	if(result) {
		(obj.*set)(val);
	}
	return result;

}


DLL_LOCAL bool set_hash_flag_option(VALUE hash,const char* name,const int& flag,int& val);

#define macro_attr(attr,type) macro_attr_func(attr,Get##attr(),Set##attr,wrap,unwrap<type>,true)
#define macro_attr_enum(attr,type) macro_attr_func(attr,Get##attr(),Set##attr,wrapenum<type>,unwrapenum<type>,true)
#define macro_attr_with_func(attr,getf,setf) macro_attr_func(attr,Get##attr(),Set##attr,getf,setf,true)

#define macro_attr_con(attr,type,con) macro_attr_func(attr,Get##attr(),Set##attr,wrap,unwrap<type>,con)
#define macro_attr_enum_con(attr,type,con) macro_attr_func(attr,Get##attr(),Set##attr,wrapenum<type>,unwrapenum<type>,_self->con())

#define macro_attr_pre(attr,type,pre) macro_attr_func(attr,pre().Get##attr(),pre().Set##attr,wrap,unwrap<type>,true)

#define macro_attr_bool(attr) macro_attr_func(attr,Is##attr(),Set##attr,wrap,RTEST,true)
#define macro_attr_bool2(attr,attr2) macro_attr_func(attr,Is##attr(),attr2,wrap,RTEST,true)
#define macro_attr_bool3(attr,attr2) macro_attr_func(attr,Has##attr(),attr2,wrap,RTEST,true)
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

#define singlereturn_array(func,T) \
DLL_LOCAL VALUE _##func(VALUE self)\
{\
	T ary;\
	_self->func(ary);\
	return wrap(ary);\
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
