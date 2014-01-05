#Encoding: UTF-8
require "../../ext/rwx"

class MyAboutDialog < WX::AboutDialog
  
  def custom_controls
    add_control(WX::StaticLine,:expand => true)
    add_text("Some custom text")
    add_control(WX::StaticLine,:expand => true)
  end
end

class A < WX::App
	
	def aboutinfo_minimal
		{
			:name => "Dialogs Sample",
			:version => [WX::WXVERSION,WX::WXVERSION_MINOR % 2 == 0 ? "Development" : "Stable" ],
			:description => "This sample shows different wxWidgets dialogs",
			:copyright	=> "(C) 1998-2006 wxWidgets dev team",
			:developers => "Vadim Zeitlin"
		}
	end
	
	def aboutinfo_website
		aboutinfo_minimal.merge(:web_site => ["http://www.wxwidgets.org/","wxWidgets web site"] )
	end
	
	def aboutinfo_all
		aboutinfo_website.merge(
			:developers => "A.N. Other",
			:doc_writers => ["First D. Writer", "Second One"],
			:translators => "Wun Ngo Wen (Martian)",
			:licence => <<LICENCE
                wxWindows Library Licence, Version 3.1
                ======================================

  Copyright (c) 1998-2005 Julian Smart, Robert Roebling et al

  Everyone is permitted to copy and distribute verbatim copies
  of this licence document, but changing it is not allowed.

                       WXWINDOWS LIBRARY LICENCE
     TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

                    ...and so on and so forth...
LICENCE
		) {|key,old,new| Array(old) + Array(new)	}
	end
	
	def choice_dir(exist_flag)
    WX::dir_dialog(@frame,
      :message => "Testing directory picker",
      :path => Dir.home,
      :must_exist => exist_flag 
    )
	end
	
	
	def on_init
	
	
#	
#		#f = WX::Font.new(10,:script)
#		#p f.faceName
		@frame = WX::Frame.new(nil,:icon => "../sample.xpm")
	
		@index = 0
		
		@frame.menubar = WX::MenuBar.new(nil) {|m|
			m.append("&Dialogs") {|menu|
				menu.append_normal("&Choices and selectors") {|select|
					
					select.append_normal(:single_choice,"&Single choice\tCtrl-C") {
						dialog = WX::SingleChoiceDialog.new(@frame,
							:message => "This is a small sample\nA single-choice convenience dialog",
							:title => "Please select a value",
							:choices => %w(one two three four five),
							:selection => 2
						)
						
						if dialog.show_modal == :ok
							WX::message_box(@frame,dialog.string_selection,:caption => "Got string")
						end
					}
					
					select.append_normal(:multi_choice,"M&ultiple choice\tCtrl-U") {
						
					}
					
					select.append_separator
									 
					select.append_normal(:single_choice2,"&Single choice per command") {
						choice = WX::single_choice(
							"This is a small sample\nA single-choice convenience dialog",
							"Please select a value",%w(one two three four five),
							:parent => @frame,:selection => 2
						)
						
						WX::message_box(@frame,choice,:caption => "Got string")
						
					}
					
					select.append_separator
					
					select.append_normal(:rearrange,"&Rearrange dialog\tCtrl-R") {
						dialog = WX::RearrangeDialog.new(@frame,
							:items => "A".."E", :order => 0..4, :message => "Configure the columns shown:"
						)
						dialog.add_extra_controls(WX::Panel) {|pan|
							pan.sizer = WX::BoxSizer.new {|box|
								box.add(WX::StaticText.new(pan))
								box.add(WX::TextCtrl.new(pan))
								box.add(WX::Button.new(pan, :id => :rename, :label => "&Rename"))
							}
						}
						dialog.show_modal
					}
				}
        menu.append_normal("&File operations") {|file|
          file.append_normal(:file_open,"&Open file\tCtrl-O")
          file.append_normal(:file_open2,"&Second open file\tCtrl-2")
          file.append_normal(:files_open,"Open &files\tCtrl-Q")
          file.append_normal(:file_save,"Sa&ve file\tCtrl-S")
        }
        
        menu.append_normal("&Directory operations") {|file|
        file.append_normal(:dir_choose,"&Choose a directory\tCtrl-D") {choice_dir(true)}
          file.append_normal(:dir_choose_new,"Choose a directory (with \"Ne&w\" button)\tShift-Ctrl-D") {choice_dir(false)}
        }
				
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
					
					info.append_separator
					
					info.append_normal(:wx_info,"&wxWidgets information\tCtrl-W") {
						WX::info_message_box(@frame)
					}
					
					
				}
			}
			
      m.append(:edit) {|edit|
        edit << :undo << :redo
        edit.append_separator
        edit << :cut << :copy << :paste << :clear
        edit.append_separator
        edit << :select_all
      }
			
			m.append(:help) {|help|
				help.append_normal(:about_simple,"&About (simple)...\tF1") {
					WX::about_box(@frame,aboutinfo_minimal)
				}
				help.append_normal(:about_fancy,"About (&fancy)...\tShift-F1") {
					WX::about_box(@frame,aboutinfo_website)
				}
				help.append_normal(:about_full,"About (f&ull)...\tCtrl-F1") {
          WX::about_box(@frame,aboutinfo_all)
				}
				help.append_normal(:about_custom,"About (&custom)...\tCtrl-Shift-F1") {
				  about = MyAboutDialog.new(@frame,aboutinfo_all)
				  about.show_modal
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
