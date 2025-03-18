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

#pragma once

#include "main.h"

namespace streamer::actors
{
    struct Actor : public IActor
    {
        Actor();

        virtual int getID() const override;

        virtual Vector3 getPosition() const override;
        virtual void    setPosition(Vector3 position) override;

        virtual float getRotation() const override;
        virtual void  setRotation(float rotation) override;

        virtual int  getVirtualWorld() const override;
        virtual void setVirtualWorld(int vw) override;

        virtual float getHealth() const override;
        virtual void  setHealth(float health) override;

        virtual bool isInvulnerable() const override;
        virtual void setInvulnerable(bool invulnerable) override;

        virtual std::optional<Anim> getAnimation() const override;
        virtual void                applyAnimation(const Anim& anim) override;
        virtual void                clearAnimation() override;

        int                 actorId;
        std::optional<AMX*> amx;
        SharedCell          cell;
        float               comparableStreamDistance;
        float               health;
        bool                inverseAreaChecking;
        bool                invulnerable;
        int                 modelId;
        float               originalComparableStreamDistance;
        Eigen::Vector3f     position;
        Eigen::Vector3f     positionOffset;
        int                 priority;
        int                 references;
        float               rotation;
        float               streamDistance;

        std::optional<Anim> anim;

        std::unordered_set<int>                   areas;
        std::vector<int>                          extras;
        std::unordered_map<int, std::vector<int>> extraExtras;
        std::unordered_set<int>                   interiors;
        std::bitset<PLAYER_POOL_SIZE>             players;
        std::unordered_set<int>                   worlds;

        static Identifier identifier;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };

    Item::SharedActor GetDynamicActor(int actorId);
    Item::SharedActor CreateDynamicActor(std::optional<AMX*> amx, int modelId, Eigen::Vector3f position, float rotation, bool invulnerable = true, float health = 100.0, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_ACTOR_SD, int areaId = -1, int priority = 0);
    Item::SharedActor CreateDynamicActorEx(std::optional<AMX*> amx, int modelId, Eigen::Vector3f position, float rotation, bool invulnerable = true, float health = 100.0, float streamDistance = STREAMER_ACTOR_SD, const std::unordered_set<int>& worlds = {}, const std::unordered_set<int>& interiors = {}, const std::unordered_set<int>& players = {}, const std::unordered_set<int>& areas = {}, int priority = 0);
    bool              DestroyDynamicActor(int actorId);
    bool              IsDynamicActorStreamedIn(int actorId, int playerId);
    Item::SharedActor GetPlayerTargetDynamicActor(int playerId);
    Item::SharedActor GetPlayerCameraTargetDynActor(int playerId);
} // namespace streamer::actors
