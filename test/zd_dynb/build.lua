local LUABC_DIR = "../../"
package.path = package.path .. ";" .. LUABC_DIR .. "?.lua"

local luabc = require("luabc")
local cmd   = luabc.cmd
local tool  = luabc.tool
local debug = luabc.debug

local CC     = "gcc"
local CFLAGS = { "-Wall", "-Wextra", "-I../../" }
local CSTD   = "-std=c11"
local CLEAN  = "zd_dynb"

local function build()
    local all = cmd:new()
    all:append(CC, CFLAGS, CSTD, "-o", "zd_dynb", "zd_dynb.c")

    local clean = cmd:new("clean")
    clean:call(tool.clean, CLEAN)

    luabc.build()
end
build()
