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

#ifndef QDECLARATIVECONTACTPERSONID_H
#define QDECLARATIVECONTACTPERSONID_H

#include "qdeclarativecontactdetail_p.h"
#include <qcontactpersonid.h>

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContactPersonId : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QString personid READ personid WRITE setPersonid NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_CLASSINFO("DefaultProperty", "personid")
public:
    enum FieldType {
        Personid = 0
    };

    QDeclarativeContactPersonId(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactPersonId());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    ContactDetailType detailType() const
    {
        return QDeclarativeContactDetail::PersonId;
    }
    static QString fieldNameFromFieldType(int fieldType)
    {
        switch (fieldType) {
        case Personid:
            return QContactPersonId::FieldPersonId;
        default:
            break;
        }
        qmlInfo(0) << tr("Unknown field type.");
        return QString();
    }
    void setPersonid(const QString& v)
    {
        if (!readOnly() && v != personid()) {
            detail().setValue(QContactPersonId::FieldPersonId, v);
            emit valueChanged();
        }
    }
    QString personid() const {return detail().value(QContactPersonId::FieldPersonId).toString();}
signals:
    void valueChanged();
};

QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactPersonId))

#endif // QDECLARATIVECONTACTPERSONID_H
