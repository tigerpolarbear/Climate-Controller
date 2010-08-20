
name = "A Test Game"
version = 0.1
desiredfps = 40

-- the game must define an "initialize" function to be called before the game begins
function initialize()
	z = cc.frame_new(1,2,3,4);
	print(cc.frame_getx(z))
	cc.frame_setx(z, 3)
	print(cc.frame_getx(z))
	print(cc.frame_gety(z))
	cc.frame_sety(z, 3)
	print(cc.frame_gety(z))
	print(cc.frame_getw(z))
	cc.frame_setw(z, 4)
	print(cc.frame_getw(z))
	print(cc.frame_geth(z))
	cc.frame_seth(z, 3)
	print(cc.frame_geth(z))
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
