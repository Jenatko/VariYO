from PyQt5 import QtWidgets
from PyQt5.QtGui import QPainter
from PyQt5.QtWidgets import QStyle, QStyleOption


class MyBaseWidget(QtWidgets.QWidget):

    def __init__(self, *args, **kwargs):

        super(MyBaseWidget, self).__init__(*args, **kwargs)

    def paintEvent(self, QPaintEvent):
        opt = QStyleOption()
        opt.initFrom(self)
        p = QPainter(self)
        self.style().drawPrimitive(QStyle.PE_Widget, opt, p, self)
