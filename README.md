# qtBrowser

A simple browser demo implemented by qt

## QWebEngineView 无法捕捉 MouseEvent

QWebEngineView 基本不捕获任何事件（除了鼠标进入和退出， 最近又添加了键盘事件），
几乎所有的事件（类似鼠标移动或绘制）都由 QWebEngineView 私有类型的子委托
RenderWidgetHostViewDelegateWidget 负责处理，该私有类型派生自 QOpenGLWidget。

因此 QT 原生的 installEventFilter 和 eventFilter 对 QWebEngineView 无效，
如果想要模拟事件发给 QWebEngineView，可以将事件发送给 focusProxy。

- [How can I get paint events with QtWebEngine?](https://stackoverflow.com/questions/30566101/how-can-i-get-paint-events-with-qtwebengine)
- [QWebEngineView的鼠标事件的处理](https://blog.csdn.net/wjjontheway/article/details/121808858)
- [QWebEngineView 过滤事件](https://blog.csdn.net/moyayi/article/details/119646294)
- [QWebEngineView not send to eventFilter QEvent::MouseMove](https://bugreports.qt.io/browse/QTBUG-60701)

## 托盘 {#sys-tray-icon}

- [QT6 System Tray Icon Example](https://doc.qt.io/qt-6/qtwidgets-desktop-systray-example.html)

## 多语言

lupdate lrelease

https://doc.qt.io/qt-6/qtlinguist-index.html

使用 qmake 没有问题，使用 cmake 的时候 lupdate 会报错:

```
Starting external tool "C:\Qt\6.4.2\msvc2019_64\bin\lupdate.exe C:/Users/Jerry/Documents/QtProjects/test/CMakeLists.txt"
lupdate warning: no TS files specified. Only diagnostics will be produced.

"C:\Qt\6.4.2\msvc2019_64\bin\lupdate.exe" finished
```

正常情况应该是:

```
Starting external tool "C:\Qt\6.4.2\msvc2019_64\bin\lupdate.exe C:/Users/Jerry/Documents/QtProjects/multilanguages/multilanguages.pro"
Info: creating stash file C:\Users\Jerry\Documents\QtProjects\multilanguages\.qmake.stash

Updating 'multilanguages_zh_CN.ts'...
    Found 2 source text(s) (0 new and 2 already existing)

"C:\Qt\6.4.2\msvc2019_64\bin\lupdate.exe" finished
```

按照 [qt6 LinguistTools 官方文档](https://doc.qt.io/qt-6/cmake-commands-qtlinguisttools.html) 修改还是这样。
后来我看到 [Qt Forum](https://forum.qt.io/topic/141260/cmake-cannot-find-ts-file) 要使用 cmake 的编译参数'update_translations'，

```
C:\Qt\Tools\CMake_64\bin\cmake.exe -DUPDATE_TRANSLATIONS=ON -S C:\Users\Jerry\Documents\QtProjects\test\ -B C:\Users\Jerry\Documents\QtProjects\build-test-Desktop_Qt_6_4_2_MSVC2019_64bit-Debug\ 
C:\Qt\Tools\CMake_64\bin\cmake.exe --build C:\Users\Jerry\Documents\QtProjects\build-test-Desktop_Qt_6_4_2_MSVC2019_64bit-Debug\
```

这几条命令没有作用，我又发现 Qt creator -> 项目 -> 构建设置 -> CMake Configuration 里面有一项 "UPDATE_TRANSLATIONS" 可以勾选，我选中后还是无法生成qm文件。

放弃了，Qt 有这个bug，只有自家的qmake配置才能使用。或者手动执行 lupdate lrelease 命令。

```
PS C:\Users\Jerry\Documents\QtProjects\test> C:\Qt\6.4.2\msvc2019_64\bin\lupdate.exe --help
Usage:
    lupdate [options] [project-file]...
    lupdate [options] [source-file|path|@lst-file]... -ts ts-files|@lst-file

lupdate is part of Qt's Linguist tool chain. It extracts translatable
messages from Qt UI files, C++, Java and JavaScript/QtScript source code.
Extracted messages are stored in textual translation source files (typically
Qt TS XML). New and modified messages can be merged into existing TS files.

Passing .pro files to lupdate is deprecated.
Please use the lupdate-pro tool instead.

Options:
    -help  Display this information and exit.
    -no-obsolete
           Drop all obsolete and vanished strings.
    -extensions <ext>[,<ext>]...
           Process files with the given extensions only.
           The extension list must be separated with commas, not with whitespace.
           Default: 'java,jui,ui,c,c++,cc,cpp,cxx,ch,h,h++,hh,hpp,hxx,js,qs,qml,qrc'.
    -pluralonly
           Only include plural form messages.
    -silent
           Do not explain what is being done.
    -no-sort
           Do not sort contexts in TS files.
    -no-recursive
           Do not recursively scan directories.
    -recursive
           Recursively scan directories (default).
    -I <includepath> or -I<includepath>
           Additional location to look for include files.
           May be specified multiple times.
    -locations {absolute|relative|none}
           Specify/override how source code references are saved in TS files.
           absolute: Source file path is relative to target file. Absolute line
                     number is stored.
           relative: Source file path is relative to target file. Line number is
                     relative to other entries in the same source file.
           none: no information about source location is stored.
           Guessed from existing TS files if not specified.
           Default is absolute for new files.
    -no-ui-lines
           Do not record line numbers in references to UI files.
    -disable-heuristic {sametext|similartext|number}
           Disable the named merge heuristic. Can be specified multiple times.
    -project <filename>
           Name of a file containing the project's description in JSON format.
           Such a file may be generated from a .pro file using the lprodump tool.
    -pro <filename>
           Name of a .pro file. Useful for files with .pro file syntax but
           different file suffix. Projects are recursed into and merged.
           This option is deprecated. Use the lupdate-pro tool instead.
    -pro-out <directory>
           Virtual output directory for processing subsequent .pro files.
    -pro-debug
           Trace processing .pro files. Specify twice for more verbosity.
    -source-language <language>[_<region>]
           Specify the language of the source strings for new files.
           Defaults to POSIX if not specified.
    -target-language <language>[_<region>]
           Specify the language of the translations for new files.
           Guessed from the file name if not specified.
    -tr-function-alias <function>{+=,=}<alias>[,<function>{+=,=}<alias>]...
           With +=, recognize <alias> as an alternative spelling of <function>.
           With  =, recognize <alias> as the only spelling of <function>.
           Available <function>s (with their currently defined aliases) are:
             Q_DECLARE_TR_FUNCTIONS (=Q_DECLARE_TR_FUNCTIONS)
             QT_TR_N_NOOP (=QT_TR_N_NOOP)
             QT_TRID_N_NOOP (=QT_TRID_N_NOOP)
             QT_TRANSLATE_N_NOOP (=QT_TRANSLATE_N_NOOP)
             QT_TRANSLATE_N_NOOP3 (=QT_TRANSLATE_N_NOOP3)
             QT_TR_NOOP (=QT_TR_NOOP)
             QT_TRID_NOOP (=QT_TRID_NOOP)
             QT_TRANSLATE_NOOP (=QT_TRANSLATE_NOOP)
             QT_TRANSLATE_NOOP3 (=QT_TRANSLATE_NOOP3)
             QT_TR_NOOP_UTF8 (=QT_TR_NOOP_UTF8)
             QT_TRANSLATE_NOOP_UTF8 (=QT_TRANSLATE_NOOP_UTF8)
             QT_TRANSLATE_NOOP3_UTF8 (=QT_TRANSLATE_NOOP3_UTF8)
             findMessage (=findMessage)
             qtTrId (=qtTrId)
             tr (=tr)
             trUtf8 (=trUtf8)
             translate (=translate)
             qsTr (=qsTr)
             qsTrId (=qsTrId)
             qsTranslate (=qsTranslate)
    -ts <ts-file>...
           Specify the output file(s). This will override the TRANSLATIONS.
    -version
           Display the version of lupdate and exit.
    -clang-parser [compilation-database-dir]
           Use clang to parse cpp files. Otherwise a custom parser is used.
           This option needs a clang compilation database (compile_commands.json)
           for the files that needs to be parsed.
           The path to the directory containing this file can be specified on the
           command line, directly after the -clang-parser option, or in the .pro file
           by setting the variable LUPDATE_COMPILE_COMMANDS_PATH.
           A directory specified on the command line takes precedence.
           If no path is given, the compilation database will be searched
           in all parent paths of the first input file.
    -project-roots <directory>...
           Specify one or more project root directories.
           Only files below a project root are considered for translation when using
           the -clang-parser option.
    @lst-file
           Read additional file names (one per line) or includepaths (one per
           line, and prefixed with -I) from lst-file.
PS C:\Users\Jerry\Documents\QtProjects\test>
PS C:\Users\Jerry\Documents\QtProjects\test> C:\Qt\6.4.2\msvc2019_64\bin\lrelease.exe --help
Usage:
    lrelease [options] -project project-file
    lrelease [options] ts-files [-qm qm-file]

lrelease is part of Qt's Linguist tool chain. It can be used as a
stand-alone tool to convert XML-based translations files in the TS
format into the 'compiled' QM format used by QTranslator objects.

Passing .pro files to lrelease is deprecated.
Please use the lrelease-pro tool instead, or use qmake's lrelease.prf
feature.

Options:
    -help  Display this information and exit
    -idbased
           Use IDs instead of source strings for message keying
    -compress
           Compress the QM files
    -nounfinished
           Do not include unfinished translations
    -removeidentical
           If the translated text is the same as
           the source text, do not include the message
    -markuntranslated <prefix>
           If a message has no real translation, use the source text
           prefixed with the given string instead
    -project <filename>
           Name of a file containing the project's description in JSON format.
           Such a file may be generated from a .pro file using the lprodump tool.
    -silent
           Do not explain what is being done
    -version
           Display the version of lrelease and exit
PS C:\Users\Jerry\Documents\QtProjects\test>
```

通过查看命令行发现，Qt Creator 执行外部工具 lupdate 时没有指定 .ts 输出文件，lupdate 没有从 CMakeLists.txt 中取出 .ts 输出文件，
才导致报错“lupdate warning: no TS files specified. Only diagnostics will be produced.”。建议手动执行命令:

```shell
PS C:\Users\Jerry\Desktop> C:\Qt\6.4.2\msvc2019_64\bin\lupdate.exe C:\Users\Jerry\Documents\QtProjects\test\ -ts C:\Users\Jerry\Documents\QtProjects\test\test_zh_CN.ts
Scanning directory 'C:\Users\Jerry\Documents\QtProjects\test\'...
Updating '../Documents/QtProjects/test/test_zh_CN.ts'...
    Found 2 source text(s) (2 new and 0 already existing)
PS C:\Users\Jerry\Documents\QtProjects\test> C:\Qt\6.4.2\msvc2019_64\bin\lrelease.exe .\test_zh_CN.ts
Updating '.\test_zh_CN.qm'...
    Generated 1 translation(s) (1 finished and 0 unfinished)
    Ignored 1 untranslated source text(s)
```

```cmake
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools WebEngineWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools WebEngineWidgets)

#get_target_property(QT6_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
#message(${QT6_QMAKE_EXECUTABLE})
#get_target_property(Qt6_LUPDATE_EXECUTABLE Qt6::lupdate IMPORTED_LOCATION)
#message(${Qt6_LUPDATE_EXECUTABLE})

set(TS_FILES i18n/DeepLink_zh_CN.ts)
set(QM_FILES i18n/DeepLink_zh_CN.qm)

set(PROJECT_SOURCES
        main.cpp
        main_window.cpp
        main_window.h
        ${TS_FILES}
        bridge.h bridge.cpp
        webview.h webview.cpp
        devtools_window.h devtools_window.cpp
)
#qt_add_resources(PROJECT_SOURCES DeepLink.qrc)

set(RC_FILES ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.rc)
set(TRAY_ICON_FILE ${CMAKE_SOURCE_DIR}/deeplink.ico)

if (WIN32)
    set(ICON_FILE ${CMAKE_SOURCE_DIR}/deeplink.ico)
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/windows_metafile.rc.in"
        "${RC_FILES}"
    )
elseif (APPLE)
    set(ICON_FILE ${CMAKE_SOURCE_DIR}/deeplink.icns)
endif()

# genearats ts and qm file searching recursively SRC_DIR
macro(generate_translations)
    message("macro begin")
    add_custom_command(OUTPUT ${TS_FILES}
        COMMAND ${Qt6_LUPDATE_EXECUTABLE} -recursive ${PROJECT_SOURCE_DIR} -ts ${TS_FILES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Update language")

    add_custom_command(OUTPUT ${QM_FILES}
        COMMAND ${Qt6_LRELEASE_EXECUTABLE} ${TS_FILES}
#        MAIN_DEPENDENCY ${TS_FILES}
#        DEPENDS ${TS_FILES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generate qm resources")
    message("macro end")
endmacro()
#generate_translations()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RC_FILES}
    )

    qt_create_translation(QM_FILES ${PROJECT_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${RC_FILES}
    )

    qt5_create_translation(QM_FILES ${PROJECT_SOURCE_DIR} ${TS_FILES})
endif()


#qt_add_resources(${PROJECT_NAME} "systray"
#    PREFIX "/"
#    FILES
#        image/deeplink.ico
#)

qt_add_resources(${PROJECT_NAME} "i18n"
    PREFIX "/"
    FILES
        ${QM_FILES}
)
```

- [CMake 3.17+ deletes Qt translations when updating the TS files](https://gitlab.kitware.com/cmake/cmake/-/issues/21549)
- [QT6.3 CMake 多语言切换](https://blog.csdn.net/xjcwzp/article/details/125995814)

## 参考资料 {#references}

- [Qt Forum](https://forum.qt.io/)
