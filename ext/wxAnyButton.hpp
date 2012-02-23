/*
 * wxAnyButton.hpp
 *
 *  Created on: 13.02.2012
 *      Author: hanmac
 */

#ifndef WXANYBUTTON_HPP_
#define WXANYBUTTON_HPP_

#include "wxControl.hpp"

class RubyAnyButton : public wxAnyButton, RubyControl {
public:
	RubyAnyButton(VALUE klass);
};


#endif /* WXANYBUTTON_HPP_ */
