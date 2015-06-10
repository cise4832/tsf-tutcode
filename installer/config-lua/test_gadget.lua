﻿package.path = ".\\?.lua;" .. package.path
package.cpath = ".\\?.dll;" .. package.cpath

crvmgr = require("test_c")
require("init")



el_test_gadget_table = {
{"見出し語",	"(substring skk-henkan-key 0)"},
{"見出し語",	"(concat skk-henkan-key)"},

{"1a2b3c4d5",	"(car skk-num-list)"},
{"1a2b3c4d5",	"(car (cdr skk-num-list))"},
{"1a2b3c4d5",	"(car (cdr (cdr skk-num-list)))"},
{"1a2b3c4d5",	"(car (cdr (cdr (cdr skk-num-list))))"},
{"1a2b3c4d5",	"(car (cdr (cdr (cdr (cdr skk-num-list)))))"},

{"!",		"(concat \"&excl\\073\")"},
{"perl",	"(concat \"#!\\057usr\\057local\\057bin\\057perl\")"},
{"ruby",	"(concat \"#!\\057usr\\057local\\057bin\\057ruby\")"},
{"bar",		"(make-string (- fill-column 1) ?-)"},
{"line",	"(make-string (- (window-width) 5) ?-)"},
{"line",	"(make-string (- (window-width) 5) (string-to-char comment-start))"},
{"size",	"(concat \"w:\" (number-to-string (window-width)) \" h:\" (number-to-string (window-height)))"},
{"now",		"(current-time-string)"},
{"now",		"(substring (current-time-string) 11 16)"},
{"now",		"(substring (current-time-string) 11 19)"},
{"skk",		"(skk-version)"},

{"1feet",	"(skk-gadget-units-conversion \"feet\" (string-to-number (car skk-num-list)) \"cm\")"},
{"1inch",	"(skk-gadget-units-conversion \"inch\" (string-to-number (car skk-num-list)) \"cm\")"},
{"1inch",	"(skk-gadget-units-conversion \"inch\" (string-to-number (car skk-num-list)) \"feet\")"},
{"1mile",	"(skk-gadget-units-conversion \"mile\" (string-to-number (car skk-num-list)) \"km\")"},
{"1mile",	"(skk-gadget-units-conversion \"mile\" (string-to-number (car skk-num-list)) \"yard\")"},
{"1yard",	"(skk-gadget-units-conversion \"yard\" (string-to-number (car skk-num-list)) \"cm\")"},
{"1yard",	"(skk-gadget-units-conversion \"yard\" (string-to-number (car skk-num-list)) \"feet\")"},
{"1すん",	"(skk-gadget-units-conversion \"寸\" (string-to-number (car skk-num-list)) \"mm\")"},
{"1しゃく",	"(skk-gadget-units-conversion \"尺\" (string-to-number (car skk-num-list)) \"cm\")"},
{"1しゃく",	"(skk-gadget-units-conversion \"勺\" (string-to-number (car skk-num-list)) \"mL\")"},
{"1ごう",	"(skk-gadget-units-conversion \"合\" (string-to-number (car skk-num-list)) \"mL\")"},
{"1しょう",	"(skk-gadget-units-conversion \"升\" (string-to-number (car skk-num-list)) \"L\")"},
{"1と",	"(skk-gadget-units-conversion \"斗\" (string-to-number (car skk-num-list)) \"L\")"},
{"1つぼ",	"(skk-gadget-units-conversion \"坪\" (string-to-number (car skk-num-list)) \"㎡\")"},

{"おみくじ",	"(skk-omikuji)"},

{"なき",	"(concat \"(\\073_\\073)\")"},
{"なき",	"(concat \"(\\057 \\073_\\073 \\057)\")"},

{"加算",	"(+ 24 6)"},
{"減算",	"(- 24 6)"},
{"乗算",	"(* 24 6)"},
{"除算",	"(concat \"(\\057 24 6)\")"},
{"剰余",	"(% 24 7)"},
{"四則演算",	"(concat \"(+ 9 (* (\\057 24 (- 15 (% 99 10))) 3))\")"},

}



for i, v in ipairs(el_test_gadget_table) do
	print("<= \"" .. v[1] .. "\" \"".. v[2] .. "\"")
	local s = lua_skk_convert_candidate(v[1], v[2], "")

	if (s) then
		print("=> \"" .. s .. "\"\n")
	else
		print("=> nil\n")
	end
end
