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
#include<QMovie>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QFile>
#include<QPropertyAnimation>
#include<QFrame>
#include<windows.h>
#include<shlobj.h>
#include<QSettings>
#include<QScrollArea>
#include<QTimer>
#include<QLocalServer>
#include<QLocalSocket>
#include<QProcess>
#include <opencv2/opencv.hpp>
#include<QComboBox>

// Paths Collection class
class FilePaths {
public:
                    static inline const QString newFileIconPath =":/Assets/NEW_FILE_ICON.svg";
                    static inline const QString openFileIconPath =":/Assets/OPEN_FILE_ICON.svg";
                    static inline const QString openFolderIconPath =":/Assets/OPEN_FOLDER_ICON.svg";
                    static inline const QString convertToSvgIconPath = ":/Assets/CONVERT_TO_SVG.svg";
                    static inline const QString editButtonIconPath = ":/Assets/EDIT_BUTTON_ICON.svg";
                    static inline const QString downArrowIconPath = ":/Assets/DOWN_ARROW_ICON.svg";
                    static inline const QString rightArrowIconPath = ":/Assets/RIGHT_ARROW_ICON.svg";
                    static inline const QString DragDropOfTwoSvgsAnimationGifPath = ":/Assets/2_SVG_DRAG_DROP_ANIMATION.gif";
                    static inline const QString DragDropOfOneSvgsAnimationGifPath = ":/Assets/1_SVG_DRAG_DROP_ANIMATION.gif";
                    static inline const QString DragDropOfMoreThanThreeSvgsAnimationGifPath = ":/Assets/3_AND_MORE_SVG_DRAG_DROP_ANIMATION.gif";
                    static inline const QString NormalRemoveButtonIconPath = ":/Assets/NORMAL_REMOVE.svg";
                    static inline const QString RedRemoveButtonIconPath = ":/Assets/RED_REMOVE.svg";
                    static inline const QString GlowRedRemoveButtonIconPath = ":/Assets/GLOW_RED_REMOVE.svg";
                    static inline const QString DarkModeSvgFileIconPath = ":/Assets/DARK_MODE_SVG_FILE_ICON.svg";
                    static inline const QString LightModeSvgFileIconPath = ":/Assets/LIGHT_MODE_SVG_FILE_ICON.svg";
                    static inline const QString progressLoaderAnimationPath = ":/Assets/PROGRESS_LOADER.gif";
                    static inline const QString successAnimationPath = ":/Assets/SUCCESS.gif";
                    static inline const QString DarkModeSvgFileICOIcon = "DARK_MODE_SVG_FILE_ICON.ico";
                    static inline const QString LightModeSvgFileICOIcon = "LIGHT_MODE_SVG_FILE_ICON.ico";
                    static inline const QString DataFileName ="/SVGStudioData.json";
                    static inline const QString VTracerExe = "/Tools/vtracer.exe";
                };

class SVGStudioInstanceManager {
public:
    static inline const QString ServerName = "SVGStudioServer";
};

// Apply ICO in File Explorer
class ExplorerIconManager {
public:
    static void ApplySvgIcon(const QString& icoPath) {
                                                        QSettings svgClass("HKEY_CURRENT_USER\\Software\\Classes\\.svg",QSettings::NativeFormat);
                                                        svgClass.setValue(".", "SVGStudio.svg");
                                                        QSettings defaultIcon(
                                                                                "HKEY_CURRENT_USER\\Software\\Classes\\SVGStudio.svg\\DefaultIcon",
                                                                                QSettings::NativeFormat
                                                                            );
                                                        defaultIcon.setValue(".", QDir::toNativeSeparators(icoPath));
                                                        SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,nullptr,nullptr);
                                                    }

    static void ApplyDarkIcon() {
                                        QString icoPath = QCoreApplication::applicationDirPath() + "/" + FilePaths::DarkModeSvgFileICOIcon; ApplySvgIcon(icoPath);
                                    }

    static void ApplyLightIcon() {
                                    QString icoPath = QCoreApplication::applicationDirPath() + "/" + FilePaths::LightModeSvgFileICOIcon;ApplySvgIcon(icoPath);
                                }
};

// Make App Default 
class DefaultAppManager {
public:
    static void SetAsDefault() {
                                    QString exePath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
                                    QSettings svgClass("HKEY_CURRENT_USER\\Software\\Classes\\.svg",QSettings::NativeFormat);
                                    svgClass.setValue(".", "SVGStudio.svg");
                                    QSettings command("HKEY_CURRENT_USER\\Software\\Classes\\SVGStudio.svg\\shell\\open\\command",QSettings::NativeFormat);
                                    command.setValue(".","\"" + exePath + "\" \"%1\"");
                                    SHChangeNotify( SHCNE_ASSOCCHANGED,SHCNF_IDLIST,nullptr,nullptr);
                                }

    static void RemoveDefaultApp() {
                                        QSettings svgClass("HKEY_CURRENT_USER\\Software\\Classes\\.svg",QSettings::NativeFormat);
                                        svgClass.remove("");
                                        SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,nullptr,nullptr);
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
    
    // Welcome page Button(s) Style
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

    static QString ToggleOffStyle() {
                                        return R"(
                                                    QFrame {
                                                                    background-color:#444444;
                                                                    border-radius:12px;
                                                                }
                                                )";
                                    }

    static QString ToggleOnStyle() {
                                        return R"(
                                                    QFrame {
                                                                    background-color:#2ECC71;
                                                                    border-radius:12px;
                                                                }
                                                )";
                                    }

    static QString PathCardStyle() {
                                        return R"(
                                                    QFrame{
                                                                background:none;
                                                                border:none;
                                                                border-radius:8px;
                                                            }
                                                )";
                                    }

    static QString RemoveButtonStyle() {
                                            return R"(
                                                        QPushButton{
                                                                        border:none;
                                                                        background:transparent;
                                                                        color:white;
                                                                    }

                                                        QPushButton:hover{
                                                                            color:red;
                                                                        }

                                                        QPushButton:pressed{
                                                                                background:rgba(255,77,79,50);
                                                                                color:#990000;
                                                                            }
                                                    )";
                                        }

    static QString AddPathButtonStyle() {
                                            return R"(
                                                            QPushButton{
                                                                background:none;
                                                                border:nonw;
                                                                border-radius:8px;
                                                                padding:6px 14px;
                                                                font-weight:bold;
                                                            }

                                                            QPushButton:hover{
                                                                border:1px solid #00FFFF;
                                                            }

                                                            QPushButton:pressed{
                                                                background:#303030;
                                                            }
                                                        )";
                                        }

    // File Icon Card Style
    static QString FileIconCardStyle() {
                                            return R"(
                                                        QFrame {
                                                                    background-color:none;
                                                                    border:none;
                                                                    border-radius:8px;
                                                                    padding:4px;
                                                                }

                                                        QFrame:hover {
                                                                        border:1px solid #00FFFF;
                                                                        background-color:#252525;
                                                                    }
                                                    )";
                                        }

    // Edit Button Style
    static QString editButtnStyle() {
                                        return R"(
                                                        QPushButton {
                                                                        background:none;
                                                                        color:#ffffff;
                                                                        border:none;
                                                                        font-weight:bold;
                                                                    }
                                                    )";
                                    }

    static QString recentFileNameStyle() {
                                                return R"(
                                                            QPushButton {
                                                                            background:none;
                                                                            color:white;
                                                                            border:none;
                                                                            font-weight:bold;
                                                                        }
                                                            )";
                                            }
};

class SVGStudioDataManager {
public:
    static QString GetDataFilePath() {
                                        QString dataPath = QCoreApplication::applicationDirPath() + FilePaths::DataFileName;
                                        QFile file(dataPath);
                                        if(!file.exists()) {
                                                                CreateDefaultJson();
                                                            }

                                        return dataPath;
                                    }

    static void CreateDefaultJson() {
                                        QString dataPath = QCoreApplication::applicationDirPath() + FilePaths::DataFileName;
                                        QFile file(dataPath);
                                        if(file.open(QIODevice::WriteOnly)) {
                                                                                QJsonObject root;
                                                                                root["saved_paths"] = QJsonArray();
                                                                                root["recent_files"] = QJsonArray();
                                                                                root["recent_history_enabled"] = false;
                                                                                root["svg_file_icons_enabled"] = false;
                                                                                root["default_svg_app_enabled"] = false;
                                                                                root["svg_icon_mode"] = "system";
                                                                                file.write(QJsonDocument(root).toJson());
                                                                                file.close();
                                                                            }
                                    }

    static QJsonObject LoadData() {
                                        QFile file(GetDataFilePath());
                                        if(!file.open(QIODevice::ReadOnly)) {
                                                                                return QJsonObject();
                                                                            }

                                        QByteArray jsonData;
                                        jsonData = file.readAll();
                                        file.close();
                                        return QJsonDocument::fromJson(jsonData).object();
                                    }

    static void SaveData(QJsonObject data) {
                                                QFile file(GetDataFilePath());
                                                if(file.open(QIODevice::WriteOnly)) {
                                                                                        file.write(QJsonDocument(data).toJson());
                                                                                        file.close();
                                                                                    }
                                            }

    static void AddPath(QString path) {
                                            QJsonObject data;
                                            data = LoadData();
                                            QJsonArray paths;
                                            paths = data["saved_paths"].toArray();
                                            paths.append(path);
                                            data["saved_paths"] = paths;
                                            SaveData(data);
                                        }

    static QStringList GetPaths() {
                                        QStringList result;
                                        QJsonObject data;
                                        data = LoadData();
                                        QJsonArray paths;
                                        paths = data["saved_paths"].toArray();
                                        for(QJsonValue value : paths) {
                                                                            result.append(
                                                                                                value.toString()
                                                                                            );
                                                                        }

                                        return result;
                                    }

    static void RemovePath(QString path) {
                                            QJsonObject data;
                                            data = LoadData();
                                            QJsonArray paths;
                                            paths = data["saved_paths"].toArray();
                                            QJsonArray newPaths;
                                            for(QJsonValue value : paths) {
                                                                                if(value.toString() != path) {
                                                                                                                newPaths.append(value);
                                                                                                            }
                                                                            }

                                            data["saved_paths"] = newPaths;
                                            SaveData(data);
                                        }

    // Add Recents History inside JSON Storage
    static void AddRecentFile(QString filePath) {
                                    QJsonObject data;
                                    data = LoadData();
                                    QJsonArray recentFiles;
                                    recentFiles = data["recent_files"].toArray();
                                    QJsonArray newRecentFiles;
                                    newRecentFiles.append(filePath);
                                    for (QJsonValue value:recentFiles) {
                                                                            if (value.toString() != filePath) {
                                                                                                                    newRecentFiles.append(value);
                                                                                                                }
                                                                        }

                                    const int maximumFiles = GetMaximumRecentHistory();
                                    while (newRecentFiles.size() > maximumFiles) {
                                                                            newRecentFiles.removeLast();
                                                                        }
                                    data["recent_files"] = newRecentFiles;
                                    SaveData(data);
                                }

    // Get  the recent History from JSON Storage
    static QStringList GetRecentFiles() {
                                            QStringList result;
                                            QJsonObject data;
                                            data = LoadData();
                                            QJsonArray recentFiles;
                                            recentFiles = data["recent_files"].toArray();
                                            for (QJsonValue value:recentFiles) {
                                                                                    result.append(value.toString());
                                                                                }
                                            return result;
                                        }

    // Remove the Recent History  from JSON Storage
    static void RemoveRecentFile(QString filePath) {
                                                        QJsonObject data;
                                                        data = LoadData();
                                                        QJsonArray recentFiles;
                                                        recentFiles = data["recent_files"].toArray();
                                                        QJsonArray newRecentFiles;
                                                        for (QJsonValue value:recentFiles) {
                                                                                                if(value.toString() != filePath) {
                                                                                                                                    newRecentFiles.append(value);
                                                                                                                                }
                                                                                            }
                                                        data["recent_files"] = newRecentFiles;
                                                        SaveData(data);
                                                    }

    static bool IsRecentHistoryEnabled() {
                                            QJsonObject data = LoadData();
                                            return data["recent_history_enabled"].toBool(false);
                                        }

    // FIX: Get Maximum Recent History
    static int GetMaximumRecentHistory() {
                                            QJsonObject data = LoadData();
                                            return data["maximum_recent_history"].toInt(50);
                                        }

    // FIX: Save Maximum Recent History
    static void SetMaximumRecentHistory(int value) {
                                                        QJsonObject data = LoadData();
                                                        data["maximum_recent_history"] = value;
                                                        SaveData(data);
                                                    }

    static void SetRecentHistoryEnabled(bool enabled) {
                                                            QJsonObject data = LoadData();
                                                            data["recent_history_enabled"] = enabled;
                                                            SaveData(data);
                                                        }

    static bool IsSvgFileIconsEnabled() {
                                            QJsonObject data = LoadData();
                                            return data["svg_file_icons_enabled"].toBool(false);
                                            // SaveData(data);
                                        }

    static void SetSvgFileIconsEnabled(bool enabled) {
                                                            QJsonObject data = LoadData();
                                                            data["svg_file_icons_enabled"] = enabled;
                                                            SaveData(data);
                                                        }

    static void SetSvgIconMode(QString mode) {
                                                QJsonObject data = LoadData();
                                                data["svg_icon_mode"] = mode;
                                                SaveData(data);
                                            }

    static QString GetSvgIconMode() {
                                        QJsonObject data = LoadData();
                                        return data["svg_icon_mode"].toString("system");
                                    }

    static bool IsDefaultSvgAppEnabled() {
                                            QJsonObject data = LoadData();
                                            return data["default_svg_app_enabled"].toBool(false);
                                        }

    static void SetDefaultSvgAppEnabled(bool enabled) {
                                                            QJsonObject data = LoadData();
                                                            data["default_svg_app_enabled"] = enabled;
                                                            SaveData(data);
                                                        }

    // static int GetMaximumRecentHistory() {
    //                                             QJsonObject settings = LoadSettings();
    //                                             return settings.value("MaximumRecentHistory").toInt(50);
    //                                         }

    // // FIX: Save Maximum Recent History
    // static void SetMaximumRecentHistory(int value) {
    //                                                     QJsonObject settings = LoadSettings();
    //                                                     settings["MaximumRecentHistory"] = value;
    //                                                     SaveSettings(settings);
    //                                                 }
};

// Toogle ON/OFF
class SVGStudioToggle : public QWidget {
    Q_OBJECT

private:
    bool isOn;
    QFrame *background;
    QFrame *knob;
    QPropertyAnimation *animation;
    QLabel *stateLabel;

public:
    SVGStudioToggle(QWidget *parent = nullptr):QWidget(parent),isOn(false) {
        setFixedSize(90,24);

        background = new QFrame(this);
        background->setGeometry(23,0,44,24);
        background->setStyleSheet(Style::ToggleOffStyle());

        knob = new QFrame(this);
        knob->setGeometry(25,2,20,20);
        knob->setStyleSheet(
                                "background:white;"
                                "border-radius:10px;"
                            );

        stateLabel = new QLabel("OFF", this);
        stateLabel->setGeometry(0,0,22,24);
        stateLabel->setAlignment(Qt::AlignCenter);
        stateLabel->setStyleSheet(
                                    "color:white;"
                                    "font-size:9pt;"
                                    "font-weight:bold;"
                                    "background:transparent;"
                                );

        background->lower();
        stateLabel->raise();
        knob->raise();

        animation = new QPropertyAnimation(knob,"pos",this);
        animation->setDuration(150);
    }

    bool IsChecked() const {
                                return isOn;
                            }

    void SetChecked(bool checked) {
                                        isOn = checked;
                                        animation->stop();
                                        if(isOn) {
                                                    stateLabel->setText("ON");
                                                    stateLabel->setGeometry(70,0,20,24);
                                                    background->setStyleSheet(Style::ToggleOnStyle());

                                                    animation->setStartValue(knob->pos());
                                                    animation->setEndValue(QPoint(45,2));
                                                }
                                        else {
                                                    stateLabel->setText("OFF");
                                                    stateLabel->setGeometry(0,0,20,24);
                                                    background->setStyleSheet(Style::ToggleOffStyle());

                                                    animation->setStartValue(knob->pos());
                                                    animation->setEndValue(QPoint(25,2));
                                                }

                                        animation->start();
                                        emit toggled(isOn);
                                    }

signals:
    void toggled(bool checked);

protected:
    void mousePressEvent(QMouseEvent *event) override {
                                                            SetChecked(!isOn);
                                                            QWidget::mousePressEvent(event);
                                                        }
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
    static void CreateShortcutRow(QTableWidget *table,QString number,QString shortcut,QString action) {
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

    static inline QTimer* systemThemeTimer = nullptr;
    static void StartSystemThemeWatcher() {
                                                if(systemThemeTimer)
                                                    return;

                                                systemThemeTimer = new QTimer;
                                                QObject::connect(systemThemeTimer,&QTimer::timeout,[]() {
                                                                                                            if(!SVGStudioDataManager::IsSvgFileIconsEnabled())
                                                                                                                return;

                                                                                                            if(SVGStudioDataManager::GetSvgIconMode() != "system")
                                                                                                                return;

                                                                                                            ApplySystemSvgIcon();
                                                                                                        }
                                                                );

                                                systemThemeTimer->start(5000); // 1000 = 1 second
                                            }

    static void ApplySystemSvgIcon() {
                                        QSettings personalize("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
                                        bool darkTheme = personalize.value("AppsUseLightTheme",1).toInt() == 0;
                                        if(darkTheme) {
                                                            ExplorerIconManager::ApplyDarkIcon();
                                                        }
                                        else {
                                                ExplorerIconManager::ApplyLightIcon();
                                            }
                                    }

    static void SystemIconApply(QWidget* parent) {
                                                    SVGStudioDataManager::SetSvgIconMode("system");
                                                    ApplySystemSvgIcon();
                                                    StartSystemThemeWatcher();
                                                    QMessageBox::information(
                                                                                parent,
                                                                                "SVG Studio",
                                                                                "System SVG File Icon Applied"
                                                                            );
                                                }

    static void StartupChecks() {
                                    if(!SVGStudioDataManager::IsSvgFileIconsEnabled())
                                        return;

                                    QString mode = SVGStudioDataManager::GetSvgIconMode();
                                    if(mode == "system") {
                                                            ApplySystemSvgIcon();
                                                            StartSystemThemeWatcher();
                                                        }
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

    static void EmptyFileName(QWidget *parent) {
                                                    QMessageBox::warning(
                                                                            parent,
                                                                            "Empty File Name",
                                                                            "New File Input cannot be empty."
                                                                        );
                                                }

    static void EmptyFolderPath(QWidget *parent) {
                                                    QMessageBox::warning(
                                                                            parent,
                                                                            "Empty Folder Path",
                                                                            "Please select a folder."
                                                                        );
                                                }

    static QMessageBox* FileAlreadyExists(QWidget *parent) {
                                                                QMessageBox *msg;
                                                                msg = new QMessageBox(parent);
                                                                msg->setWindowTitle("File Already Exists");
                                                                msg->setText("SVG file already exists.");
                                                                msg->addButton("Create Anyway",QMessageBox::AcceptRole);
                                                                msg->addButton("Overwrite",QMessageBox::DestructiveRole);
                                                                msg->addButton("Cancel",QMessageBox::RejectRole);
                                                                return msg;
                                                            }

    static QMessageBox::StandardButton ConfirmRemovePath( QWidget *parent,QString path) {
                                                                                            return QMessageBox::question(
                                                                                                                            parent,
                                                                                                                            "Remove Path",
                                                                                                                            QString("Do You Really want to Remove\n%1 ?").arg(path),
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

    // FIX: Prepare image before VTracer
    QString PrepareImageForTracing(const QString& inputImage,const QString& quality) {
                                                                                    Q_UNUSED(quality);

                                                                                    // Load Image
                                                                                    cv::Mat image = cv::imread(inputImage.toStdString(),cv::IMREAD_UNCHANGED);

                                                                                    if (image.empty()) {
                                                                                                            return inputImage;
                                                                                                        }

                                                                                    // Convert BGRA -> BGR
                                                                                    if (image.channels() == 4) {
                                                                                                                    cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
                                                                                                                }

                                                                                    // CLAHE Contrast Enhancement
                                                                                    cv::Mat lab;
                                                                                    cv::cvtColor(image, lab, cv::COLOR_BGR2Lab);
                                                                                    std::vector<cv::Mat> channels;
                                                                                    cv::split(lab, channels);
                                                                                    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.5, cv::Size(8,8));
                                                                                    clahe->apply(channels[0], channels[0]);
                                                                                    cv::merge(channels, lab);
                                                                                    cv::cvtColor(lab, image, cv::COLOR_Lab2BGR);

                                                                                    // Edge Preserving Filter
                                                                                    cv::Mat edgeImage;
                                                                                    cv::edgePreservingFilter(image,edgeImage,cv::RECURS_FILTER,60,0.4f);
                                                                                    image = edgeImage;

                                                                                    // 4× Upscale
                                                                                    cv::Mat upscale;
                                                                                    cv::resize(
                                                                                                    image,
                                                                                                    upscale,
                                                                                                    cv::Size(),
                                                                                                    4.0,
                                                                                                    4.0,
                                                                                                    cv::INTER_LANCZOS4
                                                                                                );

                                                                                    // Unsharp Mask
                                                                                    cv::Mat blur;
                                                                                    cv::GaussianBlur(upscale,blur,cv::Size(),1.5);
                                                                                    cv::addWeighted(upscale,1.5,blur,-0.5,0,upscale);

                                                                                    // Bilateral Filter
                                                                                    cv::Mat bilateralImage;
                                                                                    cv::bilateralFilter(
                                                                                                            upscale,
                                                                                                            bilateralImage,
                                                                                                            9,
                                                                                                            50,
                                                                                                            50
                                                                                                        );
                                                                                    upscale = bilateralImage;

                                                                                    // Save Temporary PNG
                                                                                    QString tempFile = QDir::tempPath() + "/SVGStudio_Preprocessed.png";

                                                                                    cv::imwrite(tempFile.toStdString(), upscale);

                                                                                    return tempFile;
                                                                                }
};

class SVGStudioButtonLogic {
private:
    SVGStudioLogic logic;

public:
    static void OpenSvgInTab(QString filePath,QTabWidget *tabWidget) {
                                                                        QFile file(filePath);
                                                                        SVGStudioEditorTab *editorTab;
                                                                        editorTab = new SVGStudioEditorTab;
                                                                        editorTab->getPreview()->load(filePath);
                                                                        if(file.open(QIODevice::ReadOnly)) {
                                                                                                                editorTab->getEditor()->setPlainText(
                                                                                                                                                        file.readAll()
                                                                                                                                                    );

                                                                                                                file.close();
                                                                                                            }

                                                                        tabWidget->addTab(editorTab, QFileInfo(filePath).fileName());
                                                                        tabWidget->setTabToolTip(tabWidget->indexOf(editorTab),filePath);
                                                                        tabWidget->setCurrentWidget(editorTab);
                                                                        if(SVGStudioDataManager::IsRecentHistoryEnabled()) {
                                                                                                                                SVGStudioDataManager::AddRecentFile(filePath);
                                                                                                                            }
                                                                    }

    // Open a Existing SVG File Using Open File... Button on WelcomePage
    void openFileButtonLogic(QWidget *parent,QTabWidget *tabWidget) {
                                                                        QStringList filePaths;
                                                                        filePaths = logic.openSVG(parent);
                                                                        for (QString filePath:filePaths) {
                                                                                                            OpenSvgInTab(filePath,tabWidget);
                                                                                                        }
                                                                    }
                        
    // Create New SVG  File Using ctrl + N & New File Button on WelcomePage
    void newFileButtonLogic(QWidget *parent,QTabWidget *tabWidget) {
                                                                        // Member - Dialog
                                                                        QDialog dialog(parent);

                                                                        // Member -( Title + New file + Location + .svg extension) Labels(s)
                                                                        QLabel *titleLabel;
                                                                        QLabel *fileNameLabel;
                                                                        QLabel *locationLabel;
                                                                        QLabel *svgLabel;

                                                                        //Member - (New File + File Loaction) Input(s)
                                                                        QLineEdit *fileNameEdit;
                                                                        QLineEdit *pathEdit;

                                                                        // Memebrs - (Browse + Create + cancel) Button(s)
                                                                        QPushButton *browseButton;
                                                                        QPushButton *createButton;
                                                                        QPushButton *cancelButton;

                                                                        // Members - Create a box + Holds Paths
                                                                        QButtonGroup *pathGroup;
                                                                        QGroupBox *quickPathsGroup;

                                                                        // Members - New File Dialog Layout(s)
                                                                        QVBoxLayout *mainLayout;
                                                                        QHBoxLayout *nameLayout;
                                                                        QVBoxLayout *quickPathsLayout;
                                                                        QHBoxLayout *buttonLayout;
                                                                        QHBoxLayout *locationLayout;

                                                                        auto CreateWidgets = [&]() {
                                                                                                        // Create Dialog Box
                                                                                                        dialog.setWindowTitle("New SVG File");
                                                                                                        dialog.resize(550,320);

                                                                                                        // Label - New File Label
                                                                                                        titleLabel = new QLabel("New SVG File");

                                                                                                        // Label - New File Name Label
                                                                                                        fileNameLabel = new QLabel("File Name");

                                                                                                        // Label - Location Label
                                                                                                        locationLabel = new QLabel("Location");

                                                                                                        // Label - .svg label
                                                                                                        svgLabel = new QLabel(".svg");

                                                                                                        // Input - New File Input
                                                                                                        fileNameEdit = new QLineEdit;
                                                                                                        fileNameEdit->setPlaceholderText("Logo");
                                                                                                        fileNameEdit->setFocus();

                                                                                                        // Input - Path Input
                                                                                                        pathEdit = new QLineEdit;

                                                                                                        // Buutton - Browse Button
                                                                                                        browseButton = new QPushButton("Browse");
                                                                                                        browseButton->setCursor(Qt::PointingHandCursor);
                                                                                                        browseButton->setToolTip("Browse Path to CreateSave ifle");

                                                                                                        // Button - create Button
                                                                                                        createButton = new QPushButton("Create");
                                                                                                        createButton->setCursor(Qt::PointingHandCursor);
                                                                                                        createButton->setToolTip("create New SVG File");
                                                                                                    
                                                                                                        // Button - cancel Button
                                                                                                        cancelButton = new QPushButton("Cancel");
                                                                                                        cancelButton->setCursor(Qt::PointingHandCursor);
                                                                                                        cancelButton->setToolTip("Cancel");

                                                                                                        // Group - bOXof Recommendation Paths
                                                                                                        pathGroup = new QButtonGroup(&dialog);
                                                                                                        quickPathsGroup = new QGroupBox("Quick Access Paths");
                                                                                                    };

                                                                        auto CreateLayouts = [&]() {
                                                                                                        mainLayout = new QVBoxLayout;
                                                                                                        quickPathsLayout = new QVBoxLayout;

                                                                                                        QStringList paths;
                                                                                                        paths = SVGStudioDataManager::GetPaths();
                                                                                                        for(QString path : paths) {
                                                                                                                                        QRadioButton *radio;
                                                                                                                                        radio = new QRadioButton(path);
                                                                                                                                        radio->setCursor(Qt::PointingHandCursor);
                                                                                                                                        radio->setToolTip(path);
                                                                                                                                        pathGroup->addButton(radio);
                                                                                                                                        quickPathsLayout->addWidget(radio);
                                                                                                                                        QObject::connect(radio,&QRadioButton::toggled,[radio, path, pathEdit]() {
                                                                                                                                                                                                                    if(radio->isChecked()) {
                                                                                                                                                                                                                                                pathEdit->setText(path);
                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                }
                                                                                                                                                );

                                                                                                                                    }

                                                                                                        quickPathsGroup->setLayout(quickPathsLayout);

                                                                                                        mainLayout->addWidget(titleLabel);
                                                                                                        mainLayout->addWidget(fileNameLabel);

                                                                                                        // Layout - New File Name Layout (New File Label + New File Input + .svg Label)
                                                                                                        nameLayout = new QHBoxLayout;
                                                                                                        nameLayout->addWidget(fileNameEdit);
                                                                                                        nameLayout->addWidget(svgLabel);
                                                                                                        mainLayout->addLayout(nameLayout);

                                                                                                        // 10 px Spacing Between File Name Layout and Location Layout
                                                                                                        mainLayout->addSpacing(10);

                                                                                                        // Layout - Location Layout (Location Label + Location Input + Browse Button)
                                                                                                        mainLayout->addWidget(locationLabel);
                                                                                                        locationLayout = new QHBoxLayout;
                                                                                                        locationLayout->addWidget(pathEdit);
                                                                                                        locationLayout->addWidget(browseButton);
                                                                                                        mainLayout->addLayout(locationLayout);

                                                                                                        // Button Layout
                                                                                                        buttonLayout = new QHBoxLayout;
                                                                                                        buttonLayout->addStretch();
                                                                                                        buttonLayout->addWidget(cancelButton);
                                                                                                        buttonLayout->addWidget(createButton);

                                                                                                        // Quick Access Paths Layout (Quck Access Group + Raddio Buttons of Paths)
                                                                                                        mainLayout->addWidget(quickPathsGroup);
                                                                                                        mainLayout->addStretch();
                                                                                                        mainLayout->addLayout(buttonLayout);

                                                                                                        // Apply Layout
                                                                                                        mainLayout->addLayout(buttonLayout);
                                                                                                        dialog.setLayout(mainLayout);
                                                                                                    };

                                                                        // cionnect - Browse + Cancel + create Button(s) connectons
                                                                        auto CreateConnections = [&]() {

                                                                                                        // connect - Browse Button Logic
                                                                                                        QObject::connect(browseButton,&QPushButton::clicked,[&]() {
                                                                                                                                                                        QString path;
                                                                                                                                                                        path = QFileDialog::getExistingDirectory(
                                                                                                                                                                                                                    parent,
                                                                                                                                                                                                                    "Select Folder"
                                                                                                                                                                                                                );

                                                                                                                                                                        if(!path.isEmpty()) {
                                                                                                                                                                                                pathGroup->setExclusive(false);
                                                                                                                                                                                                for (auto button:pathGroup->buttons()) {
                                                                                                                                                                                                                                            button->setChecked(false);
                                                                                                                                                                                                                                        }
                                                                                                                                                                                                pathGroup->setExclusive(true);
                                                                                                                                                                                                pathEdit->setText(path);
                                                                                                                                                                                            }
                                                                                                                                                                    }
                                                                                                                    );

                                                                                                            // connect - Cancel Button Logic
                                                                                                            QObject::connect(cancelButton,&QPushButton::clicked,[&]() {
                                                                                                                                                                dialog.reject();
                                                                                                                                                            }
                                                                                                                    );

                                                                                                            // connect - Create Button Logic
                                                                                                            QObject::connect(createButton,&QPushButton::clicked,[&]() {
                                                                                                                                                                        QString fileName;
                                                                                                                                                                        fileName = fileNameEdit->text().trimmed();
                                                                                                                                                                        QString folderPath;
                                                                                                                                                                        folderPath = pathEdit->text().trimmed();
                                                                                                                                                                        QString fullPath;
                                                                                                                                                                        fullPath = QDir(folderPath).filePath(fileName + ".svg");
                                                                                                                                                                        if (QFile::exists(fullPath)) {
                                                                                                                                                                                                        QMessageBox *msg;
                                                                                                                                                                                                        msg = SVGStudioMessages::FileAlreadyExists(parent);
                                                                                                                                                                                                        msg->exec();

                                                                                                                                                                                                        QString clickedButton;
                                                                                                                                                                                                        clickedButton = msg->clickedButton()->text();
                                                                                                                                                                                                        if (clickedButton == "Cancel") {
                                                                                                                                                                                                                                            delete msg;
                                                                                                                                                                                                                                            return;
                                                                                                                                                                                                                                        }

                                                                                                                                                                                                        if (clickedButton == "Create Anyway") {
                                                                                                                                                                                                                                                int counter;
                                                                                                                                                                                                                                                counter = 1;
                                                                                                                                                                                                                                                QString newPath;
                                                                                                                                                                                                                                                do {
                                                                                                                                                                                                                                                        newPath = QDir(folderPath).filePath(
                                                                                                                                                                                                                                                                                                QString("%1 (%2).svg")
                                                                                                                                                                                                                                                                                                .arg(fileName)
                                                                                                                                                                                                                                                                                                .arg(counter)
                                                                                                                                                                                                                                                                                            );

                                                                                                                                                                                                                                                        counter++;

                                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                                while(QFile::exists(newPath));
                                                                                                                                                                                                                                                fullPath = newPath;
                                                                                                                                                                                                                                            }

                                                                                                                                                                                                        delete msg;
                                                                                                                                                                                                    }
                                                                                                                                                                        if (fileName.isEmpty()) {
                                                                                                                                                                                                        SVGStudioMessages::EmptyFileName(parent);
                                                                                                                                                                                                        return;
                                                                                                                                                                                                    }
                                                                                                                                                                            if (folderPath.isEmpty()) {
                                                                                                                                                                                                            SVGStudioMessages::EmptyFolderPath(parent);
                                                                                                                                                                                                            return;
                                                                                                                                                                                                        }

                                                                                                                                                                            if (QFile::exists(fullPath)) {

                                                                                                                                                                            }

                                                                                                                                                                            QFile file(fullPath);
                                                                                                                                                                            if (file.open(QIODevice::WriteOnly)) {
                                                                                                                                                                                                                    file.write(
                                                                                                                                                                                                                                    R"(<?xml version="1.0" encoding="UTF-8"?>
                                                                                                                                                                                                                                    <svg xmlns="http://www.w3.org/2000/svg"
                                                                                                                                                                                                                                        width="800"
                                                                                                                                                                                                                                        height="600"
                                                                                                                                                                                                                                        viewBox="0 0 800 600">

                                                                                                                                                                                                                                        <rect width="800"
                                                                                                                                                                                                                                            height="600"
                                                                                                                                                                                                                                            fill="white"/>

                                                                                                                                                                                                                                        <text x="400"
                                                                                                                                                                                                                                            y="300"
                                                                                                                                                                                                                                            text-anchor="middle"
                                                                                                                                                                                                                                            font-size="32">
                                                                                                                                                                                                                                            New SVG File
                                                                                                                                                                                                                                        </text>

                                                                                                                                                                                                                                    </svg>)"
                                                                                                                                                                                                                                );
                                                                                                                                                                                                                    file.close();
                                                                                                                                                                                                                }
                                                                                                                                                                            for(int i = 0; i < tabWidget->count(); i++) {
                                                                                                                                                                                                                            if(tabWidget->tabToolTip(i) == fullPath) {
                                                                                                                                                                                                                                                                        QWidget *tab;
                                                                                                                                                                                                                                                                        tab = tabWidget->widget(i);
                                                                                                                                                                                                                                                                        tabWidget->removeTab(i);
                                                                                                                                                                                                                                                                        delete tab;
                                                                                                                                                                                                                                                                        break;
                                                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                        }
                                                                                                                                                                            SVGStudioEditorTab *editorTab;
                                                                                                                                                                            editorTab = new SVGStudioEditorTab;
                                                                                                                                                                            editorTab->getPreview()->load(fullPath);
                                                                                                                                                                            tabWidget->addTab(editorTab,QFileInfo(fullPath).fileName());
                                                                                                                                                                            tabWidget->setTabToolTip(tabWidget->indexOf(editorTab),fullPath);
                                                                                                                                                                            QFile readFile(fullPath);
                                                                                                                                                                            if(readFile.open(QIODevice::ReadOnly)) {
                                                                                                                                                                                                                        editorTab->getEditor()->setPlainText(readFile.readAll());
                                                                                                                                                                                                                    }
                                                                                                                                                                            readFile.close();
                                                                                                                                                                            tabWidget->setCurrentWidget(editorTab);
                                                                                                                                                                            dialog.accept();
                                                                                                                                                                        }
                                                                                                            );
                                                                                                        };

                                                                        CreateWidgets();
                                                                        CreateLayouts();
                                                                        CreateConnections();
                                                                        dialog.exec();
                                                                    }

    // Open  Every .svg(s) that  exists inside a folder
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
                                                                                                            OpenSvgInTab(
                                                                                                                            fullpath,
                                                                                                                            tabWidget
                                                                                                                        );
                                                                                                        }

                                                                        }

    void convertToSvgButtonLogic() {
                                        auto dialog = new QDialog;
                                        auto titleLabel = (QLabel*)nullptr;
                                        auto inputImageLabel = (QLabel*)nullptr;
                                        auto outputSvgLabel = (QLabel*)nullptr;
                                        auto qualityLabel = (QLabel*)nullptr;
                                        auto imageInput = (QLineEdit*)nullptr;
                                        auto outputSvgLineEdit = (QLineEdit*)nullptr;
                                        auto broseButton = (QPushButton*)nullptr;
                                        auto browseOutputButton = (QPushButton*)nullptr;
                                        auto cancelButton = (QPushButton*)nullptr;
                                        auto convertButton = (QPushButton*)nullptr;
                                        auto qualityFastRadioButton = (QRadioButton*)nullptr;
                                        auto qualityBalancedRadioButton = (QRadioButton*)nullptr;
                                        auto qualityBestRadioButton = (QRadioButton*)nullptr;
                                        auto compareOrginalwithGnnerated = (QCheckBox*)nullptr;
                                        auto mainLayout = (QVBoxLayout*)nullptr;
                                        auto quickPathsLayout = (QVBoxLayout*)nullptr;
                                        auto quickPathsGroup = (QGroupBox*)nullptr;
                                        auto imageInputLayout = (QHBoxLayout*)nullptr;
                                        auto svgOutputLayout = (QHBoxLayout*)nullptr;
                                        auto qualityLayout = (QHBoxLayout*)nullptr;
                                        auto cancelAndCovertButtonLayout = (QHBoxLayout*)nullptr;
                                        auto pathGroup = (QButtonGroup*)nullptr;

                                        auto CreateWidgets = [&]() {
                                                                        dialog->setWindowTitle("Convert To SVG");
                                                                        dialog->resize(800,600);

                                                                        // Label - Title Label
                                                                        titleLabel = new QLabel("Convert To SVG");

                                                                        inputImageLabel = new QLabel("Imge Location");

                                                                        // Input - Input of resourses
                                                                        imageInput = new QLineEdit();

                                                                        outputSvgLabel = new QLabel("Output");

                                                                        outputSvgLineEdit = new QLineEdit();


                                                                        // Button - Browse Button
                                                                        broseButton = new QPushButton("Browse");
                                                                        broseButton->setCursor(Qt::PointingHandCursor);
                                                                        broseButton->setToolTip("Browse");

                                                                        // Button - Output Browse Button
                                                                        browseOutputButton = new QPushButton("Browse");
                                                                        browseOutputButton->setCursor(Qt::PointingHandCursor);
                                                                        browseOutputButton->setToolTip("Browse Save Location");

                                                                        // Labbel -  Quality Label
                                                                        qualityLabel = new QLabel("Quality");

                                                                        // Quality -> Button - Fast Radio Button
                                                                        qualityFastRadioButton = new QRadioButton("fast");
                                                                        qualityFastRadioButton->setCursor(Qt::PointingHandCursor);
                                                                        qualityFastRadioButton->setToolTip("Fast");
                                                                        
                                                                        // Quality -> Button - Balanced Radio Button
                                                                        qualityBalancedRadioButton = new QRadioButton("Balanced");
                                                                        qualityBalancedRadioButton->setCursor(Qt::PointingHandCursor);
                                                                        qualityBalancedRadioButton->setToolTip("Balanced");
                                                                        
                                                                        // Quality -> Button - Fast Radio Button
                                                                        qualityBestRadioButton = new QRadioButton("Best");
                                                                        qualityBestRadioButton->setCursor(Qt::PointingHandCursor);
                                                                        qualityBestRadioButton->setToolTip("Best");

                                                                        // Checkbox - Comapare Orginal with Gennerated
                                                                        compareOrginalwithGnnerated = new QCheckBox("Compare Orginal With Results");
                                                                        compareOrginalwithGnnerated->setToolTip("Compare Orginal With Results");
                                                                        compareOrginalwithGnnerated->setCursor(Qt::PointingHandCursor);

                                                                        // Button - Cancel Button
                                                                        cancelButton = new QPushButton("Cancel");
                                                                        cancelButton->setCursor(Qt::PointingHandCursor);
                                                                        cancelButton->setToolTip("Cancel");

                                                                        // Button - Cancel Button
                                                                        convertButton = new QPushButton("Convert");
                                                                        convertButton->setCursor(Qt::PointingHandCursor);
                                                                        convertButton->setToolTip("Convert");

                                                                        pathGroup = new QButtonGroup(dialog);
                                                                        quickPathsGroup = new QGroupBox("Quick Access Paths");
                                                                        mainLayout = new QVBoxLayout(dialog);
                                                                    };

                                        auto CreateLayouts = [&]() {
                                                                        mainLayout->addWidget(titleLabel);

                                                                        // Layout - A Horizontal Layout (Image Location + Input + Browse)
                                                                        imageInputLayout = new QHBoxLayout;
                                                                        imageInputLayout->addWidget(inputImageLabel);
                                                                        imageInputLayout->addWidget(imageInput);
                                                                        imageInputLayout->addWidget(broseButton);
                                                                        mainLayout->addLayout(imageInputLayout);

                                                                        // Layout - A Horizontal Layout (Output Save Location + Input + Browse)
                                                                        svgOutputLayout = new QHBoxLayout;
                                                                        svgOutputLayout->addWidget(outputSvgLabel);
                                                                        svgOutputLayout->addWidget(outputSvgLineEdit);
                                                                        svgOutputLayout->addWidget(browseOutputButton);
                                                                        mainLayout->addLayout(svgOutputLayout);

                                                                        quickPathsLayout = new QVBoxLayout;

                                                                                                        QStringList paths;
                                                                                                        paths = SVGStudioDataManager::GetPaths();
                                                                                                        for(QString path : paths) {
                                                                                                                                        QRadioButton *radio;
                                                                                                                                        radio = new QRadioButton(path);
                                                                                                                                        radio->setCursor(Qt::PointingHandCursor);
                                                                                                                                        radio->setToolTip(path);
                                                                                                                                        pathGroup->addButton(radio);
                                                                                                                                        quickPathsLayout->addWidget(radio);
                                                                                                                                        QObject::connect(radio,&QRadioButton::toggled,[radio, path, outputSvgLineEdit]() {
                                                                                                                                                                                                                            if(radio->isChecked()) {
                                                                                                                                                                                                                                                        outputSvgLineEdit->setText(path);
                                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                        }
                                                                                                                                                );

                                                                                                                                    }
                                                                        quickPathsGroup->setLayout(quickPathsLayout);
                                                                        mainLayout->addWidget(quickPathsGroup);

                                                                        // Layout - Horizontal  Layout (Quality + Fast + Balanced + Best)
                                                                        qualityLayout = new QHBoxLayout();
                                                                        qualityLayout->addWidget(qualityLabel);
                                                                        qualityLayout->addWidget(qualityFastRadioButton);
                                                                        qualityLayout->addWidget(qualityBalancedRadioButton);
                                                                        qualityLayout->addWidget(qualityBestRadioButton);
                                                                        mainLayout->addLayout(qualityLayout);

                                                                        mainLayout->addWidget(compareOrginalwithGnnerated);

                                                                        mainLayout->addStretch();

                                                                        cancelAndCovertButtonLayout = new QHBoxLayout();
                                                                        cancelAndCovertButtonLayout->addStretch();
                                                                        cancelAndCovertButtonLayout->addWidget(cancelButton);
                                                                        cancelAndCovertButtonLayout->addWidget(convertButton);
                                                                        mainLayout->addLayout(cancelAndCovertButtonLayout);
                                                                    };

                                        auto CreateConnections = [&]() {
                                                                            /// connect - cancel Button connections
                                                                            QObject::connect(cancelButton,&QPushButton::clicked,dialog,&QDialog::close);

                                                                            // connect - Output Browse Button connection
                                                                            QObject::connect(browseOutputButton,&QPushButton::clicked,dialog,[=]() {
                                                                                                                                                        QString folder = QFileDialog::getExistingDirectory(
                                                                                                                                                                                                                dialog,
                                                                                                                                                                                                                "Select Output Folder"
                                                                                                                                                                                                            );
                                                                                                                                                        if(!folder.isEmpty()) {
                                                                                                                                                                                    outputSvgLineEdit->setText(folder);
                                                                                                                                                                                }
                                                                                                                                                    }
                                                                                                );
                                                                        

                                                                            QObject::connect(broseButton,&QPushButton::clicked,dialog,[=]() {
                                                                                                                                                        QString folder = QFileDialog::getOpenFileName (
                                                                                                                                                                                                            dialog,
                                                                                                                                                                                                            "Select Image File",
                                                                                                                                                                                                            QString(),
                                                                                                                                                                                                            "Images (*.png *.jpg *.jpeg *.bmp *.webp *.ico)"
                                                                                                                                                                                                        );
                                                                                                                                                        if(!folder.isEmpty()) {
                                                                                                                                                                                    imageInput->setText(folder);
                                                                                                                                                                                }
                                                                                                                                                    }
                                                                                                );

                                                                            QObject::connect(convertButton,&QPushButton::clicked,dialog,[=]() {
                                                                                                                                                QString inputPath = imageInput->text().trimmed();
                                                                                                                                                QString outputPath = outputSvgLineEdit->text().trimmed();
                                                                                                                                                if(inputPath.isEmpty()) {
                                                                                                                                                                            QMessageBox::warning(
                                                                                                                                                                                                    dialog,
                                                                                                                                                                                                    "SVG Studio",
                                                                                                                                                                                                    "Please select an image."
                                                                                                                                                                                                );
                                                                                                                                                                            return;
                                                                                                                                                                        }

                                                                                                                                                if(outputPath.isEmpty()) {
                                                                                                                                                                            QMessageBox::warning(
                                                                                                                                                                                                    dialog,
                                                                                                                                                                                                    "SVG Studio",
                                                                                                                                                                                                    "Please select an output folder."
                                                                                                                                                                                                );
                                                                                                                                                                                return;
                                                                                                                                                                            }

                                                                                                                                                if(!QFile::exists(inputPath)) {
                                                                                                                                                                                    QMessageBox::warning(
                                                                                                                                                                                                            dialog,
                                                                                                                                                                                                            "SVG Studio",
                                                                                                                                                                                                            "Input image does not exist."
                                                                                                                                                                                                        );
                                                                                                                                                                                    return;
                                                                                                                                                                                }

                                                                                                                                                QDir outputDir(outputPath);
                                                                                                                                                if(!outputDir.exists()) {
                                                                                                                                                                            QMessageBox::warning(
                                                                                                                                                                                                    dialog,
                                                                                                                                                                                                    "SVG Studio",
                                                                                                                                                                                                    "Output folder does not exist."
                                                                                                                                                                                                );
                                                                                                                                                                            return;
                                                                                                                                                                        }

                                                                                                                                                QString extension = QFileInfo(inputPath).suffix().toLower();
                                                                                                                                                QStringList supportedFormats = {
                                                                                                                                                                                    "png",
                                                                                                                                                                                    "jpg",
                                                                                                                                                                                    "jpeg",
                                                                                                                                                                                    "bmp",
                                                                                                                                                                                    "webp",
                                                                                                                                                                                    "ico"
                                                                                                                                                                                };

                                                                                                                                                if(!supportedFormats.contains(extension)) {
                                                                                                                                                                                                QMessageBox::warning(
                                                                                                                                                                                                                        dialog,
                                                                                                                                                                                                                        "SVG Studio",
                                                                                                                                                                                                                        "Unsupported image format."
                                                                                                                                                                                                                    );
                                                                                                                                                                                                return;
                                                                                                                                                                                            }

                                                                                                                                                QString quality = "Balanced";
                                                                                                                                                if(qualityFastRadioButton->isChecked()) {
                                                                                                                                                                                            quality = "Fast";
                                                                                                                                                                                        }
                                                                                                                                                else if(qualityBestRadioButton->isChecked()) {
                                                                                                                                                                                                quality = "Best";
                                                                                                                                                                                            }
                                                                                                                                                RunVTracer(
                                                                                                                                                            imageInput->text(),
                                                                                                                                                            outputSvgLineEdit->text(),
                                                                                                                                                            quality,
                                                                                                                                                            compareOrginalwithGnnerated->isChecked()
                                                                                                                                                        );
                                                                                                                                            }
                                                                                            );
                                                                            };
                                                                        

                                        CreateWidgets();
                                        CreateLayouts();
                                        CreateConnections();

                                        dialog->exec();
                                        delete dialog;
                                    }

    // Run VTracer conversion
    void RunVTracer(QString inputImage,QString outputFolder,QString quality,bool compareResult) {
                                                                                    QDialog loadingDialog;
                                                                                    loadingDialog.setWindowFlags(
                                                                                                                    Qt::Dialog |
                                                                                                                    Qt::FramelessWindowHint
                                                                                                                );
                                                                                    loadingDialog.setModal(true);
                                                                                    loadingDialog.setFixedSize(320,320);

                                                                                    QVBoxLayout *layout = new QVBoxLayout(&loadingDialog);
                                                                                    QLabel *loader = new QLabel();
                                                                                    loader->setMinimumSize(320,320);
                                                                                    loader->setAlignment(Qt::AlignCenter);

                                                                                    QMovie *movie = new QMovie(FilePaths::progressLoaderAnimationPath);
                                                                                    movie->setScaledSize(QSize(320,320));
                                                                                    loader->setMovie(movie);
                                                                                    movie->start();

                                                                                    QLabel *text = new QLabel("Converting Image...");
                                                                                    text->setAlignment(Qt::AlignCenter);
                                                                                    layout->addStretch();
                                                                                    layout->addWidget(loader,0,Qt::AlignCenter);
                                                                                    layout->addSpacing(5);
                                                                                    layout->addWidget(text);
                                                                                    layout->addStretch();
                                                                                    loadingDialog.show();
                                                                                    QApplication::processEvents();
                                                                                    inputImage = logic.PrepareImageForTracing(inputImage, quality);

                                                                                    QString program = QCoreApplication::applicationDirPath() + FilePaths::VTracerExe;
                                                                                    QString tempSvg = QDir::tempPath() + "/SVGStudio_Temp.svg";
                                                                                    QString preset = "photo";

                                                                                    QStringList arguments;
                                                                                    arguments
                                                                                                << "--input" << inputImage
                                                                                                << "--output" << tempSvg
                                                                                                << "--mode" << "spline";

                                                                                    if (quality == "Fast") {
                                                                                                                arguments
                                                                                                                    <<"--filter_speckle"<<"8"
                                                                                                                    <<"--color_precision"<<"6"
                                                                                                                    <<"--gradient_step"<<"8"
                                                                                                                    <<"--corner_threshold"<<"80"
                                                                                                                    <<"--segment_length"<<"8"
                                                                                                                    <<"--splice_threshold"<<"45"
                                                                                                                    <<"--path_precision"<<"2";
                                                                                                            }
                                                                                    else if (quality == "Balanced") {
                                                                                                                        arguments
                                                                                                                            <<"--filter_speckle"<<"4"
                                                                                                                            <<"--color_precision"<<"8"
                                                                                                                            <<"--gradient_step"<<"4"
                                                                                                                            <<"--corner_threshold"<<"10"
                                                                                                                            <<"--segment_length"<<"5"
                                                                                                                            <<"--splice_threshold"<<"30"
                                                                                                                            <<"--path_precision"<<"4";
                                                                                                                    }
                                                                                    else if (quality == "Best") {
                                                                                                                    arguments
                                                                                                                        <<"--filter_speckle"<<"1"
                                                                                                                        <<"--color_precision"<<"8"
                                                                                                                        <<"--gradient_step"<<"1"
                                                                                                                        <<"--corner_threshold"<<"175"
                                                                                                                        << "--segment_length" << "3.5"
                                                                                                                        <<"--splice_threshold"<<"10"
                                                                                                                        <<"--path_precision"<<"8";
                                                                                                                }

                                                                                    QProcess process;
                                                                                    process.start(program,arguments);
                                                                                    while (!process.waitForFinished(50)) {
                                                                                                                                QApplication::processEvents();
                                                                                                                            }

                                                                                    loadingDialog.close();
                                                                                    if(process.exitStatus() != QProcess::NormalExit) {
                                                                                                                                        QMessageBox::critical(
                                                                                                                                                                nullptr,
                                                                                                                                                                "SVG Studio",
                                                                                                                                                                "Failed to start VTracer."
                                                                                                                                                            );
                                                                                                                                        return;
                                                                                                                                    }

                                                                                    loadingDialog.close();
                                                                                    if(process.exitCode() != 0) {
                                                                                                                    QMessageBox::critical(
                                                                                                                                            nullptr,
                                                                                                                                            "SVG Studio",
                                                                                                                                            process.readAllStandardError()
                                                                                                                                        );
                                                                                                                    return;
                                                                                                                }

                                                                                    loadingDialog.close();

                                                                                    if(compareResult) {
                                                                                                            ShowCompareWindow(
                                                                                                                                inputImage,
                                                                                                                                tempSvg,
                                                                                                                                outputFolder
                                                                                                                            );
                                                                                                            return;
                                                                                                        }
                                                                                    QDialog successDialog;
                                                                                    successDialog.setWindowFlags(
                                                                                                                    Qt::Dialog |
                                                                                                                    Qt::FramelessWindowHint
                                                                                                                );

                                                                                    successDialog.setModal(true);
                                                                                    successDialog.setFixedSize(360,320);
                                                                                    QVBoxLayout *successLayout = new QVBoxLayout(&successDialog);
                                                                                    QLabel *successGif = new QLabel();
                                                                                    successGif->setAlignment(Qt::AlignCenter);

                                                                                    QMovie *successMovie = new QMovie(":/Assets/SUCCESS.gif");
                                                                                    successMovie->setScaledSize(QSize(180,180));
                                                                                    successGif->setMovie(successMovie);
                                                                                    successMovie->start();

                                                                                    // Disappear the window after 2 seconds
                                                                                    QTimer::singleShot(5000,&successDialog,&QDialog::accept);

                                                                                    QLabel *successText = new QLabel("SVG Created Successfully!");
                                                                                    successText->setAlignment(Qt::AlignCenter);
                                                                                    successText->setStyleSheet("font-size:18px;""font-weight:bold;""color:white;");

                                                                                    successLayout->addStretch();
                                                                                    successLayout->addWidget(successGif,0,Qt::AlignCenter);
                                                                                    successLayout->addSpacing(10);
                                                                                    successLayout->addWidget(successText);
                                                                                    successLayout->addSpacing(15);
                                                                                    successLayout->addStretch();

                                                                                    successDialog.exec();
                                                                            }

    // Compare Dialog
    void ShowCompareWindow(QString originalImage,QString tempSvg,QString outputFolder) {
                                                                                            QDialog dialog;
                                                                                            dialog.setWindowTitle("Compare");
                                                                                            dialog.resize(1200,700);
                                                                                            QHBoxLayout *mainLayout = new QHBoxLayout(&dialog);

                                                                                            QLabel *original = new QLabel();
                                                                                            original->setAlignment(Qt::AlignCenter);
                                                                                            original->setPixmap(
                                                                                                                    QPixmap(originalImage).scaled(
                                                                                                                                                    550,
                                                                                                                                                    650,
                                                                                                                                                    Qt::KeepAspectRatio,
                                                                                                                                                    Qt::SmoothTransformation
                                                                                                                                                )
                                                                                                                );

                                                                                            QSvgWidget *svg = new QSvgWidget(tempSvg);
                                                                                            svg->setMinimumSize(550,650);

                                                                                            mainLayout->addWidget(original);
                                                                                            mainLayout->addWidget(svg);
                                                                                            dialog.exec();
                                                                                            QFile::remove(tempSvg);
                                                                                        }
};

// Dynamic Hover Effect on card class
class PathCardHoverFilter : public QObject {
private:
    QPushButton* editButton;
    QPushButton* removeButton;

public:
    PathCardHoverFilter(
                            QPushButton* editBtn,
                            QPushButton* removeBtn
                        )
                        {
                            editButton = editBtn;
                            removeButton = removeBtn;
                        }

protected:
    bool eventFilter(QObject* watched,QEvent* event) override {
                                                                    if(event->type() == QEvent::Enter) {
                                                                                                            editButton->show();
                                                                                                            removeButton->show();
                                                                                                            removeButton->setIcon(QIcon(FilePaths::NormalRemoveButtonIconPath));
                                                                                                        }

                                                                    if(event->type() == QEvent::Leave) {
                                                                                                            editButton->hide();
                                                                                                            removeButton->hide();
                                                                                                            removeButton->setIcon(QIcon(FilePaths::NormalRemoveButtonIconPath));
                                                                                                        }

                                                                    return QObject::eventFilter(watched,event);
                                                                }
};

// class - Remove and Edit Button SVG Switch while hover and REMOVE Button 
class RemoveButtonHoverFilter : public QObject {
private:
    QPushButton* button;

public:
    RemoveButtonHoverFilter(QPushButton* btn) {
                                                button = btn;
                                            }

protected:
    bool eventFilter(QObject* watched,QEvent* event) override {
        if(event->type() == QEvent::Enter) {
                                                button->setIcon(QIcon(FilePaths::RedRemoveButtonIconPath));
                                            }

        if(event->type() == QEvent::Leave) {
                                                button->setIcon(QIcon(FilePaths::NormalRemoveButtonIconPath));
                                            }

        return QObject::eventFilter(watched,event);
    }
};

// Sho wand Hide Priview and Apply Button class
class FileIconCardHoverFilter : public QObject {
// Members
private:
    QPushButton* previewButton;
    QPushButton* applyButton;

// Methods
public:
    FileIconCardHoverFilter(QPushButton* previewBtn,QPushButton* applyBtn) {
                                                                                previewButton = previewBtn;
                                                                                applyButton = applyBtn;
                                                                            }

protected:
    bool eventFilter(QObject* watched,QEvent* event) override {
                                                                    // Results - Show the Priview + Apply Button(s) while Hover
                                                                    if(event->type() == QEvent::Enter) {
                                                                                                            previewButton->show();
                                                                                                            applyButton->show();
                                                                                                        }

                                                                    // Results - Hide the Priview + Apply Button(s) while Leave or Mous eOutside
                                                                    if(event->type() == QEvent::Leave) {
                                                                                                            previewButton->hide();
                                                                                                            applyButton->hide();
                                                                                                        }

                                                                    return QObject::eventFilter(watched,event);
                                                                }
};

// Settings Class
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
    QLabel *recentHistoryLabel;
    SVGStudioToggle *recentHistoryToggle;
    QLabel *recentHistoryStatusLabel;
    QVBoxLayout *savedPathsLayout;
    QButtonGroup *pathGroup;
    QHBoxLayout *addButtonLayout;
    QFrame *pathCard;
    QHBoxLayout *cardLayout;
    QPushButton *removeButton;
    QPushButton *editButton;
    QRadioButton *pathRadio;
    QString pendingRemovePath;
    QFrame* pendingRemoveCard;
    QLabel *svgFileIconsLabel;
    SVGStudioToggle *svgFileIconsToggle;
    QLabel *darkFileIconLabel;
    QLabel *lightFileIconLabel;
    QLabel *systemFileIconLabel;
    QPushButton *applyButton;
    QPushButton *priviewButton;
    QHBoxLayout *buttonRow;
    QHBoxLayout *iconToggleLayout;
    QPushButton *darkPreviewButton;
    QPushButton *darkApplyButton;
    QPushButton *lightPreviewButton;
    QPushButton *lightApplyButton;
    QPushButton *systemPreviewButton;
    QPushButton *systemApplyButton;
    QScrollArea *scrollArea;
    QLabel *defaultSvgAppLabel;
    SVGStudioToggle *defaultSvgAppToggle;
    QHBoxLayout *defaultAppLayout;
    QLabel *maximumRecentHistoryLabel;
    QComboBox *maximumRecentHistoryCombo;

public:
    QFrame* CreateSeparator() {
                                QFrame *line;
                                line = new QFrame;
                                line->setFrameShape(QFrame::HLine);
                                line->setFrameShadow(QFrame::Sunken);
                                return line;
                            }

void SetupRemoveButtonStates(QPushButton* removeButton) {
                                                            connect(removeButton,&QPushButton::pressed,this,[=]() {
                                                                                                                        removeButton->setIcon(QIcon(FilePaths::GlowRedRemoveButtonIconPath));
                                                                                                                }
                                                                    );

                                                            connect(removeButton,&QPushButton::released,this,[=]() {
                                                                                                                        removeButton->setIcon(QIcon(FilePaths::RedRemoveButtonIconPath));
                                                                                                                    }
                                                                    );
                                                        }

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
                                    QWidget *scrollContent;
                                    scrollContent = new QWidget;

                                    QVBoxLayout *layout;
                                    QHBoxLayout *recentHistoryLayout;

                                    layout = new QVBoxLayout;
                                    savedPathsLayout = new QVBoxLayout;
                                    recentHistoryLayout = new QHBoxLayout;

                                    pathGroup = new QButtonGroup(this);
                                    pathGroup->setExclusive(true);

                                    QStringList savedPaths;
                                    savedPaths = SVGStudioDataManager::GetPaths();

                                    for(QString path : savedPaths) {
                                                                        // RadioButton - Path Radio Button
                                                                        QRadioButton *pathRadio;
                                                                        pathRadio = new QRadioButton(path);
                                                                        pathRadio->setCursor(Qt::PointingHandCursor);
                                                                        pathRadio->setToolTip(path);
                                                                        pathGroup->addButton(pathRadio);

                                                                        // Frame
                                                                        QFrame *pathCard;
                                                                        pathCard = new QFrame;
                                                                        pathCard->setMouseTracking(true);

                                                                        // Layout - Card Layout
                                                                        QHBoxLayout *cardLayout;
                                                                        cardLayout = new QHBoxLayout;

                                                                        pathCard->setFrameShape(QFrame::NoFrame);
                                                                        pathCard->setStyleSheet(Style::PathCardStyle());

                                                                        // Button - Remove Buton
                                                                        QPushButton *removeButton;
                                                                        removeButton = new QPushButton;
                                                                        SetupRemoveButtonStates(removeButton);
                                                                        removeButton->setIcon(QIcon(FilePaths::NormalRemoveButtonIconPath));
                                                                        removeButton->setIconSize(QSize(48,48));
                                                                        removeButton->setFixedSize(28,28);
                                                                        removeButton->setCursor(Qt::PointingHandCursor);
                                                                        removeButton->setToolTip("Remove Path");
                                                                        removeButton->setStyleSheet(Style::RemoveButtonStyle());
                                                                        removeButton->hide();
                                                                        removeButton->installEventFilter(new RemoveButtonHoverFilter(removeButton));
                                                                        connect(removeButton,&QPushButton::clicked,this,[=]() {
                                                                                                                                    RemovePathCard(
                                                                                                                                                        path,
                                                                                                                                                        pathCard
                                                                                                                                                    );
                                                                                                                                }
                                                                                );

                                                                        // Edit Button 
                                                                        editButton = new QPushButton;
                                                                        editButton->setCursor(Qt::PointingHandCursor);
                                                                        editButton->setToolTip("Edit Path");
                                                                        editButton->setIcon(QIcon(FilePaths::editButtonIconPath));
                                                                        editButton->setText("Edit");
                                                                        editButton->setStyleSheet(Style::editButtnStyle());
                                                                        editButton->hide();
                                                                        connect(editButton,&QPushButton::clicked,this,[=]() {
                                                                                                                                ShowEditPathDialog(path,pathRadio);
                                                                                                                            }
                                                                                );

                                                                        cardLayout->addWidget(pathRadio);
                                                                        cardLayout->addStretch();
                                                                        cardLayout->addWidget(editButton);
                                                                        cardLayout->addWidget(removeButton);
                                                                        pathCard->setLayout(cardLayout);
                                                                        pathCard->installEventFilter(new PathCardHoverFilter(
                                                                                                                                editButton,
                                                                                                                                removeButton
                                                                                                                            )
                                                                                                    );
                                                                        savedPathsLayout->addWidget(pathCard);
                                                                    }

                                    QLabel *savedPathsLabel;
                                    savedPathsLabel = new QLabel("Saved Paths");

                                    // Label - Recent History Label
                                    recentHistoryLabel = new QLabel("Collect Recent History");
                                    recentHistoryStatusLabel = new QLabel;

                                    // Maximum Recent History
                                    maximumRecentHistoryLabel = new QLabel("Maximum Recent Files");
                                    maximumRecentHistoryCombo = new QComboBox;
                                    maximumRecentHistoryCombo->addItems(
                                                                            {
                                                                                "1",
                                                                                "5",
                                                                                "10",
                                                                                "20",
                                                                                "30",
                                                                                "40",
                                                                                "50",
                                                                                "60",
                                                                                "80",
                                                                                "100",
                                                                                "Custom..."
                                                                            }
                                                                        );

                                    maximumRecentHistoryCombo->setCurrentText(QString::number(SVGStudioDataManager::GetMaximumRecentHistory()));
                                    recentHistoryToggle = new SVGStudioToggle;
                                    recentHistoryToggle->SetChecked(SVGStudioDataManager::IsRecentHistoryEnabled());
                                    maximumRecentHistoryCombo->setEnabled(recentHistoryToggle->IsChecked());

                                    recentHistoryToggle->SetChecked(
                                                                        SVGStudioDataManager::IsRecentHistoryEnabled()
                                                                    );
                                    if(recentHistoryToggle->IsChecked()) {
                                                                                recentHistoryStatusLabel->setText("Do Not Collect Recent History");
                                                                            }
                                    else {
                                                recentHistoryStatusLabel->setText("Collect Recent History");
                                            }

                                    // Button - Add Path Button
                                    addPathButton = new QPushButton("Add New Path");
                                    addPathButton->setFixedWidth(120);
                                    addPathButton->setFixedHeight(32);
                                    addPathButton->setToolTip("Add New Paths");
                                    addPathButton->setCursor(Qt::PointingHandCursor);
                                    addPathButton->setStyleSheet(Style::AddPathButtonStyle());

                                    // Label - Defualt Mode Label + Toggle
                                    defaultSvgAppLabel = new QLabel("Make SVG Studio Default SVG App");
                                    defaultSvgAppToggle = new SVGStudioToggle;
                                    defaultSvgAppToggle->SetChecked(SVGStudioDataManager::IsDefaultSvgAppEnabled());

                                    // Toggle - ON/OFF Toggle for App Icon (Enable SVG File Icons + Toggle)
                                    svgFileIconsLabel = new QLabel("Enable SVG File Icons");
                                    svgFileIconsToggle = new SVGStudioToggle;

                                    // Frame - Drak Card Frame/Card
                                    QFrame *darkCard = new QFrame;
                                    darkCard->setStyleSheet(Style::FileIconCardStyle());

                                    QHBoxLayout *darkLayout = new QHBoxLayout;

                                    // Label - Create Drak Mode SVG File Label
                                    QLabel *darkLabel =new QLabel("Apply Dark Mode SVG File Icon");

                                    // Button - Priview Button
                                    darkPreviewButton = new QPushButton("Preview");
                                    darkPreviewButton->setToolTip("Priview of File Icon");
                                    darkPreviewButton->setCursor(Qt::PointingHandCursor);

                                    // Button - Apply Button
                                    darkApplyButton = new QPushButton("Apply");
                                    darkApplyButton->setCursor(Qt::PointingHandCursor);
                                    darkApplyButton->setToolTip("Apply File Icon");

                                    darkPreviewButton->hide();
                                    darkApplyButton->hide();

                                    darkLayout->addWidget(darkLabel);
                                    darkLayout->addStretch();
                                    darkLayout->addWidget(darkPreviewButton);
                                    darkLayout->addWidget(darkApplyButton);

                                    darkCard->setLayout(darkLayout);
                                    darkCard->installEventFilter(new FileIconCardHoverFilter(
                                                                                                darkPreviewButton,
                                                                                                darkApplyButton
                                                                                            )
                                                                                        );
                                                                                
                                    // Frame - Light Card Frame/Card
                                    QFrame *lightCard = new QFrame;
                                    lightCard->setStyleSheet(Style::FileIconCardStyle());

                                    QHBoxLayout *lightLayout = new QHBoxLayout;

                                    // Label - Create Light Mode SVG File Label
                                    QLabel *lightLabel = new QLabel("Apply Light Mode SVG File Icon");

                                    // Button - Priview Button
                                    lightPreviewButton = new QPushButton("Preview");
                                    lightPreviewButton->setToolTip("Priview of File Icon");
                                    lightPreviewButton->setCursor(Qt::PointingHandCursor);

                                    // Button - Apply Button
                                    lightApplyButton = new QPushButton("Apply");
                                    lightApplyButton->setCursor(Qt::PointingHandCursor);
                                    lightApplyButton->setToolTip("Apply File Icon");

                                    lightPreviewButton->hide();
                                    lightApplyButton->hide();

                                    lightLayout->addWidget(lightLabel);
                                    lightLayout->addStretch();
                                    lightLayout->addWidget(lightPreviewButton);
                                    lightLayout->addWidget(lightApplyButton);

                                    lightCard->setLayout(lightLayout);
                                    lightCard->installEventFilter(new FileIconCardHoverFilter(
                                                                                                lightPreviewButton,
                                                                                                lightApplyButton
                                                                                            )
                                                                                        );

                                    // Frame - System Card Frame/Card
                                    QFrame *systemCard = new QFrame;
                                    systemCard->setStyleSheet(Style::FileIconCardStyle());

                                    QHBoxLayout *systemLayout = new QHBoxLayout;

                                    // Label - Create Light Mode SVG File Label
                                    QLabel *systemLabel =new QLabel("Apply System Mode SVG File Icon");

                                    // Button - Priview Button
                                    systemPreviewButton = new QPushButton("Preview");
                                    systemPreviewButton->setToolTip("Priview of Both File Icon(s)");
                                    systemPreviewButton->setCursor(Qt::PointingHandCursor);

                                    // Button - Apply Button
                                    systemApplyButton = new QPushButton("Apply");
                                    systemApplyButton->setCursor(Qt::PointingHandCursor);
                                    systemApplyButton->setToolTip("Apply File Icon");

                                    systemPreviewButton->hide();
                                    systemApplyButton->hide();

                                    systemLayout->addWidget(systemLabel);
                                    systemLayout->addStretch();
                                    systemLayout->addWidget(systemPreviewButton);
                                    systemLayout->addWidget(systemApplyButton);

                                    systemCard->setLayout(systemLayout);
                                    systemCard->installEventFilter(new FileIconCardHoverFilter(
                                                                                                systemPreviewButton,
                                                                                                systemApplyButton
                                                                                            )
                                                                                        );

                                                                                    
                                    // Make Button Diable  while toggle off
                                    bool enabled;
                                    enabled = SVGStudioDataManager::IsSvgFileIconsEnabled();
                                    svgFileIconsToggle->SetChecked(enabled);
                                    darkPreviewButton->setEnabled(enabled);
                                    darkApplyButton->setEnabled(enabled);
                                    lightPreviewButton->setEnabled(enabled);
                                    lightApplyButton->setEnabled(enabled);
                                    systemPreviewButton->setEnabled(enabled);
                                    systemApplyButton->setEnabled(enabled);

                                    layout->addWidget(savedPathsLabel);
                                    layout->addLayout(savedPathsLayout);

                                    // Layout - Add Button Layout
                                    addButtonLayout = new QHBoxLayout;
                                    addButtonLayout->addStretch();
                                    addButtonLayout->addWidget(addPathButton);
                                    layout->addLayout(addButtonLayout);
                                    layout->addWidget(CreateSeparator());

                                    // Layout - Add Layout (Collect Recents History + Toggle)
                                    recentHistoryLayout->addWidget(recentHistoryStatusLabel);
                                    recentHistoryLayout->addStretch();
                                    recentHistoryLayout->addWidget(recentHistoryToggle);
                                    recentHistoryLayout->addSpacing(15);
                                    recentHistoryLayout->addWidget(maximumRecentHistoryLabel);
                                    recentHistoryLayout->addWidget(maximumRecentHistoryCombo);
                                    layout->addWidget(recentHistoryLabel);
                                    layout->addLayout(recentHistoryLayout);
                                    layout->addWidget(CreateSeparator());

                                    // Layout - Default Mode Layout
                                    defaultAppLayout = new QHBoxLayout;
                                    defaultAppLayout->addWidget(defaultSvgAppLabel);
                                    defaultAppLayout->addStretch();
                                    defaultAppLayout->addWidget(defaultSvgAppToggle);
                                    layout->addWidget(CreateSeparator());
                                    layout->addLayout(defaultAppLayout);
                                    layout->addWidget(CreateSeparator());

                                    // Layout - Apply File Icon Section Layout
                                    iconToggleLayout = new QHBoxLayout;
                                    iconToggleLayout->addWidget(svgFileIconsLabel);
                                    iconToggleLayout->addStretch();
                                    iconToggleLayout->addWidget(svgFileIconsToggle);
                                    layout->addLayout(iconToggleLayout);

                                    // Layout - Icon(s) Cards Layout
                                    layout->addWidget(darkCard);
                                    layout->addWidget(lightCard);
                                    layout->addWidget(systemCard);
                                    layout->addWidget(CreateSeparator());

                                    // Apply Layout
                                    scrollContent->setLayout(layout);
                                    scrollArea = new QScrollArea;
                                    scrollArea->setWidgetResizable(true);
                                    scrollArea->setWidget(scrollContent);
                                    QVBoxLayout *tabLayout;
                                    tabLayout = new QVBoxLayout;
                                    tabLayout->addWidget(scrollArea);
                                    customizeTab->setLayout(tabLayout);
                                    settingsTabs->addTab(customizeTab,"Customize");
                                }

    // Show Preview Dialog for Dark and Light
    void ShowIconPreview(QString svgPath) {
                                                QDialog dialog(this);
                                                dialog.setWindowTitle("SVG Icon Preview");
                                                dialog.resize(400,400);
                                                QVBoxLayout *layout = new QVBoxLayout;
                                                QSvgWidget *preview = new QSvgWidget;
                                                preview->load(svgPath);
                                                layout->addWidget(preview);
                                                dialog.setLayout(layout);
                                                dialog.exec();
                                            }

    // Show Light + Dark File Icons side by side
    void ShowSystemPreview() {
                                QDialog dialog(this);
                                QHBoxLayout *layout = new QHBoxLayout;
                                QSvgWidget *dark = new QSvgWidget;
                                dark->load(FilePaths::DarkModeSvgFileIconPath);
                                QSvgWidget *light = new QSvgWidget;
                                light->load(FilePaths::LightModeSvgFileIconPath);
                                layout->addWidget(dark);
                                layout->addWidget(light);
                                dialog.setLayout(layout);
                                dialog.exec();
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

                                // Check Path Is Valid or Invalid and Add if exists
                                connect(addButton,&QPushButton::clicked,this,[=,&dialog]() {
                                                                                                QString path;
                                                                                                path = pathEdit->text();
                                                                                                if(path.trimmed().isEmpty()) {
                                                                                                                                SVGStudioMessages::invalidPath(this);
                                                                                                                                return;
                                                                                                                            }
                                                                                                QRegularExpression regex("^[A-Z]:[/\\\\].*");
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

                                                                                                // Frame - Get Card (Radio Button with Path + Remove)
                                                                                                pathCard = new QFrame;
                                                                                                pathCard->setMouseTracking(true);

                                                                                                // Layout - Card Vertical Layout
                                                                                                cardLayout = new QHBoxLayout;

                                                                                                // RadioButton - Path Radio Button
                                                                                                pathRadio = new QRadioButton(path);
                                                                                                pathRadio->setCursor(Qt::PointingHandCursor);
                                                                                                pathRadio->setToolTip(path);

                                                                                                // Button - Remove Button
                                                                                                removeButton = new QPushButton;
                                                                                                SetupRemoveButtonStates(removeButton);
                                                                                                removeButton->setIcon(QIcon(FilePaths::NormalRemoveButtonIconPath));
                                                                                                removeButton->setFixedSize(28,28);
                                                                                                removeButton->setIconSize(QSize(48,48));
                                                                                                removeButton->setStyleSheet(Style::RemoveButtonStyle());
                                                                                                removeButton->hide();
                                                                                                removeButton->installEventFilter(new RemoveButtonHoverFilter(removeButton));
                                                                                                connect(removeButton,&QPushButton::clicked,this,[=]() {
                                                                                                                                                        RemovePathCard(
                                                                                                                                                                            path,
                                                                                                                                                                            pathCard
                                                                                                                                                                        );
                                                                                                                                                        }
                                                                                                        );

                                                                                                // Edit Button 
                                                                                                editButton = new QPushButton;
                                                                                                editButton->setCursor(Qt::PointingHandCursor);
                                                                                                editButton->setToolTip("Edit Path");
                                                                                                editButton->setIcon(QIcon(FilePaths::editButtonIconPath));
                                                                                                editButton->setStyleSheet(Style::editButtnStyle());
                                                                                                editButton->setText("Edit");
                                                                                                connect(editButton,&QPushButton::clicked,this,[=]() {
                                                                                                                                                        ShowEditPathDialog(path,pathRadio);
                                                                                                                                                    }
                                                                                                        );
                                                                                                        
                                                                                                editButton->hide();

                                                                                                pathCard->setStyleSheet(Style::PathCardStyle());

                                                                                                cardLayout->addWidget(pathRadio);
                                                                                                cardLayout->addStretch();
                                                                                                cardLayout->addWidget(editButton);
                                                                                                cardLayout->addWidget(removeButton);

                                                                                                pathCard->setLayout(cardLayout);
                                                                                                pathCard->installEventFilter(new PathCardHoverFilter(
                                                                                                                                                        editButton,
                                                                                                                                                        removeButton
                                                                                                                                                    )
                                                                                                                            );
                                                                                                                            
                                                                                                pathGroup->addButton(pathRadio);
                                                                                                savedPathsLayout->addWidget(pathCard);

                                                                                                pathRadio->setChecked(true);
                                                                                                SVGStudioDataManager::AddPath(path);
                                                                                                SVGStudioMessages::Success(this);
                                                                                                dialog.accept();
                                                                                            }
                                        );

                                dialog.exec();
                            }

    // Path Edit Window
    void ShowEditPathDialog(QString oldPath,QRadioButton* pathRadio) {
                                                                        QHBoxLayout *buttonLayout;
                                                                        buttonLayout = new QHBoxLayout;
                                                                        QVBoxLayout *mainLayout;
                                                                        mainLayout = new QVBoxLayout;

                                                                        QDialog dialog(this);
                                                                        dialog.setWindowTitle("Edit Path");
                                                                        dialog.resize(500,150);

                                                                        QLineEdit *pathEdit;
                                                                        pathEdit = new QLineEdit;
                                                                        pathEdit->setText(oldPath);
                                                                        pathEdit->setPlaceholderText(oldPath);

                                                                        QPushButton *browseButton;
                                                                        browseButton = new QPushButton("Browse");
                                                                        browseButton->setCursor(Qt::PointingHandCursor);
                                                                        browseButton->setToolTip("Browse Paths");

                                                                        QPushButton *addButton;
                                                                        addButton = new QPushButton("Save");
                                                                        addButton->setCursor(Qt::PointingHandCursor);
                                                                        addButton->setToolTip("Save Path");

                                                                        QPushButton *cancelButton;
                                                                        cancelButton = new QPushButton("Cancel");
                                                                        cancelButton->setCursor(Qt::PointingHandCursor);
                                                                        cancelButton->setToolTip("Cancel");

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

                                                                        // Check Path Is Valid or Invalid and Add if exists
                                                                        connect(addButton,&QPushButton::clicked,this,[=,&dialog]() {
                                                                                                                                        QString path;
                                                                                                                                        path = pathEdit->text();
                                                                                                                                        if(path.trimmed().isEmpty()) {
                                                                                                                                                                        SVGStudioMessages::invalidPath(this);
                                                                                                                                                                        return;
                                                                                                                                                                    }
                                                                                                                                        QRegularExpression regex("^[A-Z]:[/\\\\].*");
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

                                                                                                                                        // RadioButton - Path Radio Button
                                                                                                                                        pathRadio->setText(path);
                                                                                                                                        pathRadio->setToolTip(path);
 
                                                                                                                                        SVGStudioDataManager::RemovePath(oldPath);
                                                                                                                                        pathRadio->setText(path);
                                                                                                                                        pathRadio->setToolTip(path);
                                                                                                                                        SVGStudioDataManager::AddPath(path);
                                                                                                                                        SVGStudioMessages::Success(this);
                                                                                                                                        dialog.accept();
                                                                                                                                    }
                                                                                );

                                                                        dialog.exec();
                                                                    }

    void RemovePathCard(QString path,QFrame* pathCard) {
                                                            QMessageBox::StandardButton reply;
                                                            reply = SVGStudioMessages::ConfirmRemovePath(
                                                                                                            this,
                                                                                                            path
                                                                                                        );

                                                            if(reply == QMessageBox::Yes) {
                                                                                                SVGStudioDataManager::RemovePath(path);
                                                                                                savedPathsLayout->removeWidget(pathCard);
                                                                                                delete pathCard;
                                                                                            }
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

                            connect(recentHistoryToggle,&SVGStudioToggle::toggled,this,[=](bool checked) {
                                                                                                                SVGStudioDataManager::SetRecentHistoryEnabled(checked);
                                                                                                                recentHistoryStatusLabel->setText(checked
                                                                                                                                                            ? "Collect Recent History"
                                                                                                                                                            : "Do Not Collect Recent History"
                                                                                                                                                        );

                                                                                                                maximumRecentHistoryCombo->setEnabled(checked);
                                                                                                            }
                                    );

                            // connect - Disable Button(s) while tohggle OFF
                            connect(svgFileIconsToggle,&SVGStudioToggle::toggled,this,[=](bool checked) {
                                                                                                            SVGStudioDataManager::SetSvgFileIconsEnabled(checked);
                                                                                                            darkPreviewButton->setEnabled(checked);
                                                                                                            darkApplyButton->setEnabled(checked);

                                                                                                            lightPreviewButton->setEnabled(checked);
                                                                                                            lightApplyButton->setEnabled(checked);

                                                                                                            systemPreviewButton->setEnabled(checked);
                                                                                                            systemApplyButton->setEnabled(checked);
                                                                                                        }
                                    );

                            connect(darkPreviewButton,&QPushButton::clicked,this,[=]() {
                                                                                            ShowIconPreview(FilePaths::DarkModeSvgFileIconPath);
                                                                                        }
                                    );

                            connect(lightPreviewButton,&QPushButton::clicked,this,[=]() {
                                                                                            ShowIconPreview(FilePaths::LightModeSvgFileIconPath);
                                                                                        }
                                    );

                            connect(systemPreviewButton,&QPushButton::clicked,this,[=]() {
                                                                                                ShowSystemPreview();
                                                                                            }
                                    );

                            // connect - Apply Dark App File Icon in file Explorer
                            connect(darkApplyButton,&QPushButton::clicked,this,[=]() {
                                                                                        QString darkIco = QCoreApplication::applicationDirPath() + "/DARK_MODE_SVG_FILE_ICON.ico";
                                                                                        SVGStudioDataManager::SetSvgIconMode("dark");
                                                                                        ExplorerIconManager::ApplySvgIcon(
                                                                                                                            QDir::toNativeSeparators(darkIco)
                                                                                                                        );
                                                                                        QMessageBox::information(
                                                                                                                    this,
                                                                                                                    "SVG Studio",
                                                                                                                    "Dark File Icon Applied"
                                                                                                                );
                                                                                    }
                                    );

                            // connect - Appply Light Mod eApp File Icon in File explorer
                            connect(lightApplyButton,&QPushButton::clicked,this,[=]() {
                                                                                           SVGStudioDataManager::SetSvgIconMode("light");
                                                                                            ExplorerIconManager::ApplyLightIcon();
                                                                                            QMessageBox::information(
                                                                                                                        this,
                                                                                                                        "SVG Studio",
                                                                                                                        "Light File Icon Applied"
                                                                                                                    );
                                                                                        }
                                    );

                            // connect - Aply Light / Dark SVG File Icon  on svg files  accoridng os
                            connect(systemApplyButton,&QPushButton::clicked,this,[=]() {
                                                                                            Automate::SystemIconApply(this);
                                                                                        }
                                    );
                            
                            connect(maximumRecentHistoryCombo,&QComboBox::currentTextChanged,this,[&](const QString &text) {
                                                                                                                                if(text == "Custom...") {
                                                                                                                                                            return;
                                                                                                                                                        }

                                                                                                                                SVGStudioDataManager::SetMaximumRecentHistory(
                                                                                                                                                                                text.toInt()
                                                                                                                                                                            );
                                                                                                                            }
                                    );

                            connect(defaultSvgAppToggle,&SVGStudioToggle::toggled,this,[=](bool checked) {
                                                                                                            SVGStudioDataManager::SetDefaultSvgAppEnabled(checked);
                                                                                                            if(checked) {
                                                                                                                DefaultAppManager::SetAsDefault();
                                                                                                                QMessageBox::information(
                                                                                                                                            this,
                                                                                                                                            "SVG Studio",
                                                                                                                                            "SVG Studio is now default SVG app."
                                                                                                                                        );
                                                                                                            }
                                                                                                            else {
                                                                                                                    DefaultAppManager::RemoveDefaultApp();
                                                                                                                    QMessageBox::information(
                                                                                                                                                this,
                                                                                                                                                "SVG Studio",
                                                                                                                                                "Default SVG app disabled."
                                                                                                                                            );
                                                                                                                }
                                                                                                        }
                                    );
                        }

    void openSettings() {
                            exec();
                        }
};

class SVGStudioWelcomePage : public QWidget {
private:
    QPushButton *newFileButton;
    QPushButton *openFileButton;
    QPushButton *openFolderButton;
    QPushButton *convertToSvgButton;
    QLabel *welcomeToTheSvgStudioLabel;
    QLabel *welcomePageStartLabel;
    SVGStudioButtonLogic *buttonLogic;
    QTabWidget *tabWidget;
    QFrame *dragOverlay;
    QLabel *dragOverlayLabel;
    QSvgWidget *dragAnimation;
    QLabel *dragAnimationLabel;
    QMovie *dragAnimationMovie;
    QLabel *recentFilesLabel;
    QVBoxLayout *recentFilesLayout;
    QVBoxLayout *rightLayout;
    QVBoxLayout *leftLayout;

public:
    SVGStudioWelcomePage(SVGStudioButtonLogic *buttonLogic,QTabWidget *tabWidget,QWidget *parent = nullptr): QWidget(parent) {
                                                                                                                                    this->buttonLogic = buttonLogic;
                                                                                                                                    this->tabWidget = tabWidget;
                                                                                                                                    setAcceptDrops(true);
                                                                                                                                    CreateWidgets();
                                                                                                                                    CreateLayouts();
                                                                                                                                    CreateConnections();
                                                                                                                                    CreateDragOverlay();
                                                                                                                                    LoadRecentFiles();
                                                                                                                                }

private:
    QMap<QWidget*,QPushButton*> removeButtons;
    void LoadRecentFiles() {QLayoutItem *item;

    while((item = recentFilesLayout->takeAt(0))) {
                                                    if(item->widget()) {
                                                                            item->widget()->deleteLater();
                                                                        }

                                                    delete item;
                                                }

                                QStringList recentFiles = SVGStudioDataManager::GetRecentFiles();
                                for(QString path:recentFiles) {
                                                                    if(!QFile::exists(path)) {
                                                                                                SVGStudioDataManager::RemoveRecentFile(path);
                                                                                                continue;
                                                                                            }
                                                                    QWidget *rowWidget;
                                                                    rowWidget = new QWidget;

                                                                    QHBoxLayout *rowLayout;
                                                                    rowLayout = new QHBoxLayout(rowWidget);
                                                                    rowLayout->setContentsMargins(0,0,0,0);

                                                                    QPushButton *fileButton;
                                                                    fileButton = new QPushButton(QFileInfo(path).fileName());
                                                                    fileButton->setIcon(QIcon(FilePaths::DarkModeSvgFileICOIcon));
                                                                    fileButton->setToolTip(path);
                                                                    fileButton->setCursor(Qt::PointingHandCursor);
                                                                    fileButton->setStyleSheet(Style::recentFileNameStyle());
                                                                    QObject::connect(fileButton,&QPushButton::clicked,this,[=]() {
                                                                                                                                        SVGStudioButtonLogic::OpenSvgInTab(
                                                                                                                                                                                path,
                                                                                                                                                                                tabWidget
                                                                                                                                                                            );
                                                                                                                                    }
                                                                                    );

                                                                    QPushButton *removeButton;
                                                                    removeButton = new QPushButton();
                                                                    removeButton->setCursor(Qt::PointingHandCursor);
                                                                    removeButton->setToolTip("Remove from Recents");
                                                                    removeButton->setIcon(QIcon(FilePaths::NormalRemoveButtonIconPath));
                                                                    removeButton->setIconSize(QSize(48,48));
                                                                    removeButton->setFixedSize(24,24);
                                                                    removeButton->installEventFilter(new RemoveButtonHoverFilter(removeButton));
                                                                    removeButton->hide();
                                                                    removeButton->setStyleSheet(Style::RemoveButtonStyle());
                                                                    QObject::connect(removeButton,&QPushButton::clicked,this,[=]() {
                                                                                                                                        SVGStudioDataManager::RemoveRecentFile(path);
                                                                                                                                        recentFilesLayout->removeWidget(rowWidget);
                                                                                                                                        rowWidget->deleteLater();
                                                                                                                                    }
                                                                                    );

                                                                    rowLayout->addWidget(fileButton);
                                                                    rowLayout->addSpacing(8);
                                                                    rowLayout->addWidget(removeButton);
                                                                    rowLayout->addStretch();

                                                                    recentFilesLayout->addWidget(rowWidget);
                                                                    removeButtons[rowWidget] = removeButton;
                                                                    rowWidget->installEventFilter(this);
                                                                    fileButton->installEventFilter(this);
                                                                }
                            }

    void CreateWidgets() {
                            // Label - Add a  Label of Welcome To The Svg  Studio
                            welcomeToTheSvgStudioLabel = new QLabel("Welcome To The SVG Studio");
                            welcomeToTheSvgStudioLabel->setStyleSheet(Style::welcomeToTheSvgStudioLabelStyle());

                            welcomePageStartLabel =new QLabel("Open, Preview and Manage SVG Files");
                            welcomePageStartLabel->setStyleSheet(Style::welcomePageStartLabelStyle());

                            // Label - Recent Label
                            recentFilesLabel = new QLabel("Recent Fles");
                            recentFilesLayout = new QVBoxLayout();

                            // Button - New File... Button
                            newFileButton = new QPushButton("New File...");
                            newFileButton->setToolTip("Create New SVG File");
                            newFileButton->setCursor(Qt::PointingHandCursor);
                            newFileButton->setIcon(QIcon(FilePaths::newFileIconPath));
                            newFileButton->setIconSize(QSize(32,32));
                            newFileButton->setStyleSheet(Style::welcomeButtonStyle());
                            QObject::connect(newFileButton,&QPushButton::clicked,this,[=](){
                                                                                                buttonLogic->newFileButtonLogic(
                                                                                                                                    this,tabWidget
                                                                                                                                );
                                                                                            }
                                            );

                            // Button - Open File... Button
                            openFileButton = new QPushButton("Open File...");
                            openFileButton->setToolTip("Open SVG File");
                            openFileButton->setCursor(Qt::PointingHandCursor);
                            openFileButton->setIcon(QIcon(FilePaths::openFileIconPath));
                            openFileButton->setIconSize(QSize(32,32));
                            openFileButton->setStyleSheet(Style::welcomeButtonStyle());
                            QObject::connect(openFileButton,&QPushButton::clicked,this,[=](){
                                                                                                buttonLogic->openFileButtonLogic(
                                                                                                                                    this,tabWidget
                                                                                                                                );
                                                                                            }
                                            );

                            // Button - Open Folder... Button// Button - New File... Button
                            openFolderButton =new QPushButton("Open Folder...");
                            openFolderButton->setToolTip("Open all SVG(s) From Folder");
                            openFolderButton->setCursor(Qt::PointingHandCursor);
                            openFolderButton->setIcon(QIcon(FilePaths::openFolderIconPath));
                            openFolderButton->setIconSize(QSize(32,32));
                            openFolderButton->setStyleSheet(Style::welcomeButtonStyle());
                            QObject::connect(openFolderButton,&QPushButton::clicked,this,[=](){
                                                                                                    buttonLogic->openFolderButtonLogic(
                                                                                                                                            this,
                                                                                                                                            tabWidget
                                                                                                                                        );
                                                                                                }
                                            );

                            convertToSvgButton =new QPushButton("Convert to SVG...");
                            convertToSvgButton->setToolTip("Convert to SVG File");
                            convertToSvgButton->setCursor(Qt::PointingHandCursor);
                            convertToSvgButton->setIcon(QIcon(FilePaths::convertToSvgIconPath));
                            convertToSvgButton->setIconSize(QSize(32,32));
                            convertToSvgButton->setStyleSheet(Style::welcomeButtonStyle());
                            QObject::connect(convertToSvgButton,&QPushButton::clicked,this,[=]() {
                                                                                                    buttonLogic->convertToSvgButtonLogic();
                                                                                                }
                                            );
                        }

    void CreateLayouts() {
                            QVBoxLayout *welcomeLayout;
                            welcomeLayout = new QVBoxLayout;

                            QHBoxLayout *welcomeMainLayout;
                            welcomeMainLayout = new QHBoxLayout;

                            leftLayout = new QVBoxLayout;
                            leftLayout->addStretch();
                            leftLayout->addWidget(newFileButton);
                            leftLayout->addWidget(openFileButton);
                            leftLayout->addWidget(openFolderButton);
                            leftLayout->addWidget(convertToSvgButton);

                            rightLayout = new QVBoxLayout;
                            rightLayout->addWidget(recentFilesLabel);
                            rightLayout->addLayout(recentFilesLayout);
                            rightLayout->addStretch();

                            welcomeLayout->addWidget(welcomeToTheSvgStudioLabel);
                            welcomeLayout->addWidget(welcomePageStartLabel);
                            welcomeMainLayout->addLayout(leftLayout,1);
                            welcomeMainLayout->addLayout(rightLayout,1);
                            welcomeLayout->addLayout(welcomeMainLayout);
                            setLayout(welcomeLayout);
                        }

    void CreateConnections() {
                            }

    void CreateDragOverlay() {
                                dragOverlay = new QFrame(this);
                                dragAnimationLabel = new QLabel(dragOverlay);
                                dragAnimationLabel->setGeometry(dragOverlay->rect());
                                dragAnimationLabel->setScaledContents(true);

                                dragAnimationMovie = new QMovie(FilePaths::DragDropOfMoreThanThreeSvgsAnimationGifPath);
                                dragAnimationLabel->setMovie(dragAnimationMovie);
                                dragAnimationMovie->start();
                                
                                dragOverlay->setGeometry(rect());
                                dragOverlay->raise();
                                dragOverlay->setStyleSheet("background-color:black;");
                                dragOverlay->hide();
                            }

protected:
    // Result - Show the Drag And Drop Animation While file seems  Dropping and tells Drop Here
    void dragEnterEvent(QDragEnterEvent *event) override {
                                                            if(event->mimeData()->hasUrls()) {
                                                                                                QList<QUrl> urls;
                                                                                                urls = event->mimeData()->urls();
                                                                                                int fileCount;
                                                                                                fileCount = urls.count();
                                                                                                if(fileCount == 1) {
                                                                                                                        dragAnimationMovie->stop();
                                                                                                                        dragAnimationMovie->setFileName(FilePaths::DragDropOfOneSvgsAnimationGifPath);
                                                                                                                        dragAnimationMovie->start();
                                                                                                                    }
                                                                                                else if(fileCount == 2) {
                                                                                                                            dragAnimationMovie->stop();
                                                                                                                            dragAnimationMovie->setFileName(FilePaths::DragDropOfTwoSvgsAnimationGifPath);
                                                                                                                            dragAnimationMovie->start();
                                                                                                                        }
                                                                                                else {
                                                                                                        dragAnimationMovie->stop();
                                                                                                        dragAnimationMovie->setFileName(FilePaths::DragDropOfMoreThanThreeSvgsAnimationGifPath);
                                                                                                        dragAnimationMovie->start();
                                                                                                    }

                                                                                                event->acceptProposedAction();
                                                                                                dragOverlay->show();
                                                                                            }
                                                            }

    // Result - +Hide The Animation While File Didn't Uploaded
    void dragLeaveEvent(QDragLeaveEvent *event) override {
                                                            dragOverlay->hide();
                                                            event->accept();
                                                        }
    
    // Result - Stop the Drag Drop Animation  then Add Tabs or Tab
    void dropEvent(QDropEvent *event) override {
                                                    dragOverlay->hide();
                                                    QList<QUrl> urls = event->mimeData()->urls();
                                                    for(QUrl url : urls) {
                                                                            QString filePath;
                                                                            filePath = url.toLocalFile();
                                                                            SVGStudioButtonLogic::OpenSvgInTab(
                                                                                                                filePath,
                                                                                                                tabWidget
                                                                                                            );
                                                                        }
                                                }

    void resizeEvent(QResizeEvent *event) override {
                                                        QWidget::resizeEvent(event);
                                                        dragOverlay->setGeometry(rect());
                                                        dragAnimationLabel->setGeometry(dragOverlay->rect());
                                                    }
                                            
    // Remove Button Hide and show
    bool eventFilter(QObject *obj,QEvent *event) override {
                                                                QWidget *widget = qobject_cast<QWidget*>(obj);
                                                                if(widget && removeButtons.contains(widget)) {
                                                                                                                    if(event->type() == QEvent::Enter) {
                                                                                                                                                            removeButtons[widget]->show();
                                                                                                                                                        }

                                                                                                                    if(event->type() == QEvent::Leave) {
                                                                                                                                                            removeButtons[widget]->hide();
                                                                                                                                                        }
                                                                                                                }

                                                                return QWidget::eventFilter(obj,event);
                                                            }

    void showEvent(QShowEvent *event) override {
                                                    QWidget::showEvent(event);
                                                    LoadRecentFiles();
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
                                                                                                                                                                                                                                    SVGStudioButtonLogic::OpenSvgInTab(
                                                                                                                                                                                                                                                                            filePath,
                                                                                                                                                                                                                                                                            tabWidget
                                                                                                                                                                                                                                                                        );
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
    static void nextTabShortcut(QWidget *window,QTabWidget *tabWidget) {
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
    // Creation - Ctrl + Shift + Tab Shortcut
    static void previousTabShortcut(QWidget *window,QTabWidget *tabWidget) {
                                                                                QShortcut *previousTabShortcut;
                                                                                previousTabShortcut = new QShortcut(QKeySequence("Ctrl + Shift + Tab"),window);
                                                                                QObject::connect(previousTabShortcut,&QShortcut::activated,window,[=]() {
                                                                                                                                                            if(tabWidget->count() > 1) {
                                                                                                                                                                                            int previousIndex;
                                                                                                                                                                                            previousIndex = (tabWidget->currentIndex() - 1 + tabWidget->count())% tabWidget->count();
                                                                                                                                                                                            tabWidget->setCurrentIndex(previousIndex);
                                                                                                                                                                                        }
                                                                                                                                                        }
                                                                                                );
                                                                            }

    // Creation - Ctrl + Shift + T Shortcut
   static void restoreClosedTabShortcut(QWidget *window,QTabWidget *tabWidget,QStack<QString> *closedTabs) {
                                                                                                            QShortcut *restoreShortcut;
                                                                                                            restoreShortcut = new QShortcut(QKeySequence("Ctrl + Shift + T"),window);
                                                                                                            QObject::connect(restoreShortcut,&QShortcut::activated,window,[=]() {
                                                                                                                                                                                    if (!closedTabs->isEmpty()) {
                                                                                                                                                                                                                    QString filePath;
                                                                                                                                                                                                                    filePath = closedTabs->pop();
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

    // Creation - Ctrl + Shift + N Shortcut
    static void NewWelcomeTabShortcut(QWidget *window,QTabWidget *tabWidget,SVGStudioButtonLogic *buttonLogic) {
                                                                                                                    QShortcut *newTabShortcut;
                                                                                                                    newTabShortcut = new QShortcut(QKeySequence("Ctrl+Shift+N"),window);
                                                                                                                    QObject::connect(newTabShortcut,&QShortcut::activated,window,[=]() {
                                                                                                                                                                                            QWidget *welcomePage;
                                                                                                                                                                                            welcomePage = new SVGStudioWelcomePage(buttonLogic,tabWidget,window);
                                                                                                                                                                                            tabWidget->addTab(welcomePage,"Welcome");
                                                                                                                                                                                            tabWidget->setCurrentWidget(welcomePage);
                                                                                                                                                                                        }
                                                                                                                                    );
                                                                                                                }

    // Creation - Ctrl + Alt + N Shortcut
    static void createNewWindowShortcut(QWidget *window,std::function<void() > createWindow) {
                                                                                                QShortcut *shortcut;
                                                                                                shortcut = new QShortcut(QKeySequence("Ctrl + Alt + N"),window);
                                                                                                QObject::connect(shortcut,&QShortcut::activated,window,[=]() {
                                                                                                                                                                createWindow();
                                                                                                                                                            }
                                                                                                                );
                                                                                            }

    // Creation - Ctrl + Shift + W Shortcut
    static void closeWindowShortcut(QWidget *window) {
                                                        QShortcut *shortcut;
                                                        shortcut = new QShortcut(QKeySequence("Ctrl+Shift+W"),window);
                                                        QObject::connect(shortcut,&QShortcut::activated,window,[=]() {
                                                                                                                            window->close();
                                                                                                                        }
                                                                        );
                                                    }
    // Creation - Ctrl + D Shortcut
    static void duplicateCurrentTabShortcut(QWidget *window,QTabWidget *tabWidget) {
                                                                                        QShortcut *duplicateTabShortcut;
                                                                                        duplicateTabShortcut = new QShortcut(QKeySequence("Ctrl + D"),window);
                                                                                        QObject::connect(duplicateTabShortcut,&QShortcut::activated,window,[=]() {
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
                                                                                    }

    // Creation - F2 Shortcut
    static void renameCurrentTabShortcut(QWidget *window,QTabWidget *tabWidget) {
                                                                                    QShortcut *renameShortcut;
                                                                                    renameShortcut = new QShortcut(QKeySequence(Qt::Key_F2),window);
                                                                                    QObject::connect(renameShortcut,&QShortcut::activated,window,[=]() {
                                                                                                                                                            int index;
                                                                                                                                                            index = tabWidget->currentIndex();
                                                                                                                                                            if (index == -1) {
                                                                                                                                                                                return;
                                                                                                                                                                            }
                                                                                                                                                            QString newName;
                                                                                                                                                            newName = QInputDialog::getText(window,"Rename Tab","New Name");
                                                                                                                                                            if (!newName.isEmpty()) {
                                                                                                                                                                                        tabWidget->setTabText(index,newName);
                                                                                                                                                                                    }
                                                                                                                                                        }
                                                                                                    );
                                                                                }

    // Creation - Ctrl + N Shortcut
    static void newFileShortcut(QWidget *window,SVGStudioButtonLogic *buttonLogic,QTabWidget *tabWidget) {
                                                                                                            QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl + N"), window);
                                                                                                            QObject::connect(shortcut,&QShortcut::activated,window,[=]() {
                                                                                                                                                                            buttonLogic->newFileButtonLogic(
                                                                                                                                                                                                                window,
                                                                                                                                                                                                                tabWidget
                                                                                                                                                                                                            );
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

    QTabWidget* GetTabWidget() {
                                    return tabWidget;
                                }

    void CreateCentralWidget() {
                                    centralWidget = new QWidget;

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
                                // Open SVG(s) in Tabs (Ctrl + O)
                                SVGStudioShortcuts::openFilesShortcut(openAction,this,&logic,tabWidget);

                                // connection - closing App (Ctrl + Q)
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
                                SVGStudioShortcuts::createTabNumberShortcut(
                                                                                this,
                                                                                tabWidget
                                                                            );
                            
                                // call - Ctrl + W shortvut
                                SVGStudioShortcuts::closeCurrentTabShortcut(this,
                                                                                    tabWidget,
                                                                                    &closedTabs
                                                                                );

                                // Call - Ctrl + Tab shortcut
                                SVGStudioShortcuts::nextTabShortcut(
                                                                        this,
                                                                        tabWidget
                                                                    );

                                // call - Ctrl + Shift + Tab shortcut
                                SVGStudioShortcuts::previousTabShortcut(
                                                                            this,
                                                                            tabWidget
                                                                        );

                                // Call - Ctrl + Shift + T Shortcut
                                SVGStudioShortcuts::restoreClosedTabShortcut(
                                                                                this,
                                                                                tabWidget,
                                                                                &closedTabs
                                                                            );

                                //  Call - Ctrl + Shift + N shortcut
                                SVGStudioShortcuts::NewWelcomeTabShortcut(
                                                                            this,
                                                                            tabWidget,
                                                                            &buttonLogic
                                                                        );

                                // Call - Ctrl + Alt + N Shortcut
                                SVGStudioShortcuts::createNewWindowShortcut(this,[]() {
                                                                                            SVGStudioGui *newWindow;
                                                                                            newWindow = new SVGStudioGui;
                                                                                            newWindow->show();
                                                                                        }
                                                                            );

                                // Call - Ctrl + Shift + W Shortcut
                                SVGStudioShortcuts::closeWindowShortcut(
                                                                            this
                                                                        );

                                // Call - Ctrl + D Shortcut
                                SVGStudioShortcuts::duplicateCurrentTabShortcut(
                                                                                    this,
                                                                                    tabWidget
                                                                                );

                            // Call - F2 Shortcut
                            SVGStudioShortcuts::renameCurrentTabShortcut(
                                                                            this,
                                                                            tabWidget
                                                                        );

                            // Call - Ctrl + N Shortcut
                            SVGStudioShortcuts::newFileShortcut(
                                                                    this,
                                                                    &buttonLogic,
                                                                    tabWidget
                                                                );
                            }

    void CreateWidgets() {
                            tabWidget = new QTabWidget;
                            welcomePage = new SVGStudioWelcomePage(&buttonLogic,tabWidget,this);
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
};

// Calls App
int main(int argc, char *argv[]) {
                                    QApplication app(argc, argv);
                                    QString startupFile;

                                    if(argc > 1) {
                                                        startupFile = argv[1];
                                                    }
                                    QLocalSocket socket;
                                    socket.connectToServer(SVGStudioInstanceManager::ServerName);
                                    // Condition - Chek if Ouur App Already opened  or not
                                    if(socket.waitForConnected(100)) {
                                                                        if(!startupFile.isEmpty()) {
                                                                                                        socket.write(startupFile.toUtf8());
                                                                                                        socket.flush();
                                                                                                        socket.waitForBytesWritten();
                                                                                                    }

                                                                        return 0;
                                                                    }

                                    // QString startupFile;
                                    // if(argc > 1) {
                                    //                     startupFile = argv[1];
                                    //                 }
                                    SVGStudioGui studio;
                                    studio.show();
                                    QLocalServer *server;
                                    server = new QLocalServer;
                                    server->listen(SVGStudioInstanceManager::ServerName);
                                    QObject::connect(server,&QLocalServer::newConnection,[&]() {
                                                                                                    QLocalSocket *client;
                                                                                                    client = server->nextPendingConnection();
                                                                                                    client->waitForReadyRead();
                                                                                                    QString filePath;
                                                                                                    filePath = QString::fromUtf8(client->readAll());
                                                                                                    SVGStudioButtonLogic::OpenSvgInTab(filePath,studio.GetTabWidget());
                                                                                                    client->deleteLater();
                                                                                                }
                                                    );

                                    if(!startupFile.isEmpty()) {
                                        SVGStudioButtonLogic::OpenSvgInTab(
                                                                                startupFile,
                                                                                studio.GetTabWidget()
                                                                            );
                                                                }
                                    return app.exec();
                                }

#include "SVG_Studio.moc"