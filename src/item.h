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

#ifndef ITEM_H
#define ITEM_H

#include "cell.h"
#include "identifier.h"

#include "actors.hpp"
#include "map-icons.hpp"
#include "objects.hpp"
#include "pickups.hpp"
#include "race-checkpoints.hpp"
#include "text-labels.hpp"

namespace Item
{
    struct Area
    {
        Area();

        AMX* amx;
        int areaId;
        SharedCell cell;
        float comparableSize;
        Eigen::Vector2f height;
        int priority;
        int references;
        float size;
        bool spectateMode;
        int type;

        std::variant<Polygon2d, Box2d, Box3d, Eigen::Vector2f, Eigen::Vector3f> position;

        struct Attach
        {
            Attach();

            Eigen::Vector2f height;
            std::tuple<int, int, int> object;
            int player;
            std::variant<Polygon2d, Box2d, Box3d, Eigen::Vector2f, Eigen::Vector3f> position;
            Eigen::Vector3f positionOffset;
            int references;
            int vehicle;

            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };

        std::shared_ptr<Attach> attach;

        std::unordered_set<int> areas;
        std::vector<int> extras;
        std::unordered_map<int, std::vector<int>> extraExtras;
        std::unordered_set<int> interiors;
        std::bitset<PLAYER_POOL_SIZE> players;
        std::unordered_set<int> worlds;

        static Identifier identifier;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };

    struct Checkpoint
    {
        Checkpoint();

        AMX* amx;
        SharedCell cell;
        int checkpointId;
        float comparableStreamDistance;
        bool inverseAreaChecking;
        float originalComparableStreamDistance;
        Eigen::Vector3f position;
        Eigen::Vector3f positionOffset;
        int priority;
        int references;
        float size;
        bool streamCallbacks;
        float streamDistance;

        std::unordered_set<int> areas;
        std::vector<int> extras;
        std::unordered_map<int, std::vector<int>> extraExtras;
        std::unordered_set<int> interiors;
        std::bitset<PLAYER_POOL_SIZE> players;
        std::unordered_set<int> worlds;

        static Identifier identifier;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };
}

#endif
