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

if(wx_config = find_executable('wx-config'))
    
    if `#{wx_config} --version`.chomp < "3.0.0"
        abort("wx version outdated, please update to 3.0.0 or newer")
    end
    
    ruby_cc = RbConfig::CONFIG["CC"]
    ruby_cxx = RbConfig::CONFIG["CXX"]
    # An ruby extension does need to be build against
    # the same compiler as ruby was
    unless ruby_cc && find_executable(ruby_cc)
        abort("C compiler not found!")
    end
    unless ruby_cxx && find_executable(ruby_cxx)
        abort("C++ compiler not found!")
    end
    
    cc = `#{wx_config} --cc`.chomp
    
    cxx = `#{wx_config} --cxx`.chomp
    cxxversion_wx = `#{cxx} -v 2>&1`.split("\n")
    cxxversion_rb = `#{ruby_cxx} -v 2>&1`.split("\n")
    ccversion_wx = `#{cc} -v 2>&1`.split("\n")
    ccversion_rb = `#{ruby_cc} -v 2>&1`.split("\n")
    #puts ccversion_wx[0],ccversion_rb[0]
    unless ccversion_rb.include?(ccversion_wx[0])
        abort("CC compiler missmatch %s == %s" % [ccversion_wx, ccversion_rb])
    end
    unless cxxversion_rb.include?(cxxversion_wx[0])
        abort("CXX compiler missmatch %s == %s" % [cxxversion_wx,cxxversion_rb])
    end
    puts "compilers matched"
    #earlier versions of ruby does not have that constant
    #remove bad paths in flags
    rmnonpaths = lambda {|x|
        if (x[0,2]=="-L" ||x[0,2]=="-I") then
            if File.exist?(x[2,x.length-2]) then
                x
            else
                puts "removing %s"%x 
                nil
            end
        else
            x
        end
    }
    puts "removing nonexistent paths"
    if (RbConfig::CONFIG["CXXFLAGS"]) then
        $CXXFLAGS = RbConfig::CONFIG["CXXFLAGS"]
        $CXXFLAGS=$CXXFLAGS.split()
        $CXXFLAGS.map!(&rmnonpaths)
        $CXXFLAGS=$CXXFLAGS.join(" ")
    end
    if (RbConfig::CONFIG["CPPFLAGS"]) then
        $CPPFLAGS = RbConfig::CONFIG["CPPFLAGS"]
        $CPPFLAGS=$CPPFLAGS.split()
        $CPPFLAGS.map!(&rmnonpaths)
        $CPPFLAGS=$CPPFLAGS.join(" ")
    end
    if (RbConfig::CONFIG["CFLAGS"]) then
        $CFLAGS = RbConfig::CONFIG["CFLAGS"]
        $CFLAGS=$CFLAGS.split()
        $CFLAGS.map!(&rmnonpaths)
        $CFLAGS=$CFLAGS.join(" ")
    end
    if (RbConfig::CONFIG["LDFLAGS"]) then
        $LDFLAGS = RbConfig::CONFIG["LDFLAGS"]
        $LDFLAGS=$LDFLAGS.split()
        $LDFLAGS.map!(&rmnonpaths)
        $LDFLAGS=$LDFLAGS.join(" ")
    end
    #set up special flags for testing
    moreflags = ""
    with_cflags(" -x c++ ") {
        moreflags += " -DHAVE_TYPE_TRAITS " if have_header("type_traits")
        moreflags += " -DHAVE_TR1_TYPE_TRAITS " if have_header("tr1/type_traits")
        moreflags += " -DHAVE_STD_UNORDERED_MAP " if have_header("unordered_map")
        moreflags += " -DHAVE_TR1_UNORDERED_MAP " if have_header("tr1/unordered_map")
    }
    print "moreflags set"
    $CXXFLAGS = RbConfig::CONFIG["CXXFLAGS"] unless defined?($CXXFLAGS)
    #for some function add the base classes
    extra_libs = []
    case `#{wx_config} --basename`
        when /gtk2/
        extra_libs << "gtk+-x11-2.0" << "gdk-x11-2.0"
        when /gtk3/
        extra_libs << "gtk+-x11-3.0" << "gdk-x11-3.0"
    end
    puts "fixing",extra_libs
    extra_libs.each {|l|
        pkg = pkg_config(l)
        #because pkg forgot to add the include paths to cxx flags
        $CXXFLAGS << " " << pkg[0] if pkg && !$CXXFLAGS[pkg[0]]
    }
    all = " -fvisibility-inlines-hidden"
    $CFLAGS << all << " -x c++ -g -Wall "
    $CXXFLAGS << all << " -g -Wall "
    $CPPFLAGS << all << " -g -x c++ "
    $LDFLAGS << all << " "
    # add the wx-config flags
    puts "cflags"
    $CFLAGS << `#{wx_config} --cflags`.chomp
    puts `#{wx_config} --cflags`
    puts "cxxflags"
    $CXXFLAGS << `#{wx_config} --cxxflags`.chomp
    puts `#{wx_config} --cxxflags`
    puts "cppflags"
    $CPPFLAGS << `#{wx_config} --cppflags`.chomp
    #puts `#{wx_config} --cppflags`
    puts "ldflags"
    $LDFLAGS << `#{wx_config} --libs all`.chomp
    #puts `#{wx_config} --libs all`
    # TODO add extra check if a lib of wx is missing
    with_cflags(" -x c++ ") {
        # need c++ for some of the tests
        CONFIG["CC"] = CONFIG["CXX"]
        #C++03tr1 c++11 differences
        have_header("type_traits")
        have_header("tr1/type_traits")
        if try_header("unordered_map")
            checking_for "unordered_map" do
                $defs.push(format("-DHAVE_STD_%s", "unordered_map".tr_cpp))
            end
        end
        have_header("tr1/unordered_map")
        if try_header("unordered_set")
            checking_for "unordered_set" do
                $defs.push(format("-DHAVE_STD_%s", "unordered_set".tr_cpp))
            end
        end
    }
    with_cflags(" -x c++ "+moreflags) {
        # need c++ for some of the tests
        CONFIG["CC"] = CONFIG["CXX"]
        #C++03tr1 c++11 differences
        have_header("tr1/unordered_set")
        have_header("wx/preferences.h","wx/defs.h")
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

#drop some of the warn flags because they are not valid for C++
CONFIG["warnflags"].gsub!(
                          Regexp.union(
                                       "-Wdeclaration-after-statement",
                                       "-Wimplicit-function-declaration",
                                       "-Wextra" #wxAUI is a bit buggy
                                       ), "")
#with_cppflags("-std=c++11") {
create_header
create_makefile "rwx"
#}
