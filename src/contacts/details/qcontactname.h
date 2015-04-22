/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTNAME_H
#define QCONTACTNAME_H

#include <QtContacts/qcontactdetail.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactFilter;

/* Leaf class */
class Q_CONTACTS_EXPORT QContactName : public QContactDetail
{
public:
#ifndef Q_QDOC
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactName)
#else
    static const DetailType Type;
#endif

    enum NameField {
        FieldPrefix = 0,
        FieldFirstName,
        FieldMiddleName,
        FieldLastName,
        FieldSuffix
    };

    QString prefix() const {return value(FieldPrefix).toString();}
    QString firstName() const {return value(FieldFirstName).toString();}
    QString middleName() const {return value(FieldMiddleName).toString();}
    QString lastName() const {return value(FieldLastName).toString();}
    QString suffix() const {return value(FieldSuffix).toString();}

    void setPrefix(const QString& _prefix) {setValue(FieldPrefix, _prefix);}
    void setFirstName(const QString& _firstName) {setValue(FieldFirstName, _firstName);}
    void setMiddleName(const QString& _middleName) {setValue(FieldMiddleName, _middleName);}
    void setLastName(const QString& _lastName) {setValue(FieldLastName, _lastName);}
    void setSuffix(const QString& _suffix) {setValue(FieldSuffix, _suffix);}

    // Convenience filter
    static QContactFilter match(const QString& name);
    static QContactFilter match(const QString& firstName, const QString& lastName);
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTNAME_H
