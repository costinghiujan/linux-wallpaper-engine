import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "black"

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true 
    }

    ShaderEffect {
        id: waterShader
        anchors.fill: parent

        fragmentShader: "qrc:/assets/shaders/water.frag.qsb"

        property real time: 0.0
        property vector2d resolution: Qt.vector2d(root.width, root.height)
        
        property vector2d mouse: Qt.vector2d(
            mouseArea.mouseX / root.width,
            mouseArea.mouseY / root.height
        )

        // --- FIX PENTRU RESTART ---
        // Folosim un multiplu de Pi (314.159) pentru ca bucla să fie perfectă.
        // Durata este ajustată pentru a păstra aceeași viteză ca înainte.
        NumberAnimation on time {
            from: 0
            to: 314.159265
            duration: 314159
            loops: Animation.Infinite
            running: true
        }
    }
}