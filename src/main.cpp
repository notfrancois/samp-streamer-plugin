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

#include "main.h"
#include "Server/Components/Pawn/pawn.hpp"

#include "core.h"
#include "natives.h"
#include "utility.h"
#include "omp-nethack.h"
#include "./events.hpp"
#include "objects.hpp"

extern void* pAMXFunctions;

streamer::IObject::Material::Main::Main()
    : references(0) {}
streamer::IObject::Material::Text::Text()
    : references(0) {}

AMX_NATIVE_INFO natives[] = {
    // Settings
    { "Streamer_GetTickRate", Natives::Streamer_GetTickRate },
    { "Streamer_SetTickRate", Natives::Streamer_SetTickRate },
    { "Streamer_GetPlayerTickRate", Natives::Streamer_GetPlayerTickRate },
    { "Streamer_SetPlayerTickRate", Natives::Streamer_SetPlayerTickRate },
    { "Streamer_ToggleChunkStream", Natives::Streamer_ToggleChunkStream },
    { "Streamer_IsToggleChunkStream", Natives::Streamer_IsToggleChunkStream },
    { "Streamer_GetChunkTickRate", Natives::Streamer_GetChunkTickRate },
    { "Streamer_SetChunkTickRate", Natives::Streamer_SetChunkTickRate },
    { "Streamer_GetChunkSize", Natives::Streamer_GetChunkSize },
    { "Streamer_SetChunkSize", Natives::Streamer_SetChunkSize },
    { "Streamer_GetMaxItems", Natives::Streamer_GetMaxItems },
    { "Streamer_SetMaxItems", Natives::Streamer_SetMaxItems },
    { "Streamer_GetVisibleItems", Natives::Streamer_GetVisibleItems },
    { "Streamer_SetVisibleItems", Natives::Streamer_SetVisibleItems },
    { "Streamer_GetRadiusMultiplier", Natives::Streamer_GetRadiusMultiplier },
    { "Streamer_SetRadiusMultiplier", Natives::Streamer_SetRadiusMultiplier },
    { "Streamer_GetTypePriority", Natives::Streamer_GetTypePriority },
    { "Streamer_SetTypePriority", Natives::Streamer_SetTypePriority },
    { "Streamer_GetCellDistance", Natives::Streamer_GetCellDistance },
    { "Streamer_SetCellDistance", Natives::Streamer_SetCellDistance },
    { "Streamer_GetCellSize", Natives::Streamer_GetCellSize },
    { "Streamer_SetCellSize", Natives::Streamer_SetCellSize },
    { "Streamer_ToggleItemStatic", Natives::Streamer_ToggleItemStatic },
    { "Streamer_IsToggleItemStatic", Natives::Streamer_IsToggleItemStatic },
    { "Streamer_ToggleItemInvAreas", Natives::Streamer_ToggleItemInvAreas },
    { "Streamer_IsToggleItemInvAreas", Natives::Streamer_IsToggleItemInvAreas },
    { "Streamer_ToggleItemCallbacks", Natives::Streamer_ToggleItemCallbacks },
    { "Streamer_IsToggleItemCallbacks", Natives::Streamer_IsToggleItemCallbacks },
    { "Streamer_ToggleErrorCallback", Natives::Streamer_ToggleErrorCallback },
    { "Streamer_IsToggleErrorCallback", Natives::Streamer_IsToggleErrorCallback },
    { "Streamer_AmxUnloadDestroyItems", Natives::Streamer_AmxUnloadDestroyItems },
    // Updates
    { "Streamer_ProcessActiveItems", Natives::Streamer_ProcessActiveItems },
    { "Streamer_ToggleIdleUpdate", Natives::Streamer_ToggleIdleUpdate },
    { "Streamer_IsToggleIdleUpdate", Natives::Streamer_IsToggleIdleUpdate },
    { "Streamer_ToggleCameraUpdate", Natives::Streamer_ToggleCameraUpdate },
    { "Streamer_IsToggleCameraUpdate", Natives::Streamer_IsToggleCameraUpdate },
    { "Streamer_ToggleItemUpdate", Natives::Streamer_ToggleItemUpdate },
    { "Streamer_IsToggleItemUpdate", Natives::Streamer_IsToggleItemUpdate },
    { "Streamer_GetLastUpdateTime", Natives::Streamer_GetLastUpdateTime },
    { "Streamer_Update", Natives::Streamer_Update },
    { "Streamer_UpdateEx", Natives::Streamer_UpdateEx },
    // Data Manipulation
    { "Streamer_GetFloatData", Natives::Streamer_GetFloatData },
    { "Streamer_SetFloatData", Natives::Streamer_SetFloatData },
    { "Streamer_GetIntData", Natives::Streamer_GetIntData },
    { "Streamer_SetIntData", Natives::Streamer_SetIntData },
    { "Streamer_RemoveIntData", Natives::Streamer_RemoveIntData },
    { "Streamer_HasIntData", Natives::Streamer_HasIntData },
    { "Streamer_GetArrayData", Natives::Streamer_GetArrayData },
    { "Streamer_SetArrayData", Natives::Streamer_SetArrayData },
    { "Streamer_IsInArrayData", Natives::Streamer_IsInArrayData },
    { "Streamer_AppendArrayData", Natives::Streamer_AppendArrayData },
    { "Streamer_RemoveArrayData", Natives::Streamer_RemoveArrayData },
    { "Streamer_HasArrayData", Natives::Streamer_HasIntData }, // Alias.
    { "Streamer_GetArrayDataLength", Natives::Streamer_GetArrayDataLength },
    { "Streamer_GetUpperBound", Natives::Streamer_GetUpperBound },
    // Miscellaneous
    { "Streamer_GetDistanceToItem", Natives::Streamer_GetDistanceToItem },
    { "Streamer_ToggleItem", Natives::Streamer_ToggleItem },
    { "Streamer_IsToggleItem", Natives::Streamer_IsToggleItem },
    { "Streamer_ToggleAllItems", Natives::Streamer_ToggleAllItems },
    { "Streamer_GetItemInternalID", Natives::Streamer_GetItemInternalID },
    { "Streamer_GetItemStreamerID", Natives::Streamer_GetItemStreamerID },
    { "Streamer_IsItemVisible", Natives::Streamer_IsItemVisible },
    { "Streamer_DestroyAllVisibleItems", Natives::Streamer_DestroyAllVisibleItems },
    { "Streamer_CountVisibleItems", Natives::Streamer_CountVisibleItems },
    { "Streamer_DestroyAllItems", Natives::Streamer_DestroyAllItems },
    { "Streamer_CountItems", Natives::Streamer_CountItems },
    { "Streamer_GetNearbyItems", Natives::Streamer_GetNearbyItems },
    { "Streamer_GetAllVisibleItems", Natives::Streamer_GetAllVisibleItems },
    { "Streamer_GetItemPos", Natives::Streamer_GetItemPos },
    { "Streamer_SetItemPos", Natives::Streamer_SetItemPos },
    { "Streamer_GetItemOffset", Natives::Streamer_GetItemOffset },
    { "Streamer_SetItemOffset", Natives::Streamer_SetItemOffset },
    // Objects
    { "CreateDynamicObject", Natives::CreateDynamicObject },
    { "DestroyDynamicObject", Natives::DestroyDynamicObject },
    { "IsValidDynamicObject", Natives::IsValidDynamicObject },
    { "GetDynamicObjectPos", Natives::GetDynamicObjectPos },
    { "SetDynamicObjectPos", Natives::SetDynamicObjectPos },
    { "GetDynamicObjectRot", Natives::GetDynamicObjectRot },
    { "SetDynamicObjectRot", Natives::SetDynamicObjectRot },
    { "GetDynamicObjectNoCameraCol", Natives::GetDynamicObjectNoCameraCol },
    { "SetDynamicObjectNoCameraCol", Natives::SetDynamicObjectNoCameraCol },
    { "MoveDynamicObject", Natives::MoveDynamicObject },
    { "StopDynamicObject", Natives::StopDynamicObject },
    { "IsDynamicObjectMoving", Natives::IsDynamicObjectMoving },
    { "AttachCameraToDynamicObject", Natives::AttachCameraToDynamicObject },
    { "AttachDynamicObjectToObject", Natives::AttachDynamicObjectToObject },
    { "AttachDynamicObjectToPlayer", Natives::AttachDynamicObjectToPlayer },
    { "AttachDynamicObjectToVehicle", Natives::AttachDynamicObjectToVehicle },
    { "EditDynamicObject", Natives::EditDynamicObject },
    { "IsDynamicObjectMaterialUsed", Natives::IsDynamicObjectMaterialUsed },
    { "RemoveDynamicObjectMaterial", Natives::RemoveDynamicObjectMaterial },
    { "GetDynamicObjectMaterial", Natives::GetDynamicObjectMaterial },
    { "SetDynamicObjectMaterial", Natives::SetDynamicObjectMaterial },
    { "IsDynamicObjectMaterialTextUsed", Natives::IsDynamicObjectMaterialTextUsed },
    {
        "RemoveDynamicObjectMaterialText",
        Natives::RemoveDynamicObjectMaterialText,
    },
    { "GetDynamicObjectMaterialText", Natives::GetDynamicObjectMaterialText },
    { "SetDynamicObjectMaterialText", Natives::SetDynamicObjectMaterialText },
    { "GetPlayerCameraTargetDynObject", Natives::GetPlayerCameraTargetDynObject },
    // Pickups
    { "CreateDynamicPickup", Natives::CreateDynamicPickup },
    { "DestroyDynamicPickup", Natives::DestroyDynamicPickup },
    { "IsValidDynamicPickup", Natives::IsValidDynamicPickup },
    // Checkpoints
    { "CreateDynamicCP", Natives::CreateDynamicCP },
    { "DestroyDynamicCP", Natives::DestroyDynamicCP },
    { "IsValidDynamicCP", Natives::IsValidDynamicCP },
    { "IsPlayerInDynamicCP", Natives::IsPlayerInDynamicCP },
    { "GetPlayerVisibleDynamicCP", Natives::GetPlayerVisibleDynamicCP },
    // Race Checkpoints
    { "CreateDynamicRaceCP", Natives::CreateDynamicRaceCP },
    { "DestroyDynamicRaceCP", Natives::DestroyDynamicRaceCP },
    { "IsValidDynamicRaceCP", Natives::IsValidDynamicRaceCP },
    { "IsPlayerInDynamicRaceCP", Natives::IsPlayerInDynamicRaceCP },
    { "GetPlayerVisibleDynamicRaceCP", Natives::GetPlayerVisibleDynamicRaceCP },
    // Map Icons
    { "CreateDynamicMapIcon", Natives::CreateDynamicMapIcon },
    { "DestroyDynamicMapIcon", Natives::DestroyDynamicMapIcon },
    { "IsValidDynamicMapIcon", Natives::IsValidDynamicMapIcon },
    // 3D Text Labels
    { "CreateDynamic3DTextLabel", Natives::CreateDynamic3DTextLabel },
    { "DestroyDynamic3DTextLabel", Natives::DestroyDynamic3DTextLabel },
    { "IsValidDynamic3DTextLabel", Natives::IsValidDynamic3DTextLabel },
    { "GetDynamic3DTextLabelText", Natives::GetDynamic3DTextLabelText },
    { "UpdateDynamic3DTextLabelText", Natives::UpdateDynamic3DTextLabelText },
    // Areas
    { "CreateDynamicCircle", Natives::CreateDynamicCircle },
    { "CreateDynamicCylinder", Natives::CreateDynamicCylinder },
    { "CreateDynamicSphere", Natives::CreateDynamicSphere },
    { "CreateDynamicRectangle", Natives::CreateDynamicRectangle },
    { "CreateDynamicCuboid", Natives::CreateDynamicCuboid },
    { "CreateDynamicCube", Natives::CreateDynamicCuboid },
    { "CreateDynamicPolygon", Natives::CreateDynamicPolygon },
    { "DestroyDynamicArea", Natives::DestroyDynamicArea },
    { "IsValidDynamicArea", Natives::IsValidDynamicArea },
    { "GetDynamicAreaType", Natives::GetDynamicAreaType },
    { "GetDynamicPolygonPoints", Natives::GetDynamicPolygonPoints },
    { "GetDynamicPolygonNumberPoints", Natives::GetDynamicPolygonNumberPoints },
    { "IsPlayerInDynamicArea", Natives::IsPlayerInDynamicArea },
    { "IsPlayerInAnyDynamicArea", Natives::IsPlayerInAnyDynamicArea },
    { "IsAnyPlayerInDynamicArea", Natives::IsAnyPlayerInDynamicArea },
    { "IsAnyPlayerInAnyDynamicArea", Natives::IsAnyPlayerInAnyDynamicArea },
    { "GetPlayerDynamicAreas", Natives::GetPlayerDynamicAreas },
    { "GetPlayerNumberDynamicAreas", Natives::GetPlayerNumberDynamicAreas },
    { "IsPointInDynamicArea", Natives::IsPointInDynamicArea },
    { "IsPointInAnyDynamicArea", Natives::IsPointInAnyDynamicArea },
    { "IsLineInDynamicArea", Natives::IsLineInDynamicArea },
    { "IsLineInAnyDynamicArea", Natives::IsLineInAnyDynamicArea },
    { "GetDynamicAreasForPoint", Natives::GetDynamicAreasForPoint },
    { "GetNumberDynamicAreasForPoint", Natives::GetNumberDynamicAreasForPoint },
    { "GetDynamicAreasForLine", Natives::GetDynamicAreasForLine },
    { "GetNumberDynamicAreasForLine", Natives::GetNumberDynamicAreasForLine },
    { "AttachDynamicAreaToObject", Natives::AttachDynamicAreaToObject },
    { "AttachDynamicAreaToPlayer", Natives::AttachDynamicAreaToPlayer },
    { "AttachDynamicAreaToVehicle", Natives::AttachDynamicAreaToVehicle },
    { "ToggleDynAreaSpectateMode", Natives::ToggleDynAreaSpectateMode },
    { "IsToggleDynAreaSpectateMode", Natives::IsToggleDynAreaSpectateMode },
    // Actors
    { "CreateDynamicActor", Natives::CreateDynamicActor },
    { "DestroyDynamicActor", Natives::DestroyDynamicActor },
    { "IsValidDynamicActor", Natives::IsValidDynamicActor },
    { "IsDynamicActorStreamedIn", Natives::IsDynamicActorStreamedIn },
    { "GetDynamicActorVirtualWorld", Natives::GetDynamicActorVirtualWorld },
    { "SetDynamicActorVirtualWorld", Natives::SetDynamicActorVirtualWorld },
    { "GetDynamicActorAnimation", Natives::GetDynamicActorAnimation },
    { "ApplyDynamicActorAnimation", Natives::ApplyDynamicActorAnimation },
    { "ClearDynamicActorAnimations", Natives::ClearDynamicActorAnimations },
    { "GetDynamicActorFacingAngle", Natives::GetDynamicActorFacingAngle },
    { "SetDynamicActorFacingAngle", Natives::SetDynamicActorFacingAngle },
    { "GetDynamicActorPos", Natives::GetDynamicActorPos },
    { "SetDynamicActorPos", Natives::SetDynamicActorPos },
    { "GetDynamicActorHealth", Natives::GetDynamicActorHealth },
    { "SetDynamicActorHealth", Natives::SetDynamicActorHealth },
    { "SetDynamicActorInvulnerable", Natives::SetDynamicActorInvulnerable },
    { "IsDynamicActorInvulnerable", Natives::IsDynamicActorInvulnerable },
    { "GetPlayerTargetDynamicActor", Natives::GetPlayerTargetDynamicActor },
    { "GetPlayerCameraTargetDynActor", Natives::GetPlayerCameraTargetDynActor },
    // Extended
    { "CreateDynamicObjectEx", Natives::CreateDynamicObjectEx },
    { "CreateDynamicPickupEx", Natives::CreateDynamicPickupEx },
    { "CreateDynamicCPEx", Natives::CreateDynamicCPEx },
    { "CreateDynamicRaceCPEx", Natives::CreateDynamicRaceCPEx },
    { "CreateDynamicMapIconEx", Natives::CreateDynamicMapIconEx },
    { "CreateDynamic3DTextLabelEx", Natives::CreateDynamic3DTextLabelEx },
    { "CreateDynamicCircleEx", Natives::CreateDynamicCircleEx },
    { "CreateDynamicCylinderEx", Natives::CreateDynamicCylinderEx },
    { "CreateDynamicSphereEx", Natives::CreateDynamicSphereEx },
    { "CreateDynamicRectangleEx", Natives::CreateDynamicRectangleEx },
    { "CreateDynamicCuboidEx", Natives::CreateDynamicCuboidEx },
    { "CreateDynamicCubeEx", Natives::CreateDynamicCuboidEx },
    { "CreateDynamicPolygonEx", Natives::CreateDynamicPolygonEx },
    { "CreateDynamicActorEx", Natives::CreateDynamicActorEx },
    // Deprecated
    { "Streamer_TickRate", Natives::Streamer_SetTickRate },
    { "Streamer_MaxItems", Natives::Streamer_SetMaxItems },
    { "Streamer_VisibleItems", Natives::Streamer_SetVisibleItems },
    { "Streamer_CellDistance", Natives::Streamer_SetCellDistance },
    { "Streamer_CellSize", Natives::Streamer_SetCellSize },
    { "Streamer_CallbackHook", Natives::Streamer_CallbackHook },
    { "DestroyAllDynamicObjects", Natives::DestroyAllDynamicObjects },
    { "CountDynamicObjects", Natives::CountDynamicObjects },
    { "DestroyAllDynamicPickups", Natives::DestroyAllDynamicPickups },
    { "CountDynamicPickups", Natives::CountDynamicPickups },
    { "DestroyAllDynamicCPs", Natives::DestroyAllDynamicCPs },
    { "CountDynamicCPs", Natives::CountDynamicCPs },
    { "DestroyAllDynamicRaceCPs", Natives::DestroyAllDynamicRaceCPs },
    { "CountDynamicRaceCPs", Natives::CountDynamicRaceCPs },
    { "DestroyAllDynamicMapIcons", Natives::DestroyAllDynamicMapIcons },
    { "CountDynamicMapIcons", Natives::CountDynamicMapIcons },
    { "DestroyAllDynamic3DTextLabels", Natives::DestroyAllDynamic3DTextLabels },
    { "CountDynamic3DTextLabels", Natives::CountDynamic3DTextLabels },
    { "DestroyAllDynamicAreas", Natives::DestroyAllDynamicAreas },
    { "CountDynamicAreas", Natives::CountDynamicAreas },
    { "TogglePlayerDynamicCP", Natives::TogglePlayerDynamicCP },
    { "TogglePlayerAllDynamicCPs", Natives::TogglePlayerAllDynamicCPs },
    { "TogglePlayerDynamicRaceCP", Natives::TogglePlayerDynamicRaceCP },
    { "TogglePlayerAllDynamicRaceCPs", Natives::TogglePlayerAllDynamicRaceCPs },
    { "TogglePlayerDynamicArea", Natives::TogglePlayerDynamicArea },
    { "TogglePlayerAllDynamicAreas", Natives::TogglePlayerAllDynamicAreas },
    { 0, 0 }
};

class OmpStreamerComponent final : public CoreEventHandler, public PawnEventHandler, public SingleNetworkOutEventHandler, public IOmpStreamerComponent {
    PROVIDE_UID(0x11897f0dbabe4f7c);

    StringView componentName() const override
    {
        return "open.mp streamer";
    }

    SemanticVersion componentVersion() const override
    {
        return SemanticVersion(0, 0, 0, 1);
    }

    void onLoad(ICore* c) override
    {
        core.reset(new Core);
        omp_core = c;
        players  = &c->getPlayers();
        c->printLn("\n\n ***Streamer Plugin v%s by Incognito loaded ***", PLUGIN_VERSION);
    }

    void onInit(IComponentList* components) override
    {
        ompgdk::GDKManager::Get()->Init(omp_core, components);
        streamerEventHandler.Init(components, players);
        pawnComponent = components->queryComponent<IPawnComponent>();
        if (pawnComponent == nullptr)
        {
            StringView name = componentName();
            omp_core->logLn(
                LogLevel::Error,
                "Error loading component %.*s: Pawn component not loaded",
                name.length(),
                name.data());
            return;
        }

        // add event handlers
        for (auto network : omp_core->getNetworks())
        {
            network->getPerPacketOutEventDispatcher().addEventHandler(this, 207);
        }
        pawnComponent->getEventDispatcher().addEventHandler(this);
        omp_core->getEventDispatcher().addEventHandler(this);
        pAMXFunctions = (void*)&pawnComponent->getAmxFunctions();
        streamerEventHandler.addEvents();
    }

    void onReady() override
    {
        // Fire events here at earliest
    }

    void onTick(Microseconds elapsed, TimePoint now) override
    {
        core->getStreamer()->startAutomaticUpdate();
    }

    void onAmxLoad(IPawnScript& script) override
    {
        core->getData()->interfaces.insert(script.GetAMX());
        core->getData()->amxUnloadDestroyItems.insert(script.GetAMX());
        Utility::checkInterfaceAndRegisterNatives(script.GetAMX(), natives);
    };

    void onAmxUnload(IPawnScript& script) override
    {
        core->getData()->interfaces.erase(script.GetAMX());
        if (core->getData()->amxUnloadDestroyItems.find(script.GetAMX()) != core->getData()->amxUnloadDestroyItems.end())
        {
            Utility::destroyAllItemsInInterface(script.GetAMX());
            core->getData()->amxUnloadDestroyItems.erase(script.GetAMX());
        }
    };

    void onFree(IComponent* component) override
    {
        if (component == pawnComponent)
        {
            pawnComponent = nullptr;
            pAMXFunctions = nullptr;
            omp_core->printLn("\n\n*** Streamer Plugin v%s by Incognito unloaded ***\n\n", PLUGIN_VERSION);
            core.reset();
        }
    }

    void reset() override
    {
    }

    void free() override
    {
        if (pawnComponent != nullptr)
        {
            pawnComponent->getEventDispatcher().removeEventHandler(this);
            omp_core->getEventDispatcher().removeEventHandler(this);
        }
        delete this;
    }

    bool onSend(IPlayer* peer, NetworkBitStream& bs) override
    {
        OMPNetHack::Process(players, peer, bs);
        return true;
    }

    ~OmpStreamerComponent()
    {
        // Clean up what you did above
    }

    // actors
    std::shared_ptr<streamer::IActor> getDynamicActor(int actorId)
    {
        return streamer::actors::GetDynamicActor(actorId);
    }

    std::shared_ptr<streamer::IActor> createDynamicActor(int modelId, const Vector3& position, float rotation, bool invulnerable, float health, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        return streamer::actors::CreateDynamicActor(std::nullopt, modelId, Eigen::Vector3f { position.x, position.y, position.z }, rotation, invulnerable, health, worldId, interiorId, playerId, streamDistance, areaId, priority);
    }

    std::shared_ptr<streamer::IActor> createDynamicActorEx(int modelId, const Vector3& position, float rotation, bool invulnerable, float health, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        return streamer::actors::CreateDynamicActorEx(std::nullopt, modelId, Eigen::Vector3f { position.x, position.y, position.z }, rotation, invulnerable, health, streamDistance, worlds, interiors, players, areas, priority);
    }

    bool destroyDynamicActor(int actorId)
    {
        return streamer::actors::DestroyDynamicActor(actorId);
    }

    bool isDynamicActorStreamedIn(int actorId, int playerId)
    {
        return streamer::actors::IsDynamicActorStreamedIn(actorId, playerId);
    }

    std::shared_ptr<streamer::IActor> getPlayerTargetDynamicActor(int playerId)
    {
        return streamer::actors::GetPlayerTargetDynamicActor(playerId);
    }

    std::shared_ptr<streamer::IActor> getPlayerCameraTargetDynActor(int playerId)
    {
        return streamer::actors::GetPlayerCameraTargetDynActor(playerId);
    }

    // checkpoints
    std::shared_ptr<streamer::ICheckpoint> getDynamicCheckpoint(int checkpointId)
    {
        return streamer::checkpoints::GetDynamicCheckpoint(checkpointId);
    }

    std::shared_ptr<streamer::ICheckpoint> createDynamicCheckpoint(const Vector3& position, float size, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        return streamer::checkpoints::CreateDynamicCheckpoint(std::nullopt, Eigen::Vector3f { position.x, position.y, position.z }, size, worldId, interiorId, playerId, streamDistance, areaId, priority);
    }

    std::shared_ptr<streamer::ICheckpoint> createDynamicCheckpointEx(const Vector3& position, float size, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        return streamer::checkpoints::CreateDynamicCheckpointEx(std::nullopt, Eigen::Vector3f { position.x, position.y, position.z }, size, streamDistance, worlds, interiors, players, areas, priority);
    }

    bool destroyDynamicCheckpoint(int checkpointId)
    {
        return streamer::checkpoints::DestroyDynamicCheckpoint(checkpointId);
    }

    // map icons
    std::shared_ptr<streamer::IMapIcon> getDynamicMapIcon(int mapIconId)
    {
        return streamer::mapicons::GetDynamicMapIcon(mapIconId);
    }

    std::shared_ptr<streamer::IMapIcon> createDynamicMapIcon(int modelId, const Vector3& position, int type, int color, int worldId, int interiorId, int playerId, float streamDistance, MapIconStyle style, int areaId, int priority)
    {
        return streamer::mapicons::CreateDynamicMapIcon(std::nullopt, Eigen::Vector3f { position.x, position.y, position.z }, type, color, worldId, interiorId, playerId, streamDistance, style, areaId, priority);
    }

    std::shared_ptr<streamer::IMapIcon> createDynamicMapIconEx(int modelId, const Vector3& position, int type, int color, int style, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        return streamer::mapicons::CreateDynamicMapIconEx(std::nullopt, Eigen::Vector3f { position.x, position.y, position.z }, type, color, style, streamDistance, worlds, interiors, players, areas, priority);
    }

    bool destroyDynamicMapIcon(int mapIconId)
    {
        return streamer::mapicons::DestroyDynamicMapIcon(mapIconId);
    }

    // objects
    std::shared_ptr<streamer::IObject> getDynamicObject(int objectId)
    {
        return streamer::objects::GetDynamicObject(objectId);
    }

    std::shared_ptr<streamer::IObject> createDynamicObject(int modelId, const Vector3& position, const Vector3& rotation, int worldId, int interiorId, int playerId, float streamDistance, float drawDistance, int areaId, int priority)
    {
        return streamer::objects::CreateDynamicObject(std::nullopt, modelId, Eigen::Vector3f { position.x, position.y, position.z }, Eigen::Vector3f { rotation.x, rotation.y, rotation.z }, worldId, interiorId, playerId, streamDistance, drawDistance, areaId, priority);
    }

    std::shared_ptr<streamer::IObject> createDynamicObjectEx(int modelId, const Vector3& position, const Vector3& rotation, float streamDistance, float drawDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        return streamer::objects::CreateDynamicObjectEx(std::nullopt, modelId, Eigen::Vector3f { position.x, position.y, position.z }, Eigen::Vector3f { rotation.x, rotation.y, rotation.z }, streamDistance, drawDistance, worlds, interiors, players, areas, priority);
    }

    bool destroyDynamicObject(int objectId)
    {
        return streamer::objects::DestroyDynamicObject(objectId);
    }

    std::shared_ptr<streamer::IObject> getPlayerCameraTargetDynObject(int playerId)
    {
        return streamer::objects::GetPlayerCameraTargetDynObject(playerId);
    }

    // pickups
    std::shared_ptr<streamer::IPickup> getDynamicPickup(int pickupId)
    {
        return streamer::pickups::GetDynamicPickup(pickupId);
    }

    std::shared_ptr<streamer::IPickup> createDynamicPickup(int modelId, int type, const Vector3& position, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        return streamer::pickups::CreateDynamicPickup(std::nullopt, modelId, type, Eigen::Vector3f { position.x, position.y, position.z }, worldId, interiorId, playerId, streamDistance, areaId, priority);
    }

    std::shared_ptr<streamer::IPickup> createDynamicPickupEx(int modelId, int type, const Vector3& position, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        return streamer::pickups::CreateDynamicPickupEx(std::nullopt, modelId, type, Eigen::Vector3f { position.x, position.y, position.z }, streamDistance, worlds, interiors, players, areas, priority);
    }

    bool destroyDynamicPickup(int pickupId)
    {
        return streamer::pickups::DestroyDynamicPickup(pickupId);
    }

    // race checkpoints
    std::shared_ptr<streamer::IRaceCheckpoint> getDynamicRaceCheckpoint(int raceCheckpointId)
    {
        return streamer::racecheckpoints::GetDynamicRaceCheckpoint(raceCheckpointId);
    }

    std::shared_ptr<streamer::IRaceCheckpoint> createDynamicRaceCheckpoint(int type, const Vector3& position, const Vector3& next, float size, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        return streamer::racecheckpoints::CreateDynamicRaceCheckpoint(std::nullopt, type, Eigen::Vector3f { position.x, position.y, position.z }, Eigen::Vector3f { next.x, next.y, next.z }, size, worldId, interiorId, playerId, streamDistance, areaId, priority);
    }

    std::shared_ptr<streamer::IRaceCheckpoint> createDynamicRaceCheckpointEx(int type, const Vector3& position, const Vector3& next, float size, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        return streamer::racecheckpoints::CreateDynamicRaceCheckpointEx(std::nullopt, type, Eigen::Vector3f { position.x, position.y, position.z }, Eigen::Vector3f { next.x, next.y, next.z }, size, streamDistance, worlds, interiors, players, areas, priority);
    }

    bool destroyDynamicRaceCheckpoint(int raceCheckpointId)
    {
        return streamer::racecheckpoints::DestroyDynamicRaceCheckpoint(raceCheckpointId);
    }

    // textlabels
    std::shared_ptr<streamer::ITextLabel> getDynamicTextLabel(int textlabelId)
    {
        return streamer::textlabels::GetDynamicTextLabel(textlabelId);
    }

    std::shared_ptr<streamer::ITextLabel> createDynamicTextLabel(const std::string& text, int color, const Vector3& position, float drawDistance, int attachedPlayerId, int attachedVehicleId, bool testLOS, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        return streamer::textlabels::CreateDynamicTextLabel(std::nullopt, text, color, Eigen::Vector3f { position.x, position.y, position.z }, drawDistance, attachedPlayerId, attachedVehicleId, testLOS, worldId, interiorId, playerId, streamDistance, areaId, priority);
    }

    std::shared_ptr<streamer::ITextLabel> createDynamicTextLabelEx(const std::string& text, int color, const Vector3& position, float drawDistance, int attachedPlayerId, int attachedVehicleId, bool testLOS, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        return streamer::textlabels::CreateDynamicTextLabelEx(std::nullopt, text, color, Eigen::Vector3f { position.x, position.y, position.z }, drawDistance, attachedPlayerId, attachedVehicleId, testLOS, streamDistance, worlds, interiors, players, areas, priority);
    }

    bool destroyDynamicTextLabel(int textlabelId)
    {
        return streamer::textlabels::DestroyDynamicTextLabel(textlabelId);
    }

private:
    ICore*          omp_core      = nullptr;
    IPlayerPool*    players       = nullptr;
    IPawnComponent* pawnComponent = nullptr;
    EventHandler    streamerEventHandler;
};

COMPONENT_ENTRY_POINT()
{
    return new OmpStreamerComponent();
}
