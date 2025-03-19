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

cell AMX_NATIVE_CALL Natives::CreateDynamic3DTextLabel(AMX *amx, cell *params)
{
	CHECK_PARAMS(15);

    auto text = Utility::convertNativeStringToString(amx, params[1]);

    int color = static_cast<int>(params[2]);

    Eigen::Vector3f position { amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]) };

    float drawDistance      = amx_ctof(params[6]);
    int   attachedPlayerId  = static_cast<int>(params[7]);
    int   attachedVehicleId = static_cast<int>(params[8]);
    bool  testLOS           = static_cast<int>(params[9]) != 0;
    int   worldId           = static_cast<int>(params[10]);
    int   interiorId        = static_cast<int>(params[11]);
    int   playerId          = static_cast<int>(params[12]);
    float streamDistance    = amx_ctof(params[13]);
    int   areaId            = static_cast<int>(params[14]);
    int   priority          = static_cast<int>(params[15]);

    auto textlabel = streamer::textlabels::CreateDynamicTextLabel(amx, text, color, position, drawDistance, attachedPlayerId, attachedVehicleId, testLOS, worldId, interiorId, playerId, streamDistance, areaId, priority);
    if (textlabel == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(textlabel->getID());
}

cell AMX_NATIVE_CALL Natives::DestroyDynamic3DTextLabel(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
    if (streamer::textlabels::DestroyDynamicTextLabel(static_cast<int>(params[1]))) return 1;
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamic3DTextLabel(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(static_cast<int>(params[1]));
	if (t != core->getData()->textLabels.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamic3DTextLabelText(AMX *amx, cell *params)
{
	CHECK_PARAMS(3);
	std::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(static_cast<int>(params[1]));
	if (t != core->getData()->textLabels.end())
	{
		cell *text = NULL;
		amx_GetAddr(amx, params[2], &text);
        amx_SetString(text, t->second->getText().c_str(), 0, 0, static_cast<size_t>(params[3]));
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::UpdateDynamic3DTextLabelText(AMX *amx, cell *params)
{
	CHECK_PARAMS(3);
	std::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(static_cast<int>(params[1]));
	if (t != core->getData()->textLabels.end())
	{
        t->second->setText(Utility::convertNativeStringToString(amx, params[3]), static_cast<int>(params[2]));
		return 1;
	}
	return 0;
}
