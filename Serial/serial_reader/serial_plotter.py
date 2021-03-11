from PyQt5.QtGui import QIntValidator
from PyQt5.QtWidgets import QGridLayout, QApplication, QPushButton, QLineEdit
from qtpy import QtWidgets

from matplot_vidgets import MplWidget
import serial

import numpy as np


class SerialPlotter(QtWidgets.QWidget):

    FREQ = 4800

    def __init__(self):
        super(SerialPlotter, self).__init__()

        self.setLayout(QGridLayout(self))

        self.grid = []

        self.mpl = MplWidget(dpi=100)
        self.layout().addWidget(self.mpl, 0, 0)

        self.iterationsLabel = QLineEdit()
        self.iterationsLabel.setText('600')
        self.iterationsLabel.setValidator(QIntValidator(0,1000))
        self.layout().addWidget(self.iterationsLabel, 1, 0)

        button = QPushButton('Read shit!')
        button.clicked.connect(self.read)
        self.layout().addWidget(button, 2, 0)

        self.port = None
        port, flag = self.scan_connections()
        if flag:
            self.port = port

    def read(self):

        if self.port is None:

            port, flag = self.scan_connections()
            if flag:
                self.port = port
            else:
                print('Vario not connected.')
                return

        iterations = int(self.iterationsLabel.text())

        with serial.Serial(self.port, self.FREQ, timeout=5) as ser:

            cache = []
            try:
                for i in range(iterations):
                    line = ser.readline()
                    if line:
                        print(line.strip())
                        cache.append(float(line))
                    else:
                        print('Reading data failed.')
                        return

            except Exception as ex:
                print(ex)

            if len(cache) > 1:
                std = np.std(cache)
                self.mpl.sc.axes.plot(cache, label='std = {:0.3e}'.format(std))
                self.mpl.sc.fig.legend()
                self.mpl.sc.draw()
            else:
                print('Reading data failed.')


    def scan_connections(self, freq=1200):

        flag_found = False
        print("Looking for ports.")
        for i in range(2, 20):
            COM0 = "COM" + repr(i)
            print("\tConnecting to port {:s}...".format(COM0), end='')
            try:
                ser = serial.Serial(COM0, freq)
                ser.close()
            except Exception as ex:
                print("Not this one.")
            else:
                flag_found = True
                print("Found open port: ", COM0)
                COM1 = "COM" + repr(i + 1)
                break
        print("Done.")
        return COM0, flag_found

if __name__ == "__main__":

    import sys
    app = QApplication(sys.argv)

    window = SerialPlotter()
    window.show()
    sys.exit(app.exec_())

