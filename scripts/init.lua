math.randomseed(time())
local _required_loaded = {}
_required_loaded["/std/game"] = launch_file("/std/game.lua")
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
