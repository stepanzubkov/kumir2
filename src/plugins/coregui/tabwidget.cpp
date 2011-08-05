#include "tabwidget.h"
#include "tabbar.h"

namespace CoreGUI {

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    TabBar * tb = new TabBar(this);
    setTabBar(tb);
}

void TabWidget::disableTabs()
{
    tabBar()->setEnabled(false);
    tabBar()->setVisible(false);
}

} // namespace CoreGUI
