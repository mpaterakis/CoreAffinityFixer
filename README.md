# CoreAffinityFixer
A DLL tool that automatically and continually sets the affinity of an app to the maximum amount of available cores.

## Usage
Only thing you need to do to enable the fix is to import the main export of the DLL (`FeedMeCores`). You can import this export on either the app/game's EXE itself, or another DLL that the executable imports, using a tool like CFF Explorer.

## Configuration
You can *optionally* use a `CoreAffinityFix.ini` file (example included in the release zip) with any of these 3 options:
* InitialDelay: The number of seconds to wait before initially setting the new core affinity (Default: 5)
* LoopInterval: The number of seconds to wait between each loop of setting the new core affinity (Default: 1).
* CreateLog: When set to `true`, a `CoreAffinityFix.log` file with some good-to-know info will be generated. Note that the log will be generated when an error occurs, **even when this setting is not present**.

### Notes
I originally made this for Splinter Cell Blacklist, which notoriously only uses 1 (!?!) core. But the dll is built in a way that allows it to be used on any executable, you just have to do the importing part.