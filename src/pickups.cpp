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

#include "pickups.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::pickups
{
    Identifier Pickup::identifier;

    Pickup::Pickup()
        : references(0) {}

    int Pickup::getID() const
    {
        return pickupId;
    }

    Vector3 Pickup::getPosition() const
    {
        return { position[0], position[1], position[2] };
    }

    Item::SharedPickup GetDynamicPickup(int pickupId)
    {
        std::unordered_map<int, Item::SharedPickup>::iterator o = core->getData()->pickups.find(pickupId);
        if (o != core->getData()->pickups.end())
        {
            return o->second;
        }
        return nullptr;
    }

    Item::SharedPickup CreateDynamicPickup(std::optional<AMX*> amx, int modelId, int type, Eigen::Vector3f position, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_PICKUP) == core->getData()->pickups.size())
        {
            return nullptr;
        }

        int                pickupId = Pickup::identifier.get();
        Item::SharedPickup pickup(new Pickup);
        pickup->amx                              = amx;
        pickup->pickupId                         = pickupId;
        pickup->inverseAreaChecking              = false;
        pickup->originalComparableStreamDistance = -1.0f;
        pickup->positionOffset                   = Eigen::Vector3f::Zero();
        pickup->streamCallbacks                  = false;
        pickup->modelId                          = modelId;
        pickup->type                             = type;
        pickup->position                         = position;
        Utility::addToContainer(pickup->worlds, worldId);
        Utility::addToContainer(pickup->interiors, interiorId);
        Utility::addToContainer(pickup->players, playerId);
        pickup->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        pickup->streamDistance           = streamDistance;
        Utility::addToContainer(pickup->areas, areaId);
        pickup->priority = priority;
        core->getGrid()->addPickup(pickup);
        core->getData()->pickups.insert(std::make_pair(pickupId, pickup));
        return pickup;
    }

    Item::SharedPickup CreateDynamicPickupEx(std::optional<AMX*> amx, int modelId, int type, Eigen::Vector3f position, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_PICKUP) == core->getData()->pickups.size())
        {
            return nullptr;
        }
        int                pickupId = Pickup::identifier.get();
        Item::SharedPickup pickup(new Pickup);
        pickup->amx                              = amx;
        pickup->pickupId                         = pickupId;
        pickup->inverseAreaChecking              = false;
        pickup->originalComparableStreamDistance = -1.0f;
        pickup->positionOffset                   = Eigen::Vector3f::Zero();
        pickup->streamCallbacks                  = false;
        pickup->modelId                          = modelId;
        pickup->type                             = type;
        pickup->position                         = position;
        pickup->comparableStreamDistance         = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        pickup->streamDistance                   = streamDistance;
        pickup->worlds.insert(worlds.begin(), worlds.end());
        pickup->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            pickup->players.set(player);
        }
        pickup->areas.insert(areas.begin(), areas.end());
        pickup->priority = priority;
        core->getGrid()->addPickup(pickup);
        core->getData()->pickups.insert(std::make_pair(pickupId, pickup));
        return pickup;
    }

    bool DestroyDynamicPickup(int pickupId)
    {
        std::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(pickupId);
        if (p != core->getData()->pickups.end())
        {
            Utility::destroyPickup(p);
            return true;
        }
        return false;
    }
} // namespace streamer::pickups
