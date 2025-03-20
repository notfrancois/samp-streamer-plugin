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
#include "objects.hpp"

#include "item.h"
#include "identifier.h"

Identifier Item::Area::identifier;
Identifier Item::Checkpoint::identifier;
Identifier Item::RaceCheckpoint::identifier;

Item::Area::Area() : references(0) {}
Item::Area::Attach::Attach() : references(0) {}
Item::Checkpoint::Checkpoint() : references(0) {}
Item::RaceCheckpoint::RaceCheckpoint() : references(0) {}
