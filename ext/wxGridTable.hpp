/*
 * wxGridTable.hpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */

#ifndef WXGRIDTABLE_HPP_
#define WXGRIDTABLE_HPP_

#include "main.hpp"

extern VALUE rb_cWXGridTable;
void Init_WXGridTable(VALUE rb_mWX);

#if wxUSE_GRID
#include <wx/grid.h>

class RubyGridTable : public wxGridTableBase
{
public:
	RubyGridTable(VALUE klass);

	int GetNumberRows();
	int GetNumberCols();

	wxString GetValue( int row, int col );
	void SetValue( int row, int col, const wxString& value );

	wxString GetTypeName( int row, int col );

	void Clear();
	bool InsertRows( size_t pos = 0, size_t numRows = 1 );
	bool AppendRows( size_t numRows = 1 );
	bool DeleteRows( size_t pos = 0, size_t numRows = 1 );
	bool InsertCols( size_t pos = 0, size_t numCols = 1 );
	bool AppendCols( size_t numCols = 1 );
	bool DeleteCols( size_t pos = 0, size_t numCols = 1 );

};

template <>
inline VALUE wrap< wxGridTableBase >(wxGridTableBase* window)
{
	return wrap(window,rb_cWXGridTable);
}


template <>
inline wxGridTableBase* wrap< wxGridTableBase* >(const VALUE &vwindow)
{
	return unwrapPtr<wxGridTableBase>(vwindow, rb_cWXGridTable);
}

#endif

#endif /* WXGRIDTABLE_HPP_ */
