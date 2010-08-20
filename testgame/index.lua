
name = "A Test Game"
version = 0.1
desiredfps = 40

-- the game must define an "initialize" function to be called before the game begins
function initialize()
	z = cc.frame_new(0,0,32,32)
	i = cc.image_new("data/woop.png")
	cc.blit(i, z, z);
	return
end

-- the game must define a "mainloop" function to be called every frame
function mainloop(delta)
	event = keyevent()
	while event ~= nil do
		-- process each key stroke/release
		event = keyevent()
	end
	return
end
