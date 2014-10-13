require "mkmf"

# add check for member function, does need default constructor for class
def have_member_func(klass,member,header)
	if have_func("#{klass}().#{member}()",header)
		$defs[-1] = "-DHAVE_#{klass.tr_cpp}_#{member.tr_cpp}" 
	end
end

dir_config "rwx"

if(wx_config = find_executable('wx-config'))
	
	if `#{wx_config} --version`.chomp < "3.0.0"
		abort("wx version outdated, please update to 3.0.0 or newer")
	end
	
	# An ruby extension does need to be build against
	# the same compiler as ruby was
	unless CONFIG["CC"] && find_executable(CONFIG["CC"])
		abort("C compiler not found!")
	end
	unless CONFIG["CXX"] && find_executable(CONFIG["CXX"])
		abort("C++ compiler not found!")
	end
	
	cc = `#{wx_config} --cc`.chomp
	unless cc == CONFIG["CC"]
		warn("CC compiler missmatch %s == %s" % [cc, CONFIG["CC"]])
	end
	
	cxx = `#{wx_config} --cxx`.chomp
	unless cxx == CONFIG["CXX"]
		warn("CXX compiler missmatch %s == %s" % [cxx, CONFIG["CXX"]])
	end
	
	#earlier versions of ruby does not have that constant
	$CXXFLAGS = CONFIG["CXXFLAGS"] unless defined?($CXXFLAGS)
	
	#for some function add the base classes
	extra_libs = []
	case `#{wx_config} --basename`
	when /gtk2/
		extra_libs << "gtk+-x11-2.0" << "gdk-x11-2.0"
	when /gtk3/
		extra_libs << "gtk+-x11-3.0" << "gdk-x11-3.0"
	end
	
	extra_libs.each {|l|
		pkg = pkg_config(l)
		#because pkg forgot to add the include paths to cxx flags
		$CXXFLAGS << pkg[0] if pkg && !$CXXFLAGS[pkg[0]]
	}
	
	all = " -fvisibility-inlines-hidden"
	$CFLAGS << all << " -x c++ -g -Wall "
	$CXXFLAGS << all << " -g -Wall "
	$CPPFLAGS << all << " -g "
	$LDFLAGS << all << " "
	
	# add the wx-config flags
	$CFLAGS << `#{wx_config} --cflags`.chomp
	$CXXFLAGS << `#{wx_config} --cxxflags`.chomp
	$CPPFLAGS << `#{wx_config} --cppflags`.chomp
	$LDFLAGS << `#{wx_config} --libs all`.chomp
	
	with_cflags(" -x c++ ") {
		# need c++ for some of the tests
		RbConfig::CONFIG["CC"] = CONFIG["CXX"]
		
		have_header("wx/preferences.h")
		
		#check for better Bind commmand
		unless have_macro("wxHAS_EVENT_BIND","wx/wx.h")
			abort("need wxHAS_EVENT_BIND, update your compiler!")
		end
		
		#check for Default-Constructors
		have_func("wxContextHelpButton()","wx/cshelp.h")
		have_func("wxNumberEntryDialog()","wx/numdlg.h")
		have_func("wxPasswordEntryDialog()","wx/textdlg.h")
		have_func("wxProgressDialog()","wx/progdlg.h")
		have_func("wxMessageDialog()","wx/msgdlg.h")
		have_func("wxGenericMessageDialog()","wx/generic/msgdlgg.h")
		have_func("wxRichMessageDialog()","wx/richmsgdlg.h")
		
		#check for instance methods, that classes need to have default constuctor
		have_member_func("wxFontPickerCtrl","GetSelectedColour","wx/fontpicker.h")
		have_member_func("wxInfoBar","GetButtonCount","wx/infobar.h")
		
    have_member_func("wxOwnerDrawnComboBox","IsListEmpty","wx/odcombo.h")
		
    
		
		#check for enum flags
		have_const("wxFD_NO_FOLLOW","wx/filedlg.h")
		have_const("wxDIRCTRL_DEFAULT_STYLE",["wx/wx.h", "wx/dirctrl.h"])
		
	}
else
		abort("wx-config executable not found!")

end

$defs.push("-DRUBY_UNTYPED_DATA_WARNING=1")

#drop some of the warn flags because they are not valid for C++
CONFIG["warnflags"].gsub!(
	Regexp.union(
		"-Wdeclaration-after-statement",
		"-Wimplicit-function-declaration",
		"-Wextra" #wxAUI is a bit buggy
	), "")

create_header
create_makefile "rwx"
