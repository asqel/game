math.randomseed(time())
local _required_loaded = {}
_required_loaded["/std/path"] = launch_file("/std/path.lua")

function require(path)
	if path:sub(1, 1) ~= "/" then
		path =  "/std/" .. path
	end

	path = _required_loaded["/std/path"].normalize(path)

	if _required_loaded[path] then
		return _required_loaded[path]
	end
	local res = {launch_file(path .. ".lua")}
	if #res ~= 1 then
		error("Error: " .. path .. " must return 1 value")
	elseif type(res[1]) ~= "table" then
		error("Error: " .. path .. " must return a table")
	end
	_required_loaded[path] = res[1]
	return res[1]
end

game = require("game")

local sprite = require("sprite")
sprite.register("player_up", 10, sprite.LOOP, "mc_u0", "mc_u1", "mc_u2")
sprite.register("player_left", 10, sprite.LOOP, "mc_l0", "mc_l1", "mc_l2")
sprite.register("player_down", 10, sprite.LOOP, "mc_d0", "mc_d1", "mc_d2")
sprite.register("player_right", 10, sprite.LOOP, "mc_r0", "mc_r1", "mc_r2")

function test(ent)
	vx = 0
	vy = 0
	if (game.act.get(game.act.UP)) > 0 then
		vy = vy - 0.1
	end
	if (game.act.get(game.act.DOWN)) > 0 then
		vy = vy + 0.1
	end
	if (game.act.get(game.act.LEFT)) > 0 then
		vx = vx - 0.1
	end
	if (game.act.get(game.act.RIGHT)) > 0 then
		vx = vx + 0.1
	end
	-- set_entity_velocity(ent, 0.1, 0.1)
	if (vx ~= 0 or vy ~= 0) then
		set_entity_velocity(ent, vx, vy)
	end
	print(ent)
	print("is moving ?", get_entity_ismoving(ent))
end

set_entity_func(0, test)
