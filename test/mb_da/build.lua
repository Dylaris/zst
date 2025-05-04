local luabc = require("luabc")
local cmd   = luabc.cmd
local tool  = luabc.tool
local debug = luabc.debug

local CC     = "gcc"
local CFLAGS = { "-Wall", "-Wextra" }
local CSTD   = "-std=c11"
local TARGET = "mb_test"
local CLEAR  = { TARGET }

local function build()
    local all = cmd:new()
    all:append(CC, CFLAGS, CSTD, "-o", TARGET, SRC)
    all:run()
end
build()
