# CoreAffinityFixer
A DLL tool that automatically sets the affinity of an app to the maximum amount of available cores.

## Usage
Only thing you need to do to enable the fix is to import the main export of the DLL (`FeedMeCores`). You can import this export on either the app/game's EXE itself, or another DLL that the executable imports, using a tool like CFF Explorer.
