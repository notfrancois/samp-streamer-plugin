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

#include "actors.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::actors
{
    Identifier Actor::identifier;

    Actor::Actor()
        : references(0) {}

    int Actor::getID() const
    {
        return actorId;
    }

    Vector3 Actor::getPosition() const
    {
        return { position[0], position[1], position[2] };
    }

    void Actor::setPosition(Vector3 newposition)
    {
        position[0] = newposition.x;
        position[1] = newposition.y;
        position[2] = newposition.z;

        for (std::unordered_set<int>::const_iterator w = worlds.begin(); w != worlds.end(); ++w)
        {
            std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.find(std::make_pair(actorId, *w));
            if (i != core->getData()->internalActors.end())
            {
                core->getGrid()->removeActor(core->getData()->actors.at(actorId), true);

                ompgdk::SetActorPos(i->second, position[0], position[1], position[2]);
            }
        }
    }

    float Actor::getRotation() const
    {
        return rotation;
    }

    void Actor::setRotation(float newrotation)
    {
        rotation = newrotation;

        for (std::unordered_set<int>::const_iterator w = worlds.begin(); w != worlds.end(); ++w)
        {
            std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.find(std::make_pair(actorId, *w));
            if (i != core->getData()->internalActors.end())
            {
                ompgdk::DestroyActor(i->second);
                i->second = ompgdk::CreateActor(modelId, position[0], position[1], position[2], rotation);
                ompgdk::SetActorInvulnerable(i->second, invulnerable);
                ompgdk::SetActorHealth(i->second, health);
                ompgdk::SetActorVirtualWorld(i->second, *w);
                if (anim)
                {
                    ompgdk::ApplyActorAnimation(i->second, anim->lib.c_str(), anim->name.c_str(), anim->delta, anim->loop, anim->lockx, anim->locky, anim->freeze, anim->time);
                }
            }
        }
    }

    int Actor::getVirtualWorld() const
    {
        return Utility::getFirstValueInContainer(worlds);
    }

    void Actor::setVirtualWorld(int vw)
    {
        Utility::setFirstValueInContainer(worlds, vw);

        for (std::unordered_set<int>::const_iterator w = worlds.begin(); w != worlds.end(); ++w)
        {
            std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.find(std::make_pair(actorId, *w));
            if (i != core->getData()->internalActors.end())
            {
                ompgdk::SetActorVirtualWorld(i->second, *w);
            }
        }
    }

    float Actor::getHealth() const
    {
        return health;
    }

    void Actor::setHealth(float newhealth)
    {
        health = newhealth;

        for (std::unordered_set<int>::const_iterator w = worlds.begin(); w != worlds.end(); ++w)
        {
            std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.find(std::make_pair(actorId, *w));
            if (i != core->getData()->internalActors.end())
            {
                ompgdk::SetActorHealth(i->second, health);
            }
        }
    }

    bool Actor::isInvulnerable() const
    {
        return invulnerable;
    }

    void Actor::setInvulnerable(bool newinvulnerable)
    {
        invulnerable = newinvulnerable;

        for (std::unordered_set<int>::const_iterator w = worlds.begin(); w != worlds.end(); ++w)
        {
            std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.find(std::make_pair(actorId, *w));
            if (i != core->getData()->internalActors.end())
            {
                ompgdk::DestroyActor(i->second);
                i->second = ompgdk::CreateActor(modelId, position[0], position[1], position[2], rotation);
                ompgdk::SetActorInvulnerable(i->second, invulnerable);
                ompgdk::SetActorHealth(i->second, health);
                ompgdk::SetActorVirtualWorld(i->second, *w);
                if (anim)
                {
                    ompgdk::ApplyActorAnimation(i->second, anim->lib.c_str(), anim->name.c_str(), anim->delta, anim->loop, anim->lockx, anim->locky, anim->freeze, anim->time);
                }
            }
        }
    }

    std::optional<IActor::Anim> Actor::getAnimation() const
    {
        return anim;
    }

    void Actor::applyAnimation(const IActor::Anim& newanim)
    {
        anim = newanim;

        for (auto& w : worlds)
        {
            std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.find(std::make_pair(actorId, w));
            if (i != core->getData()->internalActors.end())
            {
                ompgdk::ApplyActorAnimation(i->second, anim->lib.c_str(), anim->name.c_str(), anim->delta, anim->loop, anim->lockx, anim->locky, anim->freeze, anim->time);
            }
        }
    }

    void Actor::clearAnimation()
    {
        anim = std::nullopt;

        for (auto& w : worlds)
        {
            std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.find(std::make_pair(actorId, w));
            if (i != core->getData()->internalActors.end())
            {
                ompgdk::ClearActorAnimations(i->second);
            }
        }
    }

    Item::SharedActor GetDynamicActor(int actorId)
    {
        std::unordered_map<int, Item::SharedActor>::iterator o = core->getData()->actors.find(actorId);
        if (o != core->getData()->actors.end())
        {
            return o->second;
        }
        return nullptr;
    }

    Item::SharedActor CreateDynamicActor(std::optional<AMX*> amx, int modelId, Eigen::Vector3f position, float rotation, bool invulnerable, float health, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_ACTOR) == core->getData()->actors.size())
        {
            return nullptr;
        }

        int               actorId = Actor::identifier.get();
        Item::SharedActor actor(new Actor);
        actor->amx                              = amx;
        actor->actorId                          = actorId;
        actor->inverseAreaChecking              = false;
        actor->originalComparableStreamDistance = -1.0f;
        actor->positionOffset                   = Eigen::Vector3f::Zero();
        actor->modelId                          = modelId;
        actor->position                         = position;
        actor->rotation                         = rotation;
        actor->invulnerable                     = invulnerable;
        actor->health                           = health;
        Utility::addToContainer(actor->worlds, worldId);
        Utility::addToContainer(actor->interiors, interiorId);
        Utility::addToContainer(actor->players, playerId);
        actor->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        actor->streamDistance           = streamDistance;
        Utility::addToContainer(actor->areas, areaId);
        actor->priority = priority;
        core->getGrid()->addActor(actor);
        core->getData()->actors.insert(std::make_pair(actorId, actor));
        return actor;
    }

    Item::SharedActor CreateDynamicActorEx(std::optional<AMX*> amx, int modelId, Eigen::Vector3f position, float rotation, bool invulnerable, float health, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_ACTOR) == core->getData()->actors.size())
        {
            return nullptr;
        }
        int               actorId = Actor::identifier.get();
        Item::SharedActor actor(new Actor);
        actor->amx                              = amx;
        actor->actorId                          = actorId;
        actor->inverseAreaChecking              = false;
        actor->originalComparableStreamDistance = -1.0f;
        actor->modelId                          = modelId;
        actor->position                         = position;
        actor->rotation                         = rotation;
        actor->invulnerable                     = invulnerable;
        actor->health                           = health;
        actor->comparableStreamDistance         = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        actor->streamDistance                   = streamDistance;
        actor->worlds.insert(worlds.begin(), worlds.end());
        actor->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            actor->players.set(player);
        }
        actor->areas.insert(areas.begin(), areas.end());
        actor->priority = priority;
        core->getGrid()->addActor(actor);
        core->getData()->actors.insert(std::make_pair(actorId, actor));
        return actor;
    }

    bool DestroyDynamicActor(int actorId)
    {
        auto o = core->getData()->actors.find(actorId);
        if (o != core->getData()->actors.end())
        {
            Utility::destroyActor(o);
            return true;
        }
        return false;
    }

    bool IsDynamicActorStreamedIn(int actorId, int playerId)
    {
        std::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerId);
        if (p != core->getData()->players.end())
        {
            std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(actorId);
            if (a != core->getData()->actors.end())
            {
                for (std::unordered_set<int>::const_iterator w = a->second->worlds.begin(); w != a->second->worlds.end(); ++w)
                {
                    std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.find(std::make_pair(actorId, *w));
                    if (i != core->getData()->internalActors.end())
                    {
                        return ompgdk::IsActorStreamedIn(i->second, p->first);
                    }
                }
                return true;
            }
        }
        return false;
    }

    Item::SharedActor GetPlayerTargetDynamicActor(int playerId)
    {
        std::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerId);
        if (p != core->getData()->players.end())
        {
            int actorid = ompgdk::GetPlayerTargetActor(p->second.playerId);
            if (actorid != INVALID_ACTOR_ID)
            {
                for (std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i)
                {
                    if (i->second == actorid)
                    {
                        return core->getData()->actors.at(i->first.first);
                    }
                }
            }
        }
        return nullptr;
    }

    Item::SharedActor GetPlayerCameraTargetDynActor(int playerId)
    {
        std::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerId);
        if (p != core->getData()->players.end())
        {
            int actorid = ompgdk::GetPlayerCameraTargetActor(p->second.playerId);
            if (actorid != INVALID_ACTOR_ID)
            {
                for (std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i)
                {
                    if (i->second == actorid)
                    {
                        return core->getData()->actors.at(i->first.first);
                    }
                }
            }
        }
        return nullptr;
    }
} // namespace streamer::actors
