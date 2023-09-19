Name:       qt5-qtpim
Summary:    Qt PIM modules
Version:    0.0~gita102eacec29d54e609e4ac4350b70bc7f7f92d5e
Release:    1%{?dist}
License:    (LGPLv2 or LGPLv3) with exception or GPLv3 or Qt Commercial
URL:        https://www.qt.io/
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  qt5-qtcore-devel
BuildRequires:  qt5-qtxmlpatterns-devel
BuildRequires:  qt5-qtdeclarative-devel
BuildRequires:  qt5-qtdeclarative-qtquick-devel
BuildRequires:  qt5-qttest-devel
BuildRequires:  fdupes

%description
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt PIM module


%package -n qt5-qtpim-contacts
Summary:    Qt PIM Contacts
Requires(post):     /sbin/ldconfig
Requires(postun):   /sbin/ldconfig

%description -n qt5-qtpim-contacts
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt PIM Contacts module


%package -n qt5-qtpim-contacts-tests
Summary:    Qt PIM Contacts unit tests
Requires:   qt5-qtpim-contacts = %{version}-%{release}

%description -n qt5-qtpim-contacts-tests
Qt PIM Contacts unit tests.


%package -n qt5-qtpim-contacts-devel
Summary:    Qt PIM - development files
Requires:   qt5-qtpim-contacts = %{version}-%{release}

%description -n qt5-qtpim-contacts-devel
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt PIM Contacts development files


%package -n qt5-qtdeclarative-pim-contacts
Summary:    Qt PIM Contacts import for QtDeclarative
Requires:   qt5-qtdeclarative

%description -n qt5-qtdeclarative-pim-contacts
This package contains the PIM Contacts imports for QtDeclarative


%package -n qt5-qtpim-organizer
Summary:    Qt PIM Organizer
Requires(post):     /sbin/ldconfig
Requires(postun):   /sbin/ldconfig

%description -n qt5-qtpim-organizer
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt PIM Organizer module


%package -n qt5-qtpim-organizer-tests
Summary:    Qt PIM Organizer unit tests
Requires:   qt5-qtpim-organizer = %{version}-%{release}

%description -n qt5-qtpim-organizer-tests
Qt PIM Organizer unit tests.


%package -n qt5-qtpim-organizer-devel
Summary:    Qt PIM - development files
Requires:   qt5-qtpim-organizer = %{version}-%{release}

%description -n qt5-qtpim-organizer-devel
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt PIM Organizer development files


%package -n qt5-qtdeclarative-pim-organizer
Summary:    Qt PIM Organizer import for QtDeclarative
Requires:   qt5-qtdeclarative

%description -n qt5-qtdeclarative-pim-organizer
This package contains the PIM Organizer imports for QtDeclarative



%package -n qt5-qtpim-versit
Summary:    Qt PIM Versit
Requires(post):     /sbin/ldconfig
Requires(postun):   /sbin/ldconfig

%description -n qt5-qtpim-versit
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt PIM Versit module


%package -n qt5-qtpim-versit-tests
Summary:    Qt PIM Versit unit tests
Requires:   qt5-qtpim-versit = %{version}-%{release}

%description -n qt5-qtpim-versit-tests
Qt PIM Versit unit tests.


%package -n qt5-qtpim-versit-devel
Summary:    Qt PIM - development files
Requires:   qt5-qtpim-versit = %{version}-%{release}

%description -n qt5-qtpim-versit-devel
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt PIM Versit development files



%package -n qt5-qtpim-versitorganizer
Summary:    Qt PIM VersitOrganizer
Requires(post):     /sbin/ldconfig
Requires(postun):   /sbin/ldconfig

%description -n qt5-qtpim-versitorganizer
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt PIM VersitOrganizer module


%package -n qt5-qtpim-versitorganizer-tests
Summary:    Qt PIM VersitOrganizer unit tests
Requires:   qt5-qtpim-versitorganizer = %{version}-%{release}

%description -n qt5-qtpim-versitorganizer-tests
Qt PIM VersitOrganizer unit tests.


%package -n qt5-qtpim-versitorganizer-devel
Summary:    Qt PIM - development files
Requires:   qt5-qtpim-versitorganizer = %{version}-%{release}

%description -n qt5-qtpim-versitorganizer-devel
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt PIM VersitOrganizer development files


%prep
%setup -q -n %{name}-%{version}

%build
export QTDIR=/usr/share/qt5
touch .git
%qmake5
%make_build

%install
%qmake_install
# Fix wrong path in pkgconfig files
find %{buildroot}%{_libdir}/pkgconfig -type f -name '*.pc' \
-exec perl -pi -e "s, -L%{_builddir}/?\S+,,g" {} \;
# Fix wrong path in prl files
find %{buildroot}%{_libdir} -type f -name '*.prl' \
-exec sed -i -e "/^QMAKE_PRL_BUILD_DIR/d;s/\(QMAKE_PRL_LIBS =\).*/\1/" {} \;
# Remove unneeded .la files
rm -f %{buildroot}/%{_libdir}/*.la

# We don't need qt5/Qt/
rm -rf %{buildroot}/%{_qt5_includedir}/Qt

%fdupes %{buildroot}/%{_includedir}



%post -n qt5-qtpim-contacts
/sbin/ldconfig
%postun -n qt5-qtpim-contacts
/sbin/ldconfig

%post -n qt5-qtpim-organizer
/sbin/ldconfig
%postun -n qt5-qtpim-organizer
/sbin/ldconfig

%post -n qt5-qtpim-versit
/sbin/ldconfig
%postun -n qt5-qtpim-versit
/sbin/ldconfig

%post -n qt5-qtpim-versitorganizer
/sbin/ldconfig
%postun -n qt5-qtpim-versitorganizer
/sbin/ldconfig




%files -n qt5-qtpim-contacts
%defattr(-,root,root,-)
%license LICENSE.LGPLv* LGPL_EXCEPTION.txt LICENSE.GPLv3
%{_qt5_libdir}/libQt5Contacts.so.5
%{_qt5_libdir}/libQt5Contacts.so.5.*
%{_qt5_plugindir}/contacts/libqtcontacts_memory.so

%files -n qt5-qtpim-contacts-tests
%defattr(-,root,root,-)
%{_qt5_plugindir}/contacts/libmaliciousplugin.so
%{_qt5_archdatadir}/tests/qcontact/qcontact
%{_qt5_archdatadir}/tests/qcontactcollection/qcontactcollection
%{_qt5_archdatadir}/tests/qcontactdetail/qcontactdetail
%{_qt5_archdatadir}/tests/qcontactdetails/qcontactdetails
%{_qt5_archdatadir}/tests/qcontactfilter/qcontactfilter
%{_qt5_archdatadir}/tests/qcontactmanager/qcontactmanager
%{_qt5_archdatadir}/tests/qcontactmanagerdetails/qcontactmanagerdetails
%{_qt5_archdatadir}/tests/qcontactrelationship/qcontactrelationship
%{_qt5_archdatadir}/tests/qcontactsortorder/qcontactsortorder
%{_qt5_archdatadir}/tests/tst_qcontactasync/tst_qcontactasync
%{_qt5_archdatadir}/tests/unittest/unittest

%files -n qt5-qtpim-contacts-devel
%defattr(-,root,root,-)
%{_qt5_libdir}/libQt5Contacts.so
%{_qt5_libdir}/libQt5Contacts.prl
%{_qt5_libdir}/pkgconfig/Qt5Contacts.pc
%{_qt5_includedir}/QtContacts/
%{_qt5_archdatadir}/mkspecs/modules/qt_lib_contacts.pri
%{_qt5_archdatadir}/mkspecs/modules/qt_lib_contacts_private.pri
%{_qt5_libdir}/cmake/Qt5Contacts/

%files -n qt5-qtdeclarative-pim-contacts
%defattr(-,root,root,-)
%{_qt5_archdatadir}/qml/QtContacts/

%files -n qt5-qtpim-organizer
%defattr(-,root,root,-)
%{_libdir}/libQt5Organizer.so.5
%{_libdir}/libQt5Organizer.so.5.*
%{_qt5_plugindir}/organizer/libqtorganizer_memory.so

%files -n qt5-qtpim-organizer-tests
%defattr(-,root,root,-)
%{_qt5_plugindir}/organizer/libmaliciousplugin.so
%{_qt5_archdatadir}/tests/qorganizercollection/qorganizercollection
%{_qt5_archdatadir}/tests/qorganizere2e/qorganizere2e
%{_qt5_archdatadir}/tests/qorganizeritem/qorganizeritem
%{_qt5_archdatadir}/tests/qorganizeritemdetail/qorganizeritemdetail
%{_qt5_archdatadir}/tests/qorganizeritemdetails/qorganizeritemdetails
%{_qt5_archdatadir}/tests/qorganizeritemfilter/qorganizeritemfilter
%{_qt5_archdatadir}/tests/qorganizeritemsortorder/qorganizeritemsortorder
%{_qt5_archdatadir}/tests/qorganizermanager/qorganizermanager
%{_qt5_archdatadir}/tests/qorganizermanagerdetails/qorganizermanagerdetails
%{_qt5_archdatadir}/tests/tst_qorganizeritemasync/tst_qorganizeritemasync

%files -n qt5-qtpim-organizer-devel
%defattr(-,root,root,-)
%{_qt5_libdir}/libQt5Organizer.so
%{_qt5_libdir}/libQt5Organizer.prl
%{_qt5_libdir}/pkgconfig/Qt5Organizer.pc
%{_qt5_includedir}/QtOrganizer/
%{_qt5_archdatadir}/mkspecs/modules/qt_lib_organizer.pri
%{_qt5_archdatadir}/mkspecs/modules/qt_lib_organizer_private.pri
%{_libdir}/cmake/Qt5Organizer/

%files -n qt5-qtdeclarative-pim-organizer
%defattr(-,root,root,-)
%{_qt5_archdatadir}/qml/QtOrganizer/

%files -n qt5-qtpim-versit
%defattr(-,root,root,-)
%{_libdir}/libQt5Versit.so.5
%{_libdir}/libQt5Versit.so.5.*
%{_qt5_plugindir}/versit/libqtversit_backuphandler.so
%{_qt5_plugindir}/versit/libqtversit_vcardpreserver.so

%files -n qt5-qtpim-versit-tests
%defattr(-,root,root,-)
%{_qt5_archdatadir}/tests/qvcard21writer/qvcard21writer
%{_qt5_archdatadir}/tests/qvcard30writer/qvcard30writer
%{_qt5_archdatadir}/tests/qversitdocument/qversitdocument
%{_qt5_archdatadir}/tests/qversitproperty/qversitproperty
%{_qt5_archdatadir}/tests/qversitreader/qversitreader
%{_qt5_archdatadir}/tests/qversitwriter/qversitwriter
%{_qt5_archdatadir}/tests/qversitcontactexporter/qversitcontactexporter
%{_qt5_archdatadir}/tests/qversitcontactimporter/qversitcontactimporter
%{_qt5_archdatadir}/tests/tst_qversitcontactplugins/tst_qversitcontactplugins

%files -n qt5-qtpim-versit-devel
%defattr(-,root,root,-)
%{_qt5_libdir}/libQt5Versit.so
%{_qt5_libdir}/libQt5Versit.prl
%{_qt5_libdir}/pkgconfig/Qt5Versit.pc
%{_qt5_includedir}/QtVersit/
%{_qt5_archdatadir}/mkspecs/modules/qt_lib_versit.pri
%{_qt5_archdatadir}/mkspecs/modules/qt_lib_versit_private.pri
%{_qt5_libdir}/cmake/Qt5Versit/

%files -n qt5-qtpim-versitorganizer
%defattr(-,root,root,-)
%{_qt5_libdir}/libQt5VersitOrganizer.so.5
%{_qt5_libdir}/libQt5VersitOrganizer.so.5.*

%files -n qt5-qtpim-versitorganizer-tests
%defattr(-,root,root,-)
%{_qt5_archdatadir}/tests/qversitorganizerexporter/qversitorganizerexporter
%{_qt5_archdatadir}/tests/qversitorganizerimporter/qversitorganizerimporter

%files -n qt5-qtpim-versitorganizer-devel
%defattr(-,root,root,-)
%{_qt5_libdir}/libQt5VersitOrganizer.so
%{_qt5_libdir}/libQt5VersitOrganizer.prl
%{_qt5_libdir}/pkgconfig/Qt5VersitOrganizer.pc
%{_qt5_includedir}/QtVersitOrganizer/
%{_qt5_archdatadir}/mkspecs/modules/qt_lib_versitorganizer.pri
%{_qt5_archdatadir}/mkspecs/modules/qt_lib_versitorganizer_private.pri
%{_qt5_libdir}/cmake/Qt5VersitOrganizer/

