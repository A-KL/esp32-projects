# This Python file uses the following encoding: utf-8
import sys
import PySide6.QtCore

from PySide6.QtWidgets import QApplication, QWidget

# Important:
# You need to run the following command to generate the ui_form.py file
#     pyside6-uic form.ui -o ui_form.py, or
#     pyside2-uic form.ui -o ui_form.py
from ui_form import Ui_Widget

import paho.mqtt.client as mqtt

BROKER = "127.0.0.1"
PORT = 1883
TOPIC = "openstreamdeck/monitor/metrics/+/+/+" #macbookpro/RAM/Used

class Widget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.ui = Ui_Widget()
        self.ui.setupUi(self)
#        self.setWindowFlags(FramelessWindowHint)
        self.setWindowFlags(PySide6.QtCore.Qt.FramelessWindowHint)
#        self.setAttribute(PySide6.QtCore.Qt.WA_TranslucentBackground)
        self.setStyleSheet("background:rgba(50, 50, 50, 0);")


if __name__ == "__main__":
    def on_connect(client, userdata, flags, reason_code, properties=None):
        print(f"Connected with result code {reason_code}")
        client.subscribe(TOPIC)

    # Called when a message is received
    def on_message(client, userdata, msg):
        print(f"Topic: {msg.topic}")
        data = msg.payload.decode('utf-8')
#        print(f"Payload: {msg.payload.decode('utf-8')}")

        if 'CPU0/Load' in msg.topic:
            widget.ui.cpu_load.setText(data)
            return

        if 'CPU0/Temperature' in msg.topic:
            widget.ui.cpu_temp.setText(data)
            return

        if 'GPU0/Load' in msg.topic:
            widget.ui.gpu_load.setText(data)
            return

        if 'GPU0/Temperature' in msg.topic:
            widget.ui.gpu_temp.setText(data)
            return

        if 'RAM/Used' in msg.topic:
            widget.ui.ram_used.setText(data)
            return

        if 'VRAM/Used' in msg.topic:
            widget.ui.vram_used.setText(data)
            return

    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(BROKER, PORT, keepalive=60)

    # Listen forever
#    client.loop_forever()
    client.loop_start()

    app = QApplication(sys.argv)
    widget = Widget()
    widget.show()

    sys.exit(app.exec())

    client.loop_stop()
    client.disconnect()
