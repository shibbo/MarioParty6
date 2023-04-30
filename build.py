import subprocess
import sys
import os
import pathlib
import shutil

COMPILER_DEFAULT_OPT = "-O0,p"

SOURCE_COMPILER_OPT = {
    "hw_dolphin.c": "-O4,p",
    "hw_memory.c": "-O4,s"
}

COMPILER_PATH = pathlib.Path("compilers/GC/2.6/mwcceppc.exe")

tasks = list()

for root, dirs, files in os.walk("src"):
    for file in files:
        if file.endswith(".c"):
            source_path = os.path.join(root, file)
            build_path = source_path.replace("src", "build", 1).replace(".c", ".o")

            os.makedirs(os.path.dirname(build_path), exist_ok=True)
            tasks.append((source_path, build_path, file))

for task in tasks:
    source_path, build_path, file_name = task

    optimz = COMPILER_DEFAULT_OPT

    if file_name in SOURCE_COMPILER_OPT:
        optimz = SOURCE_COMPILER_OPT[file_name]

    COMPILER_ARGS = f"-c -Cpp_exceptions off -proc gekko -fp hard {optimz} -inline auto -nodefaults -i include/"

    print(f"Compiling {source_path}...")
    if subprocess.call(f"{COMPILER_PATH} {COMPILER_ARGS} {source_path} -o {build_path}", shell=True) == 1:
            sys.exit(1)