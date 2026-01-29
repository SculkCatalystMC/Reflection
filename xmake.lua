add_rules("mode.debug", "mode.release")

add_repositories("divanadiumpentaoxide-repo https://github.com/DivanadiumPentaoxide/xmake-repo.git")

add_requires("jsonc 20260129-4")
add_requires("boost_pfr 20260104")
add_requires("magic_enum 0.9.7")

if is_plat("windows") then 
    if not has_config("vs_runtime") then
        set_runtimes("MD")
    end
    set_toolchains("msvc")
else
    set_toolchains("clang")
end

target("test")
    set_kind("binary")
    set_languages("c++23")
    add_packages(
        "jsonc",
        "boost_pfr",
        "magic_enum"
    )
    add_includedirs(
        "include",
        "test"
    )
    add_files("test/**.cpp")
    if is_plat("windows") then 
        add_defines(
            "NOMINMAX",
            "UNICODE"
        )
        add_cxflags("/utf-8", "/W4")
    else 
        add_cxflags("-Wall", "-Wextra", "-stdlib=libc++")
        add_syslinks("c++")
    end

    after_build(function (target)
        local file = target:targetfile()
        local output_dir = path.join(os.projectdir(), "bin")
        os.mkdir(output_dir)
        os.cp(file, output_dir)
        cprint("${bright green}[Execuatble]: ${reset}Execuatble file already generated to " .. output_dir)
    end)