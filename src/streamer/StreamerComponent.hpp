
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

#include "sdk.hpp"

constexpr auto STREAMER_OBJECT_SD = 300.0f;
constexpr auto STREAMER_OBJECT_DD = 0.0f;

namespace streamer
{
    struct IObject : public IIDProvider, public IExtensible
    {
        struct Material
        {
            struct Main
            {
                Main();

                int materialColor;
                int modelId;
                int references;
                std::string textureName;
                std::string txdFileName;
            };

            std::shared_ptr<Main> main;

            struct Text
            {
                Text();

                int backColor;
                bool bold;
                int fontColor;
                std::string fontFace;
                int fontSize;
                int references;
                int materialSize;
                std::string materialText;
                int textAlignment;
            };

            std::shared_ptr<Text> text;
        };

        virtual Vector3 getPosition() const = 0;
        virtual void setPosition(Vector3 position) = 0;

        virtual GTAQuat getRotation() const = 0;
        virtual void setRotation(GTAQuat rotation) = 0;

        virtual bool getNoCameraCollision() const = 0;
        virtual void toggleNoCameraCollision(bool noCameraCollision) = 0;

        virtual void move(const Vector3& position, const GTAQuat& rotation, float speed) = 0;
        virtual bool stop() = 0;
        virtual bool isMoving() const = 0;

        virtual bool attachCameraToDynamicObject(int playerId) = 0;
        virtual bool attachDynamicObjectToObject(const std::shared_ptr<IObject>& object, const Vector3& positionOffset, const GTAQuat& rotation, bool syncRotation) = 0;
        virtual bool attachDynamicObjectToPlayer(int playerId, const Vector3& positionOffset, const GTAQuat& rotation) = 0;
        virtual bool attachDynamicObjectToVehicle(int vehicleId, const Vector3& positionOffset, const GTAQuat& rotation) = 0;

        virtual bool editDynamicObject(int playerId) = 0;

        virtual bool removeMaterial(int materialIndex) = 0;
        virtual std::unordered_map<int, Material>& getMaterial() = 0;
        virtual bool removeMaterialText(int materialIndex) = 0;
        virtual void setMaterialText(int materialIndex, const std::string& text, int size, const std::string& fontface, int fontsize, bool bold, int fontcolor, int backcolor, int textalignment) = 0;
    };
}

struct IOmpStreamerComponent : public IComponent
{
    PROVIDE_UID(0x11897f0dbabe4f7c);

    virtual std::shared_ptr<streamer::IObject> getDynamicObject(int objectId)                                                                                                                                                                                                                                                                                                                                                = 0;
    virtual std::shared_ptr<streamer::IObject> createDynamicObject(int modelId, const Vector3& position, const Vector3& rotation, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_OBJECT_SD, float drawDistance = STREAMER_OBJECT_DD, int areaId = -1, int priority = 0)                                                                                                           = 0;
    virtual std::shared_ptr<streamer::IObject> createDynamicObjectEx(int modelId, const Vector3& position, const Vector3& rotation, float streamDistance = STREAMER_OBJECT_SD, float drawDistance = STREAMER_OBJECT_DD, const std::unordered_set<int>& worlds = {}, const std::unordered_set<int>& interiors = {}, const std::unordered_set<int>& players = {}, const std::unordered_set<int>& areas = {}, int priority = 0) = 0;
    virtual bool destroyDynamicObject(int objectId) = 0;
    virtual std::shared_ptr<streamer::IObject> getPlayerCameraTargetDynObject(int playerId)                                                                                                                                                                                                                                                                                                                                  = 0;
};
