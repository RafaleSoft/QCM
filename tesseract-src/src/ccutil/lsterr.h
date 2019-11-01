/**********************************************************************
 * File:        lsterr.h  (Formerly listerr.h)
 * Description: Errors shared by list modules
 * Author:      Phil Cheatle
 *
 * (C) Copyright 1990, Hewlett-Packard Ltd.
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 ** http://www.apache.org/licenses/LICENSE-2.0
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 *
 **********************************************************************/

#ifndef TESSERACT_CCUTIL_LSTERR_H_
#define TESSERACT_CCUTIL_LSTERR_H_

#include "errcode.h"    //must be last include

CONSTEXPR ERRCODE DONT_CONSTRUCT_LIST_BY_COPY("Can't create a list by assignment");
CONSTEXPR ERRCODE DONT_ASSIGN_LISTS("Can't assign to lists");
CONSTEXPR ERRCODE SERIALISE_LINKS("Attempted to (de)serialise a link element");

#ifndef NDEBUG

CONSTEXPR ERRCODE NO_LIST("Iterator not set to a list");
CONSTEXPR ERRCODE NULL_OBJECT("List found this = nullptr!");
CONSTEXPR ERRCODE NULL_DATA("List would have returned a nullptr data pointer");
CONSTEXPR ERRCODE NULL_CURRENT("List current position is nullptr");
CONSTEXPR ERRCODE NULL_NEXT("Next element on the list is nullptr");
CONSTEXPR ERRCODE NULL_PREV("Previous element on the list is nullptr");
CONSTEXPR ERRCODE EMPTY_LIST("List is empty");
CONSTEXPR ERRCODE BAD_PARAMETER("List parameter error");
CONSTEXPR ERRCODE STILL_LINKED("Attempting to add an element with non nullptr links, to a list");

#endif  // !NDEBUG
#endif  // TESSERACT_CCUTIL_LSTERR_H_
