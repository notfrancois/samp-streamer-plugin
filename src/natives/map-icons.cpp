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

#include "../map-icons.hpp"

cell AMX_NATIVE_CALL Natives::CreateDynamicMapIcon(AMX *amx, cell *params)
{
	CHECK_PARAMS(12);

    Eigen::Vector3f position { amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]) };

    int   type           = static_cast<int>(params[4]);
    int   color          = static_cast<int>(params[5]);
    int   worldId        = static_cast<int>(params[6]);
    int   interiorId     = static_cast<int>(params[7]);
    int   playerId       = static_cast<int>(params[8]);
    float streamDistance = amx_ctof(params[9]);
    int   style          = static_cast<int>(params[10]);
    int   areaId         = static_cast<int>(params[11]);
    int   priority       = static_cast<int>(params[12]);

    auto mapIcon = streamer::mapicons::CreateDynamicMapIcon(amx, position, type, color, worldId, interiorId, playerId, streamDistance, style, areaId, priority);
    if (mapIcon == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(mapIcon->getID());
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicMapIcon(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
    if (streamer::mapicons::DestroyDynamicMapIcon(static_cast<int>(params[1]))) return 1;
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicMapIcon(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(static_cast<int>(params[1]));
	if (m != core->getData()->mapIcons.end())
	{
		return 1;
	}
	return 0;
}
