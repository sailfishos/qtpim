/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Pim Module.
**
** $QT_BEGIN_LICENSE:FDL$
** GNU Free Documentation License
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms
** and conditions contained in a signed written agreement between you
** and Nokia.
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.0
// ![Custom DetailsEditWidget is used for entering new or modifying existing contact detlais]
Item {
    width: parent.width - 30
    height: 20

    property alias label: label.text
    property alias value: valueText.text
    property alias color: valueText.color
    property alias inputFocus: valueText.focus

    property string old

    Text {
        id: label
        font {
            family: "Helvetica"
            pixelSize: 15
            bold: true
            italic: true
        }
    }

    Rectangle {
        width: 180
        height: 20
        anchors.left: label.right
        anchors.leftMargin: 6
        color: "white"
        border {
            width: 2
            color: "darkgreen"
        }

        TextInput {
            id: valueText
            anchors.fill: parent
            anchors.leftMargin: 5
            font {
                pixelSize: 16
                family: "Helvetica"
            }
            onFocusChanged: {
                if (focus === true) { // when entering text field
                    old = valueText.text
                } else { // when exiting text field
                    if (valueText.text !== old) {
                        valueText.color = "red"
                    }
                }
            }
        }
    }
}
// ![Custom DetailsEditWidget is used for entering new or modifying existing contact detlais]
