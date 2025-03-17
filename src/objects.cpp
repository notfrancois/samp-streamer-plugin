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

#include "objects.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::objects
{
    Identifier Object::identifier;

    Object::Object() : references(0) {}
    Object::Attach::Attach() : references(0) {}
    Object::Move::Move() : references(0) {}

    int Object::getID() const
    {
        return objectId;
    }

    Vector3 Object::getPosition() const
    {
        if (_move)
        {
            core->getStreamer()->processActiveItems();
        }

        return { position[0], position[1], position[2] };
    }

    void Object::setPosition(Vector3 newposition)
    {
        Eigen::Vector3f position = this->position;
        this->position = Eigen::Vector3f(newposition.x, newposition.y, newposition.z);
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
            if (i != p->second.internalObjects.end())
            {
                ompgdk::SetPlayerObjectPos(p->first, i->second, this->position[0], this->position[1], this->position[2]);
            }
        }
        if (position[0] != this->position[0] || position[1] != this->position[1])
        {
            if (this->cell)
            {
                auto objIter = core->getData()->objects.find(objectId);
                if (objIter != core->getData()->objects.end())
                {
                    core->getGrid()->removeObject(objIter->second, true);
                }
            }
        }
        if (this->_move)
        {
            this->_move->duration = static_cast<int>((static_cast<float>(boost::geometry::distance(std::get<0>(this->_move->position), this->position) / this->_move->speed) * 1000.0f));
            std::get<1>(this->_move->position) = this->position;
            std::get<2>(this->_move->position) = (std::get<0>(this->_move->position) - this->position) / static_cast<float>(this->_move->duration);
            if ((std::get<0>(this->_move->rotation).maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
            {
                std::get<1>(this->_move->rotation) = this->rotation;
                std::get<2>(this->_move->rotation) = (std::get<0>(this->_move->rotation) - this->rotation) / static_cast<float>(this->_move->duration);
            }
            this->_move->time = std::chrono::steady_clock::now();
        }
    }

    GTAQuat Object::getRotation() const
    {
        if (_move)
        {
            core->getStreamer()->processActiveItems();
        }

        return { rotation[0], rotation[1], rotation[2] };
    }

    void Object::setRotation(GTAQuat rotation)
    {
        this->rotation = Eigen::Vector3f(rotation.ToEuler().x, rotation.ToEuler().y, rotation.ToEuler().z);
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
            if (i != p->second.internalObjects.end())
            {
                ompgdk::SetPlayerObjectRot(p->first, i->second, this->rotation[0], this->rotation[1], this->rotation[2]);
            }
        }
        if (this->_move)
        {
            if ((std::get<0>(this->_move->rotation).maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
            {
                std::get<1>(this->_move->rotation) = this->rotation;
                std::get<2>(this->_move->rotation) = (std::get<0>(this->_move->rotation) - this->rotation) / static_cast<float>(this->_move->duration);
            }
        }
    }

    bool Object::getNoCameraCollision() const
    {
        return noCameraCollision != 0;
    }

    void Object::toggleNoCameraCollision(bool noCameraCollision)
    {
        this->noCameraCollision = true;
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
            if (i != p->second.internalObjects.end())
            {
                ompgdk::SetPlayerObjectNoCameraCol(p->first, i->second);
            }
        }
    }

    void Object::move(const Vector3& newposition, const GTAQuat& newrotation, float speed)
    {
        if (attach)
        {
            Utility::logError("MoveDynamicObject: Object is currently attached and cannot be moved.");
            return;
        }

        const Eigen::Vector3f position{ newposition.x, newposition.y, newposition.z };
        const Eigen::Vector3f rotation{ newrotation.ToEuler().x, newrotation.ToEuler().y, newrotation.ToEuler().z };

        _move = std::make_shared<streamer::objects::Object::Move>();
        _move->duration = static_cast<int>((static_cast<float>(boost::geometry::distance(position, position) / speed) * 1000.0f));
        std::get<0>(_move->position) = position;
        std::get<1>(_move->position) = position;
        std::get<2>(_move->position) = (position - position) / static_cast<float>(_move->duration);
        std::get<0>(_move->rotation) = rotation;
        if ((std::get<0>(_move->rotation).maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
        {
            std::get<1>(_move->rotation) = rotation;
            std::get<2>(_move->rotation) = (rotation - rotation) / static_cast<float>(_move->duration);
        }
        _move->speed = speed;
        _move->time = std::chrono::steady_clock::now();
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
            if (i != p->second.internalObjects.end())
            {
                ompgdk::StopPlayerObject(p->first, i->second);
                ompgdk::MovePlayerObject(p->first, i->second, std::get<0>(_move->position)[0], std::get<0>(_move->position)[1], std::get<0>(_move->position)[2], _move->speed, std::get<0>(_move->rotation)[0], std::get<0>(_move->rotation)[1], std::get<0>(_move->rotation)[2]);
            }
        }
        core->getStreamer()->movingObjects.insert(core->getData()->objects.at(objectId));
    }

    bool Object::stop()
    {
        if (_move)
        {
            for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
            {
                std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
                if (i != p->second.internalObjects.end())
                {
                    ompgdk::StopPlayerObject(p->first, i->second);
                }
            }
            _move.reset();
            core->getStreamer()->movingObjects.erase(core->getData()->objects.at(objectId));
            return true;
        }
        return false;
    }

    bool Object::isMoving() const
    {
        return _move != nullptr;
    }

    bool Object::attachCameraToDynamicObject(int playerId)
    {
        std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(playerId));
        if (p != core->getData()->players.end())
        {
            ompgdk::AttachCameraToPlayerObject(p->first, objectId);
            return true;
        }
        return false;
    }

    bool Object::attachDynamicObjectToObject(const std::shared_ptr<IObject>& object, const Vector3& positionOffset, const GTAQuat& rotation, bool syncRotation)
    {
        if (_move)
        {
            Utility::logError("AttachDynamicObjectToObject: Object is currently moving and must be stopped first.");
            return false;
        }

        attach = std::make_shared<streamer::objects::Object::Attach>();
        attach->player = INVALID_PLAYER_ID;
        attach->vehicle = INVALID_VEHICLE_ID;
        attach->object = object->getID();
        attach->positionOffset = Eigen::Vector3f(positionOffset.x, positionOffset.y, positionOffset.z);
        attach->rotation = Eigen::Vector3f(rotation.ToEuler().x, rotation.ToEuler().y, rotation.ToEuler().z);
        attach->syncRotation = syncRotation;
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
            if (i != p->second.internalObjects.end())
            {
                std::unordered_map<int, int>::iterator j = p->second.internalObjects.find(attach->object);
                if (j != p->second.internalObjects.end())
                {
                    ompgdk::AttachPlayerObjectToObject(p->first, i->second, j->second, attach->positionOffset[0], attach->positionOffset[1], attach->positionOffset[2], attach->rotation[0], attach->rotation[1], attach->rotation[2]);
                    for (std::unordered_map<int, streamer::objects::Object::Material>::iterator m = materials.begin(); m != materials.end(); ++m)
                    {
                        if (m->second.main)
                        {
                            ompgdk::SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelId, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
                        }
                        else if (m->second.text)
                        {
                            ompgdk::SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
                        }
                    }
                }
            }
        }

        auto& objPtr = core->getData()->objects.at(objectId);
        if (object != nullptr)
        {
            std::unordered_map<int, Item::SharedObject>::iterator p = core->getData()->objects.find(object->getID());
            if (p != core->getData()->objects.end())
            {
                if (comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && p->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF)
                {
                    originalComparableStreamDistance = comparableStreamDistance;
                    comparableStreamDistance = p->second->comparableStreamDistance + static_cast<float>(boost::geometry::comparable_distance(position, p->second->position));
                }
            }
            core->getStreamer()->attachedObjects.insert(objPtr);
        }
        else
        {
            if (originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF)
            {
                comparableStreamDistance = originalComparableStreamDistance;
            }
            attach.reset();
            core->getStreamer()->attachedObjects.erase(objPtr);
            core->getGrid()->removeObject(objPtr, true);
        }

        return true;
    }

    bool Object::attachDynamicObjectToPlayer(int playerId, const Vector3& positionOffset, const GTAQuat& rotation)
    {
        if (_move)
        {
            Utility::logError("AttachDynamicObjectToPlayer: Object is currently moving and must be stopped first.");
            return false;
        }
        attach = std::make_shared<streamer::objects::Object::Attach>();
        attach->object = INVALID_STREAMER_ID;
        attach->vehicle = INVALID_VEHICLE_ID;
        attach->player = playerId;
        attach->positionOffset = Eigen::Vector3f(positionOffset.x, positionOffset.y, positionOffset.z);
        attach->rotation = Eigen::Vector3f(rotation.ToEuler().x, rotation.ToEuler().y, rotation.ToEuler().z);
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
            if (i != p->second.internalObjects.end())
            {
                ompgdk::AttachPlayerObjectToPlayer(p->first, i->second, attach->player, attach->positionOffset[0], attach->positionOffset[1], attach->positionOffset[2], attach->rotation[0], attach->rotation[1], attach->rotation[2]);
                for (std::unordered_map<int, streamer::objects::Object::Material>::iterator m = materials.begin(); m != materials.end(); ++m)
                {
                    if (m->second.main)
                    {
                        ompgdk::SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelId, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
                    }
                    else if (m->second.text)
                    {
                        ompgdk::SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
                    }
                }
            }
        }

        auto& objPtr = core->getData()->objects.at(objectId);
        if (playerId != INVALID_PLAYER_ID)
        {
            core->getStreamer()->attachedObjects.insert(objPtr);
        }
        else
        {
            attach.reset();
            core->getStreamer()->attachedObjects.erase(objPtr);
            core->getGrid()->removeObject(objPtr, true);
        }
        return true;
    }

    bool Object::attachDynamicObjectToVehicle(int vehicleId, const Vector3& positionOffset, const GTAQuat& rotation)
    {
        if (_move)
        {
            Utility::logError("AttachDynamicObjectToVehicle: Object is currently moving and must be stopped first.");
            return false;
        }
        attach = std::make_shared<streamer::objects::Object::Attach>();
        attach->object = INVALID_STREAMER_ID;
        attach->player = INVALID_PLAYER_ID;
        attach->vehicle = vehicleId;
        attach->positionOffset = Eigen::Vector3f(positionOffset.x, positionOffset.y, positionOffset.z);
        attach->rotation = Eigen::Vector3f(rotation.ToEuler().x, rotation.ToEuler().y, rotation.ToEuler().z);
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
            if (i != p->second.internalObjects.end())
            {
                ompgdk::AttachPlayerObjectToVehicle(p->first, i->second, attach->vehicle, attach->positionOffset[0], attach->positionOffset[1], attach->positionOffset[2], attach->rotation[0], attach->rotation[1], attach->rotation[2]);
                for (std::unordered_map<int, streamer::objects::Object::Material>::iterator m = materials.begin(); m != materials.end(); ++m)
                {
                    if (m->second.main)
                    {
                        ompgdk::SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelId, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
                    }
                    else if (m->second.text)
                    {
                        ompgdk::SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
                    }
                }
            }
        }

        auto& objPtr = core->getData()->objects.at(objectId);
        if (vehicleId != INVALID_VEHICLE_ID)
        {
            core->getStreamer()->attachedObjects.insert(objPtr);
        }
        else
        {
            attach.reset();
            core->getStreamer()->attachedObjects.erase(objPtr);
            core->getGrid()->removeObject(objPtr, true);
        }

        return true;
    }

    bool Object::editDynamicObject(int playerId)
    {
        return ompgdk::EditPlayerObject(playerId, objectId);
    }

    bool Object::removeMaterial(int materialIndex)
    {
        std::unordered_map<int, streamer::objects::Object::Material>::iterator m = materials.find(materialIndex);
        if (m != materials.end())
        {
            int index = materialIndex;
            materials[index].main.reset();

            for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
            {
                std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
                if (i != p->second.internalObjects.end())
                {
                    ompgdk::DestroyPlayerObject(p->first, i->second);
                    p->second.internalObjects.erase(i);
                    core->getStreamer()->startManualUpdate(p->second, STREAMER_TYPE_OBJECT);
                }
            }
            return true;
        }
        return false;
    }

    std::unordered_map<int, streamer::IObject::Material>& Object::getMaterial()
    {
        return materials;
    }

    bool Object::removeMaterialText(int materialIndex)
    {
        auto& materialDataMap = getMaterial();
        if (materialDataMap.find(materialIndex) != materialDataMap.end())
        {
            materialDataMap[materialIndex].text.reset();

            for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
            {
                std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
                if (i != p->second.internalObjects.end())
                {
                    ompgdk::DestroyPlayerObject(p->first, i->second);
                    p->second.internalObjects.erase(i);
                    core->getStreamer()->startManualUpdate(p->second, STREAMER_TYPE_OBJECT);
                }
            }
            return true;
        }
        return false;
    }

    void Object::setMaterialText(int materialIndex, const std::string& text, int size, const std::string& fontface, int fontsize, bool bold, int fontcolor, int backcolor, int textalignment)
    {
        materials[materialIndex].text = std::make_shared<streamer::objects::Object::Material::Text>();
        materials[materialIndex].text->materialText = text;
        materials[materialIndex].text->materialSize = size;
        materials[materialIndex].text->fontFace = fontface;
        materials[materialIndex].text->fontSize = fontsize;
        materials[materialIndex].text->bold = bold;
        materials[materialIndex].text->fontColor = fontcolor;
        materials[materialIndex].text->backColor = backcolor;
        materials[materialIndex].text->textAlignment = textalignment;
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectId);
            if (i != p->second.internalObjects.end())
            {
                ompgdk::SetPlayerObjectMaterialText(p->first, i->second, materials[materialIndex].text->materialText.c_str(), materialIndex, materials[materialIndex].text->materialSize, materials[materialIndex].text->fontFace.c_str(), materials[materialIndex].text->fontSize, materials[materialIndex].text->bold, materials[materialIndex].text->fontColor, materials[materialIndex].text->backColor, materials[materialIndex].text->textAlignment);
            }
        }
        materials[materialIndex].main.reset();
    }

    Item::SharedObject GetDynamicObject(int objectId)
    {
        std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectId);
        if (o != core->getData()->objects.end())
        {
            return o->second;
        }
        return nullptr;
    }

    Item::SharedObject CreateDynamicObject(std::optional<AMX*> amx, int modelId, Eigen::Vector3f position, Eigen::Vector3f rotation, int worldId, int interiorId, int playerId, float streamDistance, float drawDistance, int areaId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_OBJECT) == core->getData()->objects.size())
        {
            return nullptr;
        }
        int objectId = streamer::objects::Object::identifier.get();
        Item::SharedObject object(new Object);
        object->amx = amx;
        object->objectId = objectId;
        object->inverseAreaChecking = false;
        object->noCameraCollision = false;
        object->originalComparableStreamDistance = -1.0f;
        object->positionOffset = Eigen::Vector3f::Zero();
        object->streamCallbacks = false;
        object->modelId = modelId;
        object->position = position;
        object->rotation = rotation;
        Utility::addToContainer(object->worlds, worldId);
        Utility::addToContainer(object->interiors, interiorId);
        Utility::addToContainer(object->players, playerId);
        object->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        object->streamDistance = streamDistance;
        object->drawDistance = drawDistance;
        Utility::addToContainer(object->areas, areaId);
        object->priority = priority;
        core->getGrid()->addObject(object);
        core->getData()->objects.insert(std::make_pair(objectId, object));
        return object;
    }

    Item::SharedObject CreateDynamicObjectEx(std::optional<AMX*> amx, int modelId, Eigen::Vector3f position, Eigen::Vector3f rotation, float streamDistance, float drawDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_OBJECT) == core->getData()->objects.size())
        {
            return nullptr;
        }
        int objectId = streamer::objects::Object::identifier.get();
        Item::SharedObject object(new Object);
        object->amx = amx;
        object->objectId = objectId;
        object->inverseAreaChecking = false;
        object->noCameraCollision = false;
        object->originalComparableStreamDistance = -1.0f;
        object->positionOffset = Eigen::Vector3f::Zero();
        object->streamCallbacks = false;
        object->modelId = modelId;
        object->position = position;
        object->rotation = rotation;
        object->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        object->streamDistance = streamDistance;
        object->drawDistance = drawDistance;
        object->worlds.insert(worlds.begin(), worlds.end());
        object->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players) {
            object->players.set(player);
        }
        object->areas.insert(areas.begin(), areas.end());
        object->players = std::bitset<1000>();
        object->priority = priority;
        core->getGrid()->addObject(object);
        core->getData()->objects.insert(std::make_pair(objectId, object));
        return object;
    }

    bool DestroyDynamicObject(int objectId)
    {
        std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectId);
        if (o != core->getData()->objects.end())
        {
            Utility::destroyObject(o);
            return true;
        }
        return false;
    }

    Item::SharedObject GetPlayerCameraTargetDynObject(int playerId)
    {
        std::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerId);
        if (p != core->getData()->players.end())
        {
            int objectid = ompgdk::GetPlayerCameraTargetObject(p->second.playerId);
            if (objectid != INVALID_OBJECT_ID)
            {
                for (std::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
                {
                    if (i->second == objectid)
                    {
                        return core->getData()->objects.at(i->first);
                    }
                }
            }
        }
        return nullptr;
    }
}
