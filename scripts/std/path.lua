local module = {}

function module.normalize(path)
	while path:find("//") do
		path = path:gsub("//", "/")
	end
	local parts = {}
	if path:sub(1, 1) == "/" then
		table.insert(parts, "")
	end
	for i in path:gmatch("[^/]+") do
		if i == ".." then
			table.remove(parts)
		elseif i ~= "." then
			table.insert(parts, i)
		end
	end
	if path:sub(#path, #path) == "/" then
		table.insert(parts, "")
	end
	path = table.concat(parts, "/")
	if #path == 0 then
		return "/"
	end
	while path:find("//") do
		path = path:gsub("//", "/")
	end
	return path
end

return module
