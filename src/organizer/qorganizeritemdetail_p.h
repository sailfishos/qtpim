/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QORGANIZERITEMDETAIL_P_H
#define QORGANIZERITEMDETAIL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qorganizeritemdetail.h>

#include <QtCore/qmap.h>

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerItemDetailPrivate : public QSharedData
{
public:
    QOrganizerItemDetailPrivate(QOrganizerItemDetail::DetailType detailType)
        : QSharedData()
        , m_id(lastDetailKey().fetchAndAddOrdered(1))
        , m_detailType(detailType)
    {
    }

    QOrganizerItemDetailPrivate(const QOrganizerItemDetailPrivate &other)
        : QSharedData(other)
        , m_id(other.m_id)
        , m_detailType(other.m_detailType)
        , m_values(other.m_values)
    {
    }

    ~QOrganizerItemDetailPrivate()
    {
    }

    int m_id; // internal, unique id.
    QOrganizerItemDetail::DetailType m_detailType;
    QMap<int, QVariant> m_values;

    static QAtomicInt &lastDetailKey()
    {
        static QAtomicInt counter(0);
        return counter;
    }
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERITEMDETAIL_P_H
