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

namespace streamer::objects
{
    struct Object : public IObject
    {
        Object();

        virtual int getID() const override;

        virtual Vector3 getPosition() const override;
        virtual void setPosition(Vector3 position) override;

        virtual GTAQuat getRotation() const override;
        virtual void setRotation(GTAQuat rotation) override;

        virtual bool getNoCameraCollision() const override;
        virtual void toggleNoCameraCollision(bool noCameraCollision) override;

        virtual void move(const Vector3& position, const GTAQuat& rotation, float speed) override;
        virtual bool stop() override;
        virtual bool isMoving() const override;

        virtual bool attachCameraToDynamicObject(int playerId) override;
        virtual bool attachDynamicObjectToObject(const std::shared_ptr<IObject>& object, const Vector3& positionOffset, const GTAQuat& rotation, bool syncRotation) override;
        virtual bool attachDynamicObjectToPlayer(int playerId, const Vector3& positionOffset, const GTAQuat& rotation) override;
        virtual bool attachDynamicObjectToVehicle(int vehicleId, const Vector3& positionOffset, const GTAQuat& rotation) override;

        virtual bool editDynamicObject(int playerId) override;

        virtual bool removeMaterial(int materialIndex) override;
        virtual std::unordered_map<int, streamer::IObject::Material>& getMaterial() override;
        virtual bool removeMaterialText(int materialIndex) override;
        virtual void setMaterialText(int materialIndex, const std::string& text, int size, const std::string& fontface, int fontsize, bool bold, int fontcolor, int backcolor, int textalignment) override;

        std::optional<AMX*> amx;
        SharedCell cell;
        float comparableStreamDistance;
        float drawDistance;
        bool inverseAreaChecking;
        int modelId;
        bool noCameraCollision;
        int objectId;
        float originalComparableStreamDistance;
        Eigen::Vector3f position;
        Eigen::Vector3f positionOffset;
        int priority;
        int references;
        Eigen::Vector3f rotation;
        bool streamCallbacks;
        float streamDistance;

        struct Attach
        {
            Attach();

            int object;
            int player;
            Eigen::Vector3f position;
            Eigen::Vector3f positionOffset;
            int references;
            Eigen::Vector3f rotation;
            bool syncRotation;
            int vehicle;

            std::unordered_set<int> worlds;

            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };

        std::shared_ptr<Attach> attach;

        std::unordered_map<int, streamer::IObject::Material> materials;

        struct Move
        {
            Move();

            int duration;
            std::tuple<Eigen::Vector3f, Eigen::Vector3f, Eigen::Vector3f> position;
            int references;
            std::tuple<Eigen::Vector3f, Eigen::Vector3f, Eigen::Vector3f> rotation;
            float speed;
            std::chrono::steady_clock::time_point time;

            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };

        std::shared_ptr<Move> _move;

        std::unordered_set<int> areas;
        std::vector<int> extras;
        std::unordered_map<int, std::vector<int>> extraExtras;
        std::unordered_set<int> interiors;
        std::bitset<PLAYER_POOL_SIZE> players;
        std::unordered_set<int> worlds;

        static Identifier identifier;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };

    Item::SharedObject GetDynamicObject(int objectId);
    Item::SharedObject CreateDynamicObject(std::optional<AMX*> amx, int modelId, Eigen::Vector3f position, Eigen::Vector3f rotation, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_OBJECT_SD, float drawDistance = STREAMER_OBJECT_DD, int areaId = -1, int priority = 0);
    Item::SharedObject CreateDynamicObjectEx(std::optional<AMX*> amx, int modelId, Eigen::Vector3f position, Eigen::Vector3f rotation, float streamDistance = STREAMER_OBJECT_SD, float drawDistance = STREAMER_OBJECT_DD, const std::unordered_set<int>& worlds = {}, const std::unordered_set<int>& interiors = {}, const std::unordered_set<int>& players = {}, const std::unordered_set<int>& areas = {}, int priority = 0);
    bool DestroyDynamicObject(int objectId);
    Item::SharedObject GetPlayerCameraTargetDynObject(int playerId);
}
