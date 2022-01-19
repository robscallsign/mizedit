# MizEdit
MizEdit - a small utility to remove DCS modules from the "requiredModules" lua table inside the DCS .miz archive.

## DOWNLOAD
The latest version can be found on the release page, download it here:
https://github.com/robscallsign/mizedit/releases/download/v0.1/MizEdit_v0.1.zip

WARNING: this is an early release and may have bugs, use at your own peril.

## Description
Typically when DCS mods like the A-4E-C, VNAO T-45, or the VSN F104G are added to multiplayer missions, all clients need all of the modules to join the server. This can limit the adoption of mods in multiplayer servers.  A DCS .miz archive is simply a zip file, and inside this zip file there is a "mission" file containing the Lua code for the mission.  It is possible to edit the "requiredModules" portion of the mission lua table and remove mods.  This allows people to join without having the mod, but they see a default Su-25 for that aircraft.

```
    ["requiredModules"] = 
    {
        ["VSN_F104G"] = "VSN_F104G",
        ["476 vFG Range Targets by Noodle & Stuka"] = "476 vFG Range Targets by Noodle & Stuka",
        ["T-45"] = "T-45",
        ["A-4E-C"] = "A-4E-C",
    }, -- end of ["requiredModules"]
```

can be edited to:

```
    ["requiredModules"] = 
    {        
        ["476 vFG Range Targets by Noodle & Stuka"] = "476 vFG Range Targets by Noodle & Stuka",
    }, -- end of ["requiredModules"]
```

Although conceptually simple, the above steps require unzipping the .miz file somewhere, editing the mission lua table without messing up the lua table, and re-zipping the file.  This change has to be done every single time the mission is edited.

**MizEdit** is a small utility to automatically edit and remove DCS mods from the "requiredModules" section of a mission's lua file.  Simply go to File->Select miz files, and select the mission files you want to edit.  A dialog box will pop up:

- YES means to over-write the existing mission files
- NO means to append _mizedit to the filename of the new mission file, preserving the old mission
- CANCEL means no files will be edited or written

The list of modules that are going to be ignored are located in the Modules.txt file in the MizEdit folder.  Each line of this file contains a mod that will be removed from the requiredModules lua table.  It uses a sub-string search, so A-4E could be used and it will remove any lines containing the A-4E from the requiredModules lua table.

The default provided Modules.txt contains:

```
F104
T-45
A-4E-C
```

**TIPS**
- DO NOT HAVE THE .MIZ FILE OPEN IN THE MISSION EDITOR WHEN YOU TRY TO USE MIZEDIT TO EDIT THAT MISSION
- Multiple mission files can be selected using combiantions of control+click, shift+click
