GameScene = extends Scene {
	__construct = function(){
		super()
		var sky = Sprite().attrs {
			resAnim = res.getResAnim("bg"),
			parent = @view,
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
		
		var fire = Joystick()
		fire.parent = @view
		// fire.x = @view.width - move.width
		// fire.y = @view.height - move.height
		fire.pos = @view.size - move.size
	},
}

GameScene.instance = GameScene()
