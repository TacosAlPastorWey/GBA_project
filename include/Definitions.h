#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define LOG_USED_MEMORY 1
#define SHOW_CPU_USAGE
#define MINIGAMES_TEST

#define MINIGAMES_COLLECTIONS 15

namespace Definitions{
    const char COLLECTIONS_NAMES[MINIGAMES_COLLECTIONS][20] = {
        "Construction",
        "Pizzeria",
        "Fishing",
        "Casino",
        "Plumber",
        "WEB Designer",
        "Miner",
        "Gardener",
        "Electrician",
        "Mechanic",
        "Painter",
        "Chef",
        "Courier",
        "Photographer",
        "Musician"
    };
}

#define TILE_H_FLIP 0x400
#define TILE_V_FLIP 0x800

#endif