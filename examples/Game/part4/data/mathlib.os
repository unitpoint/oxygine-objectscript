
vec2 = extends Object {
	__construct = function(x, y){
		@x = x || 0 // numberOf(x) || throw "x number required"
		@y = y || @x // numberOf(y || x) || throw "y number required"
	},
	
	__add = function(b){
		b is vec2 || throw "vec2 required"
		return vec2(@x + b.x, @y + b.y)
	}
}