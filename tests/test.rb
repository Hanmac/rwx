#Encoding: UTF-8
require "../ext/rwx"
require "../../tiled_tmx/map"
require "nokogiri"

module TiledTmx
	class Tileset

		def images
			if @images.nil?
				@images = []
				image = WX::Image.new(@source.to_s)
				@margin.step(image.height-1,@tileheight + @spacing) {|iy|
					@margin.step(image.width-1,@tilewidth + @spacing) {|ix|
						@images << image[WX::Rect.new(ix,iy,@tilewidth,@tileheight)]
					}
				}
			end
			return @images
		end
		def draw(id,x,y,z,opacity,rot,x_scale,y_scale,&block)
			block.call.draw_bitmap(images[id].scale(x_scale,y_scale) * opacity,x,y)
		end
	end
end


STDERR.reopen("file.log","w")

class MapDialog < WX::Dialog

	def initialize(parent)
		super(parent,:title => "Map Properies",:style => DEFAULT_STYLE | RESIZE_BORDER)
		
		self.sizer = WX::BoxSizer.new {|box|
			box.orientation = true
			sbs = WX::StaticBox.new(self,:label => "Eigenschaften:").containing_sizer
			sbs.orientation = true
			sbs.add(create_text_sizer("Name:"))
			sbs.add(WX::TextCtrl.new(self),:expand => true)
			sbs.add(create_text_sizer("Anzeigename:"))
			sbs.add(WX::TextCtrl.new(self),:expand => true)
			
			sbs.add(create_text_sizer("Abmessungen:"))
			sbs.add(WX::BoxSizer.new {|siz|
				siz.add(WX::SpinCtrl.new(self))
				siz.add(create_text_sizer("×"))
				siz.add(WX::SpinCtrl.new(self))
			},:expand => true)
			
			box.add(sbs,:expand => true)
			
			sbs = WX::StaticBox.new(self,:label => "SystemSwitches:").containing_sizer
			sbs.orientation = true
			#sizer.orientation = true
			sbs.add(WX::CheckBox.new(self,:label => "Rennverbot ändern:"))
			sbs.add(WX::BoxSizer.new {|siz|
				siz.add(WX::Choice.new(self,:items =>["An","Aus","Eltern-Map"],:select=>0))
			},:expand => true)
			sbs.add(WX::CheckBox.new(self,:label => "Speicherverbot ändern:"))
			sbs.add(WX::BoxSizer.new {|siz|
				siz.add(WX::RadioButton.new(self,:label => "An",:group => true,:disabled=>true))
				siz.add(WX::RadioButton.new(self,:label => "Aus",:disabled=>true))
				siz.add(WX::RadioButton.new(self,:label => "Eltern-Map",:disabled=>true))
			},:expand => true)
			
			sbs.add(WX::BoxSizer.new {|siz|
				siz.add(WX::RadioButton.new(self,:label => "An",:group => true,:disabled=>true))
				siz.add(WX::RadioButton.new(self,:label => "Aus",:disabled=>true))
				siz.add(WX::RadioButton.new(self,:label => "Eltern-Map",:disabled=>true))
			},:expand => true)
			
			box.add(sbs,:expand => true)
			box.add(create_button_sizer(:ok,:cancel,:help,:apply),:expand => true)

		}
		fit
		layout
	end

end

class TilesetGridTable < WX::GridTable

	attr_reader :set

	def initialize(set)
		@set = set
	end
	
	def rows
		return @set.height / (@set.tileheight + @set.spacing)
	end
	def cols
		return @set.width / (@set.tilewidth + @set.spacing)
	end
	
	def [](row,col)
		return @set.images[row * cols + col]
	end
	def typename(x,y)
		return "string"
	end
	def tilesize
		@size ||= WX::Size.new(@set.tilewidth-9,@set.tileheight-6)
		#@size ||= WX::Size.new(@set.tilewidth,@set.tileheight)
		return @size
	end

end

class TilesetGridRenderer < WX::GridCellRenderer
	def draw(grid,att,dc,rect,row,col,selected)
		dc.draw_bitmap(grid.table[row,col],rect.x,rect.y)
	end
	def best_size(grid,att,dc,row,col)
		return grid.table.tilesize
	end
end

class Cat < WX::CategoryProperty 
	def initialize
		self.name = "Font1"
		self.label = "Font2"
	end
end

class A < WX::App
	def on_init
	
	
#	
#		#f = WX::Font.new(10,:script)
#		#p f.faceName
		@frame = WX::Frame.new(nil)
		@frame.title = "hallo"

#		pen1 = WX::Pen.new
#		pen2 = WX::Pen.new
#		pen1.width = 2
#		p pen2.color
#		#p @frame.icon
#		@history = WX::FileHistory.new
#		@history << "b" << "a"
#		#f= @frame.font
#		#p f.weight
#		#c = WX::Control.new
#		#g = WX::Gauge.new(@frame)
#		#g = WX::Gauge.allocate
#		#g.range = 10
#		#g.value = 5
#		#@history.save
#		#g.foregroundColour = WX::Color.new(255,0,0)
#		# g.foregroundColour
#		#g.pulse
#		#g.pulse
		@frame.menuBar = WX::Menu::Bar.new(nil) {|m|
			m.append("Datei") {|menu|
				menu << :new
				menu.appendNormal(:open) {|e|
				
					p e.id
					wiz = WX::Wizard.new(@frame)
					pages = []
					pages << wiz.addPage(WX::WizardPage) {|page| }
					#pages << wiz.addPage(MyPage) {|page| p page}
					pages << wiz.addPage(WX::WizardPage,:id => :hello) {|page|}
					wiz.chainPages(*pages)
#					
					wiz.showPage(pages[0])
					wiz.show_modal
					#wiz.runWizard(pages[0])
				}
#				menu.appendNormal("last files") {|o| @history >> o }
#				menu << 5003
				menu.appendNormal(:about) {|e|
					WX.about_box(
						:icon => "ruby32x32.png",
						:developers => "Hanmac",
						:translators => "Quintus <sutniuq@gmx.net>",
						:artists => ["Tango project ( http://tango.freedesktop.org )",
						"Yukihiro Matsumoto ( http://www.rubyidentity.org )"],
						:copyright => "Copyright © 2010 OpenRubyRMK Team",
						:web_site => "http://devel.pegasus-alpha.eu/projects/openrubyrmk"
					)
				}
				menu.appendNormal(:exit) {
					@frame.close(false)#(true)
					#WX.exit
				}
			@frame.bind(:menu_selected) {|e| p e.id}
			}
			
		}
#		#i=WX::Image.new("new16x16.png")


		t = @frame.createToolBar

#		#t.addNormal(WX::TimePickerCtrl)
#		#t.addNormal(WX::RadioButton) {|g| g.label_text = "abc" }
		#t.addNormal(WX::ColorPicker){|g|
		#	g.bind(:colorpicker_changed) {|e| p e.color }
		#}
		#t.addNormal(WX::Slider){|g|
		#	g.value = 5
		#	g.max = 10
		#}
		#t.addNormal(WX::Button,:id => :cancel)
		
		#t.addNormal(WX::FontPicker) {|g|
#		}
#		t.addNormal(WX::StaticBitmap) {|g|
#			g.bitmap = "new16x16.png"
#		}

#		t.addNormal(WX::HyperLink) {|g|
#			g.label =  "abc"
#			g.url = "http://google.de"
#		}
#		#time	= Time.now
#		#c=WX::TimePickerCtrl.new(@frame)
#		#c.value = time
#		#p time, c.value
#		
#		#c=WX::RadioButton.new(@frame)
#		#c.label_text = "abc"
##		t.addNormal(WX::TimePickerCtrl.new(t))
#		
		sc = WX::ColorPicker.new(@frame)
		
		sc.each_child {|c| p c}
		
		t.addNormal(:new,nil) {|e|
			MapDialog.new(@frame).show_modal
		}
#			#@t = WX::Timer.new() {|e| p e }
#			#@t.start(2000)
#			#WX::ProgressDialog.new(nil,:title=>"title") { |pd|p 1; pd.update(50,"message") }
#			#@p.update(50,"message")
#			#@p.pulse("pulseing")
#			
#			#WX.busy {
#			#	gets
#			#}
#		}
		#t.addNormal(-1,nil,"new16x16.png"){
#			#p WX.dir_dialog
		#	d=WX::DirDialog.new(@frame)
#			#d.path = ".."
		#	d.show_modal
#			#p d.path
#			
#			#cdlg=WX::ColorDialog.new(@frame)
#			#cdlg.custom_colors = [WX::Color.new(255,0,0)]
#			#cdlg.show_modal
		#}
#		t.addRadio(5003,nil,"new16x16.png")
#		t.addRadio(5003,nil,"new16x16.png")
#		
#		
#		#p t.each_tool.to_a
		t.realize
		
		#p @frame.createStatusBar
#		popup = WX::Menu.new("") {|menu| menu.appendNormal(:new_menu,"new Map") }
		doc = File.open("project.xml"){|f| Nokogiri::XML(f) }

#=begin

		@frame.aui {|aui|
##		
##			tb = WX::ToolBar.new(@frame) 
##			tb.addNormal(:new,nil)
##		
			aui.add(WX::ChoiceBookCtrl.new(@frame,:id=>:tilemap))
			
			WX::TreeCtrl.new(@frame,:id=>:treemap) {|tc|
				tc.root(doc.root[:name]) {|item|
					add_map_to_tree(item,doc.root)
				}
				tc.bind(:tree_item_activated) {|e|
					map = e.item.instance_variable_get(:@map)
					@frame[:paint].add_page(WX::Window,e.item.text) {|pwnd|
						pwnd.instance_variable_set(:@map,map)
						pwnd.bind(:paint) {
							pwnd.draw {|dc|
								x = 10
								y = 10
								map.draw(x,y,0) { dc }
								
								pen = WX::Pen.new
								pen.color = WX::Color.new(255,0,0)
								dc.pen = pen
								
								brush = WX::Brush.new
								brush.color = WX::Color.new(255,0,0,128)
								dc.brush = brush
								
								point = pwnd.screen_to_client(WX.mouse_position)
								xi = (point.x - x) / map.tilewidth
								yi = (point.y - y)/ map.tileheight
								x += xi * map.tilewidth
								y += yi * map.tileheight
								grid = @frame[:tilemap].current_page
								#p grid.selected_cells, grid.selected_topleft, grid.selected_bottomright
								if xi >= 0 && xi < map.width && yi >= 0 && yi < map.height && grid.selection?
								
									cells = grid.selected_cells
									unless cells.empty?
										x_off = (cells.map {|x,y| x}.minmax.inject(:+) / 2.0).ceil
										y_off = (cells.map {|x,y| y}.minmax.inject(:+) / 2.0).ceil
									
										cells.each {|xi,yi|
											rect = WX::Rect.new(
												x + (xi-x_off)*map.tilewidth,
												y + (yi-y_off)*map.tileheight,
												map.tilewidth,map.tileheight
											)
										
											dc.draw_rectangle(rect)
											dc.draw_bitmap(grid.table[yi,xi] * 0.5,
												rect.x,rect.y)

										}
									end
#									tl = grid.selected_topleft
#									br = grid.selected_bottomright
#									
#									tl.size.times {|i|

#										rect = WX::Rect.new(
#											x + map.tilewidth * (tl[i][0] - tl[0][0]),
#											y + map.tileheight * (br[i][1] - br[0][1]),
#											map.tilewidth * (br[i][0] - tl[i][0] + 1),
#											map.tileheight * (br[i][1] - tl[i][1] + 1)
#										)
#										dc.draw_rectangle(rect)

#										tl[i][0].upto(br[i][0]).with_index {|xi,xii|
#											tl[i][1].upto(br[i][1]).with_index {|yi,yii|
#												dc.draw_bitmap(
#													grid.table[yi,xi] * 0.5,
#													x + map.tilewidth * (0 + xii),
#													y + map.tileheight * (0 + yii)
#												)
#											}
#										}
#										
#									}


#									
##									if !tl.nil? && !br.nil?
##										tl[0].upto(br[0]).with_index {|xi,xii|
##											tl[1].upto(br[1]).with_index {|yi,yii|
##												dc.draw_bitmap(grid.table[yi,xi] * 0.5,
##													x + map.tilewidth * xii,y + map.tileheight * yii)
##											}
##										}
##									end
								end
							}
						}
						pwnd.bind(:motion) {|e|
							
							pwnd.refresh
						}
						
						
#						pwnd.bind(:left_down) {|e|
#							layer = @frame[:side][:layerlist].selection.instance_variable_get(:@layer)

#								x = 10
#								y = 10
#								point = pwnd.screen_to_client(WX.mouse_position)
#								x = (point.x - x) / map.tilewidth
#								y = (point.y - y)/ map.tileheight
#								grid = @frame[:tilemap].current_page

#								if x >= 0 && x < map.width && y >= 0 && y < map.height && grid.selection?
#									tl = grid.selected_topleft
#									p tl
#									tl = tl[0]
#									br = grid.selected_bottomright[0]

#									br = tl if br.nil?

#									if !tl.nil? && !br.nil?
##										tl[0].upto(br[0]).with_index {|xi,xii|
##											tl[1].upto(br[1]).with_index {|yi,yii|
##												dc.draw_bitmap(grid.table[yi,xi] * 0.5,
##													x + map.tilewidth * xii,y + map.tileheight * yii)
##											}
##										}
#										table = grid.table
#										id = map.tilesets.key(table.set)
#										layer[y * map.width + x] =  id + tl[0] + table.cols * tl[1]
#										#p x,y,tl,br
#									end
#								
#								end
#						}
					} if map
				
				}
				
				
##					tc.bind(:tree_item_menu) {|e|
##							tc.bind(:menu_selected,:new_menu) {p e.item.instance_variable_get(:@map_data)}
##						tc.popupmenu(popup)
##						}
				aui.add(tc)
			}


		WX::AuiNoteBookCtrl.new(@frame,:id => :paint) {|nbc|
		
#			nbc.add_page(WX::Window,"map1",:id=>:ok) {|pwnd|
#				pwnd.bind(:paint) {
#					pwnd.draw {|dc|
#						@map1.draw(0,0,0) { dc }
#					}
#				}
#			}
			nbc.bind(:auinotebook_page_changed) {|e|
				dv = @frame[:side][:layerlist]
				dv.delete_items
				map = nbc.page(e.selection).instance_variable_get(:@map)
				if map
					map.tilesets.each_value{|s|
						@frame[:tilemap].add_page(WX::Grid,s.name) {|grid|
							grid.default_renderer = TilesetGridRenderer.new
							grid.table = TilesetGridTable.new(s)
							grid.row_label_size = 0
							grid.col_label_size = 0
							
							grid.editable = false
							
							grid.autosize
							
							grid.bind(:grid_select_cell) {|e| p :select }
						}
					}

					map.layers.each {|z,l|
						dv.prepend_item(l.visible,"new16x16.png",l.name).instance_variable_set(:@layer,l)
					}
				end
			}
			
			aui.add(nbc,:direction=>:center)
		}

#		
		

			WX::Window.new(@frame,:id=>:side) {|wnd|
				wnd.sizer = WX::BoxSizer.new {|siz|
					siz.orientation = true

					siz.add(WX::BoxSizer.new {|bar_siz|
						bar_siz.add(WX::StaticText.new(wnd, :label => "Sichtbarkeit:"),:align=>:center)
						
						bar_siz.add(WX::Slider.new(wnd,
							:min => 0,:max => 256,:value => 128,:id=>:slider) {|slider|
								slider.bind(:scroll_changed) {|e|
									sel = wnd[:layerlist].selection.instance_variable_get(:@layer)
									sel.opacity = e.position / 256.0 if sel
									@frame[:paint].refresh if @frame[:paint]
								}
								slider.bind(:scroll_thumbtrack) {|e|
									sel = wnd[:layerlist].selection.instance_variable_get(:@layer)
									sel.opacity = e.position / 256.0 if sel
									@frame[:paint].refresh if @frame[:paint]
								}
								
							},
							:expand => true,:proportion=>1)
					},:expand => true)

					siz.add(WX::DataViewList.new(wnd,:id=>:layerlist) {|dv|
						dv.append_toggle_column("toogle")
						dv.append_icontext_column("icon")
						dv.append_text_column("text")

#						@map1.layers.each {|z,l|
#							dv.prepend_item(l.visible,"new16x16.png",l.name).instance_variable_set(:@layer,l)
#						}
						dv.bind(:dataview_selection_changed) {|e|
							unless e.item.nil? || wnd[:slider].nil?
								wnd[:slider].value = e.item.instance_variable_get(:@layer).opacity * 256
							end
						}
						dv.bind(:dataview_value_changed) {|e|
							case e.column
							when 0
								e.item.instance_variable_get(:@layer).visible = e.value
							end
							@frame[:paint].refresh if @frame[:paint]
						}
						},:expand => true,:proportion=>1)
			
					siz.add(WX::ToolBar.new(wnd) {|tb|
						tb.addNormal(:new,nil)
					})
				}
				wnd.layout
				aui.add(wnd)
			}
##			
##			#aui.add(tb)



		#tb = WX::AuiToolBar.new(@frame)
		#tb.addNormal(:new,nil)
#		tb.addNormal(:new,nil)
		#aui.add(tb)
##			#aui['tree'] = @tc
##			#aui['tb'] = tb
		}
#=end
#		pg = WX::PropertyGrid.new(@frame,:extra_style => WX::PropertyGrid::EX_HELP_AS_TOOLTIPS)

#		pg = WX::PropertyGridManager.new(@frame,
#			:style => WX::PropertyGridManager::TOOLBAR,
#			:extra_style => WX::PropertyGridManager::EX_MODE_BUTTONS
#		)

#		fp = WX::CategoryProperty.new
#		fp.name = "Font1"
#		fp.label = "Font2"
		
#		pg.append(Cat)
=begin
		map = TiledTmx::Map.load_xml("../../../unbenannt.tmx")
		grid = WX::Grid.new(@frame)
		grid.table = TilesetGridTable.new(map.tilesets[1])
		
		grid.row_label_size = 0
		grid.col_label_size = 0
		
		grid.default_renderer = TilesetGridRenderer.new

		grid.autosize
=end
		#stc = WX::STC.new(@frame)
		#stc.use_tabs = true
		#stc.text = File.read("test.rb")
		#stc.test_style
		
		#p WX::STC::Version
		#nbc.add_page(WX::Button,"cancel",:id => :cancel)
		#		WX::TreeCtrl.new(@frame)
#		tc.bind(:tree_item_gettooltip) {|e|
#			p e.item
#		}
#		slider = WX::Slider.new(@frame)
#		slider.max = 10
#		slider.value = 5
		

#		@frame.bind(:paint) {
#			@frame.draw {|dc|
#				@map1.draw(0,0,0) { dc }
#			}
#		}

		@frame.show
#		#p @frame.each.to_
	end
	
	def add_map_to_tree(item,map)
		item.instance_variable_set(:@map,TiledTmx::Map.load_xml(map[:file])) if map[:file]
		map.xpath("map").each {|m| add_map_to_tree(item.append(m[:name]),m) }
	end
end

A.new.main_loop
