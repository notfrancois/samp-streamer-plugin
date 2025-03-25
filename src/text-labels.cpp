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

#include "text-labels.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::textlabels
{
    Identifier TextLabel::identifier;

    TextLabel::TextLabel()
        : references(0) {}

    TextLabel::Attach::Attach()
        : references(0) {}

    int TextLabel::getID() const
    {
        return textLabelId;
    }

    Vector3 TextLabel::getPosition() const
    {
        return { position[0], position[1], position[2] };
    }

    const std::string& TextLabel::getText() const
    {
        return text;
    }

    void TextLabel::setText(const std::string& newtext)
    {
        text = newtext;
        uint32_t color = 0xFFFFFFFF;
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(textLabelId);
            if (i != p->second.internalTextLabels.end())
            {
                ompgdk::UpdatePlayer3DTextLabelText(p->first, i->second, color, text.c_str());
            }
        }
    }

    void TextLabel::setText(const std::string& newtext, uint32_t newcolor)
    {
        color = newcolor;
        text  = newtext;
        for (std::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
        {
            std::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(textLabelId);
            if (i != p->second.internalTextLabels.end())
            {
                ompgdk::UpdatePlayer3DTextLabelText(p->first, i->second, newcolor, text.c_str());
            }
        }
    }

    Item::SharedTextLabel GetDynamicTextLabel(int textlabelId)
    {
        std::unordered_map<int, Item::SharedTextLabel>::iterator o = core->getData()->textLabels.find(textlabelId);
        if (o != core->getData()->textLabels.end())
        {
            return o->second;
        }
        return nullptr;
    }

    Item::SharedTextLabel CreateDynamicTextLabel(std::optional<AMX*> amx, const std::string& text, int color, Eigen::Vector3f position, float drawDistance, int attachedPlayerId, int attachedVehicleId, bool testLOS, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_3D_TEXT_LABEL) == core->getData()->textLabels.size())
        {
            return nullptr;
        }
        int                   textLabelId = TextLabel::identifier.get();
        Item::SharedTextLabel textLabel(new TextLabel);
        textLabel->amx                              = amx;
        textLabel->textLabelId                      = textLabelId;
        textLabel->inverseAreaChecking              = false;
        textLabel->originalComparableStreamDistance = -1.0f;
        textLabel->positionOffset                   = Eigen::Vector3f::Zero();
        textLabel->streamCallbacks                  = false;
        textLabel->text                             = text;
        textLabel->color                            = color;
        textLabel->position                         = position;
        textLabel->drawDistance                     = drawDistance;
        if (attachedPlayerId != INVALID_PLAYER_ID || attachedVehicleId != INVALID_VEHICLE_ID)
        {
            textLabel->attach          = std::make_shared<TextLabel::Attach>();
            textLabel->attach->player  = attachedPlayerId;
            textLabel->attach->vehicle = attachedVehicleId;
            if (textLabel->position.cwiseAbs().maxCoeff() > 50.0f)
            {
                textLabel->position.setZero();
            }
            core->getStreamer()->attachedTextLabels.insert(textLabel);
        }
        textLabel->testLOS = testLOS;
        Utility::addToContainer(textLabel->worlds, worldId);
        Utility::addToContainer(textLabel->interiors, interiorId);
        Utility::addToContainer(textLabel->players, playerId);
        textLabel->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        textLabel->streamDistance           = streamDistance;
        Utility::addToContainer(textLabel->areas, areaId);
        textLabel->priority = priority;
        core->getGrid()->addTextLabel(textLabel);
        core->getData()->textLabels.insert(std::make_pair(textLabelId, textLabel));
        return textLabel;
    }

    Item::SharedTextLabel CreateDynamicTextLabelEx(std::optional<AMX*> amx, const std::string& text, int color, Eigen::Vector3f position, float drawDistance, int attachedPlayerId, int attachedVehicleId, bool testLOS, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_3D_TEXT_LABEL) == core->getData()->textLabels.size())
        {
            return nullptr;
        }
        int                   textLabelId = TextLabel::identifier.get();
        Item::SharedTextLabel textLabel(new TextLabel);
        textLabel->amx                              = amx;
        textLabel->textLabelId                      = textLabelId;
        textLabel->inverseAreaChecking              = false;
        textLabel->originalComparableStreamDistance = -1.0f;
        textLabel->positionOffset                   = Eigen::Vector3f::Zero();
        textLabel->streamCallbacks                  = false;
        textLabel->text                             = text;
        textLabel->color                            = color;
        textLabel->position                         = position;
        textLabel->drawDistance                     = drawDistance;
        if (attachedPlayerId != INVALID_PLAYER_ID || attachedVehicleId != INVALID_VEHICLE_ID)
        {
            textLabel->attach          = std::make_shared<TextLabel::Attach>();
            textLabel->attach->player  = attachedPlayerId;
            textLabel->attach->vehicle = attachedVehicleId;
            if (textLabel->position.cwiseAbs().maxCoeff() > 50.0f)
            {
                textLabel->position.setZero();
            }
            core->getStreamer()->attachedTextLabels.insert(textLabel);
        }
        textLabel->testLOS                  = testLOS;
        textLabel->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
        textLabel->streamDistance           = streamDistance;
        textLabel->worlds.insert(worlds.begin(), worlds.end());
        textLabel->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            textLabel->players.set(player);
        }
        textLabel->areas.insert(areas.begin(), areas.end());
        textLabel->priority = priority;
        core->getGrid()->addTextLabel(textLabel);
        core->getData()->textLabels.insert(std::make_pair(textLabelId, textLabel));
        return textLabel;
    }

    bool DestroyDynamicTextLabel(int textlabelId)
    {
        auto o = core->getData()->textLabels.find(textlabelId);
        if (o != core->getData()->textLabels.end())
        {
            Utility::destroyTextLabel(o);
            return true;
        }
        return false;
    }
} // namespace streamer::textlabels
