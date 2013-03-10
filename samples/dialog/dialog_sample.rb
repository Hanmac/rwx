#Encoding: UTF-8
require "../../ext/rwx"

class A < WX::App
	def on_init
	
	
#	
#		#f = WX::Font.new(10,:script)
#		#p f.faceName
		@frame = WX::Frame.new(nil)
	
		@index = 0
		
		@frame.menubar = WX::MenuBar.new(nil) {|m|
			m.append("&Dialogs") {|menu|			
				menu.append_normal("&Informative dialogs") {|info|
					info.append_normal(:info_simple,"Simple &info bar\tCtrl-I") {
						@index += 1
						@info.show_message("Message #%d in the info bar." % @index) 
					}
					info.append_normal(:info_adv,"&Advanced info bar\tShift-Ctrl-I") {
					
						@info_adv.show_message("Sorry, it didn't work out.")
					}
				}
			}
		}
		
		@frame.sizer = WX::BoxSizer.new {|box|
			box.orientation = true
			
			box.add(@info = WX::InfoBar.new(@frame),:expand => true)
			box.add(@info_adv = WX::InfoBar.new(@frame),:expand => true)
		}	
		
		@frame.show
	end
end

A.new.main_loop
