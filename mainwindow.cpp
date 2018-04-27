#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QActionGroup>
#include <QDir>


void switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    bool result = translator.load(filename);
    qDebug("translator.load(%s) %s", filename.toLatin1().data(), result ? "true" : "false" );

    if(!result) {
        qWarning("*** Failed translator.load(\"%s\")", filename.toLatin1().data());
        return;
    }
    qApp->installTranslator(&translator);
}


// See: https://wiki.qt.io/How_to_create_a_multi_language_application
//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createLanguageMenu();

    // macOS setNativeMenuBar(false)
    ui->menuBar->setNativeMenuBar(false);

    QString resourceFileName = ":/resource/translation/TranslationExample_de.qm";
    switchTranslator(m_translator, resourceFileName);
     // ui->retranslateUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// we create the menu entries dynamically, dependent on the existing translations.
void MainWindow::createLanguageMenu(void)
{
    // format systems language
    QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"
    m_langPath = ":/resource";
    m_langPath.append("/translation");
    QDir dir(m_langPath);
    qDebug("exists %s ", dir.exists() ? "true" : "false" );

    // Create Language Menu to match qm translation files found
    QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);
    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));
    qDebug("%s", m_langPath.toLatin1().data());

    QStringList fileNames = dir.entryList(QStringList("TranslationExample_*.qm"));
    for (int i = 0; i < fileNames.size(); ++i) {
        // get locale extracted by filename
        QString locale;
        locale = fileNames[i]; // "TranslationExample_de.qm"

        locale.truncate(locale.lastIndexOf('.')); // "TranslationExample_de"
        locale.remove(0, locale.indexOf('_') + 1); // "de"

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

        QAction *action = new QAction(ico, lang, this);
        action->setCheckable(true);
        // action->setData(resourceFileName);
        action->setData(locale);

        ui->menuLanguage->addAction(action);
        langGroup->addAction(action);

        // set default translators and language checked
        if (defaultLocale == locale)
        {
            action->setChecked(true);
        }
    } // for: end
}

// Called every time, when a menu entry of the language menu is called
void MainWindow::slotLanguageChanged(QAction* action)
{
    if(0 == action) {
        return;
    }

    // load the language dependant on the action content
    qDebug("action->data %s", action->data().toString().toLatin1().data());
    loadLanguage(action->data().toString());
    setWindowIcon(action->icon());
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
    if(m_currLang == rLanguage) {
        return;
    }
    m_currLang = rLanguage;
    qDebug("loadLanguage %s", rLanguage.toLatin1().data());

    QLocale locale = QLocale(m_currLang);
    QLocale::setDefault(locale);
    QString languageName = QLocale::languageToString(locale.language());

    // m_translator contains the app's translations
    QString resourceFileName = QString("%1/TranslationExample_%2.qm").arg(m_langPath).arg(rLanguage);
    switchTranslator(m_translator, resourceFileName);

    // m_translatorQt contains the translations for qt
    // Why do I need this ? Common dialogs?
    //
    // http://doc.qt.io/qt-5/linguist-programmers.html
    // switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));

    ui->statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
}

void MainWindow::changeEvent(QEvent* event)
{
    if(0 != event) {
        switch(event->type()) {
        // this event is send if a translator is loaded
        case QEvent::LanguageChange:
            // UI will not update unless you call retranslateUi
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
