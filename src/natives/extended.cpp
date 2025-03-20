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

#include "../main.h"

#include "../natives.h"
#include "../core.h"
#include "../utility.h"
#include "../objects.hpp"
#include "../map-icons.hpp"

cell AMX_NATIVE_CALL Natives::CreateDynamicObjectEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(18);

    int modelId = static_cast<int>(params[1]);

    Eigen::Vector3f position { amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) };
    Eigen::Vector3f rotation { amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]) };

    float streamDistance = amx_ctof(params[8]);
    float drawDistance   = amx_ctof(params[9]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[10], params[15], worlds);
    Utility::convertArrayToContainer(amx, params[11], params[16], interiors);
    Utility::convertArrayToContainer(amx, params[12], params[17], players);
    Utility::convertArrayToContainer(amx, params[13], params[18], areas);

    int priority = static_cast<int>(params[14]);

    auto object = streamer::objects::CreateDynamicObjectEx(amx, modelId, position, rotation, streamDistance, drawDistance, worlds, interiors, players, areas, priority);
    if (!object) return INVALID_STREAMER_ID;
    return static_cast<cell>(object->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicPickupEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(15);

    int modelId = static_cast<int>(params[1]);
    int type    = static_cast<int>(params[2]);

    Eigen::Vector3f position { amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]) };

    float streamDistance = amx_ctof(params[6]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[7], params[12], worlds);
    Utility::convertArrayToContainer(amx, params[8], params[13], interiors);
    Utility::convertArrayToContainer(amx, params[9], params[14], players);
    Utility::convertArrayToContainer(amx, params[10], params[15], areas);

    int priority = static_cast<int>(params[11]);

    auto pickup = streamer::pickups::CreateDynamicPickupEx(amx, modelId, type, position, streamDistance, worlds, interiors, players, areas, priority);
    if (pickup == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(pickup->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCPEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(14);
    if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_CP) == core->getData()->checkpoints.size())
    {
        return INVALID_STREAMER_ID;
    }
    int checkpointId = Item::Checkpoint::identifier.get();
    Item::SharedCheckpoint checkpoint(new Item::Checkpoint);
    checkpoint->amx = amx;
    checkpoint->checkpointId = checkpointId;
    checkpoint->inverseAreaChecking = false;
    checkpoint->originalComparableStreamDistance = -1.0f;
    checkpoint->positionOffset = Eigen::Vector3f::Zero();
    checkpoint->streamCallbacks = false;
    checkpoint->position = Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));
    checkpoint->size = amx_ctof(params[4]);
    checkpoint->comparableStreamDistance = amx_ctof(params[5]) < STREAMER_STATIC_DISTANCE_CUTOFF ? amx_ctof(params[5]) : amx_ctof(params[5]) * amx_ctof(params[5]);
    checkpoint->streamDistance = amx_ctof(params[5]);
    Utility::convertArrayToContainer(amx, params[6], params[11], checkpoint->worlds);
    Utility::convertArrayToContainer(amx, params[7], params[12], checkpoint->interiors);
    Utility::convertArrayToContainer(amx, params[8], params[13], checkpoint->players);
    Utility::convertArrayToContainer(amx, params[9], params[14], checkpoint->areas);
    checkpoint->priority = static_cast<int>(params[10]);
    core->getGrid()->addCheckpoint(checkpoint);
    core->getData()->checkpoints.insert(std::make_pair(checkpointId, checkpoint));
    return static_cast<cell>(checkpointId);
}

cell AMX_NATIVE_CALL Natives::CreateDynamicRaceCPEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(18);
    if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_RACE_CP) == core->getData()->raceCheckpoints.size())
    {
        return INVALID_STREAMER_ID;
    }
    int raceCheckpointId = Item::RaceCheckpoint::identifier.get();
    Item::SharedRaceCheckpoint raceCheckpoint(new Item::RaceCheckpoint);
    raceCheckpoint->amx = amx;
    raceCheckpoint->raceCheckpointId = raceCheckpointId;
    raceCheckpoint->inverseAreaChecking = false;
    raceCheckpoint->originalComparableStreamDistance = -1.0f;
    raceCheckpoint->positionOffset = Eigen::Vector3f::Zero();
    raceCheckpoint->streamCallbacks = false;
    raceCheckpoint->type = static_cast<int>(params[1]);
    raceCheckpoint->position = Eigen::Vector3f(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
    raceCheckpoint->next = Eigen::Vector3f(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]));
    raceCheckpoint->size = amx_ctof(params[8]);
    raceCheckpoint->comparableStreamDistance = amx_ctof(params[9]) < STREAMER_STATIC_DISTANCE_CUTOFF ? amx_ctof(params[9]) : amx_ctof(params[9]) * amx_ctof(params[9]);
    raceCheckpoint->streamDistance = amx_ctof(params[9]);
    Utility::convertArrayToContainer(amx, params[10], params[15], raceCheckpoint->worlds);
    Utility::convertArrayToContainer(amx, params[11], params[16], raceCheckpoint->interiors);
    Utility::convertArrayToContainer(amx, params[12], params[17], raceCheckpoint->players);
    Utility::convertArrayToContainer(amx, params[13], params[18], raceCheckpoint->areas);
    raceCheckpoint->priority = static_cast<int>(params[14]);
    core->getGrid()->addRaceCheckpoint(raceCheckpoint);
    core->getData()->raceCheckpoints.insert(std::make_pair(raceCheckpointId, raceCheckpoint));
    return static_cast<cell>(raceCheckpointId);
}

cell AMX_NATIVE_CALL Natives::CreateDynamicMapIconEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(16);
    Eigen::Vector3f position { amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]) };

    int   type           = static_cast<int>(params[4]);
    int   color          = static_cast<int>(params[5]);
    int   style          = static_cast<int>(params[6]);
    float streamDistance = amx_ctof(params[7]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[8], params[13], worlds);
    Utility::convertArrayToContainer(amx, params[9], params[14], interiors);
    Utility::convertArrayToContainer(amx, params[10], params[15], players);
    Utility::convertArrayToContainer(amx, params[11], params[16], areas);

    int priority = static_cast<int>(params[12]);

    auto mapIcon = streamer::mapicons::CreateDynamicMapIconEx(amx, position, type, color, style, streamDistance, worlds, interiors, players, areas, priority);
    if (mapIcon == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(mapIcon->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamic3DTextLabelEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(19);

    auto text = Utility::convertNativeStringToString(amx, params[1]);

    int color = static_cast<int>(params[2]);

    Eigen::Vector3f position { amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]) };

    float drawDistance      = amx_ctof(params[6]);
    int   attachedPlayerId  = static_cast<int>(params[7]);
    int   attachedVehicleId = static_cast<int>(params[8]);
    bool  testLOS           = static_cast<int>(params[9]) != 0;
    int   worldId           = static_cast<int>(params[10]);
    float streamDistance    = amx_ctof(params[11]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[11], params[16], worlds);
    Utility::convertArrayToContainer(amx, params[12], params[17], interiors);
    Utility::convertArrayToContainer(amx, params[13], params[18], players);
    Utility::convertArrayToContainer(amx, params[14], params[19], areas);

    int priority = static_cast<int>(params[15]);

    auto textlabel = streamer::textlabels::CreateDynamicTextLabelEx(amx, text, color, position, drawDistance, attachedPlayerId, attachedVehicleId, testLOS, streamDistance, worlds, interiors, players, areas, priority);
    if (textlabel == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(textlabel->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCircleEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(10);
    if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
    {
        return INVALID_STREAMER_ID;
    }
    int areaId = Item::Area::identifier.get();
    Item::SharedArea area(new Item::Area);
    area->amx = amx;
    area->areaId = areaId;
    area->spectateMode = true;
    area->type = STREAMER_AREA_TYPE_CIRCLE;
    area->position = Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2]));
    area->comparableSize = amx_ctof(params[3]) * amx_ctof(params[3]);
    area->size = amx_ctof(params[3]);
    Utility::convertArrayToContainer(amx, params[4], params[8], area->worlds);
    Utility::convertArrayToContainer(amx, params[5], params[9], area->interiors);
    Utility::convertArrayToContainer(amx, params[6], params[10], area->players);
    area->priority = static_cast<int>(params[7]);
    core->getGrid()->addArea(area);
    core->getData()->areas.insert(std::make_pair(areaId, area));
    return static_cast<cell>(areaId);
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCylinderEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(12);
    if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
    {
        return INVALID_STREAMER_ID;
    }
    int areaId = Item::Area::identifier.get();
    Item::SharedArea area(new Item::Area);
    area->amx = amx;
    area->areaId = areaId;
    area->spectateMode = true;
    area->type = STREAMER_AREA_TYPE_CYLINDER;
    area->position = Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2]));
    area->height = Eigen::Vector2f(amx_ctof(params[3]), amx_ctof(params[4]));
    area->comparableSize = amx_ctof(params[5]) * amx_ctof(params[5]);
    area->size = amx_ctof(params[5]);
    Utility::convertArrayToContainer(amx, params[6], params[10], area->worlds);
    Utility::convertArrayToContainer(amx, params[7], params[11], area->interiors);
    Utility::convertArrayToContainer(amx, params[8], params[12], area->players);
    area->priority = static_cast<int>(params[9]);
    core->getGrid()->addArea(area);
    core->getData()->areas.insert(std::make_pair(areaId, area));
    return static_cast<cell>(areaId);
}

cell AMX_NATIVE_CALL Natives::CreateDynamicSphereEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(11);
    if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
    {
        return INVALID_STREAMER_ID;
    }
    int areaId = Item::Area::identifier.get();
    Item::SharedArea area(new Item::Area);
    area->amx = amx;
    area->areaId = areaId;
    area->spectateMode = true;
    area->type = STREAMER_AREA_TYPE_SPHERE;
    area->position = Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));
    area->comparableSize = amx_ctof(params[4]) * amx_ctof(params[4]);
    area->size = amx_ctof(params[4]);
    Utility::convertArrayToContainer(amx, params[5], params[9], area->worlds);
    Utility::convertArrayToContainer(amx, params[6], params[10], area->interiors);
    Utility::convertArrayToContainer(amx, params[7], params[11], area->players);
    area->priority = static_cast<int>(params[8]);
    core->getGrid()->addArea(area);
    core->getData()->areas.insert(std::make_pair(areaId, area));
    return static_cast<cell>(areaId);
}

cell AMX_NATIVE_CALL Natives::CreateDynamicRectangleEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(11);
    if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
    {
        return INVALID_STREAMER_ID;
    }
    int areaId = Item::Area::identifier.get();
    Item::SharedArea area(new Item::Area);
    area->amx = amx;
    area->areaId = areaId;
    area->spectateMode = true;
    area->type = STREAMER_AREA_TYPE_RECTANGLE;
    area->position = Box2d(Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2])), Eigen::Vector2f(amx_ctof(params[3]), amx_ctof(params[4])));
    boost::geometry::correct(std::get<Box2d>(area->position));
    area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(std::get<Box2d>(area->position).min_corner(), std::get<Box2d>(area->position).max_corner()));
    area->size = static_cast<float>(boost::geometry::distance(std::get<Box2d>(area->position).min_corner(), std::get<Box2d>(area->position).max_corner()));
    Utility::convertArrayToContainer(amx, params[5], params[9], area->worlds);
    Utility::convertArrayToContainer(amx, params[6], params[10], area->interiors);
    Utility::convertArrayToContainer(amx, params[7], params[11], area->players);
    area->priority = static_cast<int>(params[8]);
    core->getGrid()->addArea(area);
    core->getData()->areas.insert(std::make_pair(areaId, area));
    return static_cast<cell>(areaId);
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCuboidEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(13);
    if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
    {
        return INVALID_STREAMER_ID;
    }
    int areaId = Item::Area::identifier.get();
    Item::SharedArea area(new Item::Area);
    area->amx = amx;
    area->areaId = areaId;
    area->spectateMode = true;
    area->type = STREAMER_AREA_TYPE_CUBOID;
    area->position = Box3d(Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])), Eigen::Vector3f(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6])));
    boost::geometry::correct(std::get<Box3d>(area->position));
    area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(std::get<Box3d>(area->position).min_corner()[0], std::get<Box3d>(area->position).min_corner()[1]), Eigen::Vector2f(std::get<Box3d>(area->position).max_corner()[0], std::get<Box3d>(area->position).max_corner()[1])));
    area->size = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(std::get<Box3d>(area->position).min_corner()[0], std::get<Box3d>(area->position).min_corner()[1]), Eigen::Vector2f(std::get<Box3d>(area->position).max_corner()[0], std::get<Box3d>(area->position).max_corner()[1])));
    Utility::convertArrayToContainer(amx, params[7], params[11], area->worlds);
    Utility::convertArrayToContainer(amx, params[8], params[12], area->interiors);
    Utility::convertArrayToContainer(amx, params[9], params[13], area->players);
    area->priority = static_cast<int>(params[10]);
    core->getGrid()->addArea(area);
    core->getData()->areas.insert(std::make_pair(areaId, area));
    return static_cast<cell>(areaId);
}

cell AMX_NATIVE_CALL Natives::CreateDynamicPolygonEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(11);
    if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
    {
        return INVALID_STREAMER_ID;
    }
    if (static_cast<int>(params[4]) < 6 || static_cast<int>(params[4]) % 2)
    {
        Utility::logError("CreateDynamicPolygonEx: Number of points must be divisible by 2 and bigger or equal to 6.");
        return INVALID_STREAMER_ID;
    }
    int areaId = Item::Area::identifier.get();
    Item::SharedArea area(new Item::Area);
    area->amx = amx;
    area->areaId = areaId;
    area->spectateMode = true;
    area->type = STREAMER_AREA_TYPE_POLYGON;
    Utility::convertArrayToPolygon(amx, params[1], params[4], std::get<Polygon2d>(area->position));
    area->height = Eigen::Vector2f(amx_ctof(params[2]), amx_ctof(params[3]));
    Box2d box = boost::geometry::return_envelope<Box2d>(std::get<Polygon2d>(area->position));
    area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(box.min_corner(), box.max_corner()));
    area->size = static_cast<float>(boost::geometry::distance(box.min_corner(), box.max_corner()));
    Utility::convertArrayToContainer(amx, params[5], params[9], area->worlds);
    Utility::convertArrayToContainer(amx, params[6], params[10], area->interiors);
    Utility::convertArrayToContainer(amx, params[7], params[11], area->players);
    area->priority = static_cast<int>(params[8]);
    core->getGrid()->addArea(area);
    core->getData()->areas.insert(std::make_pair(areaId, area));
    return static_cast<cell>(areaId);
}

cell AMX_NATIVE_CALL Natives::CreateDynamicActorEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(17);

    int modelId = static_cast<int>(params[1]);

    Eigen::Vector3f position { amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) };

    float rotation       = amx_ctof(params[5]);
    bool  invulnerable   = static_cast<int>(params[6]) != 0;
    float health         = amx_ctof(params[7]);
    float streamDistance = amx_ctof(params[8]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[9], params[14], worlds);
    Utility::convertArrayToContainer(amx, params[10], params[15], interiors);
    Utility::convertArrayToContainer(amx, params[11], params[16], players);
    Utility::convertArrayToContainer(amx, params[12], params[17], areas);

    int priority = static_cast<int>(params[13]);

    auto actor = streamer::actors::CreateDynamicActorEx(amx, modelId, position, rotation, invulnerable, health, streamDistance, worlds, interiors, players, areas, priority);
    if (actor == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(actor->getID());
}
