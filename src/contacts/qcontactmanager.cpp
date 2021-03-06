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

#include "qcontactmanager.h"

#include <QtCore/qmetaobject.h>

#include "qcontact_p.h"
#include "qcontactfilter.h"
#include "qcontactmanager_p.h"

QT_BEGIN_NAMESPACE_CONTACTS
/*!
  \class QContactManager
  \brief The QContactManager class provides an interface which allows clients with access to contact information stored in a particular backend.
  \inmodule QtContacts

  \ingroup contacts-main

  This class provides an abstraction of a datastore or aggregation of datastores which contains contact information.
  It provides methods to retrieve and manipulate contact information and contact relationship information.
  It also provides metadata and error information reporting.

  The functions provided by QContactManager are purely synchronous; to access the same functionality in an
  asynchronous manner, clients should use the use-case-specific classes derived from QContactAbstractRequest.

  Some functionality provided by QContactManager directly is not accessible using the asynchronous API. See
  the \l{Qt Contacts Synchronous API}{synchronous} and \l{Qt Contacts Asynchronous API}{asynchronous} API
  information in the \l{Qt Contacts C++ API} documentation.
 */

/*!
  \fn QContactManager::dataChanged()
  This signal is emitted by the manager if its internal state changes, and it is unable to determine the changes
  which occurred, or if the manager considers the changes to be radical enough to require clients to reload all data.
  If this signal is emitted, no other signals will be emitted for the associated changes.
 */

/*!
  \fn QContactManager::contactsAdded(const QList<QContactId>& contactIds)
  This signal is emitted at some point once the contacts identified by \a contactIds have been added to a datastore managed by this manager.
  This signal must not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QContactManager::contactsChanged(const QList<QContactId>& contactIds, const QList<QContactDetail::DetailType> &typesChanged)
  This signal is emitted at some point once the contacts identified by \a contactIds have been modified in a datastore managed by this manager.
  The set of contact detail types modified in the reported changes is a subset of those listed in \a typesChanged,
  unless \a typesChanged is empty, in which case no limitation on the reported changes may be assumed.
  This signal must not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QContactManager::contactsRemoved(const QList<QContactId>& contactIds)
  This signal is emitted at some point once the contacts identified by \a contactIds have been removed from a datastore managed by this manager.
  This signal must not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QContactManager::relationshipsAdded(const QList<QContactId>& affectedContactIds)
  This signal is emitted at some point after relationships have been added to the manager which involve the contacts identified by \a affectedContactIds.
  This signal must not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QContactManager::relationshipsRemoved(const QList<QContactId>& affectedContactIds)
  This signal is emitted at some point after relationships have eben removed from the manager which involve the contacts identified by \a affectedContactIds.
  This signal must not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QContactManager::selfContactIdChanged(const QContactId& oldId, const QContactId& newId)
  This signal is emitted at some point after the id of the self-contact is changed from \a oldId to \a newId in the manager.
  If the \a newId is the invalid, then the self contact was deleted or no self contact exists.
  This signal must not be emitted if the dataChanged() signal was previously emitted for this change.
 */



#define makestr(x) (#x)
#define makename(x) makestr(x)

/*!
    Returns a list of available manager ids that can be used when constructing
    a QContactManager.  If an empty id is specified to the constructor, the
    first value in this list will be used instead.

    The QTCONTACTS_MANAGER_OVERRIDE environment variable may be set to
    override the default engine.
  */
QStringList QContactManager::availableManagers()
{
    QStringList ret;
    QContactManagerData::loadFactoriesMetadata();
    ret = QContactManagerData::m_managerNames;

    // Unless overridden, the default must be 'invalid' so that malicious plugins
    // do not become the default selection without explicit selection
    ret.prepend(QStringLiteral("invalid"));

    // bump memory to the end of the list
    if (ret.removeAll(QStringLiteral("memory")))
        ret.append(QStringLiteral("memory"));

#if defined(Q_CONTACTS_DEFAULT_ENGINE)
    // now swizzle the default engine to pole position
    const QString defaultManagerName = QString::fromLatin1(makename(Q_CONTACTS_DEFAULT_ENGINE));
    if (ret.removeAll(defaultManagerName)) {
        ret.prepend(defaultManagerName);
    }
#endif

    // and prefer the override engine if specified in the environment
    const QString overrideManagerName = QString::fromLatin1(qgetenv("QTCONTACTS_MANAGER_OVERRIDE"));
    if (!overrideManagerName.isEmpty() && ret.contains(overrideManagerName)) {
        ret.removeAll(overrideManagerName);
        ret.prepend(overrideManagerName);
    }

    return ret;
}

/*!
    Splits the given \a uri into the manager name and parameters that it describes,
    and places the information into the memory addressed by \a managerName and \a params respectively.
    Returns true if \a uri could be split successfully, otherwise returns false.
*/
bool QContactManager::parseUri(const QString &uri, QString *managerName, QMap<QString, QString> *params)
{
    return QContactManagerData::parseUri(uri, managerName, params, false);
}

/*!
    Returns a URI that describes a manager name and parameters with which to instantiate
    a manager object, from the given \a managerName and \a params.
*/
QString QContactManager::buildUri(const QString &managerName, const QMap<QString, QString> &params)
{
    return QContactManagerData::buildUri(managerName, params);
}

/*!
   Returns a URI that completely describes a manager implementation, datastore, and the parameters
   with which to instantiate the manager, from the given \a managerName, \a params and an optional
   \a implementationVersion.  This function is generally useful only if you intend to construct a
   manager with the \l fromUri() function, or wish to construct a contact id
   manually (for synchronization or other purposes).  Most clients will not need to use this function.
*/
QString QContactManager::buildUri(const QString &managerName, const QMap<QString, QString> &params, int implementationVersion)
{
    QMap<QString, QString> params_(params);
    if (implementationVersion != -1) {
        params_.insert(QString::fromLatin1(QTCONTACTS_IMPLEMENTATION_VERSION_NAME),
                       QString::number(implementationVersion));
    }

    return QContactManager::buildUri(managerName, params_);
}

/*!
  Constructs a QContactManager whose implementation version, manager name and specific parameters
  are specified in the given \a managerUri, and whose parent object is \a parent.
 */
QContactManager* QContactManager::fromUri(const QString &managerUri, QObject *parent)
{
    if (managerUri.isEmpty()) {
        return new QContactManager(QString(), QMap<QString, QString>(), parent);
    } else {
        QString id;
        QMap<QString, QString> parameters;
        if (parseUri(managerUri, &id, &parameters)) {
            return new QContactManager(id, parameters, parent);
        } else {
            // invalid
            return new QContactManager(QStringLiteral("invalid"), QMap<QString, QString>(), parent);
        }
    }
}

/*!
  Constructs a QContactManager whose parent QObject is \a parent.
  The default implementation for the platform will be created.
 */
QContactManager::QContactManager(QObject *parent)
    : QObject(parent),
    d(new QContactManagerData)
{
    createEngine(QString(), QMap<QString, QString>());
}

/*!
  Constructs a QContactManager whose implementation is identified by \a managerName with the given \a parameters.

  The \a parent QObject will be used as the parent of this QContactManager.

  If an empty \a managerName is specified, the default implementation for the platform will
  be used.
 */
QContactManager::QContactManager(const QString &managerName, const QMap<QString, QString> &parameters, QObject *parent)
    : QObject(parent),
    d(new QContactManagerData)
{
    createEngine(managerName, parameters);
}

void QContactManager::createEngine(const QString &managerName, const QMap<QString, QString> &parameters)
{
    d->createEngine(managerName, parameters);

    QContactManagerData::m_aliveEngines.insert(this);
}

/*!
  Constructs a QContactManager whose backend has the name \a managerName and version \a implementationVersion, where the manager
  is constructed with the provided \a parameters.

  The \a parent QObject will be used as the parent of this QContactManager.

  If an empty \a managerName is specified, the default implementation for the platform will be instantiated.
  If the specified implementation version is not available, the manager with the name \a managerName with the default implementation version is instantiated.
 */
QContactManager::QContactManager(const QString &managerName, int implementationVersion, const QMap<QString, QString> &parameters, QObject *parent)
    : QObject(parent),
    d(new QContactManagerData)
{
    QMap<QString, QString> params = parameters;
    params[QString(QStringLiteral(QTCONTACTS_IMPLEMENTATION_VERSION_NAME))] = QString::number(implementationVersion);
    createEngine(managerName, params);
}

/*! Frees the memory used by the QContactManager */
QContactManager::~QContactManager()
{
    QContactManagerData::m_aliveEngines.remove(this);
    delete d;
}


/*!
   \fn QContactManager::ParameterSignalSources()
   The string constant for the parameter key which holds the value for signal sources.
   If a manager supports suppressing change signals depending on the value given for
   this construction parameter, clients can request that signals be suppressed if the
   changes which might cause a signal to be emitted do not match particular criteria.

   If the parameter (or value given for the parameter) is not supported by the manager,
   the manager may still be constructed, however the parameter will not be reported
   to the client if the client calls managerParameters() subsequent to manager construction.

   The default (assumed) value for this parameter, if this parameter is not given,
   is that the client wants to be notified of all changes to the data, regardless of
   the source of the change.
 */

/*!
   \fn QContactManager::ParameterSignalDefinitions()
   The string constant for the parameter key which holds the names of detail definitions.
   If a manager supports suppressing change signals depending on the value given for
   this construction parameter, clients can request that signals be suppressed if the
   changes which might otherwise cause a signal to be emitted, involve details whose
   definition name is not contained in the given list.

   That is, if a detail in a contact is changed, but that detail's definition name is
   not listed in the value for this parameter, the manager will not emit a change signal
   for that change.

   If this parameter is not specified at construction time, changes to any detail of a contact
   will cause a change signal to be emitted.

   The value of this parameter should be a comma (,) separated list of definition names.  Any
   commas which might be part of a definition name must be escaped with a single backslash
   (\) character prior to concatenation.  Any backslash character which might be part of a
   definition name must also be escaped with a backslash.

   If the parameter (or value given for the parameter) is not supported by the manager,
   the manager may still be constructed, however the parameter will not be reported
   to the client if the client calls managerParameters() subsequent to manager construction.
 */

/*!
   \fn QContactManager::ParameterValueOnlyOtherManagers()
   This value tells the manager to only emit signals for changes which
   are made in other manager instances.  That is, the client wishes to receive
   change signals when another client (or background service) changes
   the data as it is stored in the backend, but does not wish to be
   notified of changes (or side effects) which it has caused itself.
 */

/*!
   \fn QContactManager::ParameterValueOnlyOtherProcesses()
   This value tells the manager to only emit signals for changes which
   are made in other processes.  That is, the client wishes to receive
   change signals when a client (or background service) in another process changes
   the data as it is stored in the backend, but does not wish to be
   notified of changes (or side effects) which were caused in the current client's
   process, even if those changes were made in a different manager instance to this
   one.
 */

/*!
  \enum QContactManager::Error

  This enum specifies an error that occurred during the most recent operation:

  \value NoError The most recent operation was successful
  \value DoesNotExistError The most recent operation failed because the requested contact does not exist
  \value AlreadyExistsError The most recent operation failed because the specified contact  already exists
  \value InvalidDetailError The most recent operation failed because the specified contact contains details which do not conform to their definition
  \value InvalidRelationshipError The most recent operation failed because the specified relationship is circular or references an invalid local contact
  \value InvalidContactTypeError The most recent operation failed because the contact type specified was not valid for the operation
  \value LockedError The most recent operation failed because the datastore specified is currently locked
  \value DetailAccessError The most recent operation failed because a detail was modified or removed and its access method does not allow that
  \value PermissionsError The most recent operation failed because the caller does not have permission to perform the operation
  \value OutOfMemoryError The most recent operation failed due to running out of memory
  \value VersionMismatchError The most recent operation failed because the backend of the manager is not of the required version
  \value LimitReachedError The most recent operation failed because the limit for that type of object has been reached
  \value NotSupportedError The most recent operation failed because the requested operation is not supported in the specified store
  \value BadArgumentError The most recent operation failed because one or more of the parameters to the operation were invalid
  \value TimeoutError The most recent operation failed because it took longer than expected.  It may be possible to try again.
  \value UnspecifiedError The most recent operation failed for an undocumented reason
  \value MissingPlatformRequirementsError The most recent operation failed due to all storage locations are unavailable.
 */

/*!
  Return the error code of the most recent operation.
  For batch operations, if the error code is not equal to
  \c QContactManager::NoError, detailed per-input errors
  may be retrieved by calling \l errorMap().
  \sa errorMap()
 */
QContactManager::Error QContactManager::error() const
{
    return d->m_lastError;
}

/*!
  Returns per-input error codes for the most recent operation.
  This function only returns meaningful information if the most
  recent operation was a batch operation.
  Each key in the map is the index of the element in the input list
  for which the error (whose error code is stored in the value for
  that key in the map) occurred during the batch operation.
  \sa error(), contacts(), saveContacts(), removeContacts(), saveRelationships(), removeRelationships()
 */
QMap<int, QContactManager::Error> QContactManager::errorMap() const
{
    return d->m_lastErrorMap;
}

/*!
  Return the list of contact ids, sorted according to the given list of \a sortOrders
 */
QList<QContactId> QContactManager::contactIds(const QList<QContactSortOrder> &sortOrders) const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->contactIds(QContactFilter(), sortOrders, &h.error);
}

/*!
  Returns a list of contact ids that match the given \a filter, sorted according to the given list of \a sortOrders.
  Depending on the backend, this filtering operation may involve retrieving all the contacts.
 */
QList<QContactId> QContactManager::contactIds(const QContactFilter &filter, const QList<QContactSortOrder> &sortOrders) const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->contactIds(filter, sortOrders, &h.error);
}

/*!
  Returns the list of contacts stored in the manager sorted according to the given list of \a sortOrders.

  The \a fetchHint parameter describes the optimization hints that a manager may take.
  If the \a fetchHint is the default constructed hint, all existing details, relationships and
  action preferences in the matching contact will be returned.  If a client makes changes to an
  contact which has been retrieved with a fetch hint, they should save it back using a partial save,
  masked by the same set of detail names in order to avoid information loss.

  \sa QContactFetchHint
 */
QList<QContact> QContactManager::contacts(const QList<QContactSortOrder> &sortOrders, const QContactFetchHint &fetchHint) const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->contacts(QContactFilter(), sortOrders, fetchHint, &h.error);
}

/*!
  Returns a list of contacts that match the given \a filter, sorted according to the given list of \a sortOrders.

  Depending on the manager implementation, this filtering operation might be slow and involve retrieving all the
  contacts and testing them against the supplied filter - see the \l isFilterSupported() function.

  The \a fetchHint parameter describes the optimization hints that a manager may take.
  If the \a fetchHint is the default constructed hint, all existing details, relationships and
  action preferences in the matching contact will be returned.  If a client makes changes to an
  contact which has been retrieved with a fetch hint, they should save it back using a partial save,
  masked by the same set of detail names in order to avoid information loss.

  \sa QContactFetchHint
 */
QList<QContact> QContactManager::contacts(const QContactFilter &filter, const QList<QContactSortOrder>& sortOrders, const QContactFetchHint &fetchHint) const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->contacts(filter, sortOrders, fetchHint, &h.error);
}

/*!
  Returns the contact in the database identified by \a contactId.

  If the contact does not exist, an empty, default constructed QContact will be returned,
  and the error returned by \l error() will be \c QContactManager::DoesNotExistError.

  The \a fetchHint parameter describes the optimization hints that a manager may take.
  If the \a fetchHint is the default constructed hint, all existing details, relationships and
  action preferences in the matching contact will be returned.  If a client makes changes to an
  contact which has been retrieved with a fetch hint, they should save it back using a partial save,
  masked by the same set of detail names in order to avoid information loss.


  \sa QContactFetchHint
 */
QContact QContactManager::contact(const QContactId &contactId, const QContactFetchHint &fetchHint) const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->contact(contactId, fetchHint, &h.error);
}

/*!
  Returns a list of contacts given a list of ids (\a contactIds).

  Returns the list of contacts with the ids given by \a contactIds.  There is a one-to-one
  correspondence between the returned contacts and the supplied \a contactIds.

  If there is an invalid id in \a contactIds, then an empty QContact will take its place in the
  returned list.  The \a errorMap parameter can be supplied to store per-input errors in.
  In all cases, calling \l errorMap() will return the per-input errors for the latest batch function.

  The \a fetchHint parameter describes the optimization hints that a manager may take.
  If the \a fetchHint is the default constructed hint, all existing details, relationships and
  action preferences in the matching contact will be returned.  If a client makes changes to an
  contact which has been retrieved with a fetch hint, they should save it back using a partial save,
  masked by the same set of detail names in order to avoid information loss.

  \sa QContactFetchHint
 */
QList<QContact> QContactManager::contacts(const QList<QContactId> &contactIds, const QContactFetchHint &fetchHint, QMap<int, QContactManager::Error> *errorMap) const
{
    QContactManagerSyncOpErrorHolder h(this, errorMap);

    return d->m_engine->contacts(contactIds, fetchHint, &h.errorMap, &h.error);
}

/*!
  Adds the given \a contact to the database if \a contact has
  the (default constructed) null id.

  If the manager URI of the id of the \a contact is neither empty nor equal to the URI of
  this manager, or id of the \a contact is not null but does not exist in the
  manager, the operation will fail and calling error() will return
  \c QContactManager::DoesNotExistError.

  Alternatively, the function will update the existing contact in the database if \a contact
  has a id which is not null and currently exists in the database.

  If the \a contact contains one or more details whose types are not
  supported by the used engine, the operation will fail and calling
  error() will return \c QContactManager::UnsupportedError.

  If the \a contact has had its relationships reordered, the manager
  will check to make sure that every relationship that the contact is currently
  involved in is included in the reordered list, and that no relationships which
  either do not involve the contact, or have not been saved in the manager are
  included in the list.  If these conditions are not met, the function will
  return \c false and calling error() will return
  \c QContactManager::InvalidRelationshipError.

  Returns false on failure, or true on
  success.  On successful save of an contact with a null id, its
  id will be set to a new, non-null id.

  The manager is not required to fetch updated details of the contact on save,
  and as such, clients should fetch a contact if they want the most up-to-date information
  by calling \l QContactManager::contact().

  \sa managerUri()
 */
bool QContactManager::saveContact(QContact *contact)
{
    QContactManagerSyncOpErrorHolder h(this);

    if (contact) {
        return d->m_engine->saveContact(contact, &h.error);
    } else {
        h.error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Remove the contact identified by \a contactId from the database,
  and also removes any relationships in which the contact was involved.
  Returns true if the contact was removed successfully, otherwise
  returns false.
 */
bool QContactManager::removeContact(const QContactId &contactId)
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->removeContact(contactId, &h.error);
}

/*!
  Adds the list of contacts given by \a contacts list to the database.
  Returns true if the contacts were saved successfully, otherwise false.

  The \a errorMap parameter can be supplied to store per-input errors in.
  In all cases, calling \l errorMap() will return the per-input errors for the latest batch function.
  The \l QContactManager::error() function will only return \c QContactManager::NoError
  if all contacts were saved successfully.

  For each newly saved contact that was successful, the id of the contact
  in the \a contacts list will be updated with the new value.

  \sa QContactManager::saveContact()
 */
bool QContactManager::saveContacts(QList<QContact> *contacts, QMap<int, QContactManager::Error> *errorMap)
{
    QContactManagerSyncOpErrorHolder h(this, errorMap);

    if (contacts) {
        return d->m_engine->saveContacts(contacts, &h.errorMap, &h.error);
    } else {
        h.error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Adds the list of contacts given by \a contacts list to the database.
  Returns true if the contacts were saved successfully, otherwise false.

  This function accepts a \a typeMask, which specifies which details of
  the contacts should be updated.  Details with types not included in
  the typeMask will not be updated or added.

  The \a errorMap parameter can be supplied to store per-input errors in.
  In all cases, calling \l errorMap() will return the per-input errors for the latest batch function.
  The \l QContactManager::error() function will only return \c QContactManager::NoError
  if all contacts were saved successfully.

  For each newly saved contact that was successful, the id of the contact
  in the \a contacts list will be updated with the new value.

  \sa QContactManager::saveContact()
 */
bool QContactManager::saveContacts(QList<QContact> *contacts, const QList<QContactDetail::DetailType> &typeMask, QMap<int, QContactManager::Error> *errorMap)
{
    QContactManagerSyncOpErrorHolder h(this, errorMap);

    if (contacts) {
        return d->m_engine->saveContacts(contacts, typeMask, &h.errorMap, &h.error);
    } else {
        h.error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Remove every contact whose id is contained in the list of contacts ids
  \a contactIds.  Returns true if all contacts were removed successfully,
  otherwise false.

  Any contact that was removed successfully will have the relationships
  in which it was involved removed also.

  The \a errorMap parameter can be supplied to store per-input errors in.
  In all cases, calling \l errorMap() will return the per-input errors for the latest batch function.
  The \l QContactManager::error() function will
  only return \c QContactManager::NoError if all contacts were removed
  successfully.

  If the given list of contact ids \a contactIds is empty, the function will return false
  and calling error() will return \c QContactManager::BadArgumentError.  If the list is non-empty
  and contains ids which do not identify a valid contact in the manager, the function will
  remove any contacts which are identified by ids in the \a contactIds list, insert
  \c QContactManager::DoesNotExist entries into the \a errorMap for the indices of invalid ids
  in the \a contactIds list, return false, and set the overall operation error to
  \c QContactManager::DoesNotExistError.

  \sa QContactManager::removeContact()
 */
bool QContactManager::removeContacts(const QList<QContactId> &contactIds, QMap<int, QContactManager::Error> *errorMap)
{
    QContactManagerSyncOpErrorHolder h(this, errorMap);

    if (!contactIds.isEmpty()) {
        return d->m_engine->removeContacts(contactIds, &h.errorMap, &h.error);
    } else {
        h.error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Sets the id of the "self" contact to the given \a contactId.
  Returns true if the "self" contact id was set successfully.
  If the given \a contactId does not identify a contact
  stored in this manager, the error will be set to
  \c QContactManager::DoesNotExistError and the function will
  return false; if the backend does not support the
  concept of a "self" contact then the error will be set to
  \c QContactManager::NotSupportedError and the function will
  return false.
 */
bool QContactManager::setSelfContactId(const QContactId &contactId)
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->setSelfContactId(contactId, &h.error);
}

/*!
  Returns the id of the "self" contact which has previously been set.
  If no "self" contact has been set, or if the self contact was removed
  from the manager after being set, or if the backend does not support
  the concept of a "self" contact, an invalid id will be returned
  and the error will be set to \c QContactManager::DoesNotExistError.
 */
QContactId QContactManager::selfContactId() const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->selfContactId(&h.error);
}

/*!
  Returns a list of relationships in which the contact identified by \a participantId participates in the given \a role.
  If \a participantId is default-constructed, \a role is ignored and all relationships are returned.
 */
QList<QContactRelationship> QContactManager::relationships(const QContactId &participantId, QContactRelationship::Role role) const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->relationships(QString(), participantId, role, &h.error);
}

/*!
  Returns a list of relationships of the given \a relationshipType in which the contact identified by \a participantId participates in the given \a role.
  If \a participantId is default-constructed, \a role is ignored and all relationships of the given \a relationshipType are returned.
  If \a relationshipType is empty, relationships of any type are returned.
 */
QList<QContactRelationship> QContactManager::relationships(const QString &relationshipType, const QContactId &participantId, QContactRelationship::Role role) const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->relationships(relationshipType, participantId, role, &h.error);
}

/*!
  Saves the given \a relationship in the database.  If the relationship already exists in the database, this function will
  return \c false and the error will be set to \c QContactManager::AlreadyExistsError.
  If the relationship is saved successfully, this function will return \c true and error will be set
  to \c QContactManager::NoError.  Note that relationships cannot be updated directly using this function; in order
  to update a relationship, you must remove the old relationship, make the required modifications, and then save it.

  The given relationship is invalid if it is circular (the first contact is the second contact), or
  if it references a non-existent local contact (either the first or second contact).  If the given \a relationship is invalid,
  the function will return \c false and the error will be set to \c QContactManager::InvalidRelationshipError.
  If the given \a relationship could not be saved in the database (due to backend limitations)
  the function will return \c false and error will be set to \c QContactManager::NotSupportedError.
 */
bool QContactManager::saveRelationship(QContactRelationship *relationship)
{
    QContactManagerSyncOpErrorHolder h(this);
    if (relationship) {
        return d->m_engine->saveRelationship(relationship, &h.error);
    } else {
        h.error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Saves the given \a relationships in the database and returns true if the operation was successful.
  The \a errorMap parameter can be supplied to store per-input errors in.
  In all cases, calling \l errorMap() will return the per-input errors for the latest batch function.
 */
bool QContactManager::saveRelationships(QList<QContactRelationship> *relationships, QMap<int, QContactManager::Error> *errorMap)
{
    QContactManagerSyncOpErrorHolder h(this, errorMap);

    if (relationships) {
        return d->m_engine->saveRelationships(relationships, &h.errorMap, &h.error);
    } else {
        h.error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Removes the given \a relationship from the manager.  If the relationship exists in the manager, the relationship
  will be removed, the error will be set to \c QContactManager::NoError and this function will return true.  If no such
  relationship exists in the manager, the error will be set to \c QContactManager::DoesNotExistError and this function
  will return false.
 */
bool QContactManager::removeRelationship(const QContactRelationship &relationship)
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->removeRelationship(relationship, &h.error);
}


/*!
  Removes the given \a relationships from the database and returns true if the operation was successful.
  The \a errorMap parameter can be supplied to store per-input errors in.
  In all cases, calling \l errorMap() will return the per-input errors for the latest batch function.
 */
bool QContactManager::removeRelationships(const QList<QContactRelationship> &relationships, QMap<int, QContactManager::Error> *errorMap)
{
    QContactManagerSyncOpErrorHolder h(this, errorMap);
    return d->m_engine->removeRelationships(relationships, &h.errorMap, &h.error);
}

/*!
  Returns the list of data types supported by the manager
 */
QList<QVariant::Type> QContactManager::supportedDataTypes() const
{
    return d->m_engine->supportedDataTypes();
}

/*!
  Returns true if the given \a filter is supported natively by the
  manager, and false if the filter behaviour would be emulated.

  Note: In some cases, the behaviour of an unsupported filter
  cannot be emulated.  For example, a filter that requests contacts
  that have changed since a given time depends on having that information
  available.  In these cases, the filter will fail.
 */
bool QContactManager::isFilterSupported(const QContactFilter &filter) const
{
    return d->m_engine->isFilterSupported(filter);
}

/*!
  Returns true if the manager supports the relationship type specified in \a relationshipType for
  contacts whose type is the given \a contactType.

  Note that some managers may support the relationship type for a contact in a limited manner
  (for example, only as the first contact in the relationship, or only as the second contact
  in the relationship).  In this case, it will still return true.  It will only return false
  if the relationship is entirely unsupported for the given type of contact.
 */
bool QContactManager::isRelationshipTypeSupported(const QString& relationshipType, QContactType::TypeValues  contactType) const
{
    return d->m_engine->isRelationshipTypeSupported(relationshipType, contactType);
}

/*!
  Returns the list of contact types which are supported by this manager.
  This is a convenience function, equivalent to retrieving the allowable values
  for the \c QContactType::FieldType field of the QContactType detail
  which is valid in this manager.
 */
QList<QContactType::TypeValues> QContactManager::supportedContactTypes() const
{
    return d->m_engine->supportedContactTypes();
}

/*!
  Returns the list of contact detail types which are supported by this manager.
  The returned list can be used by clients to identify incompatibilities between
  contact objects to be saved and the actual subset of detail types supported by
  this manager.
 */
QList<QContactDetail::DetailType> QContactManager::supportedContactDetailTypes() const
{
    return d->m_engine->supportedContactDetailTypes();
}

/*!
    Returns the id of a default collection managed by this manager.
    There is always only one default collection for each backend.
 */
QContactCollectionId QContactManager::defaultCollectionId() const
{
    return d->m_engine->defaultCollectionId();
}

/*!
  Returns the collection identified by the given \a collectionId which is managed by this manager.
 */
QContactCollection QContactManager::collection(const QContactCollectionId& collectionId) const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->collection(collectionId, &h.error);
}

/*!
  Returns a list of all of the collections managed by this manager.
 */
QList<QContactCollection> QContactManager::collections() const
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->collections(&h.error);
}

/*!
    Saves the given \a collection to the backend, and returns true on success or false otherwise.

    Note that certain backends may not allow modification nor adding new collections, in such cases
    the operation will fail and result a QContactManager::PermissionsError error.

    A new collection will be created in the backend store if the collection ID of it is null.
    Otherwise, an existing collection with the same ID will be updated. If the given collection ID
    does not exist in the backend, it will result a QContactManager::DoesNotExistError error.

    Note that upon successful saving, the backend may update the collection, e.g. collection ID for
    newly saved collections.
 */
bool QContactManager::saveCollection(QContactCollection* collection)
{
    QContactManagerSyncOpErrorHolder h(this);
    if (collection) {
        return d->m_engine->saveCollection(collection, &h.error);
    } else {
        h.error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
    Removes the collection identified by the given \a collectionId (and all items in the collection)
    from the manager. Returns true on success, false on failure.

    If the given \a collectionId does not exist, the operation will fail and QContactManager::DoesNotExistError
    will be returned when calling error().

    If the given \a collectionId refers to the default collection, the operation will fail and
    QContactManager::PermissionsError will be returned when calling error().
 */
bool QContactManager::removeCollection(const QContactCollectionId &collectionId)
{
    QContactManagerSyncOpErrorHolder h(this);
    return d->m_engine->removeCollection(collectionId, &h.error);
}

/*!
  Returns the engine backend implementation version number
 */
int QContactManager::managerVersion() const
{
    return d->m_engine->managerVersion();
}

/*! Returns the manager name for this QContactManager */
QString QContactManager::managerName() const
{
    return d->m_engine->managerName();
}

/*! Return the parameters relevant to the creation of this QContactManager */
QMap<QString, QString> QContactManager::managerParameters() const
{
    QMap<QString, QString> params = d->m_engine->managerParameters();

    params.remove(QString::fromLatin1(QTCONTACTS_VERSION_NAME));
    params.remove(QString::fromLatin1(QTCONTACTS_IMPLEMENTATION_VERSION_NAME));
    return params;
}

/*!
  Return the uri describing this QContactManager, consisting of the manager name and any parameters.
 */
QString QContactManager::managerUri() const
{
    return d->m_engine->managerUri();
}

/*!
    \internal

    Returns the signal that corresponds to \a proxySignal in the
    meta-object of the \a sourceObject.
*/
static QMetaMethod proxyToSourceSignal(const QMetaMethod &proxySignal, QObject *sourceObject)
{
    if (!proxySignal.isValid())
        return proxySignal;
    Q_ASSERT(proxySignal.methodType() == QMetaMethod::Signal);
    Q_ASSERT(sourceObject != 0);
    const QMetaObject *sourceMeta = sourceObject->metaObject();
    int sourceIndex = sourceMeta->indexOfSignal(proxySignal.methodSignature());
    Q_ASSERT(sourceIndex != -1);
    return sourceMeta->method(sourceIndex);
}

/*!
    \internal

    When someone connects to this manager, connect the corresponding signal from the engine, if we
    haven't before. If we have, just increment a count.

    This allows lazy evaluation on the engine side (e.g. setting up dbus watchers) and prevents
    unnecessary work.
*/
void QContactManager::connectNotify(const QMetaMethod &signal)
{
    /* For most signals we just connect from the engine to ourselves, since we just proxy, but we should connect only once */
    QMetaMethod sourceSignal = proxyToSourceSignal(signal, d->m_engine);
    connect(d->m_engine, sourceSignal, this, signal, Qt::UniqueConnection);
}

/*!
    \internal

    When someone disconnects, disconnect from the engine too if there are no more users of that signal.
*/
void QContactManager::disconnectNotify(const QMetaMethod &signal)
{
    if (!isSignalConnected(signal)) {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, d->m_engine);
        disconnect(d->m_engine, sourceSignal, this, signal);
    }
}

#include "moc_qcontactmanager.cpp"

QT_END_NAMESPACE_CONTACTS
