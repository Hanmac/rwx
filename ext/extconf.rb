require "mkmf"

# add check for member function, does need default constructor for class
def have_member_func(klass,member,header)
    if have_func("#{klass}().#{member}()",header)
        $defs[-1] = "-DHAVE_#{klass.tr_cpp}_#{member.tr_cpp}"
    end
end

def have_std_header(header, preheaders = nil, opt = "", &b)
    checking_for header do
        if try_header(cpp_include(preheaders)+cpp_include(header), opt, &b)
            $defs.push(format("-DHAVE_STD_%s", header.tr_cpp))
            true
        else
            false
        end
    end
end

def pkg_conf(pkg)
    if (pkglibs = pkg_config(pkg,"libs")) then
        if (pkgcinc = pkg_config(pkg,"cflags-only-I")) then
            pkgcflags = pkg_config(pkg,"cflags-only-other")
            else
            pkgcflags =  pkg_config(pkg,"cflags")
        end
        pkglibsonly = pkg_config(pkg,"libs-only-l")
        pkgldflags = (Shellwords.shellwords(pkglibs) - Shellwords.shellwords(pkglibsonly)).quote.join(" ")
        $CFLAGS += " " << pkgcflags
        $CFLAGS = $CFLAGS.split.uniq.join(" ")
        $CXXFLAGS += " " << pkgcflags
        $CXXFLAGS = $CXXFLAGS.split.uniq.join(" ")
        $INCFLAGS += " " << pkgcinc
        $INCFLAGS = $INCFLAGS.split.uniq.join(" ")
        $libs += " " << pkglibsonly
        $libs = $libs.split.uniq.join(" ")
    else
        abort("package configuration for %s is missing\n" % [pkg])
    end
end

unless have_macro("HAVE_RB_DATA_TYPE_T_PARENT")
    abort("rb_data_type_t needs parent attribute!")
end


dir_config "rwx"

if(wxversion = pkg_config("wx","version"))
    
    if wxversion.chomp < "3.0.0"
        abort("wx version outdated, please update to 3.0.0 or newer")
    end
    # ruby compilers
    ruby_cc = find_executable CONFIG["CC"]
    ruby_cxx = find_executable CONFIG["CXX"]
    # An ruby extension does need to be build against
    # the same compiler as ruby was
    unless ruby_cc && find_executable(ruby_cc)
        abort("C compiler %s not found!"%ruby_cc)
    end
    unless ruby_cxx && find_executable(ruby_cxx)
        abort("C++ compiler not found!")
    end
    # wx compilers
    cc = find_executable Shellwords.shellwords(pkg_config("wx","cc"))[0]
    cxx = find_executable  Shellwords.shellwords(pkg_config("wx","cxx"))[0]
    # check if cc compilers are matched
    unless (cc == ruby_cc)
        unless RbConfig::CONFIG["host_os"].include?("darwin")
            abort("CC compiler missmatch %s == %s" % [cc, ruby_cc])
        else # Apple Mac OS X
            optwx = IO.popen(["#{cc}","--version"],:err=>[:child,:out],&:read)
            optrwx = IO.popen(["#{ruby_cc}","--version"],:err=>[:child,:out],&:read)
            unless optrwx.include?(optwx)
                # good to go
            else
                # good to go
            end
        end
    end
    # check if cxx compilers are matched
    unless (cxx == ruby_cxx)
        unless RbConfig::CONFIG["host_os"].include?("darwin")
            abort("CXX compiler missmatch1 %s == %s" % [cxx, ruby_cxx])
        else # Apple Mac OS X
            optwx = IO.popen(["#{cxx}","--version"],:err=>[:child,:out],&:read)
            optrwx = IO.popen(["#{ruby_cxx}","--version"],:err=>[:child,:out],&:read)
            unless optrwx.include?(optwx)
                # need to tell clang to be compatible
                abort("Apple CXX compiler missmatch %s == %s" % [cxx, ruby_cxx]) # clang needs $CFLAGS += " -std=c++11 -stdlib=libc++ -nostdinc++"
            else
                # good to go
            end
        end
    end

    #earlier versions of ruby does not have that constant
    #remove bad paths in flags
    $CPPFLAGS=CONFIG["CPPFLAGS"]
    $CFLAGS=CONFIG["CFLAGS"]
    $CFLAGS=$CFLAGS.split.delete_if {|x| x[0,2]=="-I" && !File.exist?(x[2,x.length-2])}.join(" ")
    $CXXFLAGS=CONFIG["CXXFLAGS"]
    $LDFLAGS=CONFIG["LDFLAGS"]
    $LDFLAGS=$LDFLAGS.split.delete_if {|x| x[0,2]=="-L" && !File.exist?(x[2,x.length-2])}.join(" ")


    wxpkg = pkg_config("wx","basename")
    case wxpkg
        when /gtk2/
        pkg_conf("gdk-x11-2.0")
        pkg_conf("gtk+-x11-2.0")
        when /gtk3/
        pkg_conf("gdk-x11-3.0")
        pkg_conf("gtk+-x11-3.0")
    end
    wxcppflags=pkg_config("wx","cppflags")
    $CPPFLAGS += " " << wxcppflags
    wxcflags=pkg_config("wx","cflags")
    $CFLAGS += " " << wxcflags
    wxcxxflags=pkg_config("wx","cxxflags")
    $CXXFLAGS += " " << wxcxxflags
    wxlibs=pkg_config("wx","libs all")
    $libs += " " << wxlibs
    wxldflags=pkg_config("wx","linkdeps")
    $LDFLAGS << " " << wxldflags
    #for some function add the base classes
    all = " -fvisibility-inlines-hidden"
    $CFLAGS << all << " -x c++ -g -Wall "
    $CXXFLAGS << all << " -g -Wall "
    $CPPFLAGS << all << " -g -x c++ "
    $LDFLAGS << all << " "
    # add the wx-config flags
    # TODO add extra check if a lib of wx is missing
    #set up special flags for testing
    olddefs = $defs
    $defs.clear
    with_cflags(" -x c++ ") {
        have_header("type_traits")
        have_header("tr1/type_traits")
        have_std_header("unordered_map")
        have_header("tr1/unordered_map")
        have_std_header("unordered_set")
        have_header("tr1/unordered_set")
    }
    moreflags = $defs.join(" ")
    $defs = olddefs + $defs
    with_cflags(" -x c++ " + moreflags) {
        # need c++ for some of the tests
        CONFIG["CC"] = CONFIG["CXX"]
        #C++03tr1 c++11 differences
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
    abort("wx-config executable not found!\n for MSWindows see https://sites.google.com/site/wxconfig/")
    
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
