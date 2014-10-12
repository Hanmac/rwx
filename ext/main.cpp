/*
 * main.cpp
 *
 *  Created on: 04.05.2012
 *      Author: hanmac
 */

#include "main.hpp"

#include <wx/arrstr.h>

#include <wx/filename.h>

infoholdertype infoklassholder;
typeholdertype typeklassholder;

enumregistertype enumregister;

datatypeholdertype datatypeholder;
datatypeholdertype datatypeholder_const;

VALUE global_holder;

typedef std::map<VALUE,std::size_t> ref_countertype;
ref_countertype ref_counter;


void rwx_refobject(VALUE object)
{
	if(ref_counter[object] == 0)
		rb_hash_aset(global_holder,INT2NUM(object),object);
	ref_counter[object]++;
}

bool rwx_unrefobject(VALUE object)
{
	if(ref_counter[object] != 0)
		ref_counter[object]--;
	if(ref_counter[object] == 0) {
		rb_hash_delete(global_holder,INT2NUM(object));
		return true;
	}
	return false;
}

void registerDataType(VALUE klass, RUBY_DATA_FUNC freefunc, size_t (*sizefunc)(const void *))
{
	if(!NIL_P(klass))
	{
		rb_data_type_t str = {
			rb_class2name(klass),
			{0, freefunc, sizefunc,},
			unwrapDataType(RCLASS_SUPER(klass)), NULL,
		};

		datatypeholder[klass] = str;
		if(freefunc) {
			rb_data_type_t str_const = {
				(std::string(rb_class2name(klass)) + "(const)").c_str(),
				{0, 0, sizefunc,},
				&datatypeholder[klass], NULL,
			};
			datatypeholder_const[klass] = str_const;
		}else {
			datatypeholder_const[klass] = str;
		}
	}
}

void registerDataType(VALUE klass)
{
	registerDataType(klass, RUBY_TYPED_NEVER_FREE, NULL);
}

void* unwrapTypedPtr(const VALUE &obj, rb_data_type_t* rbdata)
{
	if(NIL_P(obj))
		return NULL;

	if(!rbdata) {
		rb_raise(rb_eTypeError,"%s unknown datatype", rb_obj_classname(obj));
		return NULL;
	}
	void* data = Check_TypedStruct(obj, rbdata);
	if(!data) {
		rb_raise(
			rb_eRuntimeError, "destroyed object of %s", rb_obj_classname(obj)
		);
		return NULL;
	}
	return data;

}

VALUE wrapClass(const wxClassInfo * info)
{
	infoholdertype::iterator it = infoklassholder.find(info);
	if(it != infoklassholder.end())
		return it->second;
	if(const wxClassInfo *base = info->GetBaseClass1())
		return wrapClass(base);
	return Qnil;
}

rb_data_type_t* unwrapDataType(const VALUE& klass)
{
	if(klass == rb_cObject)
		return NULL;
	datatypeholdertype::iterator it = datatypeholder.find(klass);
	if(it != datatypeholder.end())
		return &it->second;
	return unwrapDataType(RCLASS_SUPER(klass));
}


VALUE wrapTypedPtr(void *arg,VALUE klass)
{
	if(arg){
		rb_data_type_t* datatype = unwrapDataType(klass);
		if(!datatype)
			rb_fatal("%s unknown datatype",rb_class2name(klass));

		return TypedData_Wrap_Struct(klass, datatype, arg);
	}
	return Qnil;
}

#define type_wrap(t,get,set) \
template <>\
t unwrap< t >(const VALUE &val )\
{\
	return get(val);\
}\
template <>\
VALUE wrap< t >(const t &st )\
{\
	return set(st);\
}\

#define type_wrap_array(t) \
template <>\
VALUE wrap< t >(const t &st )\
{\
	VALUE ary = rb_ary_new();\
	for(t::const_iterator it = st.begin(); it != st.end(); ++it)\
		rb_ary_push(ary,wrap(*it));\
	return ary;\
}\
template <>\
t unwrap< t >(const VALUE &val )\
{\
	t arry;\
	if(NIL_P(val))\
		return arry;\
	VALUE dp(rb_Array(val));\
	std::size_t length = RARRAY_LEN(dp);\
	for(std::size_t i = 0; i < length; ++i)\
		arry.Add(unwrap<t::value_type>(RARRAY_AREF(dp,i)));\
	return arry;\
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

type_wrap(int,NUM2INT,INT2NUM)
type_wrap(double,NUM2DBL,DBL2NUM)
type_wrap(unsigned int,NUM2UINT,UINT2NUM)
type_wrap(long,NUM2LONG,LONG2NUM)
type_wrap(unsigned long,NUM2ULONG,ULONG2NUM)

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
		return unwrap< char* >(rb_String(val));
}

template <>
wxString unwrap< wxString >(const VALUE &val )
{
	if(NIL_P(val))
		return wxEmptyString;
	else
		return wxString(unwrap< char* >(val));
}

type_wrap_array(wxArrayString)
type_wrap_array(wxArrayInt)

template <>
VALUE wrap< wxDateTime >(const wxDateTime &st )
{
	return rb_time_new(st.GetTicks(),st.GetMillisecond() * 10);
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

enumtype* registerEnum(const std::string& name,const std::string& type ,int def)
{
	enumtype *etype = new enumtype;
	enumregister.insert(std::make_pair(type,etype));
	etype->name = name;
	etype->allow_array = true;
	etype->defaults = def;
	return etype;
}


VALUE wrapenum(const int &arg, const std::string& name)
{
	enumtype::value_type &enummap = enumregister[name]->values;
	enumtype::value_type::iterator it = enummap.find(arg);
	if(it != enummap.end())
		return ID2SYM(it->second);
	bool found = false;

	int carg(arg);

	VALUE result = rb_ary_new();
	for(it = enummap.begin();it != enummap.end();++it)
	{
		if((carg & it->first) != 0)
		{
			found = true;
			rb_ary_push(result,ID2SYM(it->second));
			carg &= ~it->first;
		}
	}
	return found ? result : Qnil;
}

int unwrapenum(const VALUE &arg, const std::string& name)
{
	enumregistertype::iterator it = enumregister.find(name);
	if(it != enumregister.end())
	{
		enumtype* etype = it->second;
		if(NIL_P(arg))
			return etype->defaults;
		else if(SYMBOL_P(arg))
		{
			ID id(SYM2ID(arg));

			for(enumtype::value_type::iterator it2 = etype->values.begin();
					it2 != etype->values.end();
					++it2)
			{
				if(it2->second == id)
					return it2->first;
			}
			rb_raise(rb_eTypeError,"%s is not a %s-Enum.",rb_id2name(id),etype->name.c_str());
		}else if(rb_obj_is_kind_of(arg,rb_cArray) && etype->allow_array )
		{
			int result = 0;
			size_t count = RARRAY_LEN(arg);
			for(size_t i = 0; i < count; ++i)
				result = result | unwrapenum(RARRAY_AREF(arg,i),name);
		}else
			return NUM2INT(arg);
	}
	return 0;
}

int unwrap_iconflag(const VALUE &val,int mask)
{
	int result = unwrapenum(val,"icon_flag");

	if((result & mask) != result)
		rb_raise(rb_eTypeError,"%"PRIsVALUE" is not a in %d mask", val, mask);
	
	return result;

}

int unwrap_buttonflag(const VALUE& val)
{
	return unwrapenum(val,"button_flag");
}

bool check_file_loadable(const wxString& path)
{
	errno = 0;
	wxFileName dir(wxPathOnly(path));
	dir.MakeAbsolute(wxGetCwd());

	wxFileName file(path);
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
		rb_sys_fail(path.c_str());
		return false;
	}
	return true;

}
bool check_file_saveable(const wxString& path)
{
	errno = 0;

	wxFileName dir(wxPathOnly(path));
	dir.MakeAbsolute(wxGetCwd());

	wxFileName file(path);
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
		rb_sys_fail(path.c_str());
		return false;
	}
	return true;
}

bool set_hash_flag_option(VALUE hash,const char* name,const int& flag,int& val)
{
	VALUE temp;
	if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern(name)))))
	{
		if(RTEST(temp))
			val |= flag;
		else
			val &= ~flag;

		return true;
	}
	return false;
}

bool nil_check(VALUE window,const char* type,bool raise)
{
	bool result = NIL_P(window);
	if(result && raise)
		rb_raise(rb_eTypeError,"%s can't be nil.",type);
	return !result;
}

bool nil_check(VALUE window,VALUE klass, bool raise)
{
	return nil_check(window,rb_class2name(klass),raise);
}

bool nil_check(VALUE window, bool raise)
{
	return nil_check(window,"window",raise);
}

bool check_index(int &cidx,const std::size_t &count)
{
	bool raise = false;
	if(cidx >= (int)count){
		raise = true;
	}else if(cidx < 0)
	{
		int tmp = cidx + count;
		if(tmp > 0 && tmp <= (int)count)
			cidx = tmp;
		else
			raise = true;
	}

	if(raise)
		rb_raise(rb_eIndexError,"%d out of index",cidx);
	return !raise;
}
