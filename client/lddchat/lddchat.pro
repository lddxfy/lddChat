#-------------------------------------------------
#
# Project created by QtCreator 2024-09-02T17:09:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = lddchat
TEMPLATE = app
DESTDIR = ./bin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    logindialog.cpp \
    registerdialog.cpp \
    global.cpp \
    httpmgr.cpp \
    timerbtn.cpp \
    clickedlabel.cpp \
    resetdialog.cpp \
    tcpmgr.cpp \
    usermgr.cpp \
    chatdialog.cpp \
    clickedbtn.cpp \
    customizeedit.cpp \
    chatuserlist.cpp \
    chatuserwid.cpp \
    listitembase.cpp \
    loadingdlg.cpp \
    chatpage.cpp \
    chatview.cpp \
    chatitembase.cpp \
    bubbleframe.cpp \
    textbubble.cpp \
    picturebubble.cpp \
    messagetextedit.cpp \
    statewidget.cpp \
    searchlist.cpp \
    adduseritem.cpp \
    findsuccessdlg.cpp \
    clickedoncelabel.cpp \
    friendlabel.cpp \
    applyfriend.cpp \
    conuseritem.cpp \
    grouptipitem.cpp \
    contactuserlist.cpp \
    applyfriendpage.cpp \
    applyfrienditem.cpp \
    applyfriendlist.cpp \
    findfaildlg.cpp \
    authenfriend.cpp \
    friendinfopage.cpp

HEADERS += \
        mainwindow.h \
    logindialog.h \
    registerdialog.h \
    global.h \
    singleton.h \
    httpmgr.h \
    timerbtn.h \
    clickedlabel.h \
    resetdialog.h \
    tcpmgr.h \
    usermgr.h \
    chatdialog.h \
    clickedbtn.h \
    customizeedit.h \
    chatuserlist.h \
    chatuserwid.h \
    listitembase.h \
    loadingdlg.h \
    chatpage.h \
    chatview.h \
    chatitembase.h \
    bubbleframe.h \
    textbubble.h \
    picturebubble.h \
    messagetextedit.h \
    statewidget.h \
    searchlist.h \
    userdata.h \
    adduseritem.h \
    findsuccessdlg.h \
    clickedoncelabel.h \
    friendlabel.h \
    applyfriend.h \
    conuseritem.h \
    grouptipitem.h \
    contactuserlist.h \
    applyfriendpage.h \
    applyfrienditem.h \
    applyfriendlist.h \
    findfaildlg.h \
    authenfriend.h \
    friendinfopage.h

FORMS += \
        mainwindow.ui \
    logindialog.ui \
    registerdialog.ui \
    resetdialog.ui \
    chatdialog.ui \
    chatuserwid.ui \
    loadingdlg.ui \
    chatpage.ui \
    adduseritem.ui \
    findsuccessdlg.ui \
    friendlabel.ui \
    applyfriend.ui \
    conuseritem.ui \
    grouptipitem.ui \
    applyfriendpage.ui \
    applyfrienditem.ui \
    findfaildlg.ui \
    authenfriend.ui \
    friendinfopage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc

DISTFILES += \
    config.ini

CONFIG(debug, debug | release) {
    #指定要拷贝的文件目录为工程目录下release目录下的所有dll、lib文件，例如工程目录在D:\QT\Test
    #PWD就为D:/QT/Test，DllFile = D:/QT/Test/release/*.dll
    TargetConfig = $${PWD}/config.ini
    #将输入目录中的"/"替换为"\"
    TargetConfig = $$replace(TargetConfig, /, \\)
    #将输出目录中的"/"替换为"\"
    OutputDir =  $${OUT_PWD}/$${DESTDIR}
    OutputDir = $$replace(OutputDir, /, \\)
    //执行copy命令
    QMAKE_POST_LINK += copy /Y \"$$TargetConfig\" \"$$OutputDir\" &
    # 首先，定义static文件夹的路径
    StaticDir = $${PWD}/static
    # 将路径中的"/"替换为"\"
    StaticDir = $$replace(StaticDir, /, \\)
    #message($${StaticDir})
    # 使用xcopy命令拷贝文件夹，/E表示拷贝子目录及其内容，包括空目录。/I表示如果目标不存在则创建目录。/Y表示覆盖现有文件而不提示。
    QMAKE_POST_LINK += xcopy /Y /E /I \"$$StaticDir\" \"$$OutputDir\\static\\\"
}else{
      #release
    message("release mode")
    #指定要拷贝的文件目录为工程目录下release目录下的所有dll、lib文件，例如工程目录在D:\QT\Test
    #PWD就为D:/QT/Test，DllFile = D:/QT/Test/release/*.dll
    TargetConfig = $${PWD}/config.ini
    #将输入目录中的"/"替换为"\"
    TargetConfig = $$replace(TargetConfig, /, \\)
    #将输出目录中的"/"替换为"\"
    OutputDir =  $${OUT_PWD}/$${DESTDIR}
    OutputDir = $$replace(OutputDir, /, \\)
    //执行copy命令
    QMAKE_POST_LINK += copy /Y \"$$TargetConfig\" \"$$OutputDir\"
    # 首先，定义static文件夹的路径
    StaticDir = $${PWD}/static
    # 将路径中的"/"替换为"\"
    StaticDir = $$replace(StaticDir, /, \\)
    #message($${StaticDir})
    # 使用xcopy命令拷贝文件夹，/E表示拷贝子目录及其内容，包括空目录。/I表示如果目标不存在则创建目录。/Y表示覆盖现有文件而不提示。
     QMAKE_POST_LINK += xcopy /Y /E /I \"$$StaticDir\" \"$$OutputDir\\static\\\"
}
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" /utf-8
