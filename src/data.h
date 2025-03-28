/*
 * Copyright (C) 2017 Incognito
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DATA_H
#define DATA_H

#include "player.h"

class Data
{
public:
    Data();

    std::size_t getGlobalChunkTickRate(int type);
    bool setGlobalChunkTickRate(int type, std::size_t value);

    std::size_t getGlobalMaxItems(int type);
    bool setGlobalMaxItems(int type, std::size_t value);

    std::size_t getGlobalMaxVisibleItems(int type);
    bool setGlobalMaxVisibleItems(int type, std::size_t value);

    float getGlobalRadiusMultiplier(int type);
    bool setGlobalRadiusMultiplier(int type, float value);

    bool errorCallbackEnabled;

    std::set<AMX*> interfaces;
    std::set<AMX*> amxUnloadDestroyItems;

    std::vector<int> destroyedActors;

    std::unordered_map<std::pair<int, int>, Item::SharedActor, pair_hash> discoveredActors;
    std::unordered_map<std::pair<int, int>, Item::SharedPickup, pair_hash> discoveredPickups;

    std::unordered_map<std::pair<int, int>, int, pair_hash> internalActors;
    std::unordered_map<std::pair<int, int>, int, pair_hash> internalPickups;

    std::unordered_map<int, Item::SharedActor> actors;
    std::unordered_map<int, Item::SharedArea> areas;
    std::unordered_map<int, Item::SharedCheckpoint> checkpoints;
    std::unordered_map<int, Item::SharedMapIcon> mapIcons;
    std::unordered_map<int, Item::SharedObject> objects;
    std::unordered_map<int, Item::SharedPickup> pickups;
    std::unordered_map<int, Item::SharedRaceCheckpoint> raceCheckpoints;
    std::unordered_map<int, Item::SharedTextLabel> textLabels;

    std::unordered_map<int, Player> players;

    std::vector<int> typePriority;
private:
    std::size_t globalChunkTickRate[STREAMER_MAX_TYPES];
    std::size_t globalMaxItems[STREAMER_MAX_TYPES];
    std::size_t globalMaxVisibleItems[STREAMER_MAX_TYPES];
    float globalRadiusMultipliers[STREAMER_MAX_TYPES];
};

#endif
