require "mkmf"

# add check for member function, does need default constructor for class
def have_member_func(klass,member,header, *args)
	if have_func("#{klass}().#{member}(#{args.join(', ')})",header)
		$defs[-1] = "-DHAVE_#{klass.tr_cpp}_#{member.tr_cpp}"
	end
end

def have_constructor(klass, header, *args)
	if have_func("#{klass}(#{args.join(', ')})",header)
		vargs = args.grep(String).flat_map {|s| s.scan(/\A\w+/) }.join
		$defs[-1] = "-DHAVE_#{klass.tr_cpp}_#{vargs.tr_cpp}"
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
# there's no actual need to check "compiler compatibility" because Linux compilers make
# compatible ABI calls anyway.
#	unless wxcc == ruby_cc
#		abort("CC compiler missmatch %s == %s" % [wxcc, ruby_cc])
#	end
	
	wxcxx = pkg_config('wx', 'cxx')
#	unless wxcxx == ruby_cxx
#		abort("CXX compiler missmatch %s == %s" % [wxcxx, ruby_cxx])
#	end
	
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
	
	all = " -fvisibility-inlines-hidden -fpermissive"
	$CFLAGS << all << " -x c++ -g "
	$CXXFLAGS << all << " -g "
	$CPPFLAGS << all << " -g "
	$LDFLAGS << all << " "
	
	# add the wx-config flags
	$CFLAGS << pkg_config('wx', 'cflags')
	$CXXFLAGS << pkg_config('wx', 'cxxflags')
	$CPPFLAGS << pkg_config('wx', 'cppflags')
	$LDFLAGS << pkg_config('wx', 'libs all')
	
	# TODO add extra check if a lib of wx is missing
	
	c11 = ""

	# wxwidgets is build against c++11 need to do it too if possible
	if have_macro("HAVE_TYPE_TRAITS","wx/setup.h")
		c11 = "-std=c++11"
	end

	# when using clang forceunwind is not defined (forceunwind is only in stdc++)
	if CONFIG["CC"] =~ /clang/ and !have_macro("HAVE_ABI_FORCEDUNWIND","wx/setup.h")
		# try to set libc++ as stdlib
		with_cflags(" -x c++ #{c11} -stdlib=libc++") {
			# when it does find the version in ciso646 it was successful
			if have_macro("_LIBCPP_VERSION","ciso646")
				c11 << " -stdlib=libc++"
			end
		}
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
		have_func("wxGenericMessageDialog()",["wx/wx.h", "wx/generic/msgdlgg.h"])
		have_func("wxRichMessageDialog()","wx/richmsgdlg.h")
		have_func("wxBusyInfoFlags()","wx/busyinfo.h")
		have_func("wxCollapsibleHeaderCtrl()",["wx/wx.h", "wx/collheaderctrl.h"])

		#check for instance methods, that classes need to have default constuctor
		have_member_func("wxFontPickerCtrl","GetSelectedColour","wx/fontpicker.h")
		have_member_func("wxInfoBar","GetButtonCount","wx/infobar.h")

		have_member_func("wxOwnerDrawnComboBox","IsListEmpty","wx/odcombo.h")

		#check for enum flags
		have_const("wxFD_NO_FOLLOW","wx/filedlg.h")
		have_const("wxDIRCTRL_DEFAULT_STYLE",["wx/wx.h", "wx/dirctrl.h"])
		have_func("wxDirCtrl()",["wx/wx.h", "wx/dirctrl.h"])
		
		have_const("wxSTC_LEX_DMAP",["wx/wx.h", "wx/stc/stc.h"])
		have_const("wxSTC_LEX_TEHEX",["wx/wx.h", "wx/stc/stc.h"])

		have_constructor("wxBitmap",["wx/wx.h", "wx/bitmap.h"], "wxImage(10, 20)", 32, 1.0)

		have_member_func("wxColourData","GetChooseAlpha",["wx/wx.h", "wx/colourdata.h"])

		have_member_func("wxStyledTextCtrl","GetTargetText",["wx/wx.h", "wx/stc/stc.h"])
		have_member_func("wxStyledTextCtrl","IndicatorGetHoverStyle",["wx/wx.h", "wx/stc/stc.h"], 0)

		have_member_func("wxStyledTextCtrl","SetTargetRange",["wx/wx.h", "wx/stc/stc.h"], 0, 0)

		have_member_func("wxStyledTextCtrl","GetCaretLineVisibleAlways",["wx/wx.h", "wx/stc/stc.h"])

		have_member_func("wxStyledTextCtrl","GetLexerLanguage",["wx/wx.h", "wx/stc/stc.h"])

		have_member_func("wxListCtrl","HasCheckBoxes", ["wx/wx.h", "wx/listctrl.h"])

		have_member_func("wxScrollBar","SetThumbSize",["wx/wx.h", "wx/scrolbar.h"], 10)

		have_const("wxALIGN_CENTER_VERTICAL","wx/sizer.h")
		have_member_func("wxSizerFlags","CenterVertical","wx/sizer.h")
		have_member_func("wxWindow","FromDIP","wx/wx.h", 1)
		have_member_func("wxWindow","ToDIP","wx/wx.h", 1)

		have_member_func("wxTextCtrl","ForceUpper","wx/wx.h")
		have_member_func("wxTextEntryDialog","ForceUpper","wx/wx.h")
	}
else
		abort("wx-config executable not found!")

end

$defs.push("-DRUBY_UNTYPED_DATA_WARNING=1")

# sorry, unimplemented!
CONFIG["optflags"].gsub!("-fexcess-precision=standard", "")

drop_warn = [
	"-Wdeclaration-after-statement",
	"-Wimplicit-function-declaration",
	"-Wimplicit-int",
	"-Wextra",  #wxAUI is a bit buggy

	'-Wno-self-assign',
	'-Wno-constant-logical-operand',
	'-Wno-parentheses-equality',
	'-Wno-tautological-compare'
]

if CONFIG["CC"] =~ /clang/
	drop_warn << "-Wall"
	drop_warn << "-tokens" #clang dont like it
	drop_warn << "-Wshorten-64-to-32" #clang dont like it, and wx have to many
	drop_warn << "-Winconsistent-missing-override" #currently wx does have to many
	drop_warn << "-Wpotentially-evaluated-expression" #currently wx does have to many
end


#drop some of the warn flags because they are not valid for C++
negate_warn = drop_warn.reject {|s| CONFIG["warnflags"].gsub!(s, "") }

negate_warn.reject! {|s| s["-Wno-"] }

#negate some warn flags that i want to removed TODO need to test if that works
CONFIG["warnflags"] << " " << negate_warn.map {|s| s.gsub("-W", "-Wno-") }.join(" ")

with_cppflags(c11) {
	create_header
	create_makefile "rwx"
}
