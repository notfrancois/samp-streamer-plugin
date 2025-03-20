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

namespace streamer::checkpoints
{
    struct Checkpoint : public ICheckpoint
    {
        Checkpoint();

        virtual int getID() const override;

        virtual Vector3 getPosition() const override;

        std::optional<AMX*> amx;
        SharedCell          cell;
        int                 checkpointId;
        float               comparableStreamDistance;
        bool                inverseAreaChecking;
        float               originalComparableStreamDistance;
        Eigen::Vector3f     position;
        Eigen::Vector3f     positionOffset;
        int                 priority;
        int                 references;
        float               size;
        bool                streamCallbacks;
        float               streamDistance;

        std::unordered_set<int>                   areas;
        std::vector<int>                          extras;
        std::unordered_map<int, std::vector<int>> extraExtras;
        std::unordered_set<int>                   interiors;
        std::bitset<PLAYER_POOL_SIZE>             players;
        std::unordered_set<int>                   worlds;

        static Identifier identifier;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };

    Item::SharedCheckpoint GetDynamicCheckpoint(int checkpointId);
    Item::SharedCheckpoint CreateDynamicCheckpoint(std::optional<AMX*> amx, Eigen::Vector3f position, float size, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority);
    Item::SharedCheckpoint CreateDynamicCheckpointEx(std::optional<AMX*> amx, Eigen::Vector3f position, float size, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority);
    bool                   DestroyDynamicCheckpoint(int checkpointId);
} // namespace streamer::checkpoints
