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

#include "../objects.hpp"

cell AMX_NATIVE_CALL Natives::CreateDynamicObject(AMX* amx, cell* params)
{
    CHECK_PARAMS(14);

    int modelId = static_cast<int>(params[1]);

    Eigen::Vector3f position { amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) };
    Eigen::Vector3f rotation { amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]) };

    int worldId = static_cast<int>(params[8]);
    int interiorId = static_cast<int>(params[9]);
    int playerId = static_cast<int>(params[10]);

    float streamDistance = amx_ctof(params[11]);
    float drawDistance = amx_ctof(params[12]);

    int areaId = static_cast<int>(params[13]);
    int priority = static_cast<int>(params[14]);

    auto object = streamer::objects::CreateDynamicObject(amx, modelId, position, rotation, worldId, interiorId, playerId, streamDistance, drawDistance, areaId, priority);
    if (object == nullptr) return INVALID_STREAMER_ID;

    return static_cast<cell>(object->getID());
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicObject(AMX* amx, cell* params)
{
    CHECK_PARAMS(1);

    if (streamer::objects::DestroyDynamicObject(static_cast<int>(params[1]))) return 1;
    return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicObject(AMX* amx, cell* params)
{
    CHECK_PARAMS(1);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicObjectPos(AMX* amx, cell* params)
{
    CHECK_PARAMS(4);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        auto pos = obj->getPosition();

        Utility::storeFloatInNative(amx, params[2], pos.x);
        Utility::storeFloatInNative(amx, params[3], pos.y);
        Utility::storeFloatInNative(amx, params[4], pos.z);
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicObjectPos(AMX* amx, cell* params)
{
    CHECK_PARAMS(4);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        obj->setPosition({ amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) });
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicObjectRot(AMX* amx, cell* params)
{
    CHECK_PARAMS(4);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        auto rot = obj->getRotation();

        Utility::storeFloatInNative(amx, params[2], rot.ToEuler().x);
        Utility::storeFloatInNative(amx, params[3], rot.ToEuler().y);
        Utility::storeFloatInNative(amx, params[4], rot.ToEuler().z);
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicObjectRot(AMX* amx, cell* params)
{
    CHECK_PARAMS(4);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        obj->setRotation({ amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) });
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicObjectNoCameraCol(AMX* amx, cell* params)
{
    CHECK_PARAMS(1);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        return obj->getNoCameraCollision();
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicObjectNoCameraCol(AMX* amx, cell* params)
{
    CHECK_PARAMS(1);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        obj->toggleNoCameraCollision(true);
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::MoveDynamicObject(AMX* amx, cell* params)
{
    CHECK_PARAMS(8);
    if (!amx_ctof(params[5]))
    {
        return 0;
    }
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        if (obj->attach)
        {
            Utility::logError("MoveDynamicObject: Object is currently attached and cannot be moved.");
            return 0;
        }
        Vector3 position(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
        Vector3 rotation(amx_ctof(params[6]), amx_ctof(params[7]), amx_ctof(params[8]));

        obj->move(position, rotation, amx_ctof(params[5]));
        return static_cast<cell>(obj->_move->duration);
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::StopDynamicObject(AMX* amx, cell* params)
{
    CHECK_PARAMS(1);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        if (obj->stop())
        {
            return 1;
        }
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::IsDynamicObjectMoving(AMX* amx, cell* params)
{
    CHECK_PARAMS(1);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        return obj->isMoving() ? 1 : 0;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::AttachCameraToDynamicObject(AMX* amx, cell* params)
{
    CHECK_PARAMS(2);
    std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
    if (p != core->getData()->players.end())
    {
        int internalId = INVALID_OBJECT_ID;
        std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(static_cast<int>(params[2]));
        if (i == p->second.internalObjects.end())
        {
            std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[2]));
            if (o != core->getData()->objects.end())
            {
                p->second.position = Eigen::Vector3f(o->second->position[0], o->second->position[1], o->second->position[2]);
                core->getStreamer()->startManualUpdate(p->second, STREAMER_TYPE_OBJECT);
            }
            std::unordered_map<int, int>::iterator j = p->second.internalObjects.find(static_cast<int>(params[2]));
            if (j != p->second.internalObjects.end())
            {
                internalId = j->second;
            }
        }
        else
        {
            internalId = i->second;
        }
        if (internalId != INVALID_OBJECT_ID)
        {
            ompgdk::AttachCameraToPlayerObject(p->first, internalId);
            return 1;
        }
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::AttachDynamicObjectToObject(AMX* amx, cell* params)
{
    CHECK_PARAMS(9);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        Item::SharedObject attachTo;
        {
            std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[2]));
            if (o != core->getData()->objects.end())
            {
                attachTo = o->second;
            }
        }

        Vector3 positionOffset(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));
        Vector3 rotation(amx_ctof(params[6]), amx_ctof(params[7]), amx_ctof(params[8]));

        if (obj->attachDynamicObjectToObject(attachTo, positionOffset, rotation, static_cast<int>(params[9]) != 0)) return 1;
        return 0;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::AttachDynamicObjectToPlayer(AMX* amx, cell* params)
{
    CHECK_PARAMS(8);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        Vector3 positionOffset(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));
        Vector3 rotation(amx_ctof(params[6]), amx_ctof(params[7]), amx_ctof(params[8]));

        if (obj->attachDynamicObjectToPlayer(static_cast<int>(params[2]), positionOffset, rotation)) return 1;
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::AttachDynamicObjectToVehicle(AMX* amx, cell* params)
{
    CHECK_PARAMS(8);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        Vector3 positionOffset(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));
        Vector3 rotation(amx_ctof(params[6]), amx_ctof(params[7]), amx_ctof(params[8]));

        if (obj->attachDynamicObjectToVehicle(static_cast<int>(params[2]), positionOffset, rotation)) return 1;
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::EditDynamicObject(AMX* amx, cell* params)
{
    CHECK_PARAMS(2);
    std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
    if (p != core->getData()->players.end())
    {
        int internalId = INVALID_OBJECT_ID;
        std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(static_cast<int>(params[2]));
        if (i == p->second.internalObjects.end())
        {
            std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[2]));
            if (o != core->getData()->objects.end())
            {
                if (o->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && o->second->originalComparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF)
                {
                    o->second->originalComparableStreamDistance = o->second->comparableStreamDistance;
                    o->second->comparableStreamDistance = -1.0f;
                }
                p->second.position = Eigen::Vector3f(o->second->position[0], o->second->position[1], o->second->position[2]);
                core->getStreamer()->startManualUpdate(p->second, STREAMER_TYPE_OBJECT);
            }
            std::unordered_map<int, int>::iterator j = p->second.internalObjects.find(static_cast<int>(params[2]));
            if (j != p->second.internalObjects.end())
            {
                internalId = j->second;
            }
        }
        else
        {
            internalId = i->second;
        }
        if (internalId != INVALID_OBJECT_ID)
        {
            ompgdk::EditPlayerObject(p->first, internalId);
            return 1;
        }
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::IsDynamicObjectMaterialUsed(AMX* amx, cell* params)
{
    CHECK_PARAMS(2);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        auto& materialDataMap = obj->getMaterial();
        if (materialDataMap.find(static_cast<int>(params[2])) != materialDataMap.end()) return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::RemoveDynamicObjectMaterial(AMX* amx, cell* params)
{
    CHECK_PARAMS(2);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;
        if (obj->removeMaterial(static_cast<int>(params[2]))) return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicObjectMaterial(AMX* amx, cell* params)
{
    CHECK_PARAMS(8);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        auto& materialDataMap = obj->getMaterial();
        if (materialDataMap.find(static_cast<int>(params[2])) != materialDataMap.end())
        {
            auto& mat = materialDataMap.at(static_cast<int>(params[2]));
            if (mat.main)
            {
                Utility::storeIntegerInNative(amx, params[3], mat.main->modelId);
                Utility::convertStringToNativeString(amx, params[4], params[7], mat.main->txdFileName);
                Utility::convertStringToNativeString(amx, params[5], params[8], mat.main->textureName);
                Utility::storeIntegerInNative(amx, params[6], mat.main->materialColor);
                return 1;
            }
        }
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicObjectMaterial(AMX* amx, cell* params)
{
    CHECK_PARAMS(6);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        int index = static_cast<int>(params[2]);

        auto& materialDataMap = obj->getMaterial();
        materialDataMap[index].main = std::make_shared<streamer::objects::Object::Material::Main>();
        materialDataMap[index].main->modelId = static_cast<int>(params[3]);
        materialDataMap[index].main->txdFileName = Utility::convertNativeStringToString(amx, params[4]);
        materialDataMap[index].main->textureName = Utility::convertNativeStringToString(amx, params[5]);
        materialDataMap[index].main->materialColor = static_cast<int>(params[6]);
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
            if (i != p->second.internalObjects.end())
            {
                ompgdk::SetPlayerObjectMaterial(p->first, i->second, index, materialDataMap[index].main->modelId, materialDataMap[index].main->txdFileName.c_str(), materialDataMap[index].main->textureName.c_str(), materialDataMap[index].main->materialColor);
            }
        }
        materialDataMap[index].text.reset();
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::IsDynamicObjectMaterialTextUsed(AMX* amx, cell* params)
{
    CHECK_PARAMS(2);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        auto& materialDataMap = obj->getMaterial();
        if (materialDataMap.find(static_cast<int>(params[2])) != materialDataMap.end())
        {
            auto& mat = materialDataMap.at(static_cast<int>(params[2]));
            if (mat.text)
            {
                return 1;
            }
        }
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::RemoveDynamicObjectMaterialText(AMX* amx, cell* params)
{
    CHECK_PARAMS(2);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        if (obj->removeMaterialText(static_cast<int>(params[2]))) return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicObjectMaterialText(AMX* amx, cell* params)
{
    CHECK_PARAMS(12);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        auto m = obj->getMaterial().find(static_cast<int>(params[2]));
        if (m != o->second->materials.end())
        {
            if (m->second.text)
            {
                Utility::convertStringToNativeString(amx, params[3], params[11], m->second.text->materialText);
                Utility::storeIntegerInNative(amx, params[4], m->second.text->materialSize);
                Utility::convertStringToNativeString(amx, params[5], params[12], m->second.text->fontFace);
                Utility::storeIntegerInNative(amx, params[6], m->second.text->fontSize);
                Utility::storeIntegerInNative(amx, params[7], m->second.text->bold != 0);
                Utility::storeIntegerInNative(amx, params[8], m->second.text->fontColor);
                Utility::storeIntegerInNative(amx, params[9], m->second.text->backColor);
                Utility::storeIntegerInNative(amx, params[10], m->second.text->textAlignment);
                return 1;
            }
        }
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicObjectMaterialText(AMX* amx, cell* params)
{
    CHECK_PARAMS(10);
    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[1]));
    if (o != core->getData()->objects.end())
    {
        auto& obj = o->second;

        int index = static_cast<int>(params[2]);
        obj->setMaterialText(index, Utility::convertNativeStringToString(amx, params[3]), static_cast<int>(params[4]), Utility::convertNativeStringToString(amx, params[5]), static_cast<int>(params[6]), static_cast<int>(params[7]) != 0, static_cast<int>(params[8]), static_cast<int>(params[9]), static_cast<int>(params[10]));
        return 1;
    }
    return 0;
}

cell AMX_NATIVE_CALL Natives::GetPlayerCameraTargetDynObject(AMX* amx, cell* params)
{
    CHECK_PARAMS(1);

    auto obj = streamer::objects::GetPlayerCameraTargetDynObject(static_cast<int>(params[1]));
    if (obj == nullptr) return INVALID_STREAMER_ID;
    return obj->objectId;
}
