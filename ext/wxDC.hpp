/*
 * wxDC.hpp
 *
 *  Created on: 07.03.2012
 *      Author: hanmac
 */

#ifndef WXDC_HPP_
#define WXDC_HPP_

#include "main.hpp"
#if wxUSE_GRAPHICS_CONTEXT
#include <wx/dcgraph.h>
#endif
extern VALUE rb_cWXDC;
void Init_WXDC(VALUE rb_mWX);

#endif /* WXDC_HPP_ */
