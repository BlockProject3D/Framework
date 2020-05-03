import os
import zipfile
import platform

ModuleList = [ # [TargetName, FolderName]
    ["BPF", "Base"],
    ["BPF.Compression", "Compression"]
]

def runCMakeBuild():
    os.system("mkdir build")
    os.chdir("build")
    flag = ""
    # Unfortunalty cmake is badly designed for crashing if the only available platform is x64 instead of just ignoring the command line
    if (platform.system() == "Windows"):
        flag = "-DCMAKE_GENERATOR_PLATFORM=x64"
    os.system("cmake " + flag + " ..")
    for v in ModuleList:
        os.system("cmake --build . --target " + v[0] + " --config Debug")
    os.system("cmake " + flag + " -DRELEASE=ON ..")
    for v in ModuleList:
        os.system("cmake --build . --target " + v[0] + " --config Release")
    os.chdir("..")

def writeZipFolder(path, handle):
    for root, dirs, files in os.walk(path):
        for file in files:
            handle.write(os.path.join(root, file))

def runZipTool():
    handle = zipfile.ZipFile("./Framework.zip", "w", zipfile.ZIP_DEFLATED)
    for v in ModuleList:
        writeZipFolder("./" + v[1] + "/include", handle)
    writeZipFolder("./CMakes", handle)
    writeZipFolder("./build/Debug", handle)
    writeZipFolder("./build/Release", handle)
    handle.write("./LICENSE.md")
    for v in ModuleList:
        handle.write("./" + v[0] + ".cmake")
    handle.close()

runCMakeBuild()
runZipTool()
