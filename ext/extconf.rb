require "mkmf"
$CFLAGS << " -fvisibility-inlines-hidden -x c++ -g -Wall "
$CPPFLAGS << " -fvisibility-inlines-hidden -g "
$LDFLAGS << " -fvisibility-inlines-hidden "

dir_config "rwx"

if(wx_config = find_executable('wx-config'))
	$CFLAGS << `#{wx_config} --cflags`.chomp + " " + `#{wx_config} --cxxflags`.chomp
	$CPPFLAGS << `#{wx_config} --cppflags`
	$LDFLAGS << `#{wx_config} --libs all`
	
	case `#{wx_config} --basename`
	when /gtk2/
		p "gtk2"
		pkg_config("gdk-x11-2.0")
		pkg_config("gtk+-x11-2.0")
	when /gtk3/
		p "gtk3"
		pkg_config("gtk-x11-3.0")
		pkg_config("gtk+-x11-3.0")
	end
	
	
else
		abort("wx-config executable not found")

end

create_makefile "rwx"
