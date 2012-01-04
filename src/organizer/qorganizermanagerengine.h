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


#ifndef QORGANIZERMANAGERENGINE_H
#define QORGANIZERMANAGERENGINE_H

#include <QSharedData>
#include <QMap>
#include <QSet>
#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QObject>

#include "qorganizercollection.h"
#include "qorganizercollectionid.h"
#include "qorganizeritem.h"
#include "qorganizermanager.h"
#include "qorganizerabstractrequest.h"
#include "qorganizeritemfetchhint.h"
#include "qorganizeritemrequests.h"

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerItemFilter;
class QOrganizerItemSortOrder;
class QOrganizerItemEngineId;

/* Forward decls, since these don't actually exist yet */
class QOrganizerItemOccurrenceFetchRequest;
class QOrganizerItemIdFetchRequest;
class QOrganizerItemFetchRequest;
class QOrganizerItemRemoveRequest;
class QOrganizerItemSaveRequest;
class QOrganizerItemFetchForExportRequest;

class Q_ORGANIZER_EXPORT QOrganizerManagerEngine : public QObject
{
    Q_OBJECT

public:
    QOrganizerManagerEngine() {}

    /* URI reporting */
    virtual QString managerName() const;
    virtual QMap<QString, QString> managerParameters() const;  // e.g. "filename=private.db"
    virtual int managerVersion() const;

    /* Default and only implementation of this */
    QString managerUri() const;

    // items
    virtual QList<QOrganizerItem> items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                        QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);

    virtual QList<QOrganizerItem> items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                        const QDateTime &endDateTime, int maxCount,
                                        const QList<QOrganizerItemSortOrder> &sortOrders,
                                        const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    virtual QList<QOrganizerItemId> itemIds(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                            const QDateTime &endDateTime, const QList<QOrganizerItemSortOrder> &sortOrders,
                                            QOrganizerManager::Error *error);

    virtual QList<QOrganizerItem> itemOccurrences(const QOrganizerItem &parentItem, const QDateTime &startDateTime,
                                                  const QDateTime &endDateTime, int maxCount,
                                                  const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    virtual QList<QOrganizerItem> itemsForExport(const QDateTime &startDateTime, const QDateTime &endDateTime,
                                                 const QOrganizerItemFilter &filter,
                                                 const QList<QOrganizerItemSortOrder> &sortOrders,
                                                 const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    virtual bool saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                           QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);

    virtual bool removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap,
                             QOrganizerManager::Error *error);

    // collections
    virtual QOrganizerCollection defaultCollection(QOrganizerManager::Error* error);
    virtual QOrganizerCollection collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error);
    virtual QList<QOrganizerCollection> collections(QOrganizerManager::Error* error);
    virtual bool saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error);
    virtual bool removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error);

    /* Asynchronous Request Support */
    virtual void requestDestroyed(QOrganizerAbstractRequest* req);
    virtual bool startRequest(QOrganizerAbstractRequest* req);
    virtual bool cancelRequest(QOrganizerAbstractRequest* req);
    virtual bool waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs);

    /* Capabilities reporting */
    virtual QList<QOrganizerItemFilter::FilterType> supportedFilters() const;
    virtual QList<QOrganizerItemDetail::DetailType> supportedItemDetails(QOrganizerItemType::ItemType itemType) const;
    virtual QList<QOrganizerItemType::ItemType> supportedItemTypes() const;

Q_SIGNALS:
    void dataChanged();
    void itemsAdded(const QList<QOrganizerItemId>& itemIds);
    void itemsChanged(const QList<QOrganizerItemId>& itemIds);
    void itemsRemoved(const QList<QOrganizerItemId>& itemIds);
    void collectionsAdded(const QList<QOrganizerCollectionId>& collectionIds);
    void collectionsChanged(const QList<QOrganizerCollectionId>& collectionIds);
    void collectionsRemoved(const QList<QOrganizerCollectionId>& collectionIds);

public:
    // Async update functions
    static void updateRequestState(QOrganizerAbstractRequest* req, QOrganizerAbstractRequest::State state);

    static void updateItemOccurrenceFetchRequest(QOrganizerItemOccurrenceFetchRequest* req, const QList<QOrganizerItem>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);
    static void updateItemIdFetchRequest(QOrganizerItemIdFetchRequest* req, const QList<QOrganizerItemId>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);
    static void updateItemFetchByIdRequest(QOrganizerItemFetchByIdRequest* req, const QList<QOrganizerItem>& result, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error>& errorMap, QOrganizerAbstractRequest::State);
    static void updateItemFetchRequest(QOrganizerItemFetchRequest* req, const QList<QOrganizerItem>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);
    static void updateItemFetchForExportRequest(QOrganizerItemFetchForExportRequest* req, const QList<QOrganizerItem>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);
    static void updateItemRemoveRequest(QOrganizerItemRemoveRequest* req, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error>& errorMap, QOrganizerAbstractRequest::State newState);
    static void updateItemSaveRequest(QOrganizerItemSaveRequest* req, const QList<QOrganizerItem>& result, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error>& errorMap, QOrganizerAbstractRequest::State newState);
    static void updateCollectionFetchRequest(QOrganizerCollectionFetchRequest* req, const QList<QOrganizerCollection>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);
    static void updateCollectionRemoveRequest(QOrganizerCollectionRemoveRequest* req, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error>& errorMap, QOrganizerAbstractRequest::State newState);
    static void updateCollectionSaveRequest(QOrganizerCollectionSaveRequest* req, const QList<QOrganizerCollection>& result, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error>& errorMap, QOrganizerAbstractRequest::State newState);

    // Other protected area update functions
    static const QOrganizerItemEngineId* engineItemId(const QOrganizerItemId& id);
    static const QOrganizerCollectionEngineId* engineCollectionId(const QOrganizerCollectionId& id);

    /* Helper functions */
    static bool isItemBetweenDates(const QOrganizerItem& item, const QDateTime& startPeriod, const QDateTime& endPeriod);
    static bool itemLessThan(const QOrganizerItem& a, const QOrganizerItem& b);
    static int compareItem(const QOrganizerItem& a, const QOrganizerItem& b, const QList<QOrganizerItemSortOrder>& sortOrders);
    static int addSorted(QList<QOrganizerItem> *sorted, const QOrganizerItem &toAdd, const QList<QOrganizerItemSortOrder> &sortOrders);
    static int compareVariant(const QVariant& first, const QVariant& second, Qt::CaseSensitivity sensitivity);
    static bool testFilter(const QOrganizerItemFilter& filter, const QOrganizerItem& item);

    static QOrganizerItemFilter canonicalizedFilter(const QOrganizerItemFilter& filter);

private:
    /* QOrganizerItemChangeSet is a utility class used to emit the appropriate signals */
    friend class QOrganizerItemChangeSet;
    friend class QOrganizerCollectionChangeSet;
};

QTORGANIZER_END_NAMESPACE

#endif
