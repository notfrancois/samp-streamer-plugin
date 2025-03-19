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

cell AMX_NATIVE_CALL Natives::CreateDynamicPickup(AMX *amx, cell *params)
{
	CHECK_PARAMS(11);

    int modelId = static_cast<int>(params[1]);
    int type    = static_cast<int>(params[2]);

    Eigen::Vector3f position { amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]) };

    int   worldId        = static_cast<int>(params[6]);
    int   interiorId     = static_cast<int>(params[7]);
    int   playerId       = static_cast<int>(params[8]);
    float streamDistance = amx_ctof(params[9]);
    int   areaId         = static_cast<int>(params[10]);
    int   priority       = static_cast<int>(params[11]);

    auto pickup = streamer::pickups::CreateDynamicPickup(amx, modelId, type, position, worldId, interiorId, playerId, streamDistance, areaId, priority);
	if (pickup == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(pickup->getID());
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicPickup(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
    if (streamer::pickups::DestroyDynamicPickup(static_cast<int>(params[1]))) return 1;
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicPickup(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(static_cast<int>(params[1]));
	if (p != core->getData()->pickups.end())
	{
		return 1;
	}
	return 0;
}
