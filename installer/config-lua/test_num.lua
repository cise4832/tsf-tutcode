﻿package.path = ".\\?.lua;" .. package.path
package.cpath = ".\\?.dll;" .. package.cpath

crvmgr = require("test_c")
require("init")

print(x)

el_test_num_table = {

{"100000000000000000000000000000000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000000", "#3"},
{"100000000000000000000000000000000", "#3"},
{"10000000000000000000000000000000", "#3"},
{"1000000000000000000000000000000", "#3"},
{"100000000000000000000000000000", "#3"},
{"10000000000000000000000000000", "#3"},
{"1000000000000000000000000000", "#3"},
{"100000000000000000000000000", "#3"},
{"10000000000000000000000000", "#3"},
{"1000000000000000000000000", "#3"},
{"100000000000000000000000", "#3"},
{"10000000000000000000000", "#3"},
{"1000000000000000000000", "#3"},
{"100000000000000000000", "#3"},
{"10000000000000000000", "#3"},
{"1000000000000000000", "#3"},
{"100000000000000000", "#3"},
{"10000000000000000", "#3"},
{"1000000000000000", "#3"},
{"100000000000000", "#3"},
{"10000000000000", "#3"},
{"1000000000000", "#3"},
{"100000000000", "#3"},
{"10000000000", "#3"},
{"1000000000", "#3"},
{"100000000", "#3"},
{"10000000", "#3"},
{"1000000", "#3"},
{"100000", "#3"},
{"10000", "#3"},
{"1000", "#3"},
{"100", "#3"},
{"10", "#3"},
{"1", "#3"},
{"123456789012345678901234", "#3"},
{"12345678901234567890123", "#3"},
{"1234567890123456789012", "#3"},
{"123456789012345678901", "#3"},
{"12345678901234567890", "#3"},
{"1234567890123456789", "#3"},
{"123456789012345678", "#3"},
{"12345678901234567", "#3"},
{"1234567890123456", "#3"},
{"123456789012345", "#3"},
{"12345678901234", "#3"},
{"1234567890123", "#3"},
{"123456789012", "#3"},
{"12345678901", "#3"},
{"123456789", "#3"},
{"12345678", "#3"},
{"1234567", "#3"},
{"123456", "#3"},
{"12345", "#3"},
{"1234", "#3"},
{"123", "#3"},
{"12", "#3"},
{"1", "#3"},
{"0", "#3"},

{"100000000000000000000000000000000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000000", "#5"},
{"100000000000000000000000000000000", "#5"},
{"10000000000000000000000000000000", "#5"},
{"1000000000000000000000000000000", "#5"},
{"100000000000000000000000000000", "#5"},
{"10000000000000000000000000000", "#5"},
{"1000000000000000000000000000", "#5"},
{"100000000000000000000000000", "#5"},
{"10000000000000000000000000", "#5"},
{"1000000000000000000000000", "#5"},
{"100000000000000000000000", "#5"},
{"10000000000000000000000", "#5"},
{"1000000000000000000000", "#5"},
{"100000000000000000000", "#5"},
{"10000000000000000000", "#5"},
{"1000000000000000000", "#5"},
{"100000000000000000", "#5"},
{"10000000000000000", "#5"},
{"1000000000000000", "#5"},
{"100000000000000", "#5"},
{"10000000000000", "#5"},
{"1000000000000", "#5"},
{"100000000000", "#5"},
{"10000000000", "#5"},
{"1000000000", "#5"},
{"100000000", "#5"},
{"10000000", "#5"},
{"1000000", "#5"},
{"100000", "#5"},
{"10000", "#5"},
{"1000", "#5"},
{"100", "#5"},
{"10", "#5"},
{"1", "#5"},
{"125456789012345678901234", "#5"},
{"12345678901234567890123", "#5"},
{"1234567890123456789012", "#5"},
{"123456789012345678901", "#5"},
{"12345678901234567890", "#5"},
{"1234567890123456789", "#5"},
{"123456789012345678", "#5"},
{"12345678901234567", "#5"},
{"1234567890123456", "#5"},
{"123456789012345", "#5"},
{"12345678901234", "#5"},
{"1234567890123", "#5"},
{"123456789012", "#5"},
{"12345678901", "#5"},
{"123456789", "#5"},
{"12345678", "#5"},
{"1234567", "#5"},
{"123456", "#5"},
{"12345", "#5"},
{"1234", "#5"},
{"123", "#5"},
{"12", "#5"},
{"1", "#5"},
{"0", "#5"},

{"9876543210", "#0"},
{"9876543210", "#1"},
{"9876543210", "#2"},

{"9", "#8"},
{"98", "#8"},
{"987", "#8"},
{"9876", "#8"},
{"98765", "#8"},
{"987654", "#8"},
{"9876543", "#8"},
{"98765432", "#8"},
{"987654321", "#8"},
{"9876543210", "#8"},
{"98765432109", "#8"},
{"987654321098", "#8"},
{"9876543210987", "#8"},
{"98765432109876", "#8"},

{"98", "#9"},
{"141", "#9"},

{"1000000000001", "#3"},
{"10000000000001", "#3"},
{"100000000000001", "#3"},
{"1000000000000001", "#3"},

}

for i, v in ipairs(el_test_num_table) do
	print("<= \"" .. v[1] .. "\" \"".. v[2] .. "\"")
	local s = lua_skk_convert_candidate(v[1], v[2])

	if (s) then
		print("=> \"" .. s .. "\"\n")
	else
		print("=> nil\n")
	end
end

