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

cell AMX_NATIVE_CALL Natives::CreateDynamicCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(10);

    Eigen::Vector3f position { amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]) };

    float size           = amx_ctof(params[4]);
    int   worldId        = static_cast<int>(params[5]);
    int   interiorId     = static_cast<int>(params[6]);
    int   playerId       = static_cast<int>(params[7]);
    float streamDistance = amx_ctof(params[8]);
    int   areaId         = static_cast<int>(params[9]);
    int   priority       = static_cast<int>(params[10]);

    auto checkpoint = streamer::checkpoints::CreateDynamicCheckpoint(amx, position, size, worldId, interiorId, playerId, streamDistance, areaId, priority);
    if (checkpoint == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(checkpoint->getID());
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
    if (streamer::checkpoints::DestroyDynamicCheckpoint(static_cast<int>(params[1]))) return 1;
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(static_cast<int>(params[1]));
	if (c != core->getData()->checkpoints.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsPlayerInDynamicCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		if (p->second.activeCheckpoint == static_cast<int>(params[2]))
		{
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetPlayerVisibleDynamicCP(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		return p->second.visibleCheckpoint;
	}
	return 0;
}
