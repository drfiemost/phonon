/*  This file is part of the KDE project
    Copyright (C) 2008 Matthias Kretz <kretz@kde.org>

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

#ifndef PHONON_X_ABSTRACTAUDIODATAOUTPUT_P_H
#define PHONON_X_ABSTRACTAUDIODATAOUTPUT_P_H

#include "abstractaudiodataoutput.h"
#include "../medianode_p.h"
#include "phonondefs_p.h"

namespace Phonon
{
namespace Experimental
{

class AbstractAudioDataOutputPrivate : public Phonon::MediaNodePrivate
{
    P_DECLARE_PUBLIC(AbstractAudioDataOutput)
    protected:
        virtual bool aboutToDeleteBackendObject() { return true; }
        virtual void createBackendObject();
        void setupBackendObject();

    private:
        bool isRunning;
        QSet<AudioFormat> allowedFormats;
};

} // namespace Experimental
} // namespace Phonon

#endif // PHONON_X_ABSTRACTAUDIODATAOUTPUT_P_H
