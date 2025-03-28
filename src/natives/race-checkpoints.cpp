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

cell AMX_NATIVE_CALL Natives::CreateDynamicRaceCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(14);

    int type = static_cast<int>(params[1]);

    Eigen::Vector3f position { amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) };
    Eigen::Vector3f next { amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]) };

    float size           = amx_ctof(params[8]);
    int   worldId        = static_cast<int>(params[9]);
    int   interiorId     = static_cast<int>(params[10]);
    int   playerId       = static_cast<int>(params[11]);
    float streamDistance = amx_ctof(params[12]);
    int   areaId         = static_cast<int>(params[13]);
    int   priority       = static_cast<int>(params[14]);

	auto raceCheckpoint = streamer::racecheckpoints::CreateDynamicRaceCheckpoint(amx, type, position, next, size, worldId, interiorId, playerId, streamDistance, areaId, priority);
    if (raceCheckpoint == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(raceCheckpoint->getID());
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicRaceCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(static_cast<int>(params[1]));
	if (r != core->getData()->raceCheckpoints.end())
	{
		Utility::destroyRaceCheckpoint(r);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicRaceCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(static_cast<int>(params[1]));
	if (r != core->getData()->raceCheckpoints.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsPlayerInDynamicRaceCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		if (p->second.activeRaceCheckpoint == static_cast<int>(params[2]))
		{
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetPlayerVisibleDynamicRaceCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		return p->second.visibleRaceCheckpoint;
	}
	return 0;
}
