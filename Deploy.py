import os
import zipfile

ModuleList = [ # [TargetName, FolderName]
    ["BPF", "Base"]
]

def runCMakeBuild():
    os.system("mkdir build")
    os.chdir("build")
    os.system("cmake ..")
    for v in ModuleList:
        os.system("cmake --build . --target " + v[0] + " --config Debug")
    os.system("cmake -DRELEASE=ON ..")
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
    handle.close()

runCMakeBuild()
runZipTool()
