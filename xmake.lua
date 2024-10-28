add_rules("mode.debug", "mode.release")

set_description("seraph unity trainer")
add_requires("minhook")

set_languages("c++20", "c11")

local base_dir = "$(projectdir)"

add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })

target("sls_trainer")
    set_kind("shared")
    add_files("src/*.cpp")
    add_links("user32")
    add_packages("minhook")


