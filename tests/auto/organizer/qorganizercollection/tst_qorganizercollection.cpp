/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QtTest/QtTest>

#include <QtCore/qset.h>

#include <QtOrganizer/qorganizer.h>
#include <QtOrganizer/qorganizeritemid.h>

//TESTED_COMPONENT=src/organizer

QTORGANIZER_USE_NAMESPACE

static inline QOrganizerCollectionId makeId(const QString &managerName, uint id)
{
    return QOrganizerCollectionId(QStringLiteral("qtorganizer:%1:").arg(managerName), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}


class tst_QOrganizerCollection: public QObject
{
Q_OBJECT

public:
    tst_QOrganizerCollection();
    virtual ~tst_QOrganizerCollection();

private slots:
    void metaData();
    void compare();
    void idComparison();
    void idHash();
    void idStringFunctions();
    void hash();
    void datastream();
    void traits();
    void idTraits();
};

tst_QOrganizerCollection::tst_QOrganizerCollection()
{
}

tst_QOrganizerCollection::~tst_QOrganizerCollection()
{
}

void tst_QOrganizerCollection::metaData()
{
    QOrganizerCollection c;
    QVERIFY(c.metaData().isEmpty());
    c.setExtendedMetaData(QString(QStringLiteral("test")), 5);
    QCOMPARE(c.extendedMetaData(QString(QStringLiteral("test"))).toInt(), 5);
    QCOMPARE(c.extendedMetaData().keys(), QList<QString>() << QStringLiteral("test"));
    QCOMPARE(c.extendedMetaData().value(QString(QStringLiteral("test"))).toInt(), 5);

    QMap<QOrganizerCollection::MetaDataKey, QVariant> mdm;
    mdm.insert(QOrganizerCollection::KeyName, QString(QStringLiteral("test2")));
    c.setMetaData(mdm);
    QCOMPARE(c.metaData(), mdm);
    QCOMPARE(c.metaData(QOrganizerCollection::KeyName).toString(), QString(QStringLiteral("test2")));
}

void tst_QOrganizerCollection::compare()
{
    QOrganizerCollection c, c2;
    QVERIFY(c == c2);
    c.setExtendedMetaData(QStringLiteral("test"), 5);
    QVERIFY(c != c2);
    c2.setExtendedMetaData(QStringLiteral("test"), 5);
    QVERIFY(c == c2);

    QMap<QOrganizerCollection::MetaDataKey, QVariant> mdm;
    mdm.insert(QOrganizerCollection::KeyName, QStringLiteral("test2"));
    c.setMetaData(mdm);
    QVERIFY(c != c2);
    c2.setMetaData(mdm);
    QVERIFY(c == c2);

    c2 = QOrganizerCollection();
    QVERIFY(c != c2);
    c2 = c;
    QVERIFY(c == c2);

    c.setId(makeId(QStringLiteral("a"), 1));
    QVERIFY(c != c2);
    c2.setId(makeId(QStringLiteral("a"), 1));
    QVERIFY(c == c2);
    c.setId(makeId(QStringLiteral("b"), 1));
    QVERIFY(c != c2);
    c2.setId(c.id());
    QVERIFY(c == c2);
    c.setId(makeId(QStringLiteral("b"), 2));
}

void tst_QOrganizerCollection::idComparison()
{
    QOrganizerCollectionId id1(makeId("a", 1));
    QOrganizerCollectionId id2(makeId("a", 1));
    QVERIFY(!(id1 < id2));
    QVERIFY(!(id2 < id1));
    QVERIFY(id1 == id2);

    QOrganizerCollectionId id3(makeId("a", 2));
    QOrganizerCollectionId id4(makeId("b", 1));
    QOrganizerCollectionId id5(makeId("b", 2));
    QVERIFY((((id1 < id3) && !(id3 < id1)) || ((id3 < id1) && !(id1 < id3))) && (id1 != id3));
    QVERIFY((((id1 < id4) && !(id4 < id1)) || ((id4 < id1) && !(id1 < id4))) && (id1 != id4));
    QVERIFY((((id3 < id4) && !(id4 < id3)) || ((id4 < id3) && !(id3 < id4))) && (id3 != id4));
    QVERIFY((((id1 < id5) && !(id5 < id1)) || ((id5 < id1) && !(id1 < id5))) && (id3 != id4));

    QOrganizerCollectionId id6;
    QOrganizerCollectionId id7(QString(), "1");
    QOrganizerCollectionId id8(QString(), "2");
    QOrganizerCollectionId id9(QStringLiteral("qtorganizer:basic:"), QByteArray());
    QVERIFY(id6.isNull());
    QVERIFY(id7.isNull());
    QVERIFY(id8.isNull());
    QVERIFY(id9.isNull());
    QVERIFY(id6 == id7);
    QVERIFY(!(id6 < id7));
    QVERIFY(id7 == id6);
    QVERIFY(!(id7 < id6));
    QVERIFY(id7 == id8);
    QVERIFY(!(id7 < id8));
    QVERIFY(id8 == id7);
    QVERIFY(!(id9 < id8));
    QVERIFY(id8 == id9);
    QVERIFY(!(id8 < id9));
    QVERIFY(id9 == id8);
    QVERIFY(!(id9 < id8));

    QVERIFY(!(id1 == id6));
    QVERIFY(!(id1 < id6));
    QVERIFY(id6 < id1);
    QVERIFY(!(id1 == id7));
    QVERIFY(!(id1 < id7));
    QVERIFY(id7 < id1);
    QVERIFY(!(id1 == id8));
    QVERIFY(!(id1 < id8));
    QVERIFY(id8 < id1);
    QVERIFY(!(id1 == id9));
    QVERIFY(!(id1 < id9));
    QVERIFY(id9 < id1);
}

void tst_QOrganizerCollection::idHash()
{
    QOrganizerCollectionId id1(makeId("a", 1));
    QOrganizerCollectionId id2(makeId("a", 1));
    QOrganizerCollectionId id3(makeId("b", 1));
    QOrganizerCollectionId id4(makeId("a", 2));
    // note that the hash function ignores the managerUri
    QCOMPARE(qHash(id1), qHash(id2));
    QCOMPARE(qHash(id1), qHash(id3));
    QVERIFY(qHash(id1) != qHash(id4));

    QSet<QOrganizerCollectionId> set;
    set.insert(id1);
    set.insert(id2);
    set.insert(id3);
    set.insert(id4);
    QCOMPARE(set.size(), 3);
}

void tst_QOrganizerCollection::idStringFunctions()
{
    // TODO: review test
    QOrganizerCollectionId id1(makeId("a", 1));
    QOrganizerCollectionId id2(makeId("a", 1));
    QOrganizerCollectionId id3(makeId("b", 1));
    QOrganizerCollectionId id4(makeId("a", 2));
    QVERIFY(qHash(id1) == qHash(id2));
    QVERIFY(qHash(id1) != qHash(id4));

    // note that the toString and fromString functions are
    // engine and id specific.  This test merely checks that
    // the API is hooked up correctly.

    QVERIFY(id1.toString() == id2.toString());
    QVERIFY(id1.toString() != id3.toString());
    QVERIFY(id1.toString() != id4.toString());
    QVERIFY(id3.toString() != id4.toString());

    // this should "work" -- string of the correct format
    const uint numericId2 = 2u;
    const QByteArray localId2 = QByteArray(reinterpret_cast<const char *>(&numericId2), sizeof(uint));
    QString prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString("::") + localId2.toHex();
    QOrganizerCollectionId rebuiltid = QOrganizerCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string has the right format and one parameter, but requires a working backend
    prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString(":") + QString("key=value") + QString(":") + localId2.toHex();
    rebuiltid = QOrganizerCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid != id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string has the right format and some parameters, but requires a working backend
    prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString(":") + QString("key=value&key2=value2") + QString(":") + localId2.toHex();
    rebuiltid = QOrganizerCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid != id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string has the right format but misses the value for a parameter
    prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString(":") + QString("key=value&key2=") + QString(":") + localId2.toHex();
    rebuiltid = QOrganizerCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid != id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string misses a field (the parameters)
    prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString(":") + localId2.toHex();
    rebuiltid = QOrganizerCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QOrganizerCollectionId()); // invalid so should be null.

    // this string misses two fields (params plus manager uri)
    prebuiltidstring = QString("qtorganizer") + QString(":") + QString::number(2);
    rebuiltid = QOrganizerCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QOrganizerCollectionId()); // invalid so should be null.

    // this string misses the prefix (qtorganizer)
    prebuiltidstring = QString("notorganizer") + QString(":") + QString("a") + QString("::") + localId2.toHex();
    rebuiltid = QOrganizerCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QOrganizerCollectionId()); // invalid so should be null.

    // this string misses the manager uri
    prebuiltidstring = QString("notorganizer") + QString(":::") + localId2.toHex();
    rebuiltid = QOrganizerCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QOrganizerCollectionId()); // invalid so should be null.
}

void tst_QOrganizerCollection::hash()
{
    // TODO: review tests
    QOrganizerCollectionId id(makeId("a", 1));
    QOrganizerCollection c1;
    c1.setId(id);
    c1.setExtendedMetaData("key", "value");
    QOrganizerCollection c2;
    c2.setId(id);
    c2.setExtendedMetaData("key", "value");
    QOrganizerCollection c3;
    c3.setId(id);
    c3.setExtendedMetaData("key", "another value");
    QOrganizerCollection c4; // no details
    c4.setId(id);
    QOrganizerCollection c5;
    c5.setId(id);
    c5.setExtendedMetaData("key", "value");
    QVERIFY(qHash(c1) == qHash(c2));
    QVERIFY(qHash(c1) != qHash(c3));
    QVERIFY(qHash(c1) != qHash(c4));
    QVERIFY(qHash(c1) == qHash(c5));
}

void tst_QOrganizerCollection::datastream()
{
    // collection datastreaming
    QByteArray buffer;
    QOrganizerCollection collectionIn;
    collectionIn.setExtendedMetaData("key", "value");
    QOrganizerCollection collectionOut;
    QOrganizerCollectionId originalId;

    // first, stream an item with a complete id
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        QOrganizerManager om("memory");
        QVERIFY(om.saveCollection(&collectionIn)); // fill in its ID
        originalId = collectionIn.id();
        stream1 << collectionIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> collectionOut;
        QCOMPARE(collectionOut, collectionIn); // can use QCOMPARE for collections, since no detail keys.
    }

    // second, stream an item with an id with the mgr uri set, local id null
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        collectionIn.setId(QOrganizerCollectionId());
        stream1 << collectionIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> collectionOut;
        QCOMPARE(collectionOut, collectionIn); // can use QCOMPARE for collections, since no detail keys.
    }

    /* TODO: Review tests
    // third, stream an item with an id with the mgr uri null, local id set
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        QOrganizerCollectionId modifiedId = originalId;
        modifiedId.setManagerUri(QString()); // this will clear the local id!
        modifiedId.setId(originalId.localId()); // so reset it and make sure nothing bad happens.
        collectionIn.setId(modifiedId);
        stream1 << collectionIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> collectionOut;
        QVERIFY(collectionOut.metaData() == collectionIn.metaData());
        QVERIFY(collectionOut.id() != collectionIn.id()); // no manager uri of input :. won't be serialized.
    }*/

    // fourth, stream an item with a null id
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        collectionIn.setId(QOrganizerCollectionId());
        stream1 << collectionIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> collectionOut;
        QVERIFY(collectionOut.metaData() == collectionIn.metaData());
        QVERIFY(collectionOut.id() == collectionIn.id()); // should both be null ids.
    }

    // id datastreaming
    buffer.clear();
    QOrganizerCollectionId inputId;
    QOrganizerCollectionId outputId;

    // first, stream the whole id (mgr uri set, local id set)
    {
        inputId = originalId;
        QString serializedId = inputId.toString();
        outputId = QOrganizerCollectionId::fromString(serializedId);
        QCOMPARE(inputId, outputId);

        inputId = originalId;
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }

    /* TODO: review test
    // second, stream a partial id (mgr uri null, local id set)
    {
        inputId.setManagerUri(QString());
        inputId.setId(originalId.localId());
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;

        // because the manager uri is null, we cannot stream it in.
        QVERIFY(outputId.isNull());
        QVERIFY(!inputId.isNull());
    }

    // third, stream a partial id (mgr uri set, local id null).
    {
        inputId.setManagerUri(originalId.managerUri());
        inputId.setId(QOrganizerCollectionId());
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }*/

    // fourth, stream a null id
    {
        inputId = QOrganizerCollectionId();
        QString serializedId = inputId.toString();
        outputId = QOrganizerCollectionId::fromString(serializedId);
        QCOMPARE(inputId, outputId);

        inputId = QOrganizerCollectionId();
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }

    /* TODO: review test
    // fifth, stream an id after changing it's manager uri.
    {
        inputId.setManagerUri(originalId.managerUri());
        inputId.setId(originalId.localId());
        inputId.setManagerUri("test manager uri"); // should clear the local id.
        QVERIFY(inputId.localId() == QOrganizerCollectionId());
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }

    // sixth, stream an id after changing it's manager uri and resetting the local id.
    // this should cause great problems, because the manager doesn't exist so it shouldn't
    // be able to deserialize.  Make sure it's handled gracefully.
    {
        inputId.setManagerUri(originalId.managerUri());
        inputId.setManagerUri("test manager uri"); // should clear the local id.
        inputId.setId(originalId.localId());
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QVERIFY(outputId.isNull());
    } */
}

void tst_QOrganizerCollection::traits()
{
    QCOMPARE(sizeof(QOrganizerCollection), sizeof(void *));
    QVERIFY(QTypeInfo<QOrganizerCollection>::isComplex);
    QVERIFY(!QTypeInfo<QOrganizerCollection>::isStatic);
    QVERIFY(!QTypeInfo<QOrganizerCollection>::isLarge);
    QVERIFY(!QTypeInfo<QOrganizerCollection>::isPointer);
    QVERIFY(!QTypeInfo<QOrganizerCollection>::isDummy);
}

void tst_QOrganizerCollection::idTraits()
{
    QCOMPARE(sizeof(QOrganizerCollectionId), 2*sizeof(void *));
    QVERIFY(QTypeInfo<QOrganizerCollectionId>::isComplex);
    QVERIFY(!QTypeInfo<QOrganizerCollectionId>::isStatic);
    QVERIFY(QTypeInfo<QOrganizerCollectionId>::isLarge);
    QVERIFY(!QTypeInfo<QOrganizerCollectionId>::isPointer);
    QVERIFY(!QTypeInfo<QOrganizerCollectionId>::isDummy);
}

QTEST_MAIN(tst_QOrganizerCollection)
#include "tst_qorganizercollection.moc"
