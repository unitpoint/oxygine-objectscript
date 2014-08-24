MainMenuScene = extends Scene {
	__construct = function(){
		super()
		var sky = Sprite().attrs {
			resAnim = res.getResAnim("bg"),
			parent = @view,
		}
		var logo = Sprite()
		logo.attrs {		
			resAnim = res.getResAnim("logo2"),
			parent = @view,
			pos = @view.size - logo.size,
		}
		var btn = MyButton().attrs {
			name = "play",
			resAnim = res.getResAnim("play"),		
			anchor = vec2(0.5f, 0.5f),
			pos = @view.size / 2,
			parent = @view,	
		}
	},
}
