
-- these should be overwritten in the game's index.lua if need be
desiredfps = 30
keyevents = {}

function keystroke(k)
	if sdlk[k] ~= nil then
		table.insert(keyevents, {sdlk[k], 1})
	end
end

function keyrelease(k)
	if sdlk[k] ~= nil then
		table.insert(keyevents, {sdlk[k], 0})
	end
end

function keyevent()
	return table.remove(keyevents, 1)
end
