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

#pragma once

#include "main.h"

namespace streamer::pickups
{
    struct Pickup : public IPickup
    {
        Pickup();

        virtual int getID() const override;

        virtual Vector3 getPosition() const override;

        std::optional<AMX*> amx;
        SharedCell          cell;
        float               comparableStreamDistance;
        bool                inverseAreaChecking;
        int                 modelId;
        float               originalComparableStreamDistance;
        int                 pickupId;
        Eigen::Vector3f     position;
        Eigen::Vector3f     positionOffset;
        int                 priority;
        int                 references;
        bool                streamCallbacks;
        float               streamDistance;
        int                 type;

        std::unordered_set<int>                   areas;
        std::vector<int>                          extras;
        std::unordered_map<int, std::vector<int>> extraExtras;
        std::unordered_set<int>                   interiors;
        std::bitset<PLAYER_POOL_SIZE>             players;
        std::unordered_set<int>                   worlds;

        static Identifier identifier;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };

    Item::SharedPickup GetDynamicPickup(int pickupId);
    Item::SharedPickup CreateDynamicPickup(std::optional<AMX*> amx, int modelId, int type, Eigen::Vector3f position, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority);
    Item::SharedPickup CreateDynamicPickupEx(std::optional<AMX*> amx, int modelId, int type, Eigen::Vector3f position, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority);
    bool               DestroyDynamicPickup(int pickupId);
} // namespace streamer::pickups
