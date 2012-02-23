require "mkmf"

$CFLAGS += " -x c++ -g -Wall " + `wx-config --cflags`.chomp + " " + `wx-config --cxxflags`.chomp
$CPPFLAGS += " " + `wx-config --cppflags`
$LDFLAGS += " " + `wx-config --libs`

dir_config "rwx"
create_makefile "rwx"
