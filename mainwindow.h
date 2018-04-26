#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTranslator>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

// https://wiki.qt.io/How_to_create_a_multi_language_application
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    // this event is called, when a new translator is loaded or the system language is changed
    void changeEvent(QEvent*);

protected slots:
    // this slot is called by the language menu actions
    void slotLanguageChanged(QAction* action);


private:
    // loads a language by the given language shortcur (e.g. de, en)
    void loadLanguage(const QString& rLanguage);

    // creates the language menu dynamically from the content of m_langPath
    void createLanguageMenu(void);

    Ui::MainWindow *ui;

    QTranslator m_translator; // contains the translations for this application
    QTranslator m_translatorQt; // contains the translations for qt

    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to /languages.
};

#endif // MAINWINDOW_H
