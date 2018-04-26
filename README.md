# qt-translation-example

Use menu to switch from German and English

Tested on macOS and Android.

Based on this example:
* https://wiki.qt.io/How_to_create_a_multi_language_application

## Working with Qt Translation
1) Add languages you wish to support to your Qt project file (*.pro) and run qmake again
````
TRANSLATIONS += \
    languages/TranslationExample_en.ts  \
    languages/TranslationExample_de.ts
````

2) Use Qt lupdate to auto-generate your inital ts files.
   - From Qt Creator menu select:
       - Tools > External > Linguist > Update Translations (lupdate)

3) For each ````<message>```` found in each TSL file translate the ````<source>```` text and paste into ````<translation>````
    - Manually change ````translation/@type```` attribute from "unfinished" to "finished".
    - Or delete ````translation/@type```` attribute after translation.  
    - When you run lupdate again, it will removed all ````translation/@type=finished```` attributes
        - Missing type attribute is same as "finished"
    - For small one-by-word projects, use free web browser Google Transalte.
        - https://translate.google.com/?
    - I made this node.js script that requires a commercial Google Trnaslate API key:
        - https://github.com/esutton/i18n-translate-qt-ts

4) Use Qt lrelease to generate compressed *.qm files from translated *.ts files
    - From Qt Creator menu select:
        - Tools > External > Linguist > Release Translations (lrelease)

5) Copy *.qm files to the embedded resource folder ./res/translation
   - ToDo: Add code to *.pro file to copy *.qm to ./res/translation

## Automated Transaltion Using Google Translate

I made this node.js script that requires a commercial Google Translate API key:
- https://github.com/esutton/i18n-translate-qt-ts