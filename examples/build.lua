local LUABC_DIR = "../"
package.path = package.path .. ";" .. LUABC_DIR .. "?.lua"

local luabc = require("luabc")
local cmd   = luabc.cmd
local tool  = luabc.tool

local CC     = "gcc"
local CFLAGS = { "-Wall", "-Wextra", "-I../" }
local CSTD   = "-std=c11"
local SRC    = tool.match_file_extension(".c")
local TARGET = tool.replace_files_extension(SRC, (luabc.os == "WIN") and ".exe" or "")

local function build()
    for i = 1, #SRC do
        local all = cmd:new()
        if SRC[i] == "./zd_dynasm.c" then
            CSTD = "-std=gnu11"
        else
            CSTD = "-std=c11"
        end
        all:append(CC, CFLAGS, CSTD, "-o", TARGET[i], SRC[i])
    end

    local clean = cmd:new("clean")
    clean:call(tool.clean, TARGET)

    luabc.build()
end
build()
