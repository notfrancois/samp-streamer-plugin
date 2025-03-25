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

namespace streamer::textlabels
{
    struct TextLabel : public ITextLabel
    {
        TextLabel();

        virtual int getID() const override;

        virtual Vector3 getPosition() const override;

        virtual const std::string& getText() const override;
        virtual void               setText(const std::string& text) override;
        virtual void               setText(const std::string& text, uint32_t color) override;

        std::optional<AMX*> amx;
        SharedCell          cell;
        uint32_t           color;
        float               comparableStreamDistance;
        float               drawDistance;
        bool                inverseAreaChecking;
        float               originalComparableStreamDistance;
        Eigen::Vector3f     position;
        Eigen::Vector3f     positionOffset;
        int                 priority;
        int                 references;
        bool                streamCallbacks;
        float               streamDistance;
        bool                testLOS;
        std::string         text;
        int                 textLabelId;

        struct Attach
        {
            Attach();

            int             player;
            Eigen::Vector3f position;
            int             references;
            int             vehicle;

            std::unordered_set<int> worlds;

            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };

        std::shared_ptr<Attach> attach;

        std::unordered_set<int>                   areas;
        std::vector<int>                          extras;
        std::unordered_map<int, std::vector<int>> extraExtras;
        std::unordered_set<int>                   interiors;
        std::bitset<PLAYER_POOL_SIZE>             players;
        std::unordered_set<int>                   worlds;

        static Identifier identifier;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };

    Item::SharedTextLabel GetDynamicTextLabel(int textlabelId);
    Item::SharedTextLabel CreateDynamicTextLabel(std::optional<AMX*> amx, const std::string& text, int color, Eigen::Vector3f position, float drawDistance, int attachedPlayerId = -1, int attachedVehicleId = -1, bool testLOS = false, int worldId = -1, int interiorId = -1, int playerId = -1, float streamDistance = STREAMER_3D_TEXT_LABEL_SD, int areaId = -1, int priority = 0);
    Item::SharedTextLabel CreateDynamicTextLabelEx(std::optional<AMX*> amx, const std::string& text, int color, Eigen::Vector3f position, float drawDistance, int attachedPlayerId, int attachedVehicleId, bool testLOS, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority);
    bool                  DestroyDynamicTextLabel(int textlabelId);
} // namespace streamer::textlabels
