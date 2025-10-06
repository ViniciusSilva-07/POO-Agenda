
#pragma once
#include "IView.h"
#include <memory>
#include <string>
#include <windows.h>

class Controller;

class GUIView : public IView {
public:
    GUIView();
    ~GUIView();

    GUIView(const GUIView&) = delete;
    GUIView& operator=(const GUIView&) = delete;

    void setController(std::shared_ptr<Controller> c);
    void run();
    void displayMessage(const std::string& msg) override;

private:
    void createControls();
    void refreshList();

    HWND hwndMain = nullptr;
    HWND hwndList = nullptr;
    HWND hwndTitle = nullptr;
    HWND hwndStartDate = nullptr;
    HWND hwndStartTime = nullptr;
    HWND hwndEndDate = nullptr;
    HWND hwndEndTime = nullptr;
    HWND hwndTags = nullptr;
    HWND hwndStatus = nullptr;
    HWND hwndMonthLabel = nullptr;
    std::weak_ptr<Controller> controller;
    std::string lastMessage;

    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};