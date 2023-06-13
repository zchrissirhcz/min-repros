import sled

obj_files = [
    'build/linux-x64/CMakeFiles/hello.dir/hello.cpp.o',
    'build/linux-x64/CMakeFiles/rect.dir/rect.cpp.o'
]

for file in obj_files:
    #cmd = f"nm -A -a -C {file} | ag 'MYRECT'"
    cmd = f"nm -A -a -C {file}"
    out, err = sled.CommandRunner.run(cmd, False)
    print(f'out:\n{out}')
    if len(err.split()) > 1:
        print(f'error:\n{err}')
    