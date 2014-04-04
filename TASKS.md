# In Progress
* Managing maps
** add walls (not passable parts between coordinates)
** translate metric distances to coordinate distances (scale)

# Planned
* Loading maps from files
* Placing entities on a map
** get entities within rage (metric, coordinates)
* Scripting entities (Lua embedding)
* Entity interaction ($actions property)
* Basic path-finding algorithms
* Documentation
* Terrains
* Import C/C++ library plugins for cost intensive functions

# Done
* Base engine (init, close)
* Entity Managamenet
** Entity blueprint loading
** Entity cloning
** Entity property inheritance ($base property)
** Setting and getting proeprties
* Map Management
** Exclude single coordinates from map
** Exclude coordinates
** Include coordinates outside of map
** adapt height/width calculation to check for most left/down coordinates