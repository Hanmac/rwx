/*
 * wxXRC.hpp
 *
 *  Created on: 04.11.2012
 *      Author: hanmac
 */

#ifndef WXXRC_HPP_
#define WXXRC_HPP_

#include "wxDialog.hpp"

DLL_LOCAL void Init_WXXRC(VALUE rb_mWX);

#if wxUSE_XRC
#include <wx/xrc/xh_all.h>
#endif



#endif /* WXXRC_HPP_ */
