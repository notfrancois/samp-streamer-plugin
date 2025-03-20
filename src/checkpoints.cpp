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

#include "checkpoints.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::checkpoints
{
    Identifier Checkpoint::identifier;

    Checkpoint::Checkpoint()
        : references(0) {}

    int Checkpoint::getID() const
    {
        return checkpointId;
    }

    Vector3 Checkpoint::getPosition() const
    {
        return { position[0], position[1], position[2] };
    }

    Item::SharedCheckpoint GetDynamicCheckpoint(int checkpointId)
    {
        auto o = core->getData()->checkpoints.find(checkpointId);
        if (o != core->getData()->checkpoints.end())
        {
            return o->second;
        }
        return nullptr;
    }

    Item::SharedCheckpoint CreateDynamicCheckpoint(std::optional<AMX*> amx, Eigen::Vector3f position, float size, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_CP) == core->getData()->checkpoints.size())
        {
            return nullptr;
        }
        int                    checkpointId = Checkpoint::identifier.get();
        Item::SharedCheckpoint checkpoint(new Checkpoint);
        checkpoint->amx                              = amx;
        checkpoint->checkpointId                     = checkpointId;
        checkpoint->inverseAreaChecking              = false;
        checkpoint->originalComparableStreamDistance = -1.0f;
        checkpoint->positionOffset                   = Eigen::Vector3f::Zero();
        checkpoint->streamCallbacks                  = false;
        checkpoint->position                         = position;
        checkpoint->size                             = size;
        Utility::addToContainer(checkpoint->worlds, worldId);
        Utility::addToContainer(checkpoint->interiors, interiorId);
        Utility::addToContainer(checkpoint->players, playerId);
        checkpoint->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        checkpoint->streamDistance           = streamDistance;
        Utility::addToContainer(checkpoint->areas, areaId);
        checkpoint->priority = priority;
        core->getGrid()->addCheckpoint(checkpoint);
        core->getData()->checkpoints.insert(std::make_pair(checkpointId, checkpoint));
        return checkpoint;
    }

    Item::SharedCheckpoint CreateDynamicCheckpointEx(std::optional<AMX*> amx, Eigen::Vector3f position, float size, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_CP) == core->getData()->checkpoints.size())
        {
            return nullptr;
        }
        int                    checkpointId = Checkpoint::identifier.get();
        Item::SharedCheckpoint checkpoint(new Checkpoint);
        checkpoint->amx                              = amx;
        checkpoint->checkpointId                     = checkpointId;
        checkpoint->inverseAreaChecking              = false;
        checkpoint->originalComparableStreamDistance = -1.0f;
        checkpoint->positionOffset                   = Eigen::Vector3f::Zero();
        checkpoint->streamCallbacks                  = false;
        checkpoint->position                         = position;
        checkpoint->size                             = size;
        checkpoint->comparableStreamDistance         = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        checkpoint->streamDistance                   = streamDistance;
        checkpoint->worlds.insert(worlds.begin(), worlds.end());
        checkpoint->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            checkpoint->players.set(player);
        }
        checkpoint->areas.insert(areas.begin(), areas.end());
        checkpoint->priority = priority;
        core->getGrid()->addCheckpoint(checkpoint);
        core->getData()->checkpoints.insert(std::make_pair(checkpointId, checkpoint));
        return checkpoint;
    }

    bool DestroyDynamicCheckpoint(int checkpointId)
    {
        auto o = core->getData()->checkpoints.find(checkpointId);
        if (o != core->getData()->checkpoints.end())
        {
            Utility::destroyCheckpoint(o);
            return true;
        }
        return false;
    }
} // namespace streamer::checkpoints
