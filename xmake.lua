add_rules("mode.debug", "mode.release")

set_description("seraph's unity trainer")
add_requires("minhook","imgui","spdlog",{config = {dx11 = true,win32  =true}})

set_languages("c++20", "c11")

local base_dir = "$(projectdir)"

add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })

target("sls_trainer")
    set_kind("shared")
    set_arch("x64")
    add_defines("_WIN64")   
    add_files("src/*.cpp","src/**/*.cpp")
    add_includedirs("include")
    add_links("user32","d3d11","dxgi")
    add_packages("minhook","imgui","spdlog")
    


