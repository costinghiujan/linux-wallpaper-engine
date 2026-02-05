import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    // Size will be overridden by C++ parent, but good for defaults
    width: 1920
    height: 1080
    color: "black"

    // 1. Mouse Tracking
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true // Track movement without clicking
    }

    // 2. Shader Effect
    ShaderEffect {
        id: waterShader
        anchors.fill: parent

        // Load shader from resources
        fragmentShader: "qrc:/assets/shaders/water.frag.qsb"

        // --- Properties sent to GLSL ---
        property real time: 0.0
        property vector2d resolution: Qt.vector2d(root.width, root.height)
        
        // Normalize mouse to 0.0 - 1.0 range
        property vector2d mouse: Qt.vector2d(
            mouseArea.mouseX / root.width,
            mouseArea.mouseY / root.height
        )

        // Animate 'time' forever
        NumberAnimation on time {
            from: 0
            to: 100
            duration: 100000
            loops: Animation.Infinite
            running: true
        }
    }
}