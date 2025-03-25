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

constexpr auto STREAMER_ACTOR_SD = 200.0f;

constexpr auto STREAMER_CP_SD = 200.0;

constexpr auto STREAMER_MAP_ICON_SD = 200.0;

constexpr auto STREAMER_OBJECT_SD = 300.0f;
constexpr auto STREAMER_OBJECT_DD = 0.0f;

constexpr auto STREAMER_PICKUP_SD = 200.0f;

constexpr auto STREAMER_RACE_CP_SD = 200.0;

constexpr auto STREAMER_3D_TEXT_LABEL_SD = 200.0f;

namespace streamer
{
    struct IActor : public IIDProvider, public IExtensible
    {
        struct Anim
        {
            float       delta;
            bool        freeze;
            std::string lib;
            bool        loop;
            bool        lockx;
            bool        locky;
            std::string name;
            int         references;
            int         time;
        };

        virtual Vector3 getPosition() const           = 0;
        virtual void    setPosition(Vector3 position) = 0;

        virtual float getRotation() const         = 0;
        virtual void  setRotation(float rotation) = 0;

        virtual int  getVirtualWorld() const       = 0;
        virtual void setVirtualWorld(int rotation) = 0;

        virtual float getHealth() const       = 0;
        virtual void  setHealth(float health) = 0;

        virtual bool isInvulnerable() const             = 0;
        virtual void setInvulnerable(bool invulnerable) = 0;

        virtual std::optional<Anim> getAnimation() const             = 0;
        virtual void                applyAnimation(const Anim& anim) = 0;
        virtual void                clearAnimation()                 = 0;
    };

    struct ICheckpoint : public IIDProvider, public IExtensible
    {
        virtual Vector3 getPosition() const = 0;
    };

    struct IMapIcon : public IIDProvider, public IExtensible
    {
        virtual Vector3 getPosition() const = 0;
    };

    struct IObject : public IIDProvider, public IExtensible
    {
        struct Material
        {
            struct Main
            {
                Main();

                int         materialColor;
                int         modelId;
                int         references;
                std::string textureName;
                std::string txdFileName;
            };

            std::shared_ptr<Main> main;

            struct Text
            {
                Text();

                int         backColor;
                bool        bold;
                int         fontColor;
                std::string fontFace;
                int         fontSize;
                int         references;
                int         materialSize;
                std::string materialText;
                int         textAlignment;
            };

            std::shared_ptr<Text> text;
        };

        virtual Vector3 getPosition() const           = 0;
        virtual void    setPosition(Vector3 position) = 0;

        virtual GTAQuat getRotation() const           = 0;
        virtual void    setRotation(GTAQuat rotation) = 0;

        virtual bool getNoCameraCollision() const                    = 0;
        virtual void toggleNoCameraCollision(bool noCameraCollision) = 0;

        virtual void move(const Vector3& position, const GTAQuat& rotation, float speed) = 0;
        virtual bool stop()                                                              = 0;
        virtual bool isMoving() const                                                    = 0;

        virtual bool attachCameraToDynamicObject(int playerId)                                                                                                      = 0;
        virtual bool attachDynamicObjectToObject(const std::shared_ptr<IObject>& object, const Vector3& positionOffset, const GTAQuat& rotation, bool syncRotation) = 0;
        virtual bool attachDynamicObjectToPlayer(int playerId, const Vector3& positionOffset, const GTAQuat& rotation)                                              = 0;
        virtual bool attachDynamicObjectToVehicle(int vehicleId, const Vector3& positionOffset, const GTAQuat& rotation)                                            = 0;

        virtual bool editDynamicObject(int playerId) = 0;

        virtual bool                               removeMaterial(int materialIndex)                                                                                                                                            = 0;
        virtual std::unordered_map<int, Material>& getMaterial()                                                                                                                                                                = 0;
        virtual bool                               removeMaterialText(int materialIndex)                                                                                                                                        = 0;
        virtual void                               setMaterialText(int materialIndex, const std::string& text, int size, const std::string& fontface, int fontsize, bool bold, int fontcolor, int backcolor, int textalignment) = 0;
    };

    struct IPickup : public IIDProvider, public IExtensible
    {
        virtual Vector3 getPosition() const = 0;
    };

    struct IRaceCheckpoint : public IIDProvider, public IExtensible
    {
        virtual Vector3 getPosition() const = 0;
    };

    struct ITextLabel : public IIDProvider, public IExtensible
    {
        virtual Vector3 getPosition() const = 0;

        virtual const std::string& getText() const                             = 0;
        virtual void               setText(const std::string& text)            = 0;
        virtual void               setText(const std::string& text, uint32_t color) = 0;
    };
} // namespace streamer

struct IOmpStreamerComponent : public IComponent
{
    PROVIDE_UID(0x11897f0dbabe4f7c);

    virtual std::shared_ptr<streamer::IActor> getDynamicActor(int actorId)                                                                                                                                                                                                                                                                                                                                              = 0;
    virtual std::shared_ptr<streamer::IActor> createDynamicActor(int modelId, const Vector3& position, float rotation, bool invulnerable = true, float health = 100.0, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_ACTOR_SD, int areaId = -1, int priority = 0)                                                                                                           = 0;
    virtual std::shared_ptr<streamer::IActor> createDynamicActorEx(int modelId, const Vector3& position, float rotation, bool invulnerable = true, float health = 100.0, float streamDistance = STREAMER_ACTOR_SD, const std::unordered_set<int>& worlds = {}, const std::unordered_set<int>& interiors = {}, const std::unordered_set<int>& players = {}, const std::unordered_set<int>& areas = {}, int priority = 0) = 0;
    virtual bool                              destroyDynamicActor(int actorId)                                                                                                                                                                                                                                                                                                                                          = 0;
    virtual bool                              isDynamicActorStreamedIn(int actorId, int playerId)                                                                                                                                                                                                                                                                                                                       = 0;
    virtual std::shared_ptr<streamer::IActor> getPlayerTargetDynamicActor(int playerId)                                                                                                                                                                                                                                                                                                                                 = 0;
    virtual std::shared_ptr<streamer::IActor> getPlayerCameraTargetDynActor(int playerId)                                                                                                                                                                                                                                                                                                                               = 0;

    virtual std::shared_ptr<streamer::ICheckpoint> getDynamicCheckpoint(int checkpointId)                                                                                                                                                                                                                                                                     = 0;
    virtual std::shared_ptr<streamer::ICheckpoint> createDynamicCheckpoint(const Vector3& position, float size, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_RACE_CP_SD, int areaId = -1, int priority = 0)                                                                                                      = 0;
    //virtual std::shared_ptr<streamer::ICheckpoint> createDynamicCheckpointEx(const Vector3& position, float size, float streamDistance = STREAMER_CP_SD, const std::unordered_set<int>& worlds = {}, const std::unordered_set<int>& interiors = {}, const std::unordered_set<int>& players = {}, const std::unordered_set<int>& areas = {}, int priority = 0) = 0;
    virtual bool                                   destroyDynamicCheckpoint(int checkpointId)                                                                                                                                                                                                                                                                 = 0;

    virtual std::shared_ptr<streamer::IMapIcon> getDynamicMapIcon(int mapIconId)                                                                                                                                                                                                                                                                                                               = 0;
    virtual std::shared_ptr<streamer::IMapIcon> createDynamicMapIcon(int modelId, const Vector3& position, int type, int color, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_MAP_ICON_SD, MapIconStyle style = MapIconStyle_Local, int areaId = -1, int priority = 0)                                                                             = 0;
    //virtual std::shared_ptr<streamer::IMapIcon> createDynamicMapIconEx(int modelId, const Vector3& position, int type, int color, int style, float streamDistance = STREAMER_MAP_ICON_SD, const std::unordered_set<int>& worlds = {}, const std::unordered_set<int>& interiors = {}, const std::unordered_set<int>& players = {}, const std::unordered_set<int>& areas = {}, int priority = 0) = 0;
    virtual bool                                destroyDynamicMapIcon(int mapIconId)                                                                                                                                                                                                                                                                                                           = 0;

    virtual std::shared_ptr<streamer::IObject> getDynamicObject(int objectId)                                                                                                                                                                                                                                                                                                                                                = 0;
    virtual std::shared_ptr<streamer::IObject> createDynamicObject(int modelId, const Vector3& position, const Vector3& rotation, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_OBJECT_SD, float drawDistance = STREAMER_OBJECT_DD, int areaId = -1, int priority = 0)                                                                                                           = 0;
    //virtual std::shared_ptr<streamer::IObject> createDynamicObjectEx(int modelId, const Vector3& position, const Vector3& rotation, float streamDistance = STREAMER_OBJECT_SD, float drawDistance = STREAMER_OBJECT_DD, const std::unordered_set<int>& worlds = {}, const std::unordered_set<int>& interiors = {}, const std::unordered_set<int>& players = {}, const std::unordered_set<int>& areas = {}, int priority = 0) = 0;
    virtual bool                               destroyDynamicObject(int objectId)                                                                                                                                                                                                                                                                                                                                            = 0;
    virtual std::shared_ptr<streamer::IObject> getPlayerCameraTargetDynObject(int playerId)                                                                                                                                                                                                                                                                                                                                  = 0;

    virtual std::shared_ptr<streamer::IPickup> getDynamicPickup(int pickupId)                                                                                                                                                                                                                                           = 0;
    virtual std::shared_ptr<streamer::IPickup> createDynamicPickup(int modelId, int type, const Vector3& position, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)                                                                                                           = 0;
    //virtual std::shared_ptr<streamer::IPickup> createDynamicPickupEx(int modelId, int type, const Vector3& position, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority) = 0;
    virtual bool                               destroyDynamicPickup(int pickupId)                                                                                                                                                                                                                                       = 0;

    virtual std::shared_ptr<streamer::IRaceCheckpoint> getDynamicRaceCheckpoint(int raceCheckpointId)                                                                                                                                                                                                                                                                                                     = 0;
    virtual std::shared_ptr<streamer::IRaceCheckpoint> createDynamicRaceCheckpoint(int type, const Vector3& position, const Vector3& next, float size, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_RACE_CP_SD, int areaId = -1, int priority = 0)                                                                                                           = 0;
    //virtual std::shared_ptr<streamer::IRaceCheckpoint> createDynamicRaceCheckpointEx(int type, const Vector3& position, const Vector3& next, float size, float streamDistance = STREAMER_RACE_CP_SD, const std::unordered_set<int>& worlds = {}, const std::unordered_set<int>& interiors = {}, const std::unordered_set<int>& players = {}, const std::unordered_set<int>& areas = {}, int priority = 0) = 0;
    virtual bool                                       destroyDynamicRaceCheckpoint(int raceCheckpointId)                                                                                                                                                                                                                                                                                                 = 0;

    virtual std::shared_ptr<streamer::ITextLabel> getDynamicTextLabel(int textlabelId)                                                                                                                                                                                                                                                                                                                                    = 0;
    virtual std::shared_ptr<streamer::ITextLabel> createDynamicTextLabel(const std::string& text, int color, const Vector3& position, float drawDistance, int attachedPlayerId = -1, int attachedVehicleId = -1, bool testLOS = false, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_3D_TEXT_LABEL_SD, int areaId = -1, int priority = 0)                                     = 0;
    //virtual std::shared_ptr<streamer::ITextLabel> createDynamicTextLabelEx(const std::string& text, int color, const Vector3& position, float drawDistance, int attachedPlayerId, int attachedVehicleId, bool testLOS, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority) = 0;
    virtual bool                                  destroyDynamicTextLabel(int textlabelId)                                                                                                                                                                                                                                                                                                                                = 0;
};
