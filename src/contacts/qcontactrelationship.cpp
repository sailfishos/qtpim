/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include <qcontactrelationship.h>
#include "qcontactrelationship_p.h"
#include <qcontact.h>

#include <QList>
#include <QPair>
#include <QString>
#include <QHash>
#include <QDebug>
#include <QDataStream>

QTCONTACTS_BEGIN_NAMESPACE

/*!
  \class QContactRelationship
  \brief The QContactRelationship class describes a one-to-one relationship
  between a locally-stored contact and another (possibly remote) contact.

  \inmodule QtContacts

  \ingroup contacts-main

  Each relationship is uniquely identified by the combination of the first
  contact, second contact, and the relationship type.

  A relationship should not contain a second contact which is the same as
  the first contact.
  Any local contacts which are referenced in the relationship (that is, any
  source contact, or any second contact whose manager URI is left empty or
  whose manager URI references the manager that stores the source contact,
  and in which the relationship will be saved) should exist.

  If any of these requirements are not met, validation of the relationship
  may fail when attempting to save the relationship in a QContactManager.

  \sa QContactRelationshipFilter
 */

/*!
  \enum QContactRelationship::Role
  Describes the roles that a contact may take in a relationship.
  \value First The contact is the first contact in the relationship
  \value Second The contact is the second contact in the relationship
  \value Either The contact is either the first or second contact in the relationship
 */

/*!
 * \fn QContactRelationship::HasMember()
 * The relationship type which identifies the first contact as being a group which includes the second contact
 */

/*!
 * \fn QContactRelationship::Aggregates()
 * The relationship type which identifies the first contact as aggregating the second contact into a metacontact
 */

/*!
 * \fn QContactRelationship::IsSameAs()
 * The relationship type which identifies the first contact as being the same contact as the second contact
 */

/*!
 * \fn QContactRelationship::HasAssistant()
 * The relationship type which identifies the second contact as being the assistant of the first contact
 */

/*!
 * \fn QContactRelationship::HasManager()
 * The relationship type which identifies the second contact as being the manager of the first contact
 */

/*!
 * \fn QContactRelationship::HasSpouse()
 * The relationship type which identifies the second contact as being the spouse of the first contact
 */

/*!
 * Constructs a new relationship
 */
QContactRelationship::QContactRelationship()
        : d(new QContactRelationshipPrivate)
{
}

/*!
 * Frees the memory in use by the relationship
 */
QContactRelationship::~QContactRelationship()
{
}

/*!
 * Creates a copy of the \a other relationship
 */
QContactRelationship::QContactRelationship(const QContactRelationship& other)
        : d(other.d)
{
}

/*!
 * Assigns this relationship to be equal to \a other
 */
QContactRelationship& QContactRelationship::operator=(const QContactRelationship& other)
{
    d = other.d;
    return *this;
}

/*!
 * Returns true if this relationship is equal to the \a other relationship, otherwise returns false.
 */
bool QContactRelationship::operator==(const QContactRelationship &other) const
{
    if (d.constData()->m_first != other.d.constData()->m_first)
        return false;
    if (d.constData()->m_second != other.d.constData()->m_second)
        return false;
    if (d.constData()->m_relationshipType != other.d.constData()->m_relationshipType)
        return false;
    return true;
}

/*!
 * Returns the hash value for \a key.
 */
uint qHash(const QContactRelationship &key)
{
    return qHash(key.first()) + qHash(key.second())
        + QT_PREPEND_NAMESPACE(qHash)(key.relationshipType());
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QContactRelationship& rel)
{
    dbg.nospace() << "QContactRelationship(" << rel.first() << ' ' << rel.relationshipType()
            << ' ' << rel.second() << ')';
    return dbg.maybeSpace();
}
#endif

#ifndef QT_NO_DATASTREAM
/*!
 * Writes \a rel to the stream \a out.
 */
QDataStream& operator<<(QDataStream& out, const QContactRelationship& rel)
{
    quint8 formatVersion = 1; // Version of QDataStream format for QContactRelationship
    return out << formatVersion << rel.first() << rel.relationshipType() << rel.second();
}

/*!
 * Reads a contact relationship from stream \a in into \a rel.
 */
QDataStream& operator>>(QDataStream& in, QContactRelationship& rel)
{
    rel = QContactRelationship();
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1) {
        QContact first;
        QString relationshipType;
        QContact second;
        in >> first >> relationshipType >> second;
        rel.setFirst(first);
        rel.setRelationshipType(relationshipType);
        rel.setSecond(second);
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}
#endif

/*!
 * \fn QContactRelationship::operator!=(const QContactRelationship& other) const
 * Returns true if this relationship is not equal to \a other, otherwise returns false.
 */

/*!
 * Returns the locally-stored contact which has a relationship of the given type with the second contact
 * \sa relationshipType(), second(), setFirst()
 */
QContact QContactRelationship::first() const
{
    return d.constData()->m_first;
}

/*!
 * Returns the contact with which the first contact has a relationship of the given type
 * \sa relationshipType(), first()
 */
QContact QContactRelationship::second() const
{
    return d.constData()->m_second;
}

/*!
 * Returns the type of relationship which the source contact has with the destination contacts
 * \sa setRelationshipType()
 */
QString QContactRelationship::relationshipType() const
{
    return d.constData()->m_relationshipType;
}

/*!
 * Sets the first contact in the relationship to \a firstContact.  This contact
 * must be stored in the manager in which the relationship is stored, and has
 * a relationship of the specified type with the second contact.
 * \sa first()
 */
void QContactRelationship::setFirst(const QContact& firstContact)
{
    d->m_first = firstContact;
}

/*!
 * Sets the second contact in the relationship to \a secondContact.  The first contact
 * has a relationship of the specified type with this contact.
 * \sa second()
 */
void QContactRelationship::setSecond(const QContact& secondContact)
{
    d->m_second = secondContact;
}

/*!
 * Sets the type of relationship that the source contact has with the destination contacts
 * to \a relationshipType.
 * \sa relationshipType()
 */
void QContactRelationship::setRelationshipType(const QString& relationshipType)
{
    d->m_relationshipType = relationshipType;
}

QTCONTACTS_END_NAMESPACE
