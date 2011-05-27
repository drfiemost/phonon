/*
    Copyright (C) 2011 Harald Sitter <sitter@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), Nokia Corporation
    (or its successors, if any) and the KDE Free Qt Foundation, which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "abstractmediaelement.h"

#include <phonon/mediaobject.h>

namespace Phonon {
namespace Declarative {

AbstractMediaElement::AbstractMediaElement() :
    m_mediaObject(0),
    m_finished(false)
{
}

AbstractMediaElement::~AbstractMediaElement()
{
}

QUrl AbstractMediaElement::source() const
{
    return m_source;
}

void AbstractMediaElement::setSource(const QUrl &url)
{
    m_source = url;
    m_finished = false;
    if (m_mediaObject)
        m_mediaObject->setCurrentSource(MediaSource(url));
    emit sourceChanged();
}

void AbstractMediaElement::initElement(QObject *parent)
{
    Q_ASSERT(!m_mediaObject);
    m_mediaObject = new MediaObject(parent);
    m_mediaObject->setCurrentSource(MediaSource(m_source));
}


} // namespace Declarative
} // namespace Phonon
