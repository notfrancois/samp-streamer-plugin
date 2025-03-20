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

#include "map-icons.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::mapicons
{
    Identifier MapIcon::identifier;

    MapIcon::MapIcon()
        : references(0) {}

    int MapIcon::getID() const
    {
        return mapIconId;
    }

    Vector3 MapIcon::getPosition() const
    {
        return { position[0], position[1], position[2] };
    }

    Item::SharedMapIcon GetDynamicMapIcon(int mapIconId)
    {
        auto o = core->getData()->mapIcons.find(mapIconId);
        if (o != core->getData()->mapIcons.end())
        {
            return o->second;
        }
        return nullptr;
    }

    Item::SharedMapIcon CreateDynamicMapIcon(std::optional<AMX*> amx, Eigen::Vector3f position, int type, int color, int worldId, int interiorId, int playerId, float streamDistance, int style, int areaId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_MAP_ICON) == core->getData()->mapIcons.size())
        {
            return nullptr;
        }
        int                 mapIconId = MapIcon::identifier.get();
        Item::SharedMapIcon mapIcon(new MapIcon);
        mapIcon->amx                              = amx;
        mapIcon->mapIconId                        = mapIconId;
        mapIcon->inverseAreaChecking              = false;
        mapIcon->originalComparableStreamDistance = -1.0f;
        mapIcon->positionOffset                   = Eigen::Vector3f::Zero();
        mapIcon->streamCallbacks                  = false;
        mapIcon->position                         = position;
        mapIcon->type                             = type;
        mapIcon->color                            = color;
        Utility::addToContainer(mapIcon->worlds, worldId);
        Utility::addToContainer(mapIcon->interiors, interiorId);
        Utility::addToContainer(mapIcon->players, playerId);
        mapIcon->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        mapIcon->streamDistance           = streamDistance;
        mapIcon->style                    = style;
        Utility::addToContainer(mapIcon->areas, areaId);
        mapIcon->priority = priority;
        core->getGrid()->addMapIcon(mapIcon);
        core->getData()->mapIcons.insert(std::make_pair(mapIconId, mapIcon));
        return mapIcon;
    }

    Item::SharedMapIcon CreateDynamicMapIconEx(std::optional<AMX*> amx, Eigen::Vector3f position, int type, int color, int style, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_MAP_ICON) == core->getData()->mapIcons.size())
        {
            return nullptr;
        }
        int                 mapIconId = MapIcon::identifier.get();
        Item::SharedMapIcon mapIcon(new MapIcon);
        mapIcon->amx                              = amx;
        mapIcon->mapIconId                        = mapIconId;
        mapIcon->inverseAreaChecking              = false;
        mapIcon->originalComparableStreamDistance = -1.0f;
        mapIcon->positionOffset                   = Eigen::Vector3f::Zero();
        mapIcon->streamCallbacks                  = false;
        mapIcon->position                         = position;
        mapIcon->type                             = type;
        mapIcon->color                            = color;
        mapIcon->style                            = style;
        mapIcon->comparableStreamDistance         = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        mapIcon->streamDistance                   = streamDistance;
        mapIcon->worlds.insert(worlds.begin(), worlds.end());
        mapIcon->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            mapIcon->players.set(player);
        }
        mapIcon->areas.insert(areas.begin(), areas.end());
        mapIcon->priority = priority;
        core->getGrid()->addMapIcon(mapIcon);
        core->getData()->mapIcons.insert(std::make_pair(mapIconId, mapIcon));
        return mapIcon;
    }

    bool DestroyDynamicMapIcon(int mapIconId)
    {
        auto o = core->getData()->mapIcons.find(mapIconId);
        if (o != core->getData()->mapIcons.end())
        {
            Utility::destroyMapIcon(o);
            return true;
        }
        return false;
    }
} // namespace streamer::mapicons
