
name = "A Test Game"
version = 0.1
desiredfps = 40

-- the game must define an "initialize" function to be called before the game begins
function initialize()
	return
end

-- the game must define an "keypress" function
-- it gets called every time a key is pressed
-- k: unicode key
-- r: was it a key release
function keypress(k, r)
	print("KEEE:", k, r)
	return
end

-- the game must define a "mainloop" function to be called every frame
function mainloop(delta)
	-- updates yr states n stuff
	return
end
