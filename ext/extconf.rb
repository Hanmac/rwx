require "mkmf"

# add check for member function, does need default constructor for class
def have_member_func(klass,member,header)
	if have_func("#{klass}().#{member}()",header)
		$defs[-1] = "-DHAVE_#{klass.tr_cpp}_#{member.tr_cpp}" 
	end
end

unless have_macro("HAVE_RB_DATA_TYPE_T_PARENT")
	abort("rb_data_type_t needs parent attribute!")
end


dir_config "rwx"

if(wxversion = pkg_config('wx', 'version'))
	
	if wxversion < "3.0.0"
		abort("wx version outdated, please update to 3.0.0 or newer")
	end
	
	ruby_cc = CONFIG["CC"]
	ruby_cxx = CONFIG["CXX"]
	# An ruby extension does need to be build against
	# the same compiler as ruby was
	unless ruby_cc && find_executable(ruby_cc)
		abort("C compiler not found!")
	end
	unless ruby_cxx && find_executable(ruby_cxx)
		abort("C++ compiler not found!")
	end
	
	wxcc = pkg_config('wx', 'cc')
	unless wxcc == ruby_cc
		abort("CC compiler missmatch %s == %s" % [wxcc, ruby_cc])
	end
	
	wxcxx = pkg_config('wx', 'cxx')
	unless wxcxx == ruby_cxx
		abort("CXX compiler missmatch %s == %s" % [wxcxx, ruby_cxx])
	end
	
	#earlier versions of ruby does not have that constant
	$CXXFLAGS = CONFIG["CXXFLAGS"] unless defined?($CXXFLAGS)
	
	#for some function add the base classes
	extra_libs = []
	case pkg_config('wx', 'basename')
	when /gtk2/
		extra_libs << "gtk+-x11-2.0" << "gdk-x11-2.0"
	when /gtk3/
		extra_libs << "gtk+-x11-3.0" << "gdk-x11-3.0"
	end
	
	extra_libs.each {|l|
		pkg = pkg_config(l)
		#because pkg forgot to add the include paths to cxx flags
		$CXXFLAGS << " " << pkg[0] if pkg && !$CXXFLAGS[pkg[0]]
	}
	
	all = " -fvisibility-inlines-hidden"
	$CFLAGS << all << " -x c++ -g -Wall "
	$CXXFLAGS << all << " -g -Wall "
	$CPPFLAGS << all << " -g "
	$LDFLAGS << all << " "
	
	# add the wx-config flags
	$CFLAGS << pkg_config('wx', 'cflags')
	$CXXFLAGS << pkg_config('wx', 'cxxflags')
	$CPPFLAGS << pkg_config('wx', 'cppflags')
	$LDFLAGS << pkg_config('wx', 'libs all')
	
	# TODO add extra check if a lib of wx is missing
	
	c11 = ""
	
	if CONFIG["CC"] =~ /clang/
		if find_library("c++", "free")
			c11 = " -std=c++11 -stdlib=libc++"
		end
	end

	with_cflags(" -x c++ #{c11}") {
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
		have_func("wxBusyInfoFlags()","wx/busyinfo.h")
		
		#check for instance methods, that classes need to have default constuctor
		have_member_func("wxFontPickerCtrl","GetSelectedColour","wx/fontpicker.h")
		have_member_func("wxInfoBar","GetButtonCount","wx/infobar.h")
		
		have_member_func("wxOwnerDrawnComboBox","IsListEmpty","wx/odcombo.h")
		
		#check for enum flags
		have_const("wxFD_NO_FOLLOW","wx/filedlg.h")
		have_const("wxDIRCTRL_DEFAULT_STYLE",["wx/wx.h", "wx/dirctrl.h"])
		have_func("wxDirCtrl()",["wx/wx.h", "wx/dirctrl.h"])
    have_const("wxSTC_LEX_DMAP",["wx/wx.h", "wx/stc/stc.h"])

		have_const("wxALIGN_CENTER_VERTICAL","wx/sizer.h")
		have_member_func("wxSizerFlags","CenterVertical","wx/sizer.h")
	}
else
		abort("wx-config executable not found!")

end

$defs.push("-DRUBY_UNTYPED_DATA_WARNING=1")


drop_warn = [
	"-Wdeclaration-after-statement",
	"-Wimplicit-function-declaration",
	"-Wextra"  #wxAUI is a bit buggy
]

if CONFIG["CC"] =~ /clang/
	drop_warn << "-tokens" #clang dont like it
	drop_warn << "-Wshorten-64-to-32" #clang dont like it
end

#drop some of the warn flags because they are not valid for C++
CONFIG["warnflags"].gsub!(Regexp.union(*drop_warn), "")

with_cppflags(c11) {
  create_header
  create_makefile "rwx"
}
