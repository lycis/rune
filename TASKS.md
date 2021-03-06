# In Progress
* Entity interaction ($actions property)
* Game Loop (single-threaded)

# Planned
* Managing maps
** init scripts
** add walls (not passable parts between coordinates)
** spawn points (?)
** scripted actions for maps (entry or movement hooks)
* Multi-threaded game loop
* Basic path-finding algorithms
* Documentation
* Terrains
* Import C/C++ library plugins for cost intensive functions
* Hooks (event bus, e.g. property P of entity E changes -> sendToEventBus(E, P, oldval, newval))
* Save and load current state

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
** Loading maps from files
** translate metric distances to coordinate distances (scale)
* Placing entities on a map
** get entities within rage (metric, coordinates)
* Load maps from data repository by using Engine
* Error handling (rune::lastError())
* Move functionality of EntityManager into Engine
* Basic scripting support
