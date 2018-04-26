#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QActionGroup>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createLanguageMenu();

    ui->menuBar->setNativeMenuBar(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// we create the menu entries dynamically, dependent on the existing translations.
void MainWindow::createLanguageMenu(void)
{

    QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);

    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

    // format systems language
    QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"

    // m_langPath = QApplication::applicationDirPath();
    // m_langPath.append("/languages");

    m_langPath = ":/resource";
    m_langPath.append("/translation");

    QDir dir(m_langPath);


    QStringList fileNames = dir.entryList(QStringList("TranslationExample_*.qm"));

    qDebug("%s", m_langPath.toLatin1().data());
    qDebug("exists %s", dir.exists() ? "true" : "false" );

    for (int i = 0; i < fileNames.size(); ++i) {
        // get locale extracted by filename
        QString locale;
        locale = fileNames[i]; // "TranslationExample_de.qm"

        QString resourceFileName = QString("%1/%2").arg(m_langPath).arg(locale);

        locale.truncate(locale.lastIndexOf('.')); // "TranslationExample_de"
        locale.remove(0, locale.indexOf('_') + 1); // "de"


        QString lang = QLocale::languageToString(QLocale(locale).language());
        QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

        QAction *action = new QAction(ico, lang, this);
        action->setCheckable(true);


        // action->setData(locale);
        action->setData(resourceFileName);

        ui->menuLanguage->addAction(action);
        langGroup->addAction(action);

        // set default translators and language checked
        if (defaultLocale == locale)
        {
            action->setChecked(true);
        }
    }
}

// Called every time, when a menu entry of the language menu is called
void MainWindow::slotLanguageChanged(QAction* action)
{
    if(0 != action) {
        // load the language dependant on the action content
        qDebug("action->data %s", action->data().toString().toLatin1().data());
        loadLanguage(action->data().toString());
        setWindowIcon(action->icon());
    }
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    bool result = translator.load(filename);
    qDebug("translator.load(%s) %s", filename.toLatin1().data(), result ? "true" : "false" );
    if(result) {
        qApp->installTranslator(&translator);
    }
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
    if(m_currLang != rLanguage) {
        m_currLang = rLanguage;
        QLocale locale = QLocale(m_currLang);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());

        // Why is this called twice?
//        switchTranslator(m_translator, QString("TranslationExample_%1.qm").arg(rLanguage));
//        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));

        // contains the translations for this application
        switchTranslator(m_translator, rLanguage);

        // contains the translations for qt
        switchTranslator(m_translatorQt, rLanguage);

        ui->statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if(0 != event) {
        switch(event->type()) {
        // this event is send if a translator is loaded
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;

            // this event is send, if the system, language changes
        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            loadLanguage(locale);
        }
            break;
        }
    }
    QMainWindow::changeEvent(event);
}
