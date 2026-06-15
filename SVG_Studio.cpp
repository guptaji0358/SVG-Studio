/*
Project - SVG Studio
---------------------
*/

#include<QApplication>
#include<QMainWindow>
#include<QMenuBar>
#include<QMenu>
#include<QKeySequence>
#include<QStatusBar>
#include<QFileDialog>
#include<QString>
#include<QMessageBox>
#include<QWidget>
#include<QLabel>
#include<QFileInfo>
#include<QVBoxLayout>
#include<QPushButton>
#include<QSvgWidget>
#include<QTabWidget>
#include<QTabBar>
#include<QInputDialog>
#include<QMouseEvent>
#include<QDragEnterEvent>
#include<QDropEvent>
#include<QMimeData>
#include<QUrl>
#include<QShortcut>
#include<QStack>
#include<QHBoxLayout>
#include<QFrame>
#include<string>
#include<iostream>
#include<QIcon>
#include<QDir>
#include<QSplitter>
#include<QPlainTextEdit>
#include<QDialog>
#include<QCheckBox>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QHeaderView>
#include<QGroupBox>
#include<QRadioButton>
#include<QButtonGroup>

// Paths Collection class
class FilePaths {
public:
                    static inline const QString newFileIconPath =":/NEW_FILE_ICON.svg";
                    static inline const QString openFileIconPath =":/OPEN_FILE_ICON.svg";
                    static inline const QString openFolderIconPath =":/OPEN_FOLDER_ICON.svg";
                    static inline const QString convertToSvgIconPath = ":/CONVERT_TO_SVG.svg";
                    static inline const QString editButtonIconPath = ":/EDIT_BUTTON_ICON.svg";
                    static inline const QString downArrowIconPath = ":/DOWN_ARROW_ICON.svg";
                    static inline const QString rightArrowIconPath = ":/RIGHT_ARROW_ICON.svg";
                };

class SVGStudioShortcutEditDialog : public QDialog {
private:
    QLabel *actionLabel;
    QLineEdit *shortcutEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;

public:
    SVGStudioShortcutEditDialog(QString action,QString shortcut,QWidget *parent = nullptr): QDialog(parent) {
                                                                                                                setWindowTitle("Edit Shortcut");
                                                                                                                resize(400,150);
                                                                                                                actionLabel = new QLabel(
                                                                                                                                            QString("Action : %1").arg(action)
                                                                                                                                        );

                                                                                                                shortcutEdit = new QLineEdit;
                                                                                                                shortcutEdit->setText(shortcut);

                                                                                                                saveButton = new QPushButton("Save");
                                                                                                                cancelButton = new QPushButton("Cancel");

                                                                                                                QHBoxLayout *buttonLayout;
                                                                                                                buttonLayout = new QHBoxLayout;

                                                                                                                buttonLayout->addStretch();
                                                                                                                buttonLayout->addWidget(saveButton);
                                                                                                                buttonLayout->addWidget(cancelButton);

                                                                                                                QVBoxLayout *mainLayout;
                                                                                                                mainLayout = new QVBoxLayout;

                                                                                                                mainLayout->addWidget(actionLabel);
                                                                                                                mainLayout->addWidget(shortcutEdit);
                                                                                                                mainLayout->addLayout(buttonLayout);

                                                                                                                setLayout(mainLayout);

                                                                                                                connect(
                                                                                                                            saveButton,
                                                                                                                            &QPushButton::clicked,
                                                                                                                            this,
                                                                                                                            &QDialog::accept
                                                                                                                        );

                                                                                                                connect(
                                                                                                                            cancelButton,
                                                                                                                            &QPushButton::clicked,
                                                                                                                            this,
                                                                                                                            &QDialog::reject
                                                                                                                        );
                                                                                                            }

    QString getShortcut() {
                            return shortcutEdit->text();
                        }
};

// Automate Shortcut Class
class Automate {
public:
    static void CreateShortcutRow(
                                    QTableWidget *table,
                                    QString number,
                                    QString shortcut,
                                    QString action
                                ) {
                                        int row;
                                        row = table->rowCount();
                                        table->insertRow(row);
                                        table->setItem(row,0,new QTableWidgetItem(number));
                                        table->setItem(row,1,new QTableWidgetItem(shortcut));
                                        table->setItem(row,2,new QTableWidgetItem(action));
                                        
                                        QPushButton *editButton;
                                        editButton = new QPushButton("Edit");
                                        editButton->setIcon(QIcon(FilePaths::editButtonIconPath));
                                        editButton->setIconSize(QSize(20,20));
                                        editButton->setCursor(Qt::PointingHandCursor);
                                        editButton->setToolTip("Edit Shortcut");
                                        QObject::connect(editButton,&QPushButton::clicked,table,[=]() {
                                                                                                            SVGStudioShortcutEditDialog dialog(
                                                                                                                                                    action,
                                                                                                                                                    shortcut
                                                                                                                                                );
                                                                                                            dialog.exec();
                                                                                                        }
                );
                                                                        
                                        table->setCellWidget(row,3,editButton);
                                    }
};

class Style {
public:
    static QString welcomeToTheSvgStudioLabelStyle() {
                                                            return
                                                            "font-size:32px;"
                                                            "font-weight:bold";
                                                        }

    static QString welcomePageStartLabelStyle() {
                                                return
                                                "font-size:14px;";
                                            }
    
    static QString welcomeButtonStyle() {
                                            return 
                                            R"(
                                                QPushButton {
                                                                background-color:none;
                                                                color:white;
                                                                border:0px;
                                                                border-radius:10px;
                                                                padding:12px;
                                                                font-size:14px;
                                                                text-align:left;
                                                                font-weight:bold;
                                                            }

                                                QPushButton:hover {
                                                                        background-color:none;
                                                                        border:1px solid #00FFFF;
                                                                    }

                                                QPushButton:pressed {
                                                                        background-color:none;
                                                                        color:#00FFFF;
                                                                    }
                                            )";
                                        }

    static QString DropOverlayLabelStyle() {
                                return
                                R"(
                                        QLabel {
                                                    color:white;
                                                    font-size:32px;
                                                    font-weight:bold;
                                                }
                                    )";
                            }
    
    static QString dragOverlayStyle() {
                                            return 
                                            R"(
                                                    QFrame {
                                                                background-color: rgba(0,0,0,180);
                                                                border: 3px dashed #00FFFF;
                                                            }
                                                )";
                                        }
};


class SVGStudioMessages {
public:
    static void invalidPath(QWidget *parent) {
                                                    QMessageBox::warning(
                                                                            parent,
                                                                            "Invalid Path",
                                                                            "Path does not exist. Retry."
                                                                        );
                                                }

    static void Success(QWidget *parent) {
                                                QMessageBox::information(
                                                                            parent,
                                                                            "Success",
                                                                            "Path Successfully Added"
                                                                        );
                                            }

    static QMessageBox::StandardButton confirmtoCreatePath(QWidget *parent) {
                                                                                return QMessageBox::question(
                                                                                                                parent,
                                                                                                                "Folder Not Found",
                                                                                                                "Path does not exist.\nCreate it?",
                                                                                                                QMessageBox::Yes | QMessageBox::No
                                                                                                            );
                                                                            }

    static QMessageBox::StandardButton ConfirmCancel(QWidget *parent) {
                                                                            return QMessageBox::question(
                                                                                                            parent,
                                                                                                            "Confirm Cancel",
                                                                                                            "Do You Really Want To Cancel This Process?",
                                                                                                            QMessageBox::Yes | QMessageBox::No
                                                                                                        );
                                                                        }
};

class SVGStudioEditorTab : public QWidget {
private:
    QSplitter *splitter;
    QPlainTextEdit *codeEditor;
    QSvgWidget *previewWidget;

public:
    SVGStudioEditorTab(QWidget *parent = nullptr):QWidget(parent) {
        QVBoxLayout *layout;
        layout = new QVBoxLayout;

        splitter = new QSplitter;
        codeEditor = new QPlainTextEdit;
        previewWidget = new QSvgWidget;

        splitter->setOrientation(Qt::Horizontal);
        splitter->addWidget(codeEditor);
        splitter->addWidget(previewWidget);

        layout->addWidget(splitter);
        setLayout(layout);
    }

    QPlainTextEdit* getEditor() {
                                    return codeEditor;
                                }

    QSvgWidget* getPreview() {
                                return previewWidget;
                            }

};

class SVGStudioLogic {
private:
    QStringList currentFilePath;

public:
    QStringList openSVG(QWidget *parent) {
                                            return QFileDialog::getOpenFileNames(
                                                                                    parent,
                                                                                    "Open SVG",
                                                                                    "",
                                                                                    "SVG Files (*.svg)"
                                                                                );
                                        }

    QString openFolder(QWidget *parent) {
                                            return QFileDialog::getExistingDirectory(
                                                                                        parent,
                                                                                        "Open Folder"
                                                                                    );
                                        }

    static bool IsAbsolutePath(QString path) {
                                                QFileInfo info(path);
                                                return info.isAbsolute();
                                            }
};

class SVGStudioButtonLogic {
private:
    SVGStudioLogic logic;

public:
    void openFileButtonLogic(QWidget *parent,QTabWidget *tabWidget) {
                                                                        QStringList filePaths;
                                                                        filePaths = logic.openSVG(parent);
                                                                        for (QString filePath:filePaths) {
                                                                                                                SVGStudioEditorTab *svgWidget;
                                                                                                                svgWidget = new SVGStudioEditorTab;
                                                                                                                svgWidget->getPreview()->load(filePath);
                                                                                                                tabWidget->addTab(svgWidget,QFileInfo(filePath).fileName());
                                                                                                                tabWidget->setTabToolTip(tabWidget->indexOf(svgWidget),filePath);
                                                                                                                tabWidget->setCurrentWidget(svgWidget);
                                                                                                            }
                                                                    }

    void newFileButtonLogic() {

    }

    void openFolderButtonLogic(QWidget *parent,QTabWidget *tabWidget) {
                                                                            QString folderPath;
                                                                            folderPath = logic.openFolder(parent);
                                                                            if(folderPath.isEmpty()) {
                                                                                                        return;
                                                                                                    }
                                                                            QDir folder(folderPath);
                                                                            QStringList svgFiles;
                                                                            svgFiles = folder.entryList({"*.svg"},QDir::Files);
                                                                            for (QString file:svgFiles) {
                                                                                                            QString fullpath;
                                                                                                            fullpath = folder.filePath(file);
                                                                                                            SVGStudioEditorTab *svgWidget;
                                                                                                            svgWidget = new SVGStudioEditorTab;
                                                                                                            svgWidget->getPreview()->load(fullpath);
                                                                                                            tabWidget->addTab(svgWidget,QFileInfo(fullpath).fileName());
                                                                                                            tabWidget->setTabToolTip(tabWidget->indexOf(svgWidget),fullpath);
                                                                                                            tabWidget->setCurrentWidget(svgWidget);
                                                                                                        }

                                                                        }

    void convertToSvgButtonLogic() {
        
    }
};

class SVGStudioSettingsDialogLogic : public QDialog {
private:
    QTabWidget *settingsTabs;
    QWidget *generalTab;
    QWidget *shortcutsTab;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QGroupBox *appearanceGroup;
    QRadioButton *darkThemeRadioButton;
    QRadioButton *lightThemeRadioButton;
    QRadioButton *robinThemeRadioButton;
    QPushButton *appearanceButton;
    QWidget *appearanceContainer;
    QWidget *customizeTab;
    QPushButton *addPathButton;
    QPushButton *removePathButton;
    QVBoxLayout *savedPathsLayout;
    QButtonGroup *pathGroup;

public:
    SVGStudioSettingsDialogLogic(QWidget *parent = nullptr):QDialog(parent) {
                                                                                    settingsTabs = new QTabWidget;
                                                                                    CreateGeneralTab();
                                                                                    CreateShortcutsTab();
                                                                                    CreateCustomiseTab();
                                                                                    connections(); 
                                                                                    QVBoxLayout *layout;
                                                                                    layout = new QVBoxLayout;
                                                                                    layout->addWidget(settingsTabs);
                                                                                    setLayout(layout);
                                                                                    resize(700,500);
                                                                                    setWindowTitle("Settings");
                                                                                }

    // Genneral Tab
    void CreateGeneralTab() {
                                generalTab = new QWidget;
                                QVBoxLayout *generalLayout;
                                generalLayout = new QVBoxLayout;
                                QCheckBox *openWelcomePageCheckBox;
                                appearanceContainer = new QWidget;
                                QVBoxLayout *appearanceLayout;
                                appearanceLayout = new QVBoxLayout;

                                openWelcomePageCheckBox = new QCheckBox("Open Welcome Page On Startup");

                                appearanceButton = new QPushButton("Appearnce");
                                appearanceButton->setIcon(QIcon(FilePaths::rightArrowIconPath));

                                darkThemeRadioButton = new QRadioButton("Dark");
                                lightThemeRadioButton = new QRadioButton("Light");
                                robinThemeRadioButton = new QRadioButton("Robin Theme");

                                darkThemeRadioButton->setChecked(true);

                                generalLayout->addWidget(openWelcomePageCheckBox);
                                generalLayout->addSpacing(20);

                                appearanceLayout->addWidget(darkThemeRadioButton);
                                appearanceLayout->addWidget(lightThemeRadioButton);
                                appearanceLayout->addWidget(robinThemeRadioButton);
                                appearanceContainer->setLayout(appearanceLayout);

                                generalLayout->addWidget(appearanceButton);
                                generalLayout->addWidget(appearanceContainer);
                                appearanceContainer->hide();
                                generalLayout->addStretch();
                                generalTab->setLayout(generalLayout);
                                settingsTabs->addTab(generalTab,"General");
                            }

    // Create Shortcut Tab In Settings
    void CreateShortcutsTab() {
                                    shortcutsTab = new QWidget;
                                    QVBoxLayout *shortcutsLayout;
                                    shortcutsLayout = new QVBoxLayout;

                                    QTableWidget *table;
                                    table = new QTableWidget;

                                    table->setColumnCount(4);
                                    table->setHorizontalHeaderLabels({
                                                                            "#",
                                                                            "Shortcut",
                                                                            "Action",
                                                                            "Edit"
                                                                        });

                                    // Addong Shortcuts
                                    Automate::CreateShortcutRow(table,"1","Ctrl + O","Open File");
                                    Automate::CreateShortcutRow(table,"2","Ctrl + W","Close Tab");
                                    Automate::CreateShortcutRow(table,"3","Ctrl + D","Duplicate Tab");
                                    Automate::CreateShortcutRow(table,"4","F2","Rename Tab");
                                    Automate::CreateShortcutRow(table,"5","Ctrl + Shift + S","Open Settings");

                                    table->horizontalHeader()->setStretchLastSection(true);
                                    shortcutsLayout->addWidget(table);
                                    shortcutsTab->setLayout(shortcutsLayout);

                                    settingsTabs->addTab(shortcutsTab,"Shortcuts");
                                }

    // Create Customize Tab in settings
    void CreateCustomiseTab() {
                                    customizeTab = new QWidget;
                                    QVBoxLayout *layout;
                                    layout = new QVBoxLayout;
                                    savedPathsLayout = new QVBoxLayout;

                                    pathGroup = new QButtonGroup(this);
                                    pathGroup->setExclusive(true);

                                    QLabel *savedPathsLabel;
                                    savedPathsLabel = new QLabel("Saved Paths");

                                    addPathButton = new QPushButton("Add Path");
                                    addPathButton->setToolTip("Add Paths");
                                    addPathButton->setCursor(Qt::PointingHandCursor);

                                    removePathButton = new QPushButton("Remove Path");
                                    removePathButton->setToolTip("Remove Path");
                                    removePathButton->setCursor(Qt::PointingHandCursor);
                                    
                                    layout->addWidget(savedPathsLabel);
                                    layout->addLayout(savedPathsLayout);
                                    layout->addWidget(addPathButton);
                                    layout->addWidget(removePathButton);
                                    layout->addStretch();

                                    customizeTab->setLayout(layout);
                                    settingsTabs->addTab(customizeTab,"Customize");
                                }

    // Path Addition Window
    void ShowAddPathDialog() {
                                QHBoxLayout *buttonLayout;
                                buttonLayout = new QHBoxLayout;
                                QVBoxLayout *mainLayout;
                                mainLayout = new QVBoxLayout;

                                QDialog dialog(this);
                                dialog.setWindowTitle("Add Path");
                                dialog.resize(500,150);

                                QLineEdit *pathEdit;
                                pathEdit = new QLineEdit;

                                QPushButton *browseButton;
                                browseButton = new QPushButton("Browse");

                                QPushButton *addButton;
                                addButton = new QPushButton("Add");

                                QPushButton *cancelButton;
                                cancelButton = new QPushButton("Cancel");

                                QHBoxLayout *pathLayout;
                                pathLayout = new QHBoxLayout;

                                pathLayout->addWidget(pathEdit);
                                pathLayout->addWidget(browseButton);

                                buttonLayout->addStretch();
                                buttonLayout->addWidget(addButton);
                                buttonLayout->addWidget(cancelButton);

                                mainLayout->addLayout(pathLayout);
                                mainLayout->addLayout(buttonLayout);

                                dialog.setLayout(mainLayout);

                                // Browse Button Connection
                                connect(browseButton,&QPushButton::clicked,this,[=]() {
                                                                                            QString path;
                                                                                            path = QFileDialog::getExistingDirectory(this,"Select Folder");
                                                                                            if(!path.isEmpty()) {
                                                                                                                    pathEdit->setText(path);
                                                                                                                }
                                                                                        }
                                        );
                                
                                // Cancel Button Connection
                                connect(cancelButton,&QPushButton::clicked,[&,this]() {
                                                                                            QMessageBox::StandardButton reply;
                                                                                            reply = SVGStudioMessages::ConfirmCancel(this);
                                                                                            if(reply == QMessageBox::Yes) {
                                                                                                dialog.reject();
                                                                                            }
                                                                                        }
                                        );

                                // Check Path Is Valid or Invalid
                                connect(addButton,&QPushButton::clicked,this,[=,&dialog]() {
                                                                                                QString path;
                                                                                                path = pathEdit->text();
                                                                                                if(path.trimmed().isEmpty()) {
                                                                                                                                SVGStudioMessages::invalidPath(this);
                                                                                                                                return;
                                                                                                                            }
                                                                                                QRegularExpression regex("^[A-Z]:[/\\\\].+");
                                                                                                if(!regex.match(path.toUpper()).hasMatch()) {
                                                                                                                                                SVGStudioMessages::invalidPath(this);
                                                                                                                                                return;
                                                                                                                                            }

                                                                                                if(!QDir(path).exists()) {
                                                                                                                            QMessageBox::StandardButton reply;
                                                                                                                            reply = SVGStudioMessages::confirmtoCreatePath(this);
                                                                                                                            if(reply == QMessageBox::Yes) {
                                                                                                                                                            QDir().mkpath(path);
                                                                                                                                                        }
                                                                                                                            else {
                                                                                                                                        return;
                                                                                                                                    }
                                                                                                                        }

                                                                                                QRadioButton *pathRadio;
                                                                                                pathRadio = new QRadioButton(path);
                                                                                                pathGroup->addButton(pathRadio);
                                                                                                savedPathsLayout->addWidget(pathRadio);
                                                                                                pathRadio->setChecked(true);
                                                                                                SVGStudioMessages::Success(this);
                                                                                                dialog.accept();
                                                                                            }
                                        );

                                dialog.exec();
                            }

    void connections() {
                            // Toggle Appearnce Dropdown
                            connect(appearanceButton,&QPushButton::clicked,this,[=]() {
                                                                                        appearanceContainer->setVisible(!appearanceContainer->isVisible());
                                                                                        if (appearanceContainer->isVisible()) {
                                                                                                                                    appearanceButton->setIcon(QIcon(FilePaths::downArrowIconPath));
                                                                                                                                }
                                                                                        else {
                                                                                                appearanceButton->setIcon(QIcon(FilePaths::rightArrowIconPath));
                                                                                            }
                                                                                        }
                                    );

                            // Call Add Path Dalog                         
                            connect(addPathButton,&QPushButton::clicked,this,[=]() {
                                                                                        ShowAddPathDialog();
                                                                                    }
                                    );

                            // Remove Button Connection
                            connect(removePathButton,&QPushButton::clicked,this,[=]() {
                                                                                            QAbstractButton *selectedButton;
                                                                                            selectedButton = pathGroup->checkedButton();
                                                                                            if(selectedButton == nullptr) {
                                                                                                                                return;
                                                                                                                            }
                                                                                            pathGroup->removeButton(selectedButton);
                                                                                            savedPathsLayout->removeWidget(selectedButton);
                                                                                            delete selectedButton;
                                                                                        }
                                );
                        }

    void openSettings() {
                            exec();
                        }
};

class SVGStudioShortcuts {
public:

    // Creation - Ctrl + O Shortcut
    static void openFilesShortcut(QAction *openAction,QWidget *window,SVGStudioLogic *logic,QTabWidget *tabWidget) {
                                                                                                                            QObject::connect(openAction,&QAction::triggered,window,[=]() {
                                                                                                                                                                                            QStringList filePaths;
                                                                                                                                                                                            filePaths = logic->openSVG(window);

                                                                                                                                                                                            for(QString filePath : filePaths) {
                                                                                                                                                                                                                                    SVGStudioEditorTab *editorTab;
                                                                                                                                                                                                                                    editorTab = new SVGStudioEditorTab;

                                                                                                                                                                                                                                    editorTab->getPreview()->load(filePath);
                                                                                                                                                                                                                                    tabWidget->addTab(editorTab,QFileInfo(filePath).fileName());
                                                                                                                                                                                                                                    tabWidget->setTabToolTip(tabWidget->indexOf(editorTab),filePath);
                                                                                                                                                                                                                                    tabWidget->setCurrentWidget(editorTab);
                                                                                                                                                                                                                                }
                                                                                                                                                                                        }
                                                                                                                                            );
                                                                                                                        }
    // Creation - Ctrl + Q Shortcut
    static void exitApplicationShortcut(QAction *exitAction,QWidget *window) {
                                                                                QObject::connect(
                                                                                                    exitAction,&QAction::triggered,window,[=]() {
                                                                                                                                                    window->close();
                                                                                                                                                }
                                                                                                );
                                                                            }
    // Creation - Tab + (1 - 9) shortcut
    static void createTabNumberShortcut(QWidget *window,QTabWidget *tabWidget) {
                                                                                    for (int i = 0; i <= 9; i++) {
                                                                                                                        QShortcut *tabSwiitchShortcut;
                                                                                                                        tabSwiitchShortcut = new QShortcut(QKeySequence(QString("Ctrl + %1").arg(i)),window);
                                                                                                                        QObject::connect(tabSwiitchShortcut,&QShortcut::activated,window,[=]() {
                                                                                                                                                                                                    if (tabWidget->count() >= i) {
                                                                                                                                                                                                                                        tabWidget->setCurrentIndex(i - 1);
                                                                                                                                                                                                                                    }
                                                                                                                                                                                                    
                                                                                                                                                                                                }
                                                                                                                                        );
                                                                                                                    }
                                                                                    
                                                                                }
    // Creation - Ctrl + W Shortcut
    static void closeCurrentTabShortcut(QWidget *window,QTabWidget *tabWidget,QStack<QString> *closedTabs) {
                                                                                                                QShortcut *closeCurrentTabShortcut;
                                                                                                                closeCurrentTabShortcut = new QShortcut(QKeySequence("Ctrl + W"),window);
                                                                                                                QObject::connect(closeCurrentTabShortcut,&QShortcut::activated,window,[=]() {
                                                                                                                                                                                                int Index;
                                                                                                                                                                                                Index = tabWidget->currentIndex();
                                                                                                                                                                                                if (Index != -1) {
                                                                                                                                                                                                                    if (!tabWidget->tabToolTip(Index).isEmpty()) {
                                                                                                                                                                                                                                                            closedTabs->push(tabWidget->tabToolTip(Index));
                                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                        QWidget *tab;
                                                                                                                                                                                                                        tab = tabWidget->widget(Index);
                                                                                                                                                                                                                        tabWidget->removeTab(Index);
                                                                                                                                                                                                                        delete tab;
                                                                                                                                                                                                                }
                                                                                                                                                                                                
                                                                                                                                                                                            }
                                                                                                                                );
                                                                                                            }

    // Creation - Ctrl + Tab Shortcut
    static void NextTabShortcut(QWidget *window,QTabWidget *tabWidget) {
                                                                            QShortcut *nextTabShortcut;
                                                                            nextTabShortcut = new QShortcut(QKeySequence("Ctrl + Tab"),window);
                                                                            QObject::connect(nextTabShortcut,&QShortcut::activated,window,[=]() {
                                                                                                                                                    if (tabWidget->count() > 1) {
                                                                                                                                                                                    int nextIndex;
                                                                                                                                                                                    nextIndex = (tabWidget->currentIndex() + 1) % tabWidget->count();
                                                                                                                                                                                    tabWidget->setCurrentIndex(nextIndex);
                                                                                                                                                                                }
                                                                                                                                                    
                                                                                                                                                }
                                                                                            );
                                                                        }

    
};

class SVGStudioGui : public QMainWindow {
                                            SVGStudioLogic logic;
                                            SVGStudioMessages messages;
                                            SVGStudioButtonLogic buttonLogic;
                                            SVGStudioSettingsDialogLogic settingsLogic;

                                            QAction *openAction;
                                            QAction *settingsAction;
                                            QShortcut *settingsShortcut;
                                            QAction *exitAction;
                                            QMenu *fileMenu;
                                            QWidget *centralWidget;
                                            QWidget *welcomePage;
                                            QLabel *welcomeToTheSvgStudioLabel;
                                            QLabel *welcomePageStartLabel;
                                            QPushButton *openFileButton;
                                            QPushButton *openFolderButton;
                                            QPushButton *newFileButton;
                                            QPushButton *convertToSvgButton;
                                            QTabWidget *tabWidget;
                                            QSvgWidget *newSvgWidget;
                                            QTabBar *tabBar;
                                            QStack<QString> closedTabs;
                                            QHBoxLayout *welcomeMainLayout;
                                            QVBoxLayout *leftLayout;
                                            QFrame *dragOverlay;
                                            QLabel *dragOverlayLabel;

public:
    SVGStudioGui() {
        // Window Setup
        CreateMainWindow();

        // Widget Creation
        CreateCentralWidget();
        CreateWidgets();

        // Layout Setup
        CreateLayouts(centralWidget);

        // Menu Setup
        CreateMenus();
        CreateStatusBar();

        // Signal Connections
        CreateConnections();
    }

    void CreateCentralWidget() {
                                    centralWidget = new QWidget;

                                    // Creating Welcome Tab
                                    welcomePage = new QWidget;
                                    welcomeToTheSvgStudioLabel = new QLabel("Welcome To The SVG Studio");
                                    welcomeToTheSvgStudioLabel->setStyleSheet(Style::welcomeToTheSvgStudioLabelStyle());

                                    // qLabel - Adding Welcome to the SVG Studio Label i welcome Tab
                                    welcomePageStartLabel = new QLabel("Open, Preview and Manage SVG Files");
                                    welcomePageStartLabel->setStyleSheet(Style::welcomePageStartLabelStyle());

                                    // Button - Adding New file Button in Welcome Tab
                                    newFileButton = new QPushButton("New File...");
                                    newFileButton->setIcon(QIcon(FilePaths::newFileIconPath));
                                    newFileButton->setIconSize(QSize(32,32));
                                    newFileButton->setStyleSheet(Style::welcomeButtonStyle());
                                    newFileButton->setCursor(Qt::PointingHandCursor);
                                    newFileButton->setToolTip("Creae New SVG File");
                                    newFileButton->setFixedWidth(140);
                                    newFileButton->setFixedHeight(42);

                                    // Button - Adding open file Button in Welcome Tab
                                    openFileButton = new QPushButton("Open File...");
                                    openFileButton->setIcon(QIcon(FilePaths::openFileIconPath));
                                    openFileButton->setIconSize(QSize(32,32));
                                    openFileButton->setStyleSheet(Style::welcomeButtonStyle());
                                    openFileButton->setCursor(Qt::PointingHandCursor);
                                    openFileButton->setToolTip("Open Fle...");
                                    openFileButton->setFixedWidth(140);
                                    openFileButton->setFixedHeight(42);
                                    connect(openFileButton,&QPushButton::clicked,this,[=](){ buttonLogic.openFileButtonLogic(this,tabWidget);});

                                    // Button - Adding open Foder Button in  welcome Tab
                                    openFolderButton = new QPushButton("Open Folder...");
                                    openFolderButton->setIcon(QIcon(FilePaths::openFolderIconPath));
                                    openFolderButton->setIconSize(QSize(32,32));
                                    openFolderButton->setStyleSheet(Style::welcomeButtonStyle());
                                    openFolderButton->setCursor(Qt::PointingHandCursor);
                                    openFolderButton->setToolTip("Open Folder...");
                                    openFolderButton->setFixedWidth(152);
                                    openFolderButton->setFixedHeight(42);
                                    connect(openFolderButton,&QPushButton::clicked,this,[=](){buttonLogic.openFolderButtonLogic(this,tabWidget);});

                                    // Button - Adding Convert to SVG Button in  Welcome Tab
                                    convertToSvgButton = new QPushButton("Convert to SVG...");
                                    convertToSvgButton->setIcon(QIcon(FilePaths::convertToSvgIconPath));
                                    convertToSvgButton->setIconSize(QSize(32,32));
                                    convertToSvgButton->setStyleSheet(Style::welcomeButtonStyle());
                                    convertToSvgButton->setCursor(Qt::PointingHandCursor);
                                    convertToSvgButton->setToolTip("Convert To SVG...");
                                    convertToSvgButton->setFixedWidth(175);
                                    convertToSvgButton->setFixedHeight(42);

                                    // Animation of Drag and Drop - Adding Drag Drop in Welcome Tab
                                    dragOverlay = new QFrame(centralWidget);
                                    dragOverlayLabel = new QLabel("Drop SVG Here",dragOverlay);
                                    dragOverlay->hide();
                                    dragOverlay->setStyleSheet(Style::dragOverlayStyle());
                                    dragOverlayLabel->setStyleSheet(Style::DropOverlayLabelStyle());

                                    QVBoxLayout *overlayLayout;
                                    overlayLayout = new QVBoxLayout;
                                    overlayLayout->addStretch();
                                    overlayLayout->addWidget(dragOverlayLabel,0,Qt::AlignCenter);
                                    overlayLayout->addStretch();
                                    dragOverlay->setLayout(overlayLayout);

                                    setCentralWidget(centralWidget);
                                }

    void CreateMainWindow() {
                                setWindowTitle("SVG Studio");
                                resize(1200, 800);
                                setAcceptDrops(true);
                            }

    void CreateMenus() {
                            // Creation of file Button Menu
                            fileMenu = menuBar()->addMenu("File");

                            // Ation - Adding open + ctrl + O
                            openAction = fileMenu->addAction("Open");
                            openAction->setShortcut(QKeySequence("Ctrl + O"));

                            // Ation - Adding settings + ctrl + Shift + S
                            settingsAction = fileMenu->addAction("Settings");
                            settingsAction->setShortcut(QKeySequence("Ctrl + Shift + S"));
                            connect(settingsAction,&QAction::triggered,this,[=](){settingsLogic.openSettings();});

                            // Ation - Adding Exit + ctrl + Q
                            exitAction = fileMenu->addAction("Exit");
                            exitAction->setShortcut(QKeySequence("Ctrl + Q"));

                            openAction->setToolTip("Open an SVG file");
                            openAction->setStatusTip("Open an SVG file");

                            addAction(openAction);
                            statusBar();
                        }

    void CreateConnections() {
                                // Oen SVG(s) in Tabs
                                SVGStudioShortcuts::openFilesShortcut(openAction,this,&logic,tabWidget);

                                // connection - closing App
                                SVGStudioShortcuts::exitApplicationShortcut(exitAction,this);

                                // Removing Tab 
                                connect(tabWidget, &QTabWidget::tabCloseRequested,this,[=](int index) {
                                                                                                            if (!tabWidget->tabToolTip(index).isEmpty()){
                                                                                                                                                            closedTabs.push(tabWidget->tabToolTip(index));
                                                                                                                                                        }
                                                                                                            QWidget *tab = tabWidget->widget(index);
                                                                                                            tabWidget->removeTab(index);
                                                                                                            delete tab;
                                                                                                        }
                                        );
                                
                                // creating connection to rename Tab
                                connect(tabBar,&QTabBar::tabBarDoubleClicked,this,[=](int index) {
                                                                                                    QString newName;
                                                                                                    newName = QInputDialog::getText(
                                                                                                                                    this,"Rename Tab","New Name:"
                                                                                                                                    );

                                                                                                    if (!newName.isEmpty()) {
                                                                                                                                tabWidget->setTabText(index, newName);
                                                                                                                            }
                                                                                                }
                                        );

                                // Creating Tab Right Click Options
                                connect(tabBar,&QTabBar::customContextMenuRequested,this,[=](const QPoint &pos) {
                                                                                                                    int index = tabBar->tabAt(pos);
                                                                                                                        if (index == -1) {
                                                                                                                            return;
                                                                                                                        }

                                                                                                                    // Create every Tab Menu 
                                                                                                                    QMenu menu;

                                                                                                                    // Action - Adding Action of Close Tab + ctrl + w
                                                                                                                    QAction *closeTab;
                                                                                                                    closeTab = menu.addAction("Close Tab");
                                                                                                                    closeTab->setShortcut(QKeySequence("Alt + F4"));

                                                                                                                    // Action - Adding Action of Rename Tab + F2
                                                                                                                    QAction *renameTab;
                                                                                                                    renameTab = menu.addAction("Rename Tab");
                                                                                                                    renameTab->setShortcut(QKeySequence("F2"));

                                                                                                                    // Action - Adding Action of Duplicate Tab + ctrl + D
                                                                                                                    QAction *duplicaateTab;
                                                                                                                    duplicaateTab = menu.addAction("Duplicate Tab");
                                                                                                                    duplicaateTab->setShortcut(QKeySequence("Ctrl + D"));

                                                                                                                    // Action - Adding Action of Moove Tab in  New Window
                                                                                                                    QAction *openSvgAsNewWindow;
                                                                                                                    openSvgAsNewWindow = menu.addAction("Move SVG in New Window");

                                                                                                                    // Action - Adding Action of Close other Tab
                                                                                                                    QAction *closeOtherTabs;
                                                                                                                    closeOtherTabs = menu.addAction("Close Other Tabs");

                                                                                                                    // Action - Adding Action of Close All Tab
                                                                                                                    QAction *closeAllTabs;
                                                                                                                    closeAllTabs = menu.addAction("Close All");

                                                                                                                    QAction *selectedAction;
                                                                                                                    selectedAction = menu.exec(mapToGlobal(pos));

                                                                                                                    // condition - close current tab while right click oon tab then pressing close
                                                                                                                    if (selectedAction == closeTab) {
                                                                                                                                                        if (!tabWidget->tabToolTip(index).isEmpty()) {
                                                                                                                                                                                                        closedTabs.push(tabWidget->tabToolTip(index));
                                                                                                                                                                                                    }
                                                                                                                                                        QWidget *tab;
                                                                                                                                                        tab = tabWidget->widget(index);
                                                                                                                                                        tabWidget->removeTab(index);
                                                                                                                                                        delete tab;
                                                                                                                                                    }

                                                                                                                    // condition - Rename Tab click  Right on Tab and  pressing rename Tab
                                                                                                                    if (selectedAction == renameTab) {
                                                                                                                                                        QString newName;
                                                                                                                                                        newName = QInputDialog::getText(this,"Rename Tab","New Name:");

                                                                                                                                                        if (!newName.isEmpty()) {
                                                                                                                                                                                    tabWidget->setTabText(index, newName);
                                                                                                                                                                                }
                                                                                                                                                    }

                                                                                                                    // Condition - Create Duplicate of Tab Using Tab Right click pressing Duplicate Tab
                                                                                                                    if (selectedAction == duplicaateTab) {
                                                                                                                                                            QString filePath;
                                                                                                                                                            filePath = tabWidget->tabToolTip(index);
                                                                                                                                                            SVGStudioEditorTab *duplicateSvgWidget;
                                                                                                                                                            duplicateSvgWidget = new SVGStudioEditorTab;
                                                                                                                                                            duplicateSvgWidget->getPreview()->load(filePath);
                                                                                                                                                            tabWidget->addTab(duplicateSvgWidget,QFileInfo(filePath).fileName());
                                                                                                                                                            tabWidget->setTabToolTip(tabWidget->indexOf(duplicateSvgWidget),filePath);
                                                                                                                                                            tabWidget->setCurrentWidget(duplicateSvgWidget);
                                                                                                                                                        }

                                                                                                                    // conndition - Move tab  from  current window to  new seperate window
                                                                                                                    if (selectedAction == openSvgAsNewWindow) {
                                                                                                                                                                    SVGStudioGui *openNewWindow;
                                                                                                                                                                    QString fileName;
                                                                                                                                                                    fileName = tabWidget->tabToolTip(index);
                                                                                                                                                                    openNewWindow = new SVGStudioGui;
                                                                                                                                                                    openNewWindow->show();

                                                                                                                                                                    SVGStudioEditorTab *editorTab;
                                                                                                                                                                    editorTab = new SVGStudioEditorTab;
                                                                                                                                                                    editorTab->getPreview()->load(fileName);
                                                                                                                                                                    tabWidget->addTab(editorTab,QFileInfo(fileName).fileName());
                                                                                                                                                                    tabWidget->setTabToolTip(tabWidget->indexOf(editorTab),fileName);
                                                                                                                                                                    tabWidget->setCurrentWidget(editorTab);
                                                                                                                                                                    openNewWindow->setWindowTitle(QString("SVG Studio - %1").arg(QFileInfo(fileName).fileName()));
                                                                                                                                                                    QWidget *tab;
                                                                                                                                                                    tab = tabWidget->widget(index);
                                                                                                                                                                    tabWidget->removeTab(index);
                                                                                                                                                                    delete tab;
                                                                                                                                                                }

                                                                                                                    // condition - Close every tabs excluding current Tab
                                                                                                                    if (selectedAction == closeOtherTabs) {
                                                                                                                                                            for (int i = tabWidget->count() - 1; i >= 0; i--) {
                                                                                                                                                                                                                if (i != index) {
                                                                                                                                                                                                                                    QWidget *tab;
                                                                                                                                                                                                                                    tab = tabWidget->widget(i);
                                                                                                                                                                                                                                    tabWidget->removeTab(i);
                                                                                                                                                                                                                                    delete tab;
                                                                                                                                                                                                                                }
                                                                                                                                                                                                                }
                                                                                                                                                        }

                                                                                                                    // condion - To close all Tabs using tab right click pressing close All option 
                                                                                                                    if (selectedAction == closeAllTabs) {
                                                                                                                                                            for (int i = tabWidget->count() - 1; i >= 0;i--) {
                                                                                                                                                                                                                    if (!tabWidget->tabToolTip(i).isEmpty()) {
                                                                                                                                                                                                                                                                closedTabs.push(tabWidget->tabToolTip(i));
                                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                    QWidget *tab;
                                                                                                                                                                                                                    tab = tabWidget->widget(i);
                                                                                                                                                                                                                    tabWidget->removeTab(i);
                                                                                                                                                                                                                    delete tab;
                                                                                                                                                                                                                }
                                                                                                                                                        }
                                                                                                                                                    }
                                        );

                                // rename Window title whle swtch tabs
                                connect(tabWidget,&QTabWidget::currentChanged,this,[=](int index) {
                                                                                                    if (index == -1)
                                                                                                                    {
                                                                                                                        return;
                                                                                                                    }
                                                                                                                    setWindowTitle(QString("SVG Studio %1").arg(tabWidget->tabText(index)));
                                                                                                }
                                        );

                                // Call - ctrl + 1 - 9 shortcut
                                SVGStudioShortcuts::createTabNumberShortcut(this,tabWidget);
                            
                                // call - Ctrl + W shortvut
                                SVGStudioShortcuts::closeCurrentTabShortcut(this,tabWidget,&closedTabs);

                                // Call - Ctrl + Tab shortcut
                                SVGStudioShortcuts::NextTabShortcut(this,tabWidget);

                                // Ctrl + Shift + Tab shortcut Creation
                                QShortcut *previousTabShortCut;
                                previousTabShortCut = new QShortcut(QKeySequence("Ctrl + Shift + Tab"),this);
                                connect(previousTabShortCut, &QShortcut::activated,this,[=]() {
                                                                                                    int index;
                                                                                                    index = tabWidget->currentIndex();
                                                                                                    if (tabWidget->count() > 0) {
                                                                                                                                    index--;
                                                                                                                                    if (index < 0) {
                                                                                                                                                        index = tabWidget->count() - 1;
                                                                                                                                                    }
                                                                                                                                    tabWidget->setCurrentIndex(index);
                                                                                                                                }
                                                                                                }
                                        );

                                // Ctrl + Shift + T Shortcut Creation
                                QShortcut *restoreTabShortCut;
                                restoreTabShortCut = new QShortcut(QKeySequence("Ctrl + Shift + T"),this);
                                connect(restoreTabShortCut, &QShortcut::activated,this,[=]() {
                                                                                                    if (!closedTabs.isEmpty()) {
                                                                                                                                    QString filePath;
                                                                                                                                    filePath = closedTabs.pop();
                                                                                                                                    SVGStudioEditorTab *editorTab;
                                                                                                                                    editorTab = new SVGStudioEditorTab;
                                                                                                                                    editorTab->getPreview()->load(filePath);
                                                                                                                                    tabWidget->addTab(editorTab,QFileInfo(filePath).fileName());
                                                                                                                                    tabWidget->setTabToolTip(tabWidget->indexOf(editorTab),filePath);
                                                                                                                                    tabWidget->setCurrentWidget(editorTab);
                                                                                                                                }
                                                                                                }
                                        );

                                // Ctrl + Shift + N shortcut creation
                                QShortcut *openNewWelcomeTabShortCut;
                                openNewWelcomeTabShortCut = new QShortcut(QKeySequence("Ctrl + Shift + N"),this);
                                connect(openNewWelcomeTabShortCut,&QShortcut::activated,this,[=]() {
                                                                                                        QSvgWidget *welcomeWidget;
                                                                                                        welcomeWidget = new QSvgWidget;
                                                                                                        tabWidget->addTab(welcomeWidget,"Welcome");
                                                                                                        tabWidget->setCurrentWidget(welcomeWidget);
                                                                                                    }
                                        );

                                // Ctrl + Alt + N Shortcut Creation
                                QShortcut *openNewWelcomeTabWindowShortCut;
                                openNewWelcomeTabWindowShortCut = new QShortcut(QKeySequence("Ctrl + Alt + N"),this);
                                connect(openNewWelcomeTabWindowShortCut,&QShortcut::activated,this,[=]() {
                                                                                                            SVGStudioGui *newWindow;
                                                                                                            newWindow = new SVGStudioGui;
                                                                                                            newWindow->show();
                                                                                                        }
                                        );

                                // Ctrl + Shift + w Shortcut Creation
                                QShortcut *CloseWindow;
                                CloseWindow = new QShortcut(QKeySequence("Ctrl + Shift + W"),this);
                                connect(CloseWindow,&QShortcut::activated,this,[=]() {
                                                                                        close();
                                                                                    }
                                        );
                                
                                // Ctrl + D Shortcut Creation
                                QShortcut *DuplicateCurrentTab;
                                DuplicateCurrentTab = new QShortcut(QKeySequence("Ctrl + D"),this);
                                connect(DuplicateCurrentTab, &QShortcut::activated,this,[=]() {
                                                                                                    int index;
                                                                                                    index = tabWidget->currentIndex();
                                                                                                    if (index == -1) {
                                                                                                                        return;
                                                                                                                    }
                                                                                                    QString filePath;
                                                                                                    filePath = tabWidget->tabToolTip(index);
                                                                                                    if (filePath.isEmpty()) {
                                                                                                                                return;
                                                                                                                            }
                                                                                                    SVGStudioEditorTab *duplicateSvgWidget;
                                                                                                    duplicateSvgWidget = new SVGStudioEditorTab;
                                                                                                    duplicateSvgWidget->getPreview()->load(filePath);
                                                                                                    tabWidget->addTab(duplicateSvgWidget,QFileInfo(filePath).fileName());
                                                                                                    tabWidget->setTabToolTip(tabWidget->indexOf(duplicateSvgWidget),filePath);
                                                                                                    tabWidget->setCurrentWidget(duplicateSvgWidget);
                                                                                                }
                            );

                            // F2 Shortcut creation 
                            QShortcut *renameTabShortCut;
                            renameTabShortCut = new QShortcut(QKeySequence("F2"),this);
                            connect(renameTabShortCut,&QShortcut::activated,this,[=]() {
                                                                                            int index;
                                                                                            index = tabWidget->currentIndex();
                                                                                            if (index == -1) {
                                                                                                                return;
                                                                                                            }
                                                                                            QString newName;
                                                                                            newName = QInputDialog::getText(this,"Rename Tab","New Name");
                                                                                            if (!newName.isEmpty()) {
                                                                                                                        tabWidget->setTabText(index,newName);
                                                                                                                    }
                                                                                        }
                                    );
                        }

    void CreateWidgets() {
                            QVBoxLayout *welcomeLayout;
                            welcomeLayout = new QVBoxLayout;
                            welcomeMainLayout = new QHBoxLayout;
                            leftLayout = new QVBoxLayout;

                            leftLayout->addStretch();
                            leftLayout->addWidget(newFileButton);
                            leftLayout->addWidget(openFileButton);
                            leftLayout->addWidget(openFolderButton);
                            leftLayout->addWidget(convertToSvgButton);

                            welcomeLayout->addWidget(welcomeToTheSvgStudioLabel);
                            welcomeLayout->addWidget(welcomePageStartLabel);

                            welcomeMainLayout->addLayout(leftLayout,1);
                            welcomeMainLayout->addStretch();

                            welcomeLayout->addLayout(welcomeMainLayout);
                            welcomePage->setLayout(welcomeLayout);

                            tabWidget = new QTabWidget;
                            tabWidget->addTab(welcomePage,"Welcome");
                            tabWidget->setMovable(true);
                            tabWidget->setTabsClosable(true);
                            tabBar = tabWidget->tabBar();
                            tabBar->setUsesScrollButtons(true);
                            tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
                        }

    void CreateLayouts(QWidget *centralWidget) {
                                                    QVBoxLayout *layout = new QVBoxLayout;
                                                    
                                                    layout->addWidget(tabWidget);
                                                    layout->setStretch(1, 1);
                                                    centralWidget->setLayout(layout);
                                                }

    void CreateStatusBar() {
                                statusBar();
                            }

protected:
    // Create File Tabs while Drpping Files from File Expplorer or else 
    void dragEnterEvent(QDragEnterEvent *event) override {
                                                                if(event->mimeData()->hasUrls()) {
                                                                                                    dragOverlay->show();
                                                                                                    event->acceptProposedAction();
                                                                                                }
                                                            }

    // Drop Files Using DraG & Drop from  file explorer
    void dropEvent(QDropEvent *event) override {
                                                    dragOverlay->hide();
                                                    QList<QUrl> urls;
                                                    urls = event->mimeData()->urls();
                                                    for (QUrl url : urls) {
                                                                                QString FilePath;
                                                                                FilePath = url.toLocalFile();
                                                                                SVGStudioEditorTab *editorTab;
                                                                                editorTab = new SVGStudioEditorTab;
                                                                                editorTab->getPreview()->load(FilePath);
                                                                                tabWidget->addTab(editorTab,QFileInfo(FilePath).fileName());
                                                                                tabWidget->setTabToolTip(tabWidget->indexOf(editorTab),FilePath);
                                                                                tabWidget->setCurrentWidget(editorTab);
                                                                            }
                                                }

    void resizeEvent(QResizeEvent *event) override {
                                                        QMainWindow::resizeEvent(event);
                                                        dragOverlay->setGeometry(centralWidget->rect());
                                                    }

    void dragLeaveEvent(QDragLeaveEvent *event) override {
                                                                dragOverlay->hide();
                                                                event->accept();
                                                            }
};

// Calls App
int main(int argc, char *argv[]) {
                                    QApplication app(argc, argv);
                                    SVGStudioGui studio;
                                    studio.show();
                                    return app.exec();
                                }

// 2760