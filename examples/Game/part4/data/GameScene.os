GameScene = extends Scene {
	__construct = function(){
		super()
		var sky = Sprite().attrs {
			resAnim = res.getResAnim("bg"),
			parent = @view,
		}
		var logo = Sprite()
		logo.resAnim = res.getResAnim("buttons")
		logo.attrs {		
			parent = @view,
			pos = @view.size - logo.size,
		}
		
		var btn = MyButton().attrs {
			name = "play",
			resAnim = res.getResAnim("menu"),		
			anchor = vec2(0.5f, 0.5f),
			pos = @view.size / 2,
			parent = @view,	
		}
		btn.addEventListener(TouchEvent.CLICK, {||
			@changeScene(MainMenuScene.instance)
		}.bind(this))
		
		var move = Joystick()
		move.parent = @view
		move.y = @view.height - move.height
		
		print "move.pos: ${move.pos}"
	},
}

GameScene.instance = GameScene()
