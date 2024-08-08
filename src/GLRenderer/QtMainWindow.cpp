#include "QtMainWindow.h"
#include "QtViewer.h"

namespace glr {
struct QtMainWindow::Data {
    QtViewer* viewer;
};

QtMainWindow::QtMainWindow()
  : d(new Data) {
    auto viewer = new QtViewer();
    setCentralWidget(viewer);
    d->viewer = viewer;
}

QtMainWindow::~QtMainWindow() {
    delete d;
}

QtViewer* QtMainWindow::getViewer() const {
    return d->viewer;
}
} // namespace glr