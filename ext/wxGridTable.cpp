/*
 * wxGridTable.cpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */
#include "wxGridTable.hpp"
#include "wxEvtHandler.hpp"
#define mRuby static_cast<RubyClientData*>(GetClientObject())->mRuby

VALUE rb_cWXGridTable;

#if wxUSE_GRID
RubyGridTable::RubyGridTable(VALUE klass) : wxGridTableBase()
{
	this->SetClientObject(new RubyClientData(wrapPtr((void*)this,klass)));
}

int RubyGridTable::GetNumberRows()
{
	return NUM2INT(rb_funcall(mRuby,rb_intern("rows"),0));
}
int RubyGridTable::GetNumberCols()
{
	return NUM2INT(rb_funcall(mRuby,rb_intern("cols"),0));
}

wxString RubyGridTable::GetValue( int row, int col )
{
	return unwrap<wxString>(rb_funcall(mRuby,rb_intern("[]"),2,INT2NUM(row),INT2NUM(col)));
}

void RubyGridTable::SetValue( int row, int col, const wxString& value )
{
	rb_funcall(mRuby,rb_intern("[]="),3,INT2NUM(row),INT2NUM(col),wrap(value));
}



wxString RubyGridTable::GetTypeName( int row, int col )
{
	return unwrap<wxString>(rb_funcall(mRuby,rb_intern("typename"),2,INT2NUM(row),INT2NUM(col)));
}

void RubyGridTable::Clear()
{
	rb_funcall(mRuby,rb_intern("clear"),0);
}

bool RubyGridTable::InsertRows( size_t pos, size_t numRows)
{
	return RTEST(rb_funcall(mRuby,rb_intern("insert_rows"),2,UINT2NUM(pos),UINT2NUM(numRows)));
}

bool RubyGridTable::AppendRows( size_t numRows)
{
	return RTEST(rb_funcall(mRuby,rb_intern("append_rows"),1,UINT2NUM(numRows)));
}
bool RubyGridTable::DeleteRows( size_t pos, size_t numRows)
{
	return RTEST(rb_funcall(mRuby,rb_intern("delete_rows"),2,UINT2NUM(pos),UINT2NUM(numRows)));
}
bool RubyGridTable::InsertCols( size_t pos, size_t numCols)
{
	return RTEST(rb_funcall(mRuby,rb_intern("insert_cols"),2,UINT2NUM(pos),UINT2NUM(numCols)));
}
bool RubyGridTable::AppendCols( size_t numCols)
{
	return RTEST(rb_funcall(mRuby,rb_intern("append_cols"),1,UINT2NUM(numCols)));
}
bool RubyGridTable::DeleteCols( size_t pos, size_t numCols)
{
	return RTEST(rb_funcall(mRuby,rb_intern("delete_cols"),2,UINT2NUM(pos),UINT2NUM(numCols)));
}



namespace RubyWX {
namespace GridTable {
#define _self unwrap<wxGridTableBase*>(self)
//macro_attr(Path,wxString)


VALUE _alloc(VALUE self)
{
	return wrapPtr(new RubyGridTable(self),self);
}

}
}

#endif

void Init_WXGridTable(VALUE rb_mWX)
{
#if wxUSE_GRID
	using namespace RubyWX::GridTable;
	rb_cWXGridTable = rb_define_class_under(rb_mWX,"GridTable",rb_cObject);
	rb_define_alloc_func(rb_cWXGridTable,_alloc);

	registerInfo<wxGridTableBase>(rb_cWXGridTable);
#endif
}


