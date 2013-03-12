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
						@info_adv.show_message("Sorry, it didn't work out.",:warning)
					}
					
					info.append_separator
					
					info.append_normal(:info_simple_generic,"Simple &info bar (generic)") {
						@index += 1
						@info_generic.show_message("Message #%d in the info bar." % @index) 
					}
					info.append_normal(:info_adv_generic,"&Advanced info bar (generic)") {
						@info_adv_generic.show_message("Sorry, it didn't work out.",:warning)
					}
				}
			}
		}
		
		@frame.sizer = WX::BoxSizer.new {|box|
			box.orientation = true
			
			box.add(@info = WX::InfoBar.new(@frame),:expand => true)
			box.add(@info_adv = WX::InfoBar.new(@frame),:expand => true)
			
			@info_adv.add_button(:undo)
			@info_adv.add_button(:redo) { @info_adv.show_message("Still no, sorry again.",:error) }
			
			@info_adv.add_button(:exit)
			@info_adv.remove_button(:exit)
			
			@info_adv.background_color = 0xc8ffff

			#@info_adv.font.weight = :bold
			f = @info_adv.font
			f.larger!
			f.weight = :bold
			@info_adv.font = f
			
			
			box.add(@info_generic = WX::InfoBarGeneric.new(@frame),:expand => true)
			box.add(@info_adv_generic = WX::InfoBarGeneric.new(@frame),:expand => true)
			
			@info_adv_generic.add_button(:undo)
			@info_adv_generic.add_button(:redo) { @info_adv_generic.show_message("Still no, sorry again.",:error) }
			
			@info_adv_generic.add_button(:exit)
			@info_adv_generic.remove_button(:exit)
			
			@info_adv_generic.background_color = 0xc8ffff

		}	
		
		@frame.show
	end
end

A.new.main_loop
