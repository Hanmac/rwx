require "mkmf"


def have_member_func(klass,member,header)
  if have_func("#{klass}().#{member}()",header)
    $defs[-1] = "-DHAVE_#{klass.tr_cpp}_#{member.tr_cpp}" 
  end
end

dir_config "rwx"

if(wx_config = find_executable('wx-config'))

  $CXXFLAGS = CONFIG["CXXFLAGS"] unless defined?($CXXFLAGS)
	
	case `#{wx_config} --basename`
	when /gtk2/
    $CXXFLAGS << pkg_config("gtk+-x11-2.0")[0]
    $CXXFLAGS << pkg_config("gdk-x11-2.0")[0]
	when /gtk3/
	  $CXXFLAGS << pkg_config("gtk+-x11-3.0")[0]
    $CXXFLAGS << pkg_config("gdk-x11-3.0")[0]
	end

	$CFLAGS << " -fvisibility-inlines-hidden -x c++ -g -Wall "
  $CXXFLAGS << " -fvisibility-inlines-hidden -g -Wall "
	$CPPFLAGS << " -fvisibility-inlines-hidden -g "
	$LDFLAGS << " -fvisibility-inlines-hidden "


	$CFLAGS << `#{wx_config} --cflags`.chomp
  $CXXFLAGS << `#{wx_config} --cxxflags`.chomp
	$CPPFLAGS << `#{wx_config} --cppflags`.chomp
	$LDFLAGS << `#{wx_config} --libs all`.chomp

	have_header("wx/preferences.h")
	
	
	with_cflags(" -x c++ ") {
    RbConfig::CONFIG["CC"] = CONFIG["CXX"] 
	  have_func("wxContextHelpButton()","wx/cshelp.h")
    have_func("wxNumberEntryDialog()","wx/numdlg.h")
    have_func("wxPasswordEntryDialog()","wx/textdlg.h")
    have_func("wxProgressDialog()","wx/progdlg.h")
        
    have_member_func("wxFontPickerCtrl","GetSelectedColour","wx/fontpicker.h")
    
    have_const("wxFD_NO_FOLLOW","wx/filedlg.h")
    
	}
else
		abort("wx-config executable not found")

end

#drop some of the warn flags because they are not valid for C++
CONFIG["warnflags"].gsub!("-Wdeclaration-after-statement","")
CONFIG["warnflags"].gsub!("-Wimplicit-function-declaration","")

#wxAUI is a bit buggy
CONFIG["warnflags"].gsub!("-Wextra","")

create_header
create_makefile "rwx"
