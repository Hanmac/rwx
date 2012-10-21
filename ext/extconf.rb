require "mkmf"
$CFLAGS += " -fvisibility-inlines-hidden -x c++ -g -Wall "
$CPPFLAGS += " -fvisibility-inlines-hidden "
$LDFLAGS += " -fvisibility-inlines-hidden "

dir_config "rwx"

if(wx_config = find_executable('wx-config'))
	$CFLAGS += `#{wx_config} --cflags`.chomp + " " + `#{wx_config} --cxxflags`.chomp
	$CPPFLAGS += `#{wx_config} --cppflags`
	$LDFLAGS += `#{wx_config} --libs all`
else
		abort("wx-config executable not found")

end

create_makefile "rwx"
