QT += testlib network  # network добавлен, так как это часть клиент-серверного приложения

CONFIG += c++17 cmdline  # cmdline для явного указания консольного приложения

SOURCES += \
    test_hash.cpp

HEADERS += \
    sha1.hpp

# Настройки деплоя (для тестов обычно не нужны, но пусть будут)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
