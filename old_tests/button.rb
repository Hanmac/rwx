#Encoding: UTF-8
require "../ext/rwx"

STDERR.reopen("file.log","w")

class A < WX::App
	def on_init

	
		@frame = WX::Frame.new(nil)
		#@frame.title = "hallo"

		@proc = proc {|e|
			p "-----Frame-Bind-----"
			p e,e.id, e.event_type,e.event_object
		}
		@frame.bind(:button_clicked,&@proc)
		@frame.bind(:checkbox_clicked,&@proc)

		@frame.bind(:filepicker_changed){|e| p e.path }
		
		#@frame.bind(:sizing){|e| p "sizing";@frame.call_after { p "after sizing" };e.skip }
		#@frame.bind(:paint,&@proc)

		@frame.sizer = WX::BoxSizer.new {|siz|


			siz.add(WX::Button.new(@frame,id: :ok) {|button|
				button.bind(:button_clicked) {|e|
					p "-----Button-Bind-----"
					p "button clicked"
					WX.about_box(nil)
					button.call_after {
						p "-----After-Bind-----"
						p "after"
					}
					e.skip
				}
			})
			siz.add(WX::CheckBox.new(@frame,label: "checkbox" ) {|checkbox|
				checkbox.bind(:checkbox_clicked) {|e| p "checkbox #{e.checked? ? '' : 'un'}checked" }
			})

			siz.add(WX::Button.new(@frame,id: :cancel) {|button|
				button.bind(:button_clicked) {button.call(:filepicker_changed) {|e| e.path="abc"}}
			})
			siz.add(WX::CheckBox.new(@frame,id: :open,label: "checkbox" ))


		}
		@frame.show
		
	end
end
	
A.new.main_loop
