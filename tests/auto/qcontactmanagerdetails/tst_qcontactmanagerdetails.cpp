/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QtTest/QtTest>
#include <QDateTime>
#include <QtContacts>

#include "../qcontactmanagerdataholder.h"

QTCONTACTS_USE_NAMESPACE

// to get QFETCH to work with the template expression...
typedef QMap<QString,QString> tst_QContactManagerDetails_QStringMap;
Q_DECLARE_METATYPE(tst_QContactManagerDetails_QStringMap)
Q_DECLARE_METATYPE(QList<QContactLocalId>)


class tst_QContactManagerDetails : public QObject
{
Q_OBJECT

public:
    tst_QContactManagerDetails();
    virtual ~tst_QContactManagerDetails();

private:
    void addManagers();

    template <typename T>
    void removeDetail( QContact& contact )
    {
        T detail = contact.detail<T>();
        contact.removeDetail( &detail );
    }

    bool saveAndLoadContact( QContactManager *cm, QContact &original, QContact &loaded );
    void saveAndVerifyContact( QContactManager *cm, QContact &original );

    QScopedPointer<QContactManagerDataHolder> managerDataHolder;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testAddress();
    void testAddress_data() {addManagers();}
    void testAnniversary();
    void testAnniversary_data() {addManagers();}
    void testAvatar();
    void testAvatar_data() {addManagers();}
    void testBirthday();
    void testBirthday_data() {addManagers();}
    void testEmailAddress();
    void testEmailAddress_data() {addManagers();}
    void testEmptyExtendedDetail();
    void testEmptyExtendedDetail_data() {addManagers();}
    void testExtendedDetail();
    void testExtendedDetail_data() {addManagers();}
    void testName();
    void testName_data() {addManagers();}
    void testNickName();
    void testNickName_data() {addManagers();}
    void testOrganisation();
    void testOrganisation_data() {addManagers();}
    void testOnlineAccount();
    void testOnlineAccount_data() {addManagers();}
    void testPhoneNumber();
    void testPhoneNumber_data() {addManagers();}
    void testUrl();
    void testUrl_data() {addManagers();}
    void testRingtone();
    void testRingtone_data() {addManagers();}

private slots:

};

tst_QContactManagerDetails::tst_QContactManagerDetails()
{
}

tst_QContactManagerDetails::~tst_QContactManagerDetails()
{
}

void tst_QContactManagerDetails::addManagers()
{
    QTest::addColumn<QString>("uri");

    QStringList managers = QContactManager::availableManagers();

    /* Known one that will not pass */
    managers.removeAll("invalid");
    managers.removeAll("testdummy");
    managers.removeAll("teststaticdummy");
    managers.removeAll("maliciousplugin");

    foreach(QString mgr, managers) {
        QMap<QString, QString> params;
        QTest::newRow(QString("mgr='%1'").arg(mgr).toLatin1().constData()) << QContactManager::buildUri(mgr, params);
    }
}

void tst_QContactManagerDetails::initTestCase()
{
    managerDataHolder.reset(new QContactManagerDataHolder());
}

void tst_QContactManagerDetails::cleanupTestCase()
{
    managerDataHolder.reset(0);
}

bool tst_QContactManagerDetails::saveAndLoadContact( QContactManager *cm, QContact &original, QContact &loaded )
{
    // Save contact
    if( cm->saveContact(&original) == false )
        return false;

    // Check the id
    if( original.localId() == 0 )
        return false;

    // Load same contact from database
    loaded = cm->contact( original.localId() );
    if( cm->error() )
        return false;

    // Ignore some details which are not relevant and will mess
    // up direct comparison between two contacts.
    removeDetail<QContactDisplayLabel>(original);
    removeDetail<QContactDisplayLabel>(loaded);
    removeDetail<QContactGuid>(original);
    removeDetail<QContactGuid>(loaded);
    removeDetail<QContactTimestamp>(original);
    removeDetail<QContactTimestamp>(loaded);

    return true;
}

void tst_QContactManagerDetails::saveAndVerifyContact( QContactManager *cm, QContact &original )
{
    QContact loaded;
    QVERIFY( saveAndLoadContact( cm, original, loaded ) );
    if (original != loaded) {
        qDebug() << "expected: " << original;
        qDebug() << "loaded: " << loaded;
        QCOMPARE( loaded.details().count(), original.details().count() );
        QCOMPARE( loaded, original );
    }
}

void tst_QContactManagerDetails::testAddress()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactAddress a;
    a.setStreet("street");
    a.setLocality("locality");
    a.setRegion("region");
    a.setPostcode("postcode");
    a.setCountry("country");
    a.setPostOfficeBox("POBox");
    c.saveDetail( &a );

    saveAndVerifyContact( cm.data(), c );
    QContact c1;

    QContactAddress a1;
    a1.setStreet("street1");
    a1.setLocality("locality1");
    a1.setRegion("region1");
    a1.setPostcode("postcode1");
    a1.setCountry("country1");
    a1.setPostOfficeBox("POBox1");
    c1.saveDetail( &a1 );

    QContactAddress a2;
    a2.setStreet("street2");
    a2.setLocality("locality2");
    a2.setRegion("region2");
    a2.setPostcode("postcode2");
    a2.setCountry("country2");
    a2.setPostOfficeBox("POBox2");
    c1.saveDetail( &a2 );

    QContact c2;
    saveAndLoadContact( cm.data(), c1, c2 );

    QList<QContactAddress> addresses = c2.details<QContactAddress>();
    QVERIFY( addresses.count() == 2 );
    QVERIFY( a1 == addresses[0] );
    QVERIFY( a2 == addresses[1] );

    QContact c3;

    // General address
    QContactAddress a3;
    a3.setStreet("street1");
    c3.saveDetail( &a3 );

    // home address
    QContactAddress a4;
    a4.setStreet("street2");
    a4.setContexts( QContactDetail::ContextHome );
    c3.saveDetail( &a4 );

    // work address
    QContactAddress a5;
    a5.setStreet("street3");
    a5.setContexts( QContactDetail::ContextWork );
    c3.saveDetail( &a5 );

    saveAndVerifyContact( cm.data(), c3 );
}

void tst_QContactManagerDetails::testAnniversary()
{
    QFETCH(QString, uri);
    if (uri == "qtcontacts:jsondb:") {
        QSKIP("JsonDb backend does not support QContactAnniversary detail!");
    } else {
        QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
        QContact c;
        QContactAnniversary a;

        a.setOriginalDate( QDate(2009,9,9) );
        c.saveDetail( &a );
        saveAndVerifyContact( cm.data(), c );
    }
}

void tst_QContactManagerDetails::testAvatar()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactAvatar a;
    a.setImageUrl( QUrl::fromLocalFile("c:\\foo\\bar.jpeg") );
    c.saveDetail( &a );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testBirthday()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactBirthday b;
    b.setDate( QDate(2001,1,1) );
    c.saveDetail( &b );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testEmailAddress()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactEmailAddress e;
    e.setEmailAddress( "foo@bar.com" );
    c.saveDetail( &e );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testName()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
    QContact c;

    QContactName n;
    n.setPrefix( "prefix" );
    n.setFirstName( "first" );
    n.setMiddleName( "middle" );
    n.setLastName( "last" );
    n.setSuffix( "suffix" );
    c.saveDetail( &n );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testNickName()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactNickname n1;
    n1.setNickname("nickname1");
    c.saveDetail( &n1 );

    saveAndVerifyContact( cm.data(), c );

    QContactNickname n2;
    n2.setNickname("nickname2");
    c.saveDetail( &n2 );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testOrganisation()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactOrganization o;
    o.setName( "Foreign legion" );
    o.setTitle( "Bicycle mechanic" );
    c.saveDetail( &o );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testOnlineAccount()
{
    QFETCH(QString, uri);
    if (uri == "qtcontacts:jsondb:") {
        QSKIP("JsonDb backend does not support QContactOnlineAccount detail!");
    } else {
        QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
        QContact c;

        QContactOnlineAccount o;
        o.setAccountUri( "john@example.com" );
        o.setProtocol(QContactOnlineAccount::ProtocolJabber);
        c.saveDetail( &o );
        saveAndVerifyContact( cm.data(), c );
    }
}

void tst_QContactManagerDetails::testPhoneNumber()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
    QContact c;

    // General landline number
    QContactPhoneNumber n2;
    n2.setNumber( "2" );
    n2.setSubTypes( QContactPhoneNumber::SubTypeLandline );
    c.saveDetail( &n2 );

    saveAndVerifyContact( cm.data(), c );

    QContact c2;

    // home mobile number
    QContactPhoneNumber n1;
    n1.setNumber( "1" );
    n1.setSubTypes( QContactPhoneNumber::SubTypeMobile );
    n1.setContexts( QContactDetail::ContextHome );
    c2.saveDetail( &n1 );

    // work landline number
    QContactPhoneNumber n3;
    n3.setNumber( "3" );
    n3.setSubTypes( QContactPhoneNumber::SubTypeLandline );
    n3.setContexts( QContactDetail::ContextWork );
    c2.saveDetail( &n3 );

    saveAndVerifyContact( cm.data(), c2 );
}

void tst_QContactManagerDetails::testUrl()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactUrl u;
    u.setUrl("http://failblog.org");
    u.setSubType(QContactUrl::SubTypeHomePage);
    c.saveDetail( &u );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testRingtone()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QUrl rtUrl(QUrl::fromLocalFile("/home/user/sample.wav"));

    QContact c;
    QContactRingtone rt;
    rt.setAudioRingtoneUrl(rtUrl);
    c.saveDetail(&rt);

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testEmptyExtendedDetail()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    // Adding an empty extended detail
    QContactExtendedDetail emptyDetail;
    QVERIFY(c.saveDetail(&emptyDetail));
    if (uri == "qtcontacts:jsondb:") {
        QSKIP("This manager does not store empty extended details, so skipping empty extended detail test!", SkipSingle);
    } else {
        saveAndVerifyContact(cm.data(), c);
    }
}

void tst_QContactManagerDetails::testExtendedDetail()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactExtendedDetail extendedDetail1, extendedDetail2;
    extendedDetail1.setName("testExtDetailVariantList");
    extendedDetail1.setData(QVariantList() << QString("QString in the QVariantlist detail.")
              << QVariant(1)
              << QString("Another QString in the QVariantlist detail.")
              << 2);
    QVERIFY(c.saveDetail(&extendedDetail1));
    saveAndVerifyContact(cm.data(), c);

    // Adding two extended details to same contact
    QContact c2;
    extendedDetail2.setName("testExtDetailInt");
    extendedDetail2.setData(2);
    QVERIFY (c2.saveDetail(&extendedDetail2));
    QVERIFY (c2.saveDetail(&extendedDetail1));
    saveAndVerifyContact(cm.data(), c2);

    QSKIP("TODO: skipping random order extended detail saving due to an issue in QContact comparison operator", SkipSingle);
    // Adding same details but in different order
    QContact c3;
    QVERIFY (c3.saveDetail(&extendedDetail1));
    QVERIFY (c3.saveDetail(&extendedDetail2));
    saveAndVerifyContact(cm.data(), c3);
}

QTEST_MAIN(tst_QContactManagerDetails)
#include "tst_qcontactmanagerdetails.moc"
