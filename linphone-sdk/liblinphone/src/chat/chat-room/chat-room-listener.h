/*
 * Copyright (c) 2010-2022 Belledonne Communications SARL.
 *
 * This file is part of Liblinphone
 * (see https://gitlab.linphone.org/BC/public/liblinphone).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _L_CHAT_ROOM_LISTENER_H_
#define _L_CHAT_ROOM_LISTENER_H_

#include <bctoolbox/defs.h>

#include "chat/chat-room/abstract-chat-room.h"

// =============================================================================

LINPHONE_BEGIN_NAMESPACE

class ChatRoomListener {
public:
	virtual ~ChatRoomListener() = default;

	virtual void onChatRoomInsertRequested(BCTBX_UNUSED(const std::shared_ptr<AbstractChatRoom> &chatRoom)) {
	}
	virtual void onChatRoomInsertInDatabaseRequested(BCTBX_UNUSED(const std::shared_ptr<AbstractChatRoom> &chatRoom)) {
	}
	virtual void onChatRoomDeleteRequested(BCTBX_UNUSED(const std::shared_ptr<AbstractChatRoom> &chatRoom)) {
	}
};

LINPHONE_END_NAMESPACE

#endif // ifndef _L_CHAT_ROOM_LISTENER_H_
