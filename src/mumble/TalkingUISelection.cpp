// Copyright 2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TalkingUISelection.h"
#include "MainWindow.h"
#include "UserModel.h"

#include <QWidget>
#include <QVariant>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

TalkingUISelection::TalkingUISelection(QWidget *widget) : m_widget(widget) {
}


void TalkingUISelection::setActive(bool active) {
	if (m_widget) {
		m_widget->setProperty("selected", active);
		// Unpolish the widget's style so that the new property can take effect
		m_widget->style()->unpolish(m_widget);
	}
}

void TalkingUISelection::apply() {
	setActive(true);
}

void TalkingUISelection::discard() {
	setActive(false);
}

bool TalkingUISelection::operator==(const TalkingUISelection &other) const {
	return m_widget == other.m_widget;
}

bool TalkingUISelection::operator!=(const TalkingUISelection &other) const {
	return m_widget != other.m_widget;
}



UserSelection::UserSelection(QWidget *widget, unsigned int userSession)
	: TalkingUISelection(widget),
	  m_userSession(userSession) {
}

void UserSelection::syncToMainWindow() const {
	if (g.mw && g.mw->pmModel) {
		g.mw->pmModel->setSelectedUser(m_userSession);
	}
}

std::unique_ptr<TalkingUISelection> UserSelection::cloneToHeap() const {
	return std::make_unique<UserSelection>(*this);
}



ChannelSelection::ChannelSelection(QWidget *widget, int channelID)
	: TalkingUISelection(widget),
	  m_channelID(channelID) {
}

void ChannelSelection::syncToMainWindow() const {
	if (g.mw && g.mw->pmModel) {
		g.mw->pmModel->setSelectedChannel(m_channelID);
	}
}

std::unique_ptr<TalkingUISelection> ChannelSelection::cloneToHeap() const {
	return std::make_unique<ChannelSelection>(*this);
}



void EmptySelection::syncToMainWindow() const {
	// Do nothing
}

std::unique_ptr<TalkingUISelection> EmptySelection::cloneToHeap() const {
	return std::make_unique<EmptySelection>(*this);
}
