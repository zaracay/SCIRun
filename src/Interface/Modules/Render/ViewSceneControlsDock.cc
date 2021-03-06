/*
For more information, please see: http://software.sci.utah.edu

The MIT License

Copyright (c) 2015 Scientific Computing and Imaging Institute,
University of Utah.

License for the specific language governing rights and limitations under
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include <Interface/Modules/Render/ViewScenePlatformCompatibility.h>
#include <Interface/Modules/Render/ViewSceneControlsDock.h>
#include <Core/Application/Preferences/Preferences.h>
#include <Core/Logging/Log.h>

using namespace SCIRun::Gui;
using namespace SCIRun::Render;


ViewSceneControlsDock::ViewSceneControlsDock(const QString& name, ViewSceneDialog* parent) : QDockWidget(parent)
{
  setupUi(this);

  setHidden(true);
  setVisible(false);
  setEnabled(false);
  setWindowTitle(name);
  setAllowedAreas(Qt::BottomDockWidgetArea);
  setFloating(true);
  setStyleSheet(parent->styleSheet());
  
  setupObjectListWidget();

  if (SCIRun::Core::Preferences::Instance().useNewViewSceneMouseControls)
  {
    mouseControlComboBox_->setCurrentIndex(1);
  }
  else
  {
    mouseControlComboBox_->setCurrentIndex(0);
  }
  
  invertZoomCheckBox_->setChecked(SCIRun::Core::Preferences::Instance().invertMouseZoom);

  updateZoomOptionVisibility();

  connect(orientationCheckBox_, SIGNAL(clicked(bool)), parent, SLOT(showOrientationChecked(bool)));
  connect(saveScreenShotOnUpdateCheckBox_, SIGNAL(stateChanged(int)), parent, SLOT(saveNewGeometryChanged(int)));
  connect(mouseControlComboBox_, SIGNAL(currentIndexChanged(int)), parent, SLOT(menuMouseControlChanged(int)));
  connect(setBackgroundColorPushButton_, SIGNAL(clicked()), parent, SLOT(assignBackgroundColor()));
  connect(contSortRadioButton_, SIGNAL(clicked(bool)), parent, SLOT(setTransparencySortTypeContinuous(bool)));
  connect(updateSortRadioButton_, SIGNAL(clicked(bool)), parent, SLOT(setTransparencySortTypeUpdate(bool)));
  connect(listSortRadioButton_, SIGNAL(clicked(bool)), parent, SLOT(setTransparencySortTypeLists(bool)));
  connect(selectAllPushButton_, SIGNAL(clicked()), parent, SLOT(selectAllClicked()));
  connect(deselectAllPushButton_, SIGNAL(clicked()), parent, SLOT(deselectAllClicked()));
  connect(invertZoomCheckBox_, SIGNAL(clicked(bool)), parent, SLOT(invertZoomClicked(bool)));
  connect(zoomSpeedHorizontalSlider_, SIGNAL(valueChanged(int)), parent, SLOT(adjustZoomSpeed(int)));

  connect(objectListWidget_, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotChanged(QListWidgetItem*)));
  connect(this, SIGNAL(itemUnselected(const QString&)), parent, SLOT(handleUnselectedItem(const QString&)));
  connect(this, SIGNAL(itemSelected(const QString&)), parent, SLOT(handleSelectedItem(const QString&)));

  setSampleColor(Qt::black);

  WidgetStyleMixin::tabStyle(tabWidget);


  /////Set unused widgets to be not visible
  ////Clipping tab
  tabWidget->removeTab(2);
  //ClippingTab->setVisible(false);
  //ClippingTab->setEnabled(false);
  //groupBox_3->setVisible(false);
  //groupBox_4->setVisible(false);
  //groupBox_5->setVisible(false);

  ///Object Tab
  tabWidget->setCurrentIndex(0);

  ////View Tab
  //groupBox->setVisible(false);

  ////Render Tab
  //groupBox_6->setVisible(false);

}

void ViewSceneControlsDock::setSampleColor(const QColor& color)
{
  QString styleSheet = "QLabel{ background: rgb(" + QString::number(color.red()) + "," +
    QString::number(color.green()) + "," + QString::number(color.blue()) + "); }";

  currentBackgroundLabel_->setStyleSheet(styleSheet);
}

void ViewSceneControlsDock::updateZoomOptionVisibility()
{
  if (SCIRun::Core::Preferences::Instance().useNewViewSceneMouseControls)
  {
    zoomBox_->setVisible(true);
    invertZoomCheckBox_->setVisible(true);
  }
  else
  {
    zoomBox_->setVisible(false);
    invertZoomCheckBox_->setVisible(false);
  }
}

void ViewSceneControlsDock::addItem(const QString& name, bool checked)
{
  auto items = objectListWidget_->findItems(name, Qt::MatchExactly);
  if (items.count() > 0)
  {
    return;
  }

  QListWidgetItem* item = new QListWidgetItem(name, objectListWidget_);

  if (checked) 
    item->setCheckState(Qt::Checked);
  else 
    item->setCheckState(Qt::Unchecked);

  objectListWidget_->addItem(item);
}

void ViewSceneControlsDock::removeItem(const QString& name)
{
  auto items = objectListWidget_->findItems(name, Qt::MatchExactly);
  Q_FOREACH(QListWidgetItem* item, items)
  {
    objectListWidget_->removeItemWidget(item);
  }
}

void ViewSceneControlsDock::removeAllItems()
{
  if (objectListWidget_->count() > 1)
  {
    LOG_DEBUG("ViewScene items cleared" << std::endl);
    objectListWidget_->clear();
  }
}

void ViewSceneControlsDock::slotChanged(QListWidgetItem* item)
{
  if (item->checkState() == Qt::Unchecked)
  {
    LOG_DEBUG("Item " << item->text().toStdString() << " Unchecked!" << std::endl);
    Q_EMIT itemUnselected(item->text());
  }
  else if (item->checkState() == Qt::Checked)
  {
    LOG_DEBUG("Item " << item->text().toStdString() << " Checked!" << std::endl);
    Q_EMIT itemSelected(item->text());
  }
}

class FixMacCheckBoxes : public QStyledItemDelegate
{
public:
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
  {
    QStyleOptionViewItem& refToNonConstOption = const_cast<QStyleOptionViewItem&>(option);
    refToNonConstOption.showDecorationSelected = false;
    QStyledItemDelegate::paint(painter, refToNonConstOption, index);
  }
};

void ViewSceneControlsDock::setupObjectListWidget()
{
  objectListWidget_->setItemDelegate(new FixMacCheckBoxes);
}