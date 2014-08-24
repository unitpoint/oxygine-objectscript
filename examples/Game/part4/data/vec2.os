vec2 = extends Object {
	__construct = function(x, y){
		@x = x || 0 // numberOf(x) || throw "x number required"
		@y = y || @x // numberOf(y || x) || throw "y number required"
	},
	
	__add = function(b){
		b is vec2 && return vec2(@x + b.x, @y + b.y)
		b = numberOf(b) || throw "number or vec2 required"
		return vec2(@x + b, @y + b)
	},
	
	__sub = function(b){
		b is vec2 && return vec2(@x - b.x, @y - b.y)
		b = numberOf(b) || throw "number or vec2 required"
		return vec2(@x - b, @y - b)
	},
	
	__mul = function(b){
		b is vec2 && return vec2(@x * b.x, @y * b.y)
		b = numberOf(b) || throw "number or vec2 required"
		return vec2(@x * b, @y * b)
	},
	
	__div = function(b){
		b is vec2 && return vec2(@x / b.x, @y / b.y)
		b = numberOf(b) || throw "number or vec2 required"
		return vec2(@x / b, @y / b)
	},
}