/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/



/**
  @file PreferencesDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */



#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/tools/QtHelpers.hpp>

#include <openfluid/ui/config.hpp>

#include "ui_PreferencesDialog.h"
#include <openfluid/ui/common/PreferencesDialog.hpp>
#include "ui_WaresSearchPathsWidget.h"
#include <openfluid/ui/common/WaresSearchPathsWidget.hpp>
#include <openfluid/ui/common/EditMarketplaceDialog.hpp>
#include <openfluid/ui/common/DetectQtDevToolsDialog.hpp>

#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>


namespace openfluid { namespace ui { namespace common {


PreferencesDialog::PreferencesDialog(QWidget* Parent, DisplayMode Mode, const QStringList& ExtsPaths):
  OpenFLUIDDialog(Parent), ui(new Ui::PreferencesDialog),
  m_RecentsChanged(false),
  m_SimPathsChanged(false), m_ObsPathsChanged(false), m_WaresWatchingChanged(false), m_TextEditorSettingsChanged(false),
  m_OriginalLangIndex(0), m_Mode(Mode)
{
  setWindowModality(Qt::ApplicationModal);

  ui->setupUi(this);

  ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                  .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));

  ui->AddMarketPlaceButton->setText("");
  ui->AddMarketPlaceButton->setIcon(QIcon(":/ui/common/icons/add.png"));
  ui->AddMarketPlaceButton->setIconSize(QSize(20,20));

  ui->EditMarketPlaceButton->setText("");
  ui->EditMarketPlaceButton->setIcon(QIcon(":/ui/common/icons/modify.png"));
  ui->EditMarketPlaceButton->setIconSize(QSize(20,20));

  ui->RemoveMarketPlaceButton->setText("");
  ui->RemoveMarketPlaceButton->setIcon(QIcon(":/ui/common/icons/remove.png"));
  ui->RemoveMarketPlaceButton->setIconSize(QSize(20,20));

  QTreeWidgetItem *PrefItem;

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Environment"));
  PrefItem->setData(0,Qt::UserRole,ENVIRONMENT_PAGE);

  if (m_Mode == MODE_BUILDER || m_Mode == MODE_FULL)
  {
    PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
    PrefItem->setText(0,tr("Interface"));
    PrefItem->setData(0,Qt::UserRole,BUILDER_PAGE);

    PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
    PrefItem->setText(0,tr("Simulations"));
    PrefItem->setData(0,Qt::UserRole,SIMULATION_PAGE);
  }

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Development tools"));
  PrefItem->setData(0,Qt::UserRole,DEVENV_PAGE);

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Code editor"));
  PrefItem->setData(0,Qt::UserRole,DEVEDITOR_PAGE);

#if OPENFLUID_MARKET_ENABLED
  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Market"));
  PrefItem->setData(0,Qt::UserRole,MARKET_PAGE);
#endif


  ui->WorkspacesPathsWidget->setAllowEmpty(false);
  ui->WaresPathsTabWidget->setCurrentIndex(0);

  if (!(m_Mode == MODE_BUILDER || m_Mode == MODE_FULL))
    ui->WaresPathsTabWidget->removeTab(2);


  initialize(ExtsPaths);

  connect(ui->LangComboBox,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(updateLanguage(const QString&)));
  connect(ui->RecentMaxSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateRecentsMax(int)));
  connect(ui->ClearRecentsButton,SIGNAL(clicked()),this,SLOT(clearRecentsList()));
  connect(ui->AutoSaveCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableAutoSaveBeforeRun(bool)));
  connect(ui->ItemRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmItemRemoval(bool)));
  connect(ui->ParamRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmParamRemoval(bool)));
  connect(ui->WatchCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableWatchers(bool)));

  connect(ui->UnitsRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmUnitsRemoval(bool)));
  connect(ui->ConnectionsRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmConnectionsRemoval(bool)));
  connect(ui->AttributesRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmAttributesRemoval(bool)));


  connect(ui->WorkspacesPathsWidget,SIGNAL(pathsUpdated()),this,SLOT(processWorkspacesPathsUpdate()));

  connect(ui->SimulatorsSearchPathsWidget,SIGNAL(userPathsUpdated()),this,SLOT(processSimUserPathsUpdate()));
  connect(ui->ObserversSearchPathsWidget,SIGNAL(userPathsUpdated()),this,SLOT(processObsUserPathsUpdate()));
  connect(ui->BuilderextsSearchPathsWidget,SIGNAL(userPathsUpdated()),this,SLOT(processBextUserPathsUpdate()));


  connect(ui->DeltaTSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateDeltaT(int)));
  connect(ui->BeginDateEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(updatePeriodBegin(const QDateTime&)));
  connect(ui->EndDateEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(updatePeriodEnd(const QDateTime&)));

  connect(ui->SyntaxHLCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableSyntaxHighlighting(bool)));
  connect(ui->CurrentLineHLCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableCurrentLineHighlighting(bool)));
  connect(ui->CurrentLineColorButton, SIGNAL(clicked()), this, SLOT(changeCurrentLineColor()));
  connect(ui->FontComboBox, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(changeCurrentFont(const QFont&)));
  connect(ui->LineWrappingCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableLineWrapping(bool)));
  connect(ui->IndentSpaceNbSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeIndentSpaceNumber(int)));
  connect(ui->TextEditorApplyButton, SIGNAL(clicked()), this, SLOT(applyTextEditorSettings()));

  connect(ui->AddMarketPlaceButton,SIGNAL(clicked()),this,SLOT(addMarketPlace()));
  connect(ui->EditMarketPlaceButton,SIGNAL(clicked()),this,SLOT(editMarketPlace()));
  connect(ui->RemoveMarketPlaceButton,SIGNAL(clicked()),this,SLOT(removeMarketPlace()));


  connect(ui->ConfigPathEnvEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateDevConfigPATH()));
  connect(ui->ConfigGeneratorEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateDevConfigGenerator()));
  connect(ui->ConfigOptionsEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateDevConfigOptions()));
  connect(ui->BuildPathEnvEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateDevBuildPATH()));
  connect(ui->ShowPathCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateDevShowPATH(bool)));

#if !defined(OPENFLUID_OS_WINDOWS)
  ui->DetectQtDevWidget->setVisible(false);
#else
  connect(ui->DetectQtDevButton,SIGNAL(clicked()),this,SLOT(detectQtDevToolsMinGW()));
#endif



  connect(ui->PrefsTreeWidget,
          SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
          this, SLOT(changePage(QTreeWidgetItem*,QTreeWidgetItem*)));

  ui->PrefsTreeWidget->setCurrentItem(ui->PrefsTreeWidget->topLevelItem(0));


  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


PreferencesDialog::~PreferencesDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::initialize(const QStringList& ExtsPaths)
{
  openfluid::base::PreferencesManager* PrefsMan =
    openfluid::base::PreferencesManager::instance();

  openfluid::base::RuntimeEnvironment* RunEnv =
      openfluid::base::RuntimeEnvironment::instance();


  // Interface
  // TODO set up a fancier languages list
  ui->LangComboBox->addItem("default");
  ui->LangComboBox->addItems(openfluid::base::PreferencesManager::getAvailableLangs());
  ui->LangComboBox->setCurrentIndex(ui->LangComboBox->findText(PrefsMan->getLang()));

  m_OriginalLangIndex = ui->LangComboBox->currentIndex();

  ui->LangRestartLabel->setText("");


  // Workspaces paths
  ui->WorkspacesPathsWidget->setPathsList(PrefsMan->getWorkspacesPaths());


  if (m_Mode == MODE_BUILDER || m_Mode == MODE_FULL)
  {
    // Wares search paths
    ui->SimulatorsSearchPathsWidget->initialize(PrefsMan->getExtraSimulatorsPaths(),
                                                openfluid::tools::toQStringList(
                                                    RunEnv->getDefaultSimulatorsPluginsPaths()));
    ui->ObserversSearchPathsWidget->initialize(PrefsMan->getExtraObserversPaths(),
                                               openfluid::tools::toQStringList(
                                                   RunEnv->getDefaultObserversPluginsPaths()));

    // interface
    ui->RecentMaxSpinBox->setValue(PrefsMan->getRecentMax());

    ui->AutoSaveCheckBox->setChecked(PrefsMan->isAutomaticSaveBeforeRun());

    ui->ItemRemovalCheckBox->setChecked(PrefsMan->isItemRemovalConfirm());
    ui->ParamRemovalCheckBox->setChecked(PrefsMan->isParamRemovalConfirm());
    ui->WatchCheckBox->setChecked(PrefsMan->isWaresWatchersActive());

    ui->UnitsRemovalCheckBox->setChecked(PrefsMan->isSpatialUnitsRemovalConfirm());
    ui->ConnectionsRemovalCheckBox->setChecked(PrefsMan->isSpatialConnsRemovalConfirm());
    ui->AttributesRemovalCheckBox->setChecked(PrefsMan->isSpatialAttrsRemovalConfirm());


    // Simulations
    ui->DeltaTSpinBox->setValue(PrefsMan->getDeltaT());

    ui->BeginDateEdit->setDateTime(QDateTime::fromString(PrefsMan->getBegin(),"yyyy-MM-dd HH:mm:ss"));
    ui->EndDateEdit->setDateTime(QDateTime::fromString(PrefsMan->getEnd(),"yyyy-MM-dd HH:mm:ss"));


    ui->BuilderextsSearchPathsWidget->initialize(PrefsMan->getExtraExtensionsPaths(),ExtsPaths);
  }

  // Development tools
  ui->ConfigPathEnvEdit->setText(PrefsMan->getWaresdevConfigEnv("PATH"));
  ui->ConfigGeneratorEdit->setText(PrefsMan->getWaresdevConfigGenerator());
  ui->ConfigOptionsEdit->setText(PrefsMan->getWaresdevConfigOptions());
  ui->BuildPathEnvEdit->setText(PrefsMan->getWaresdevBuildEnv("PATH"));
  ui->ShowPathCheckBox->setChecked(PrefsMan->isWaresdevShowCommandEnv("PATH"));

  // Code editor
  ui->SyntaxHLCheckBox->setChecked(PrefsMan->isSyntaxHighlightingEnabled());
  ui->CurrentLineHLCheckBox->setChecked(PrefsMan->isCurrentlineHighlightingEnabled());
  QString Color = PrefsMan->getCurrentlineColor();
  if(QColor::isValidColor(Color))
    ui->CurrentLineColorLabel->setStyleSheet(QString("QLabel {background-color : %1}").arg(Color));
  QString FontName = PrefsMan->getFontName();
  ui->FontLabel->setStyleSheet(QString("QLabel {font-family : %1}").arg(FontName));
  ui->FontComboBox->setCurrentFont(QFont(FontName));
  ui->LineWrappingCheckBox->setChecked(PrefsMan->isLineWrappingEnabled());
  ui->IndentSpaceNbSpinBox->setValue(PrefsMan->getIndentSpaceNb());


#if OPENFLUID_MARKET_ENABLED
  // Market
  updateMarketplacesList();
#endif
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changePage(QTreeWidgetItem* Current, QTreeWidgetItem* Previous)
{
  if (!Current) Current = Previous;
  ui->PrefsStackedWidget->setCurrentIndex(Current->data(0,Qt::UserRole).toInt());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateLanguage(const QString& Lang)
{
  openfluid::base::PreferencesManager::instance()->setLang(Lang);

  if (ui->LangComboBox->currentIndex() != m_OriginalLangIndex)
    ui->LangRestartLabel->setText(tr("Restart required"));
  else
    ui->LangRestartLabel->setText("");
}


// =====================================================================
// =====================================================================


void PreferencesDialog::clearRecentsList()
{
  openfluid::base::PreferencesManager::instance()->clearRecentProjects();
  m_RecentsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateRecentsMax(int Val)
{
  openfluid::base::PreferencesManager::instance()->setRecentMax(Val);
  m_RecentsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmItemRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setItemRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmParamRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setParamRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmUnitsRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setSpatialUnitsRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmConnectionsRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setSpatialConnsRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmAttributesRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setSpatialAttrsRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableWatchers(bool Active)
{
  openfluid::base::PreferencesManager::instance()->setWaresWatchersActive(Active);
  m_WaresWatchingChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableAutoSaveBeforeRun(bool AutoSave)
{
  openfluid::base::PreferencesManager::instance()->setAutomaticSaveBeforeRun(AutoSave);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDeltaT(int Val)
{
  openfluid::base::PreferencesManager::instance()->setDeltaT(Val);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updatePeriodBegin(const QDateTime& DT)
{
  openfluid::base::PreferencesManager::instance()->setBegin(DT.toString("yyyy-MM-dd HH:mm:ss"));
  if (ui->EndDateEdit->dateTime() <= DT)
  {
    ui->EndDateEdit->setDateTime(DT.addSecs(1));
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updatePeriodEnd(const QDateTime& DT)
{
  openfluid::base::PreferencesManager::instance()->setEnd(DT.toString("yyyy-MM-dd HH:mm:ss"));
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableSyntaxHighlighting(bool Enable)
{
  openfluid::base::PreferencesManager::instance()->setSyntaxHighlightingEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}

// =====================================================================
// =====================================================================


void PreferencesDialog::enableCurrentLineHighlighting(bool Enable)
{
  openfluid::base::PreferencesManager::instance()->setCurrentlineHighlightingEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeCurrentLineColor()
{
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();

  QColor Color = QColorDialog::getColor(PrefMgr->getCurrentlineColor(),this);

  // Cancel on QColorDialog returns an invalid Color
  if(Color.isValid())
  {
    QString ColorName = Color.name();

    ui->CurrentLineColorLabel->setStyleSheet(QString("QLabel {background-color : %1}").arg(ColorName));

    PrefMgr->setCurrentlineColor(ColorName);

    m_TextEditorSettingsChanged = true;
  }

}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeCurrentFont(const QFont& Font)
{
  QString FontName = Font.family();

  ui->FontLabel->setStyleSheet(QString("QLabel {font-family : %1}").arg(FontName));

  openfluid::base::PreferencesManager::instance()->setFontName(FontName);

  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableLineWrapping(bool Enable)
{
  openfluid::base::PreferencesManager::instance()->setLineWrappingEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeIndentSpaceNumber(int SpaceNb)
{
  openfluid::base::PreferencesManager::instance()->setIndentSpaceNb(SpaceNb);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::addMarketPlace()
{
  EditMarketplaceDialog MarketDlg(this,"","",
                                  openfluid::base::PreferencesManager::instance()->getMarketplaces());

  if (MarketDlg.exec() == QDialog::Accepted)
  {
    openfluid::base::PreferencesManager::instance()->addMarketplace(MarketDlg.getName(),MarketDlg.getURL());
    updateMarketplacesList();
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::editMarketPlace()
{
  if (ui->MarketPlacesListWidget->currentRow()>=0)
  {
    QListWidgetItem* Item = ui->MarketPlacesListWidget->currentItem();
    QStringList AssociatedData = Item->data(Qt::UserRole).toStringList();

    EditMarketplaceDialog MarketDlg(this,AssociatedData[0],AssociatedData[1],
                                    openfluid::base::PreferencesManager::instance()->getMarketplaces());

    if (MarketDlg.exec() == QDialog::Accepted)
    {
      openfluid::base::PreferencesManager::instance()->removeMarketplace(MarketDlg.getOriginalName());
      openfluid::base::PreferencesManager::instance()->addMarketplace(MarketDlg.getName(),MarketDlg.getURL());
      updateMarketplacesList();
    }
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::removeMarketPlace()
{
  if (ui->MarketPlacesListWidget->currentRow()>=0)
  {
    QListWidgetItem* Item = ui->MarketPlacesListWidget->currentItem();
    QStringList AssociatedData = Item->data(Qt::UserRole).toStringList();

    openfluid::base::PreferencesManager::instance()->removeMarketplace(AssociatedData[0]);

    updateMarketplacesList();
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateMarketplacesList()
{
  openfluid::base::PreferencesManager* PrefsMan =
    openfluid::base::PreferencesManager::instance();

  openfluid::base::PreferencesManager::MarketPlaces_t MPlaces = PrefsMan->getMarketplaces();

  openfluid::base::PreferencesManager::MarketPlaces_t::iterator MPit;
  openfluid::base::PreferencesManager::MarketPlaces_t::iterator MPitb = MPlaces.begin();
  openfluid::base::PreferencesManager::MarketPlaces_t::iterator MPite = MPlaces.end();


  // clear the list
  while(ui->MarketPlacesListWidget->count()>0)
  {
    delete ui->MarketPlacesListWidget->takeItem(0);
  }

  // populate the list
  for (MPit=MPitb;MPit!=MPite;++MPit)
  {
    QListWidgetItem* Item = new QListWidgetItem((*MPit).first+ " (" + (*MPit).second+")",ui->MarketPlacesListWidget);
    QStringList AssociatedData;
    AssociatedData << (*MPit).first << (*MPit).second;
    Item->setData(Qt::UserRole,AssociatedData);
  }

  ui->MarketPlacesListWidget->sortItems();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processSimUserPathsUpdate()
{
  openfluid::base::PreferencesManager::instance()
      ->setExtraSimulatorsPaths(ui->SimulatorsSearchPathsWidget->getUserPaths());
  m_SimPathsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processObsUserPathsUpdate()
{
  openfluid::base::PreferencesManager::instance()
      ->setExtraObserversPaths(ui->ObserversSearchPathsWidget->getUserPaths());
  m_ObsPathsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processBextUserPathsUpdate()
{
  openfluid::base::PreferencesManager::instance()
      ->setExtraExtensionsPaths(ui->BuilderextsSearchPathsWidget->getUserPaths());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processWorkspacesPathsUpdate()
{
  openfluid::base::PreferencesManager::instance()->setWorkspacesPaths(ui->WorkspacesPathsWidget->getPathsList());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevConfigPATH()
{
  openfluid::base::PreferencesManager::instance()->setWaresdevConfigEnv("PATH",ui->ConfigPathEnvEdit->text());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevConfigGenerator()
{
  openfluid::base::PreferencesManager::instance()->setWaresdevConfigGenerator(ui->ConfigGeneratorEdit->text());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevConfigOptions()
{
  openfluid::base::PreferencesManager::instance()->setWaresdevConfigOptions(ui->ConfigOptionsEdit->text());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevBuildPATH()
{
  openfluid::base::PreferencesManager::instance()->setWaresdevBuildEnv("PATH",ui->BuildPathEnvEdit->text());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevShowPATH(bool Enabled)
{
  openfluid::base::PreferencesManager::instance()->setWaresdevShowCommandEnv("PATH",Enabled);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::detectQtDevToolsMinGW()
{
#if defined(OPENFLUID_OS_WINDOWS)
  DetectQtDevToolsDialog Dlg(this);

  if (Dlg.exec() == QDialog::Accepted)
  {
    ui->ConfigPathEnvEdit->setText(QDir::toNativeSeparators(Dlg.getFrameworkPath())+";"+
                                   QDir::toNativeSeparators(Dlg.getDevToolsPath())+";%%PATH%%");
    ui->ConfigGeneratorEdit->setText("MinGW Makefiles");

    updateDevConfigGenerator();
    updateDevConfigPATH();
  }
#endif
}


// =====================================================================
// =====================================================================


void PreferencesDialog::applyTextEditorSettings()
{
  if(m_TextEditorSettingsChanged)
    emit applyTextEditorSettingsAsked();
}


} } } // namespaces
