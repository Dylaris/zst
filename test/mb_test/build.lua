local LUABC_DIR = "../../"
package.path = package.path .. ";" .. LUABC_DIR .. "?.lua"

local luabc = require("luabc")
local cmd   = luabc.cmd
local tool  = luabc.tool
local debug = luabc.debug

local CC     = "gcc"
local CFLAGS = { "-Wall", "-Wextra", "-I../../" }
local CSTD   = "-std=c11"
local SRC    = "mb_test.c"
local TARGET = "mb_test"
local CLEAR  = TARGET

local function build()
    local all = cmd:new()
    all:append(CC, CFLAGS, CSTD, "-o", TARGET, SRC)

    local clean = cmd:new("clean")
    clean:call(tool.clean, CLEAR)

    luabc.build()
end
build()
