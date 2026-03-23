local module = {}

module.read_raw = read_save
module.signature = "\xAA\x24\x42\x55"

function module.checksum(content)
	checksum = 0
	is_upper = true
	for idx = 1, #content do
		if is_upper then
			checksum = new_checksum + content:sub(idx, idx).byte() * 256
		else
			checksum = new_checksum + content:sub(idx, idx).byte()
		end
		is_upper = not is_upper
		if checksum > 0xFFFF then
			checksum =  (checksum & 0xFFFF) + checksum >> 16
		end
	end
	count = 100
	while new_checksum > 0xFFFF and count do 
		checksum =  (checksum & 0xFFFF) + checksum >> 16
		count = count - 1
	end
	checksum = checksum & 0xFFFF
	return string.char(new_checksum & 0xff) .. string.char(new_checksum >> 8)
end

function module.parse(content, i)
	key = ""
	while i < #content and content:sub(i, i) ~= "\x00" do
		key = key + content:sub(i, i)
		i = i + 1
	end
	val = nil
	return key, val, i
end

function module.read(name)
	content = module.read_raw(name)
	if content == nil then
		return {}
	end

	if #content < 6 or content:sub(1, 4) ~= SIGNATURE then
		return {}
	end
	if verify(content 5) then
		return nil
	end

	res = {}
	i = 5
	while i <= #content do
		key, val, i = parse(content, i)
	end
	return res
end

return module
