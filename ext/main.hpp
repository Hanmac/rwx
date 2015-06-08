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

#ifndef RUBY_TYPED_DEFAULT_FREE
#ifndef RUBY_DEFAULT_FREE
#define RUBY_DEFAULT_FREE ((RUBY_DATA_FUNC)-1)
#endif
#define RUBY_TYPED_DEFAULT_FREE RUBY_DEFAULT_FREE
#endif

#ifndef RUBY_TYPED_NEVER_FREE
#ifndef RUBY_NEVER_FREE
#define RUBY_NEVER_FREE ((RUBY_DATA_FUNC)0)
#endif
#define RUBY_TYPED_NEVER_FREE RUBY_NEVER_FREE
#endif

#ifdef PRIsVALUE
#define RB_OBJ_CLASSNAME(obj) rb_obj_class(obj)
#define RB_CLASSNAME(obj) (obj)
#define RB_OBJ_STRING(obj) (obj)
#else
#define PRIsVALUE "s"
#define RB_OBJ_CLASSNAME(obj) rb_obj_classname(obj)
#define RB_CLASSNAME(obj) rb_class2name(obj)
#define RB_OBJ_STRING(obj) rb_string_value_cstr((volatile VALUE*)&obj)
#endif

#ifndef RETURN_SIZED_ENUMERATOR
#define RETURN_SIZED_ENUMERATOR(obj, argc, argv, size) RETURN_ENUMERATOR(obj, argc, argv)
#endif

#ifndef Check_TypedStruct
#define Check_TypedStruct(v,t) rb_check_typeddata((VALUE)(v),(t))
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

DLL_LOCAL VALUE wrap(wxObject *obj, wxClassInfo *info);

template <typename T>
VALUE wrap(T *arg)
{
	if(!arg)
		return Qnil;
	return wrap(arg,arg->GetClassInfo());
}

template <typename T>
VALUE wrap(const T *arg)
{
	VALUE result = rb_obj_freeze(wrap(const_cast<T*>(arg)));
	RTYPEDDATA_TYPE(result) = &datatypeholder_const[CLASS_OF(result)];
	return result;
}

bool check_class(VALUE obj, VALUE klass);

DLL_LOCAL void* unwrapTypedPtr(const VALUE &obj, rb_data_type_t* rbdata);

template <typename T>
T* unwrapTypedPtr(const VALUE &obj,const VALUE &klass, rb_data_type_t* rbdata = NULL)
{
	if(NIL_P(obj))
		return NULL;

	if(rb_obj_is_instance_of(obj,rb_cClass) && rb_class_inherited(obj,klass)) {
		return unwrapTypedPtr<T>(rb_class_new_instance(0,NULL,obj), klass, rbdata);
	}else if (check_class(obj, klass)){
		return (T*)unwrapTypedPtr(obj, rbdata ? rbdata : unwrapDataType(klass));
	}else{
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
		rb_raise(rb_eArgError, "%" PRIsVALUE " has wrong parent.",
			RB_OBJ_STRING(window)
		);
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

DLL_LOCAL bool set_hash_option(VALUE hash,const char* name,VALUE& val);

template <typename T, typename T2>
DLL_LOCAL bool set_hash_option(VALUE hash,const char* name,T& val,T2 func(const VALUE&) )
{
	VALUE temp;
	if(set_hash_option(hash,name,temp))
	{
		val = func(temp);
		return true;
	}
	return false;
}

template <typename T>
DLL_LOCAL bool set_hash_option(VALUE hash,const char* name,T& val)
{
	return set_hash_option(hash, name, val, unwrap<T>);
}



template <typename C, typename C2, typename T, typename V>
DLL_LOCAL bool set_obj_option(VALUE hash,const char* name,V (C::*set)(const T&), C2 *obj,T func(const VALUE&) = unwrap<T> )
{
	T val;
	bool result = set_hash_option(hash,name,val,func);
	if(result) {
		(obj->*set)(val);
	}
	return result;

}


template <typename C, typename C2, typename T, typename V>
DLL_LOCAL bool set_obj_option(VALUE hash,const char* name,V (C::*set)(T), C2* obj,T func(const VALUE&) = unwrap<T>)
{
	T val;
	bool result = set_hash_option(hash,name,val,func);
	if(result) {
		(obj->*set)(val);
	}
	return result;

}


template <typename C, typename C2, typename V>
DLL_LOCAL bool set_obj_option(VALUE hash,const char* name,V (C::*set)(), C2* obj)
{
	bool val(false);
	bool result = set_hash_option(hash,name,val,unwrap<bool>);
	if(val) {
		(obj->*set)();
	}
	return result;

}

//*
template <typename C, typename V>
DLL_LOCAL bool set_obj_option(VALUE hash,const char* name,V (C::*set)(), C& obj)
{
	return set_obj_option(hash, name, set, &obj);
}
template <typename C,typename T, typename V>
DLL_LOCAL bool set_obj_option(VALUE hash,const char* name,V (C::*set)(T), C& obj,T func(const VALUE&) = unwrap<T>)
{
	return set_obj_option(hash, name, set, &obj, func);
}
template <typename C, typename T, typename V>
DLL_LOCAL bool set_obj_option(VALUE hash,const char* name,V (C::*set)(const T&), C& obj,T func(const VALUE&) = unwrap<T> )
{
	return set_obj_option(hash, name, set, &obj, func);
}
//*/

template <typename C, typename V>
DLL_LOCAL bool set_obj_option(VALUE hash,const char* name,V (C::*set)(const wxString&, const wxString&), C& obj)
{
	wxArrayString val;
	bool result = set_hash_option(hash,name,val);
	if(result) {
		(obj.*set)(val[0], val.size() > 1 ? val[1] : wxString() );
	}
	return result;

}

DLL_LOCAL void not_valid(VALUE val, VALUE klass);

DLL_LOCAL bool set_ruby_option(VALUE hash,const char* name,VALUE func(VALUE, VALUE), VALUE self);

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

#define macro_attr_item_func(attr,funcget,funcset,funcsize,wrapget,wrapset) \
\
DLL_LOCAL VALUE _get##attr(VALUE self,VALUE idx)\
{\
	int cidx = NUM2INT(idx);\
	if(check_index(cidx,_self->funcsize()))\
		return wrapget(_self->funcget(cidx));\
	return Qnil;\
}\
DLL_LOCAL VALUE _set##attr(VALUE self,VALUE idx,VALUE val)\
{\
	rb_check_frozen(self);\
\
	int cidx = NUM2INT(idx);\
	if(check_index(cidx,_self->funcsize()))\
		_self->funcset(cidx,wrapset(val));\
\
	return self;\
}

#define macro_attr_item(attr,funcget,funcset,funcsize,type) macro_attr_item_func(attr,funcget,funcset,funcsize,wrap,unwrap<type>)


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


DLL_LOCAL void rb_define_attr_method(VALUE klass,const std::string& name,VALUE(get)(VALUE),VALUE(set)(VALUE,VALUE));
DLL_LOCAL void rb_define_attr_method_missing(VALUE klass,const std::string& name, bool get = true, bool set = true);

#define singlefunc_if(func, con) \
DLL_LOCAL VALUE _##func(VALUE self)\
{\
	rb_check_frozen(self);\
	if(con)\
		_self->func();\
	return self;\
}

#define singlefunc(func) singlefunc_if(func, true)

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
