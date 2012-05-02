require "mkmf"

$CFLAGS += " -fvisibility-inlines-hidden -x c++ -g -Wall " + `wx-config --cflags`.chomp + " " + `wx-config --cxxflags`.chomp
$CPPFLAGS += " -fvisibility-inlines-hidden " + `wx-config --cppflags`
$LDFLAGS += " -fvisibility-inlines-hidden " + `wx-config --libs all`

dir_config "rwx"
create_makefile "rwx"
