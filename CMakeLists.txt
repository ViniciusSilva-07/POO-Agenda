cmake_minimum_required(VERSION 3.10)
project(Agenda)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Diretórios com os headers
include_directories(src/model src/controller src/view src/persistence src/utils)

# Adiciona todos os arquivos .cpp necessários
add_executable(agenda 
    src/main.cpp
    src/model/Calendar.cpp
    src/model/Event.cpp
    src/model/RecurrenceRuleDaily.cpp
    src/model/User.cpp
    src/controller/Controller.cpp
    src/view/GUIView.cpp
    src/persistence/JSONPersistence.cpp
    src/utils/TimeUtils.cpp
    src/utils/UtilTemplates.cpp
)
if (WIN32)
    target_link_libraries(agenda PRIVATE comdlg32)
endif()