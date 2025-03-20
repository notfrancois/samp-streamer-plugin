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

#include "race-checkpoints.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::racecheckpoints
{
    Identifier RaceCheckpoint::identifier;

    RaceCheckpoint::RaceCheckpoint()
        : references(0) {}

    int RaceCheckpoint::getID() const
    {
        return raceCheckpointId;
    }

    Vector3 RaceCheckpoint::getPosition() const
    {
        return { position[0], position[1], position[2] };
    }

    Item::SharedRaceCheckpoint GetDynamicRaceCheckpoint(int raceCheckpointId)
    {
        auto o = core->getData()->raceCheckpoints.find(raceCheckpointId);
        if (o != core->getData()->raceCheckpoints.end())
        {
            return o->second;
        }
        return nullptr;
    }

    Item::SharedRaceCheckpoint CreateDynamicRaceCheckpoint(std::optional<AMX*> amx, int type, Eigen::Vector3f position, Eigen::Vector3f next, float size, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_RACE_CP) == core->getData()->raceCheckpoints.size())
        {
            return nullptr;
        }
        int                        raceCheckpointId = RaceCheckpoint::identifier.get();
        Item::SharedRaceCheckpoint raceCheckpoint(new RaceCheckpoint);
        raceCheckpoint->amx                              = amx;
        raceCheckpoint->raceCheckpointId                 = raceCheckpointId;
        raceCheckpoint->inverseAreaChecking              = false;
        raceCheckpoint->originalComparableStreamDistance = -1.0f;
        raceCheckpoint->positionOffset                   = Eigen::Vector3f::Zero();
        raceCheckpoint->streamCallbacks                  = false;
        raceCheckpoint->type                             = type;
        raceCheckpoint->position                         = position;
        raceCheckpoint->next                             = next;
        raceCheckpoint->size                             = size;
        Utility::addToContainer(raceCheckpoint->worlds, worldId);
        Utility::addToContainer(raceCheckpoint->interiors, interiorId);
        Utility::addToContainer(raceCheckpoint->players, playerId);
        raceCheckpoint->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        raceCheckpoint->streamDistance           = streamDistance;
        Utility::addToContainer(raceCheckpoint->areas, areaId);
        raceCheckpoint->priority = priority;
        core->getGrid()->addRaceCheckpoint(raceCheckpoint);
        core->getData()->raceCheckpoints.insert(std::make_pair(raceCheckpointId, raceCheckpoint));
        return raceCheckpoint;
    }

    Item::SharedRaceCheckpoint CreateDynamicRaceCheckpointEx(std::optional<AMX*> amx, int type, Eigen::Vector3f position, Eigen::Vector3f next, float size, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_RACE_CP) == core->getData()->raceCheckpoints.size())
        {
            return nullptr;
        }
        int                        raceCheckpointId = RaceCheckpoint::identifier.get();
        Item::SharedRaceCheckpoint raceCheckpoint(new RaceCheckpoint);
        raceCheckpoint->amx                              = amx;
        raceCheckpoint->raceCheckpointId                 = raceCheckpointId;
        raceCheckpoint->inverseAreaChecking              = false;
        raceCheckpoint->originalComparableStreamDistance = -1.0f;
        raceCheckpoint->positionOffset                   = Eigen::Vector3f::Zero();
        raceCheckpoint->streamCallbacks                  = false;
        raceCheckpoint->type                             = type;
        raceCheckpoint->position                         = position;
        raceCheckpoint->next                             = next;
        raceCheckpoint->size                             = size;
        raceCheckpoint->comparableStreamDistance         = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        raceCheckpoint->streamDistance                   = streamDistance;
        raceCheckpoint->worlds.insert(worlds.begin(), worlds.end());
        raceCheckpoint->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            raceCheckpoint->players.set(player);
        }
        raceCheckpoint->areas.insert(areas.begin(), areas.end());
        raceCheckpoint->priority = priority;
        core->getGrid()->addRaceCheckpoint(raceCheckpoint);
        core->getData()->raceCheckpoints.insert(std::make_pair(raceCheckpointId, raceCheckpoint));
        return raceCheckpoint;
    }

    bool DestroyDynamicRaceCheckpoint(int raceCheckpointId)
    {
        auto o = core->getData()->raceCheckpoints.find(raceCheckpointId);
        if (o != core->getData()->raceCheckpoints.end())
        {
            Utility::destroyRaceCheckpoint(o);
            return true;
        }
        return false;
    }
} // namespace streamer::racecheckpoints
