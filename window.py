import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton

def create_pyqt_window():
    app = QApplication(sys.argv)
    window = QMainWindow()
    window.setWindowTitle("")
    window.setGeometry(100, 100, 400, 300)
    window.show()
    close_button = QPushButton("Close Window", window)
    close_button.clicked.connect(sys.exit())
    sys.exit(app.exec_())

if __name__ == "__main__":
    create_pyqt_window()
