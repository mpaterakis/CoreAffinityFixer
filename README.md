# CoreAffinityFixer
A DLL tool that automatically and continually sets the affinity of an app to the maximum amount of available cores.

## Usage
All you need to do to enable the fix is place the DLL in your app directory and import the main export of the DLL (`FeedMeCores`). You can import this export on either the app/game's EXE itself, or another DLL that the executable imports, using [CFF Explorer](https://ntcore.com/explorer-suite), [DxWrapper](https://github.com/elishacloud/dxwrapper), or any other tool that can handle DLL injection/importing.

**Regardless of what method you use to load the fix:** Place `CoreAffinityFixer.dll` on the same folder as the target exe or dll that the game uses.

CFF Explorer Method (Editing game files):
1. In CFF Explorer, open the target exe/dll that you want to edit. It's wise to also make a backup of that target file, since you will be editing it.
2. Go to `Import Adder` and add `CoreAffinityFixer.dll`
3. Select `FeedMeCores` from the list, click `Import By Name` and then `Rebuild Import Table`
4. Save the edited dll/exe by clicking the save icon on the top left, overwriting the original file.

DxWrapper Method (Game files are **not affected** using this method):
1. Find a suitable Stub dll for your target app (`version.dll` is pretty universal) and place it next to the app's executable. Also place `dxwrapper.dll` and `dxwrapper.ini` in the same directory.
2. Open `dxwrapper.ini` and set `LoadCustomDllPath`'s value to `CoreAffinityFixer.dll` 

## Configuration
You can *optionally* use a `CoreAffinityFix.ini` file (example included in the release zip) with any of these 3 options:
* InitialDelay: The number of seconds to wait before initially setting the new core affinity (Default: 5)
* LoopInterval: The number of seconds to wait between each loop of setting the new core affinity (Default: 1).
* CreateLog: When set to `true`, a `CoreAffinityFix.log` file with some good-to-know info will be generated. Note that the log will be generated when an error occurs, **even when this setting is not present**.

---
### Notes
* I originally made this for Splinter Cell Blacklist, which notoriously only uses 1 (!?!) core. But the dll is built in a way that allows it to be used on any executable, you just have to do the importing part.
* You will notice that when logging, the new affinity mask will be a weird number. The program uses [SetProcessAffinityMask](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setprocessaffinitymask), which uses a bit mask. The log shows that mask, represented as an integer.
