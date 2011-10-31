/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTFETCHHINT_H
#define QCONTACTFETCHHINT_H

#include <QVariant>
#include <QList>
#include <QSharedData>
#include <QSize>
#include "qcontactsglobal.h"

QTCONTACTS_BEGIN_NAMESPACE

class QContactFetchHintPrivate;
class Q_CONTACTS_EXPORT QContactFetchHint {
public:
    QContactFetchHint();
    QContactFetchHint(const QContactFetchHint& other);
    ~QContactFetchHint();
    QContactFetchHint& operator=(const QContactFetchHint& other);

    QStringList detailDefinitionsHint() const;
    void setDetailDefinitionsHint(const QStringList& definitionNames);

    QStringList relationshipTypesHint() const;
    void setRelationshipTypesHint(const QStringList& relationshipTypes);

    QSize preferredImageSize() const;
    void setPreferredImageSize(const QSize& size);

    enum OptimizationHint {
        AllRequired = 0x0,
        NoRelationships = 0x1,
        NoActionPreferences = 0x2,
        NoBinaryBlobs = 0x4
        // any other optimization hints?
    };
    Q_DECLARE_FLAGS(OptimizationHints, OptimizationHint)

    OptimizationHints optimizationHints() const;
    void setOptimizationHints(OptimizationHints hints);

    int maxCountHint() const;
    void setMaxCountHint(int count);

private:
    QSharedDataPointer<QContactFetchHintPrivate> d;
};

#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream& operator<<(QDataStream& out, const QContactFetchHint& hint);
Q_CONTACTS_EXPORT QDataStream& operator>>(QDataStream& in, QContactFetchHint& hint);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContactFetchHint& hint);
#endif

Q_DECLARE_OPERATORS_FOR_FLAGS(QContactFetchHint::OptimizationHints);

QTCONTACTS_END_NAMESPACE


#endif
