
import matplotlib
from PyQt5.QtCore import QRunnable, QThreadPool, pyqtSignal, pyqtSlot
from PyQt5.QtGui import QPalette

from my_base_widget import MyBaseWidget

matplotlib.use('Qt5Agg')

from PyQt5 import QtCore, QtGui, QtWidgets

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg, NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure

from matplotlib.widgets import RectangleSelector
import numpy as np


from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 unused import


class MplCanvas(FigureCanvasQTAgg):

    def __init__(self, parent=None, width=5, height=4, dpi=100):
        self.fig = Figure(figsize=(width, height), dpi=dpi)
        super(MplCanvas, self).__init__(self.fig)
        self.axes = self.fig.add_subplot(111)

    def minumumSizeHint(self):
        return QtCore.QSize(50,50)


class MplCanvas3D(FigureCanvasQTAgg):

    def __init__(self, parent=None, width=5, height=4, dpi=100):

        self.fig = Figure(figsize=(width, height), dpi=dpi)
        super(MplCanvas3D, self).__init__(self.fig)
        self.axes = self.fig.add_subplot(111, projection='3d')


class MplWidget(MyBaseWidget):

    def __init__(self, dpi=30, parent=None):
        super(MplWidget, self).__init__(parent)

        self.sc = MplCanvas(self, width=3, height=4, dpi=dpi)

        # Create toolbar, passing canvas as first parament, parent (self, the MainWindow) as second.
        self.toolbar = NavigationToolbar(self.sc, self)
        self.toolbar.hide()

        layout = QtWidgets.QVBoxLayout()
        layout.setContentsMargins(0,0,0,0)
        layout.addWidget(self.toolbar)
        layout.addWidget(self.sc)
        # self.setStyleSheet("border: 1px solid red")
        # pal = self.palette()
        # pal.setColor(QPalette.Background, QtCore.Qt.blue)
        # self.setAutoFillBackground(True)
        # self.setPalette(pal)

        #self.setLineWidth(0)
        # self.setContentsMargins(0,0,0,0)
        #self.setFrameStyle(QtWidgets.QFrame.Plain)
        self.setLayout(layout)

        self.sc.mpl_connect('button_press_event', self.onclick)

        #
        # The object attributes
        self.selected_stuff = None
        self.x = None
        self.y = None
        self.ploted_stuff = None

    def onclick(self, event):
        print('Click on matplotlib canvas..', event)

    def size(self) -> QtCore.QSize:
        return QtCore.QSize(100,100)

    def plot(self, x, y, *args, **kwargs):

        self.x = x
        self.y = y
        self.ploted_stuff = self.sc.axes.plot(x, y, *args, **kwargs)
        self.sc.draw()

    def select_indices(self, idx):
        if self.selected_stuff is not None:
            for s in self.selected_stuff:
                self.selected_stuff.pop().remove()
        if not self.ploted_stuff:
            return
        self.selected_stuff = self.sc.axes.plot(self.x.iloc[idx], self.y.iloc[idx], 'ro')
        self.sc.draw()

    def mouseDoubleClickEvent(self, a0: QtGui.QMouseEvent) -> None:
        print('Double click on this one.')


class MplWidget3D(QtWidgets.QWidget):

    def __init__(self, *args, **kwargs):

        super(MplWidget3D, self).__init__(*args, **kwargs)

        # This is needed to enable any key
        # events in the first place
        self.setFocusPolicy(QtCore.Qt.StrongFocus)

        self.sc = MplCanvas3D(self, width=5, height=4, dpi=100)

        # Create toolbar, passing canvas as first parament, parent (self, the MainWindow) as second.
        toolbar = NavigationToolbar(self.sc, self)

        layout = QtWidgets.QVBoxLayout()
        layout.setContentsMargins(0,0,0,0)
        layout.addWidget(toolbar)
        layout.addWidget(self.sc)

        self.setLayout(layout)

        #
        # The object attributes
        self.selected_stuff = None

    def plot(self, x, y, z, *args, **kwargs):

        self.x = x
        self.y = y
        self.z = z
        self.ploted_stuff = self.sc.axes.plot(x, y, z, *args, **kwargs)

    def scatter(self, x, y, z, *args, **kwargs):

        self.x = x
        self.y = y
        self.z = z
        self.ploted_stuff = self.sc.axes.scatter(x, y, z, *args, **kwargs)

    def select_indices(self, idx):
        print('Selecting indices', idx)

        if self.selected_stuff is not None:
            for s in self.selected_stuff:
                self.selected_stuff.pop().remove()
        self.selected_stuff = self.sc.axes.plot(self.x.iloc[idx], self.y.iloc[idx], self.z.iloc[idx], 'ro')
        self.sc.draw()


class MplInteractiveWidget(MplWidget):

    selectionChanged = pyqtSignal(object)

    def __init__(self, *args, **kwargs):

        super(MplInteractiveWidget, self).__init__(*args, **kwargs)

        # drawtype is 'box' or 'line' or 'none'
        self.RS = RectangleSelector(self.sc.axes, self.range_select_callback,
                                    drawtype='box', useblit=True,
                                    button=[1],  # don't use middle button
                                    minspanx=5, minspany=5,
                                    spancoords='pixels',
                                    interactive=True)

    def select_range(self, r):

        if self.ploted_stuff is None:
            return

        idx = np.where((self.x > r[0]) & (self.x < r[1]) & (self.y > r[2]) & (self.y < r[3]))[0]
        print(idx)
        self.select_indices(idx)
        self.selectionChanged.emit(idx)

    def range_select_callback(self, eclick, erelease) -> None:
        'eclick and erelease are the press and release events'
        # x1, y1 = eclick.xdata, eclick.ydata
        # x2, y2 = erelease.xdata, erelease.ydata
        # print("(%3.2f, %3.2f) --> (%3.2f, %3.2f)" % (x1, y1, x2, y2))
        # print(" The button you used were: %s %s" % (eclick.button, erelease.button))
        print(self.RS.extents)
        self.select_range(self.RS.extents)

    def toggle_selector(self, event) -> None:
        print(' Key pressed.', event.key())

        if event.key() in [QtCore.Qt.Key_Q] and self.RS.active:
            print(' RectangleSelector deactivated.')
            self.RS.set_active(False)

        if event.key() in [QtCore.Qt.Key_A] and not self.RS.active:
            print(' RectangleSelector activated.')
            self.RS.set_active(True)

