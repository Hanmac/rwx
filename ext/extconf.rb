require "mkmf"


dir_config "rwx"

if(wx_config = find_executable('wx-config'))

	
	case `#{wx_config} --basename`
	when /gtk2/
		#pkg_config("gdk-x11-2.0")
		#pkg_config("gtk+-x11-2.0")
	when /gtk3/
		pkg_config("gtk-x11-3.0")
		pkg_config("gtk+-x11-3.0")
	end

	$CFLAGS << " -fvisibility-inlines-hidden -x c++ -g -Wall "
	$CPPFLAGS << " -fvisibility-inlines-hidden -g "
	$LDFLAGS << " -fvisibility-inlines-hidden "


	$CFLAGS << `#{wx_config} --cflags`.chomp + " " + `#{wx_config} --cxxflags`.chomp
	$CPPFLAGS << `#{wx_config} --cppflags`.chomp
	$LDFLAGS << `#{wx_config} --libs all`.chomp


	
else
		abort("wx-config executable not found")

end

#drop some of the warn flags because they are not valid for C++
CONFIG["warnflags"].gsub!("-Wdeclaration-after-statement","")
CONFIG["warnflags"].gsub!("-Wimplicit-function-declaration","")

create_makefile "rwx"
