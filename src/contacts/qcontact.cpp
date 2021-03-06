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

#include "qcontact.h"
#include "qcontact_p.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif
#include <QtCore/qset.h>

#include "qcontactactiondescriptor.h"
#include "qcontactdetail_p.h"
#include "qcontactdetails.h"
#include "qcontactmanager_p.h"
#include "qcontactactionmanager_p.h"
#include "qcontactaction.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContact

  \brief The QContact class represents an addressbook contact.

  \inmodule QtContacts

  \ingroup contacts-main

  Individual contacts, groups, and other types of contacts are represented with
  a QContact object.  In addition to the type, a QContact consists of information
  that belongs to the contact, some information about the relationships that the
  contact has, and the preferred ways to interact with the contact.

  A QContact object has a collection of details (like a name, phone numbers and
  email addresses).  Each detail (which can have multiple fields) is stored
  in an appropriate subclass of QContactDetail, and the QContact allows
  retrieving these details in various ways.

  Depending on the details of the QContact, certain actions are available for a
  contact.  An instance of a QContact can return a list of actions that can be
  performed on it, and a list of details supported by a specific action can be
  retrieved (for example - a list of phone numbers supported by a specific "Call" action).
  It is also possible to store one detail for each type of action that is the "preferred"
  detail to use.

  A QContact may have zero or more relationships with other contacts.  For example,
  a group contact would have a \c "HasMember" relationship with the QContacts that
  are its members.  Spouses, managers and assistants can also be represented this
  way.

  A QContact instance represents the in-memory version of an addressbook contact,
  and has no tie to a specific QContactManager.  It is possible for the contents
  of a QContact to change independently of the contents that are stored persistently
  in a QContactManager.  A QContact has an ID associated with it when it is first
  retrieved from a QContactManager, or after it has been first saved, and this allows
  clients to track changes using the signals in QContactManager.

  A QContact has a number of mandatory details:
  \list
   \li A QContactType, with the type of the contact (individual contact, group etc)
  \endlist

  \sa QContactManager, QContactDetail
 */

/*!
 * \fn QList<T> QContact::details() const
 * Returns a list of details of the template parameter type.  The type must be
 * a subclass of QContactDetail.
 *
 * For example:
 *  \snippet qtcontactsdocsample/qtcontactsdocsample.cpp 3
 */

/*!
 * \fn T QContact::detail() const
 * Returns the first detail of the template parameter type, as returned by the template details() function.
 * The type must be a subclass of QContactDetail.
 */

/*!
 * \fn QContact::operator!=(const QContact &other) const
 * Returns true if this contacts id or details are different to those of the \a other contact.
 */

/*!
    Construct an empty contact.

    The contact will have an empty id, and have type \l QContactType::TypeContact.
    The isEmpty() function will return true.
*/
QContact::QContact()
    : d(new QContactData)
{
    clearDetails();
}

/*! Initializes this QContact from \a other */
QContact::QContact(const QContact& other)
    : d(other.d)
{
}

/*!
 * Returns true if this QContact is empty, false if not.
 *
 * An empty QContact has no extra details.
 * The type of the contact is irrelevant.
 */
bool QContact::isEmpty() const
{
    /* Every contact has a type field */
    return (d->m_details.count() == 1);
}

/*!
 * Removes all details of the contact.
 * This function does not modify the id or type of the contact.
 * Calling isEmpty() after calling this function will return true.
 */
void QContact::clearDetails()
{
    d->m_details.clear();

    // insert the contact type detail.
    QContactType contactType;
    contactType.setType(QContactType::TypeContact);
    contactType.d->m_access = QContactDetail::Irremovable;
    d->m_details.insert(0, contactType);
}

/*! Replace the contents of this QContact with \a other
*/
QContact& QContact::operator=(const QContact& other)
{
    d = other.d;
    return *this;
}

/*! Frees the memory used by this QContact */
QContact::~QContact()
{
}

/*!
    Returns the QContactId that identifies this contact.

    This may have been set when the contact was retrieved from
    a particular manager, or when the contact was first saved
    in a manager.  The QContactId is only valid with a specific
    manager.  See \l QContactManager::saveContact() for more
    information.

 */
QContactId QContact::id() const
{
    return d.constData()->m_id;
}

/*!
 * Sets the id of this contact to \a id.
 *
 * Note that this only affects this object, not any corresponding structures stored
 * by a QContactManager.
 *
 * If you change the id of a contact and save the contact
 * in a manager, the previously existing contact will still
 * exist.  You can do this to create copies (possibly modified)
 * of an existing contact, or to save a contact in a different manager.
 *
 * \sa QContactManager::saveContact()
 */
void QContact::setId(const QContactId& id)
{
    d->m_id = id;
}


/*!
 * Returns the type of the contact.  Every contact has exactly one type which
 * is either set manually (by saving a modified copy of the QContactType
 * in the contact, or by calling \l setType()) or synthesized automatically.
 *
 * \sa setType()
 */

QContactType::TypeValues QContact::type() const
{
    // type is detail 0
    QContactType::TypeValues type = static_cast<QContactType::TypeValues>(d.constData()->m_details.at(0).value(QContactType::FieldType).toInt());
    return type;
}


/*!
 * Sets the type of the contact to the given \a type.
 */
void QContact::setType(const QContactType::TypeValues& type)
{
    // type is detail 0
    d->m_details[0].setValue(QContactType::FieldType, type);
    d->m_details[0].d->m_access = QContactDetail::Irremovable;
}

/*!
 * Returns the list of tags for this contact.  Tags are used for non-exclusive categorization.
 *
 * \sa QContactTag
 */
QStringList QContact::tags() const
{
    QStringList tags;
    foreach (const QContactTag& tagDetail, details<QContactTag>()) {
        tags.append(tagDetail.tag());
    }
    return tags;
}

/*!
 * Removes all tags associated with the contact.
 *
 * \sa QContactTag
 */
void QContact::clearTags()
{
    d->removeOnly(QContactTag::Type);
}

/*!
 * Adds the \a tag to this contact.
 *
 * \sa QContactTag
 */
void QContact::addTag(const QString& tag)
{
    QContactTag tagDetail;
    tagDetail.setTag(tag);
    saveDetail(&tagDetail);
}

/*!
 * Sets the list of tags associated with the contact to \a tags.
 *
 * \sa QContactTag
 */
void QContact::setTags(const QStringList& tags)
{
    d->removeOnly(QContactTag::Type);
    foreach (const QString& tag, tags) {
        addTag(tag);
    }
}



/*!
    \fn QContactDetail QContact::detail(QContactDetail::DetailType type) const
    Returns the first detail stored in the contact which with the given \a type.
    The \a type argument is typically the detail type constant provided by a
    specific subclass of QContactDetail.  For example:

    \snippet qtcontactsdocsample/qtcontactsdocsample.cpp 0

    It would usually be more convenient to use the template version of this function, in
    the following manner:

    \snippet qtcontactsdocsample/qtcontactsdocsample.cpp 1
*/
QContactDetail QContact::detail(QContactDetail::DetailType type) const
{
    // If type not defined, return first detail
    if (type == QContactDetail::TypeUndefined)
        return d.constData()->m_details.first();

    // build the sub-list of matching details.
    for (int i = 0; i < d.constData()->m_details.size(); i++) {
        const QContactDetail& existing = d.constData()->m_details.at(i);
        if (existing.d->m_type == type) {
            return existing;
        }
    }

    return QContactDetail();
}

/*!
    \fn QList<QContactDetail> QContact::details(QContactDetail::DetailType type) const
    Returns a list of details of the given \a type.

    The \a type argument is typically the detail type constant provided by a
    specific subclass of QContactDetail.  For example:

    \snippet qtcontactsdocsample/qtcontactsdocsample.cpp 2

    It would usually be more convenient to use the template version of this function, in
    the following manner:

    \snippet qtcontactsdocsample/qtcontactsdocsample.cpp 3
*/
QList<QContactDetail> QContact::details(QContactDetail::DetailType type) const
{
    // build the sub-list of matching details.
    QList<QContactDetail> sublist;

    // special case
    if (type == QContactDetail::TypeUndefined) {
        sublist = d.constData()->m_details;
    } else {
        for (int i = 0; i < d->m_details.size(); i++) {
            const QContactDetail& existing = d.constData()->m_details.at(i);
            if (existing.d->m_type == type) {
                sublist.append(existing);
            }
        }
    }

    return sublist;
}

/*!
 * Appends the given \a detail to the list of stored details.
 * This is a convenience method intended to be used e.g. by backend
 * developers to populate an empty QContact object when fetching
 * data from the backend.
 * If \a detail is a QContactType, the existing contact type will
 * be overwritten with \a detail. There is never more than one contact type
 * in a contact.
 *
 * Note that if another detail of the same type and id has been previously saved in
 * this contact, that detail is duplicated. For this reason, this method
 * should not be used to update an existing contact object with a newer version
 * of an existing detail. For this use case, the clients must use the
 * saveDetail() method.
 *
 * Returns true if the detail was appended successfully, otherwise returns false.
 *
 * \sa saveDetail()
 */
bool QContact::appendDetail(const QContactDetail &detail)
{
    if (detail.isEmpty())
        return false;

    /* Also handle contact type specially - only one of them. */
    if (detail.d->m_type == QContactType::Type) {
        d->m_details[0] = detail;
        d->m_details[0].d->m_access |= QContactDetail::Irremovable;
        return true;
    }
    d->m_details.append(detail);
    return true;
}


/*!
 * Saves the given \a detail in the list of stored details, and sets the detail's id.
 * If another detail of the same type and id has been previously saved in
 * this contact, that detail is overwritten.  Otherwise, a new id is generated
 * and set in the detail, and the detail is added to the contact.
 *
 * If the detail was previously saved, and its access constraint includes
 * \c QContactDetail::ReadOnly, this function will return false if \a enforce
 * is set to \c QContact::EnforceAccessConstraints.
 * Otherwise, the detail will be saved, but any attempt to save the contact
 * in the manager may fail due to the backend enforcing the original constraints
 * (regardless of whether \a enforce is set to \c QContact::ReplaceAccessConstraints,
 * which is provided for the convenience of backend implementors).
 *
 * Details with the \c QContactDetail::ReadOnly constraint set are typically provided
 * in a contact by the manager, and are usually information that is either
 * synthesized, or not intended to be changed by the user (e.g. presence information
 * for other contacts).
 *
 * If \a detail is a QContactType, the existing contact type will
 * be overwritten with \a detail.  There is never more than one contact type
 * in a contact.
 *
 *
 * Be aware that if a contact is retrieved (or reloaded) from the backend, the
 * keys of any details it contains may have been changed by the backend, or other
 * threads may have modified the contact details in the backend.  Therefore,
 * clients should reload the detail that they wish to save in a contact after retrieving
 * the contact, in order to avoid creating unwanted duplicated details.
 *
 * Returns true if the detail was saved successfully, otherwise returns false.
 *
 * Note that the caller retains ownership of the detail.
 */
bool QContact::saveDetail(QContactDetail* detail, AccessConstraintsEnforcement enforce)
{
    if (!detail)
        return false;

    /* Also handle contact type specially - only one of them. */
    if (detail->d->m_type == QContactType::Type) {
        detail->d->m_access |= QContactDetail::Irremovable;
        d->m_details[0] = *detail;
        return true;
    }

    // try to find the "old version" of this field
    // ie, the one with the same type and id, but different value or attributes.
    for (int i = 0; i < d.constData()->m_details.size(); i++) {
        const QContactDetail& curr = d.constData()->m_details.at(i);
        if (detail->d->m_type == curr.d->m_type &&
                detail->d->m_detailId == curr.d->m_detailId) {
            // Found the old version.  Replace it with this one.
            QContactDetail::AccessConstraints oldAccess = detail->d->m_access;
            if (enforce == QContact::EnforceAccessConstraints) {
                // fail the operation if the original detail is immutable.
                if (curr.accessConstraints() & QContactDetail::ReadOnly) {
                    return false;
                }
                // enforce detail constraints of the original detail
                detail->d->m_access = curr.accessConstraints();
            } else if (enforce == QContact::IgnoreAccessConstraints) {
                // keep the detail constraints of the original detail
                // but ignore them and apply the values change in memory.
                detail->d->m_access = curr.accessConstraints();
            } // else QContact::ReplaceAccessConstraints, so keep as-is.
            // add the detail to the contact
            d->m_details[i] = *detail;
            // return the input detail to its previous state.
            detail->d->m_access = oldAccess;
            return true;
        }
    }
    // this is a new detail!  add it to the contact.
    d->m_details.append(*detail);
    return true;
}

/*!
 * Removes the \a detail from the contact.
 *
 * The detail in the contact which has the same key as that of the given \a detail
 * will be removed if it exists.  Only the key is used for comparison - that is, the
 * information in the detail may be different.
 *
 * Any action preferences for the matching detail is also removed.
 *
 * Be aware that if a contact is retrieved (or reloaded) from the backend, the
 * keys of any details it contains may have been changed by the backend, or other
 * threads may have modified the contact details in the backend.  Therefore,
 * clients should reload the detail that they wish to remove from a contact after retrieving
 * the contact, in order to ensure that the remove operation is successful.
 *
 * If the detail's access constraint includes \c QContactDetail::Irremovable,
 * this function will return false if \a enforce is set to \c QContact::EnforceAccessConstraints.
 * Otherwise, the detail will be removed, but any attempt to save the contact
 * in the manager may fail due to the backend enforcing the constraints.
 *
 * Returns true if the detail was removed successfully, false if an error occurred.
 *
 * Note that the caller retains ownership of the detail.
 */
bool QContact::removeDetail(QContactDetail* detail, AccessConstraintsEnforcement enforce)
{
    if (!detail)
        return false;

    // find the detail stored in the contact which has the same key as the detail argument
    int removeIndex = -1;
    for (int i = 0; i < d.constData()->m_details.size(); i++) {
        if (d.constData()->m_details.at(i).key() == detail->key()) {
            removeIndex = i;
            break;
        }
    }

    // make sure the detail exists (in some form) in the contact.
    if (removeIndex < 0)
        return false;

    if (enforce == QContact::EnforceAccessConstraints
            && detail->accessConstraints() & QContactDetail::Irremovable)
        return false;

    if (!d.constData()->m_details.contains(*detail))
        return false;

    // remove any preferences we may have stored for the detail.
    QStringList keys = d.constData()->m_preferences.keys();
    for (int i = 0; i < keys.size(); i++) {
        QString prefKey = keys.at(i);
        if (d.constData()->m_preferences.value(prefKey) == detail->d->m_detailId) {
            d->m_preferences.remove(prefKey);
        }
    }

    // then remove the detail.
    d->m_details.removeAt(removeIndex);
    return true;
}

/*! Returns true if this contact is equal to the \a other contact, false if either the id or stored details are not the same
*/
bool QContact::operator==(const QContact& other) const
{
    // Id must be the same
    if (other.d.constData()->m_id != d.constData()->m_id)
        return false;
    // There must be same amount of details
    if (other.d.constData()->m_details.size() != d.constData()->m_details.size())
        return false;
    // All details must match
    foreach (QContactDetail detail, other.d.constData()->m_details) {
        if (!d.constData()->m_details.contains(detail))
            return false;
    }
    // All equal
    return true;
}

/*!
    \relates QContact
    Returns the hash value for \a key.
*/
uint qHash(const QContact &key)
{
    uint hash = qHash(key.id());
    foreach (const QContactDetail& detail, key.details()) {
        hash += qHash(detail);
    }
    return hash;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QContact& contact)
{
    dbg.nospace() << "QContact(" << contact.id() << ")";
    foreach (const QContactDetail& detail, contact.details()) {
        dbg.space() << '\n' << detail;
    }
    return dbg.maybeSpace();
}
#endif

#ifndef QT_NO_DATASTREAM
/*!
 * Writes \a contact to the stream \a out.
 */
QDataStream& operator<<(QDataStream& out, const QContact& contact)
{
    quint8 formatVersion = 1; // Version of QDataStream format for QContact
    return out << formatVersion << contact.id() << contact.details() << contact.d->m_preferences;
}

/*!
 * Reads a contact from stream \a in into \a contact.
 */
QDataStream& operator>>(QDataStream& in, QContact& contact)
{
    contact = QContact();
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1) {
        QContactId id;
        QList<QContactDetail> details;
        QMap<QString, int> preferences;
        in >> id >> contact.d->m_details >> contact.d->m_preferences;
        contact.setId(id);
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}

#endif

/*!
    Returns a list of relationships of the given \a relationshipType in which this contact is a participant.

    If \a relationshipType is empty, all relationships will be returned.

    \note This function only examines the relationships that were present when this contact
    was retrieved from a manager.  You can also query the manager directly, if you require
    the most up to date information.

    \snippet qtcontactsdocsample/qtcontactsdocsample.cpp 5

    \sa QContactRelationshipFetchRequest, QContactManager::relationships()
 */
QList<QContactRelationship> QContact::relationships(const QString& relationshipType) const
{
    // if empty, then they want all relationships
    if (relationshipType.isEmpty())
        return d.constData()->m_relationshipsCache;

    // otherwise, filter on type.
    QList<QContactRelationship> retn;
    for (int i = 0; i < d.constData()->m_relationshipsCache.size(); i++) {
        QContactRelationship curr = d.constData()->m_relationshipsCache.at(i);
        if (curr.relationshipType() == relationshipType) {
            retn.append(curr);
        }
    }

    return retn;
}

/*!
    Returns a list of the ids of contacts which have a relationship of the given \a relationshipType with this contact.
    The \a role parameter describes the role that the related contacts have in the relationship.

    If \a relationshipType is empty, relationships of all types will be considered.

    \note This function only examines the relationships that were present when this contact
    was retrieved from a manager.  You can also query the manager directly, if you require
    the most up to date information.

    \snippet qtcontactsdocsample/qtcontactsdocsample.cpp 6

    \sa QContactRelationshipFetchRequest, QContactManager::relationships()
 */
QList<QContactId> QContact::relatedContacts(const QString& relationshipType, QContactRelationship::Role role) const
{
    QList<QContactId> retn;
    for (int i = 0; i < d.constData()->m_relationshipsCache.size(); i++) {
        QContactRelationship curr = d.constData()->m_relationshipsCache.at(i);
        if (relationshipType.isEmpty() || curr.relationshipType() == relationshipType) {
            // check that the other contacts fill the given role
            if (role == QContactRelationship::First) {
                if (curr.first() != d.constData()->m_id) {
                    if (!retn.contains(curr.first())) {
                        retn.append(curr.first());
                    }
                }
            } else if (role == QContactRelationship::Second) {
                if (curr.first() == d.constData()->m_id) {
                    if (!retn.contains(curr.second())) {
                        retn.append(curr.second());
                    }
                }
            } else { // role == Either.
                if (curr.first() == d.constData()->m_id) {
                    if (!retn.contains(curr.second())) {
                        retn.append(curr.second());
                    }
                } else {
                    if (!retn.contains(curr.first())) {
                        retn.append(curr.first());
                    }
                }
            }
        }
    }

    return retn;
}

QContactCollectionId QContact::collectionId() const
{
    return d->m_collectionId;
}

void QContact::setCollectionId(const QContactCollectionId &collectionId)
{
    d->m_collectionId = collectionId;
}

/*!
 * Return a list of descriptors for the actions available to be performed on this contact.
 *
 * The actions considered can be restricted by the optional parameters
 * The actions can be restricted to those provided by a specific service with the \a serviceName parameter.
 * If \a serviceName is empty, actions provided by any service will be returned if the
 * contact meets the required criteria (contains details of the correct type, etc).
 *
 * Each action that matches the above criteria will be tested to see if this contact is supported
 * by the action, and a list of the action descriptors that are supported will be returned.
 */
QList<QContactActionDescriptor> QContact::availableActions(const QString& serviceName) const
{
    QList<QContactActionDescriptor> ret;
    QList<QContactActionDescriptor> allds = QContactActionManager::instance()->availableActions(*this);
    foreach (const QContactActionDescriptor& d, allds) {
        if (serviceName.isEmpty() || d.serviceName() == serviceName) {
            ret.append(d);
        }
    }

    return ret;
}

/*!
 * Set a particular detail (\a preferredDetail) as the preferred detail for any actions with the given \a actionName.
 *
 * The \a preferredDetail object must exist in this object, and the \a actionName cannot be empty.
 *
 * Returns true if the preference could be recorded, and false otherwise.
 *
 * \sa preferredDetail()
 */
bool QContact::setPreferredDetail(const QString& actionName, const QContactDetail& preferredDetail)
{
    // if the given action name is empty, bad argument.
    if (actionName.isEmpty())
        return false;

    // check to see whether the the given preferredDetail is saved in this contact
    if (!d.constData()->m_details.contains(preferredDetail))
        return false;

    // otherwise, save the preference.
    d->m_preferences.insert(actionName, preferredDetail.d->m_detailId);
    return true;
}

/*!
 * Returns true if the given \a detail is a preferred detail for the given \a actionName,
 * or for any action if the \a actionName is empty.
 *
 * \sa preferredDetail()
 */
bool QContact::isPreferredDetail(const QString& actionName, const QContactDetail& detail) const
{
    if (!d.constData()->m_details.contains(detail))
        return false;

    if (actionName.isEmpty())
         return d.constData()->m_preferences.values().contains(detail.d->m_detailId);

    QMap<QString, int>::const_iterator it = d.constData()->m_preferences.find(actionName);
    if (it != d.constData()->m_preferences.end() && it.value() == detail.d->m_detailId)
        return true;

    return false;
}

/*!
 * Returns the preferred detail for a given \a actionName.
 *
 * If the \a actionName is empty, or there is no preference recorded for
 * the supplied \a actionName, returns an empty QContactDetail.
 *
 * \sa preferredDetails()
 */
QContactDetail QContact::preferredDetail(const QString& actionName) const
{
    // if the given action name is empty, bad argument.
    if (actionName.isEmpty())
        return QContactDetail();

    if (!d.constData()->m_preferences.contains(actionName))
        return QContactDetail();

    QContactDetail retn;
    int detId = d.constData()->m_preferences.value(actionName);
    for (int i = 0; i < d.constData()->m_details.size(); i++) {
        QContactDetail det = d.constData()->m_details.at(i);
        if (det.d->m_detailId == detId) {
            // found it.
            retn = det;
            break;
        }
    }

    return retn;
}

/*!
 * Returns the recorded detail preferences for action names.
 *
 * Each entry in the map has the action name as the key, and the corresponding
 * preferred detail as the value.
 */
QMap<QString, QContactDetail> QContact::preferredDetails() const
{
    QMap<QString, QContactDetail> ret;
    QMap<QString, int>::const_iterator it = d.constData()->m_preferences.constBegin();
    while (it != d.constData()->m_preferences.constEnd()) {
        ret.insert(it.key(), preferredDetail(it.key()));
        ++it;
    }

    return ret;
}


/* Helper functions for QContactData */
void QContactData::removeOnly(QContactDetail::DetailType type)
{
    QList<QContactDetail>::iterator dit = m_details.begin();
    while (dit != m_details.end()) {
        // XXX this doesn't check type
        if (dit->type() == type)
            dit = m_details.erase(dit);
        else
            ++dit;
    }
}

void QContactData::removeOnly(const QSet<QContactDetail::DetailType>& types)
{
    QList<QContactDetail>::iterator dit = m_details.begin();
    while (dit != m_details.end()) {
        // XXX this doesn't check type
        if (types.contains(dit->type()))
            dit = m_details.erase(dit);
        else
            ++dit;
    }
}

QT_END_NAMESPACE_CONTACTS
