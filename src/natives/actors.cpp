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

#include "../actors.hpp"

cell AMX_NATIVE_CALL Natives::CreateDynamicActor(AMX *amx, cell *params)
{
	CHECK_PARAMS(13);

    int modelId = static_cast<int>(params[1]);

    Eigen::Vector3f position { amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) };

    float rotation = amx_ctof(params[5]);
    bool invulnerable = static_cast<int>(params[6]) != 0;
    float health = amx_ctof(params[7]);

    int worldId    = static_cast<int>(params[8]);
    int interiorId = static_cast<int>(params[9]);
    int playerId   = static_cast<int>(params[10]);

    float streamDistance = amx_ctof(params[11]);
    float drawDistance   = amx_ctof(params[12]);

    int areaId   = static_cast<int>(params[13]);
    int priority = static_cast<int>(params[14]);

    auto actor = streamer::actors::CreateDynamicActor(amx, modelId, position, rotation, invulnerable, health, worldId, interiorId, playerId, streamDistance, areaId, priority);
	if (actor == nullptr) return INVALID_STREAMER_ID;
	return static_cast<cell>(actor->getID());
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicActor(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
    if (streamer::actors::DestroyDynamicActor(static_cast<int>(params[1]))) return 1;
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicActor(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsDynamicActorStreamedIn(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
    if (streamer::actors::IsDynamicActorStreamedIn(static_cast<int>(params[1]), static_cast<int>(params[2]))) return 1;
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicActorVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
		return a->second->getVirtualWorld();
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicActorVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
        a->second->setVirtualWorld(static_cast<int>(params[2]));
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicActorAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(11);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
		if (a->second->anim)
		{
			Utility::convertStringToNativeString(amx, params[2], params[10], a->second->anim->lib);
			Utility::convertStringToNativeString(amx, params[3], params[11], a->second->anim->name);
			Utility::storeFloatInNative(amx, params[4], a->second->anim->delta);
			Utility::storeIntegerInNative(amx, params[5], a->second->anim->loop != 0);
			Utility::storeIntegerInNative(amx, params[6], a->second->anim->lockx != 0);
			Utility::storeIntegerInNative(amx, params[7], a->second->anim->locky != 0);
			Utility::storeIntegerInNative(amx, params[8], a->second->anim->freeze != 0);
			Utility::storeIntegerInNative(amx, params[9], a->second->anim->time);
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::ApplyDynamicActorAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(9);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
        streamer::IActor::Anim anim {};
        anim.lib    = Utility::convertNativeStringToString(amx, params[2]);
        anim.name   = Utility::convertNativeStringToString(amx, params[3]);
        anim.delta  = amx_ctof(params[4]);
        anim.loop   = static_cast<int>(params[5]) != 0;
        anim.lockx  = static_cast<int>(params[6]) != 0;
        anim.locky  = static_cast<int>(params[7]) != 0;
        anim.freeze = static_cast<int>(params[8]) != 0;
        anim.time   = static_cast<int>(params[9]);

        a->second->applyAnimation(anim);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::ClearDynamicActorAnimations(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
        a->second->clearAnimation();
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicActorFacingAngle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
		Utility::storeFloatInNative(amx, params[2], a->second->getRotation());
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicActorFacingAngle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
        a->second->setRotation(amx_ctof(params[2]));
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicActorPos(AMX *amx, cell *params)
{
	CHECK_PARAMS(4);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
		Utility::storeFloatInNative(amx, params[2], a->second->getPosition().x);
        Utility::storeFloatInNative(amx, params[3], a->second->getPosition().y);
        Utility::storeFloatInNative(amx, params[4], a->second->getPosition().z);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicActorPos(AMX *amx, cell *params)
{
	CHECK_PARAMS(4);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
        a->second->setPosition({ amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) });
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicActorHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
		Utility::storeFloatInNative(amx, params[2], a->second->getHealth());
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicActorHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	int actorId = static_cast<int>(params[1]);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(actorId);
	if (a != core->getData()->actors.end())
	{
		a->second->setHealth(amx_ctof(params[2]));
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicActorInvulnerable(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	int actorId = static_cast<int>(params[1]);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(actorId);
	if (a != core->getData()->actors.end())
	{
        a->second->setInvulnerable(static_cast<int>(params[2]) != 0);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsDynamicActorInvulnerable(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(static_cast<int>(params[1]));
	if (a != core->getData()->actors.end())
	{
		return a->second->isInvulnerable();
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetPlayerTargetDynamicActor(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
    auto actor = streamer::actors::GetPlayerTargetDynamicActor(static_cast<int>(params[1]));
    if (!actor) return INVALID_STREAMER_ID;
    return actor->getID();
}

cell AMX_NATIVE_CALL Natives::GetPlayerCameraTargetDynActor(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
    auto actor = streamer::actors::GetPlayerTargetDynamicActor(static_cast<int>(params[1]));
    if (!actor) return INVALID_STREAMER_ID;
    return actor->getID();
}
