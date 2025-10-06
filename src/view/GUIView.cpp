#include "GUIView.h"
#include <windows.h>
#include <string>
#include <sstream>
#include <commdlg.h>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include "../controller/Controller.h"

const char* WIN_CLASS = "AgendaGUIClass";

// control IDs
constexpr int ID_TITLE = 1101;
constexpr int ID_START_DATE = 1102;
constexpr int ID_START_TIME = 1103;
constexpr int ID_END_DATE = 1104;
constexpr int ID_END_TIME = 1105;
constexpr int ID_TAGS = 1106;
constexpr int ID_LIST = 1001;
constexpr int ID_CREATE = 2001;
constexpr int ID_REFRESH = 2002;
constexpr int ID_SAVE = 2003;
constexpr int ID_LOAD = 2004;
constexpr int ID_EXIT = 2005;
constexpr int ID_LIST_BY_MONTH = 2006;

GUIView::GUIView() {}
GUIView::~GUIView() {}

void GUIView::setController(std::shared_ptr<Controller> c) {
    controller = c;
}

void GUIView::displayMessage(const std::string& msg) {
    lastMessage = msg;
    if (hwndMain) {
        MessageBoxA(hwndMain, msg.c_str(), "Mensagem", MB_OK | MB_ICONINFORMATION);
    }
}


// create UI controls including calendar grid
void GUIView::createControls() {
    // determine client size and center all controls
    RECT cr; GetClientRect(hwndMain, &cr);
    int clientW = cr.right - cr.left;
    // content width used for form and list
    const int contentW = 560;
    int rx = (clientW - contentW) / 2;
    if (rx < 10) rx = 10;

    // header label centered
    hwndMonthLabel = CreateWindowA("STATIC", "Agenda", WS_CHILD | WS_VISIBLE | SS_CENTER,
                                   rx, 10, contentW, 25, hwndMain, NULL, GetModuleHandle(NULL), NULL);

    // form fields (positions relative to rx)
    CreateWindowA("STATIC", "Titulo:", WS_CHILD | WS_VISIBLE, rx, 40, 60, 20, hwndMain, NULL, GetModuleHandle(NULL), NULL);
    hwndTitle = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rx + 70, 40, contentW - 80, 20, hwndMain, (HMENU)ID_TITLE, GetModuleHandle(NULL), NULL);

    CreateWindowA("STATIC", "Data Inicio (DD-MM-YYYY):", WS_CHILD | WS_VISIBLE, rx, 70, 200, 20, hwndMain, NULL, GetModuleHandle(NULL), NULL);
    hwndStartDate = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, rx + 210, 70, 120, 20, hwndMain, (HMENU)ID_START_DATE, GetModuleHandle(NULL), NULL);
    CreateWindowA("STATIC", "Hora Inicio (HH:MM):", WS_CHILD | WS_VISIBLE, rx, 100, 150, 20, hwndMain, NULL, GetModuleHandle(NULL), NULL);
    hwndStartTime = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, rx + 210, 100, 120, 20, hwndMain, (HMENU)ID_START_TIME, GetModuleHandle(NULL), NULL);

    CreateWindowA("STATIC", "Data Fim (DD-MM-YYYY):", WS_CHILD | WS_VISIBLE, rx, 130, 200, 20, hwndMain, NULL, GetModuleHandle(NULL), NULL);
    hwndEndDate = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, rx + 210, 130, 120, 20, hwndMain, (HMENU)ID_END_DATE, GetModuleHandle(NULL), NULL);
    CreateWindowA("STATIC", "Hora Fim (HH:MM):", WS_CHILD | WS_VISIBLE, rx, 160, 150, 20, hwndMain, NULL, GetModuleHandle(NULL), NULL);
    hwndEndTime = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, rx + 210, 160, 120, 20, hwndMain, (HMENU)ID_END_TIME, GetModuleHandle(NULL), NULL);

    CreateWindowA("STATIC", "Tags (separadas por ,):", WS_CHILD | WS_VISIBLE, rx, 190, 200, 20, hwndMain, NULL, GetModuleHandle(NULL), NULL);
    hwndTags = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, rx + 210, 190, 260, 20, hwndMain, (HMENU)ID_TAGS, GetModuleHandle(NULL), NULL);

    hwndList = CreateWindowA("LISTBOX", nullptr,
                             WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER,
                             rx, 220, contentW, 220,
                             hwndMain, (HMENU)ID_LIST, GetModuleHandle(NULL), NULL);

    // buttons placed centered under the list
    int btnY = 460;
    CreateWindowA("BUTTON", "Criar", WS_CHILD | WS_VISIBLE,
                  rx + 20, btnY, 80, 25, hwndMain, (HMENU)ID_CREATE, GetModuleHandle(NULL), NULL);
        CreateWindowA("BUTTON", "Eventos", WS_CHILD | WS_VISIBLE,
                      rx + 110, btnY, 80, 25, hwndMain, (HMENU)ID_REFRESH, GetModuleHandle(NULL), NULL);
        CreateWindowA("BUTTON", "Eventos (Mes)", WS_CHILD | WS_VISIBLE,
                      rx + 200, btnY, 100, 25, hwndMain, (HMENU)ID_LIST_BY_MONTH, GetModuleHandle(NULL), NULL);
    CreateWindowA("BUTTON", "Salvar", WS_CHILD | WS_VISIBLE,
                  rx + 320, btnY, 80, 25, hwndMain, (HMENU)ID_SAVE, GetModuleHandle(NULL), NULL);
    CreateWindowA("BUTTON", "Carregar", WS_CHILD | WS_VISIBLE,
                  rx + 410, btnY, 80, 25, hwndMain, (HMENU)ID_LOAD, GetModuleHandle(NULL), NULL);
    CreateWindowA("BUTTON", "Sair", WS_CHILD | WS_VISIBLE,
                  rx + 500, btnY, 60, 25, hwndMain, (HMENU)ID_EXIT, GetModuleHandle(NULL), NULL);

    hwndStatus = CreateWindowA("STATIC", "Pronto", WS_CHILD | WS_VISIBLE, rx, btnY + 40, contentW, 20, hwndMain, NULL, GetModuleHandle(NULL), NULL);

    refreshList();
}

// Registration dialog removed: startup no longer asks for owner name/email.

// helper: format with zero padding
static std::string pad2(int v) {
    std::ostringstream ss; ss << std::setw(2) << std::setfill('0') << v; return ss.str();
}

void GUIView::refreshList() {
    if (!hwndList) return;
    SendMessageA(hwndList, LB_RESETCONTENT, 0, 0);
    auto c = controller.lock();
    if (!c) return;
    auto items = c->getEventStrings();
    for (auto &it : items) SendMessageA(hwndList, LB_ADDSTRING, 0, (LPARAM)it.c_str());
    std::ostringstream s;
    s << items.size() << " eventos";
    SetWindowTextA(hwndStatus, s.str().c_str());
}

LRESULT CALLBACK GUIView::StaticWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    GUIView* p = nullptr;
    if (msg == WM_NCCREATE) {
        CREATESTRUCTA* cs = (CREATESTRUCTA*)lp;
        p = (GUIView*)cs->lpCreateParams;
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)p);
        p->hwndMain = hwnd;
    } else {
        p = (GUIView*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
    }
    if (p) return p->WndProc(hwnd, msg, wp, lp);
    return DefWindowProcA(hwnd, msg, wp, lp);
}

// parse date in DD-MM-YYYY into (y,m,d). returns false if invalid
static bool parseDateDDMMYYYY(const std::string &s, int &y, int &m, int &d) {
    if (s.size() < 8) return false;
    int dd=0, mm=0, yyyy=0;
    char sep1=0, sep2=0;
    std::istringstream ss(s);
    ss >> dd >> sep1 >> mm >> sep2 >> yyyy;
    if (ss.fail() || sep1 != '-' || sep2 != '-') return false;
    if (yyyy < 1900 || mm < 1 || mm > 12 || dd < 1 || dd > 31) return false;
    y = yyyy; m = mm; d = dd; return true;
}

// parse time HH:MM simple validation
static bool parseTimeHHMM(const std::string &s, int &hh, int &mm) {
    int h=0, m=0; char sep=0;
    std::istringstream ss(s);
    ss >> h >> sep >> m;
    if (ss.fail() || sep != ':') return false;
    if (h < 0 || h > 23 || m < 0 || m > 59) return false;
    hh = h; mm = m; return true;
}

LRESULT CALLBACK GUIView::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            createControls();
            return 0;
        case WM_COMMAND: {
            int id = LOWORD(wp);
            if (id == ID_CREATE) {
                auto c = controller.lock(); if (!c) break;
                char buf[1024];
                std::string title, sd, st, ed, et, tags;
                GetWindowTextA(hwndTitle, buf, sizeof(buf)); title = buf;
                GetWindowTextA(hwndStartDate, buf, sizeof(buf)); sd = buf;
                GetWindowTextA(hwndStartTime, buf, sizeof(buf)); st = buf;
                GetWindowTextA(hwndEndDate, buf, sizeof(buf)); ed = buf;
                GetWindowTextA(hwndEndTime, buf, sizeof(buf)); et = buf;
                GetWindowTextA(hwndTags, buf, sizeof(buf)); tags = buf;

                // no calendar selected-day support anymore; require explicit date in DD-MM-YYYY
                if (sd.empty()) {
                    MessageBoxA(hwndMain, "Preencha a data de inicio no formato DD-MM-YYYY.", "Erro", MB_OK | MB_ICONERROR);
                    break;
                }

                int y1,m1,d1,hh1,mm1;
                if (!parseDateDDMMYYYY(sd, y1, m1, d1) || !parseTimeHHMM(st, hh1, mm1)) {
                    MessageBoxA(hwndMain, "Data ou hora de inicio invalida. Use DD-MM-YYYY e HH:MM", "Erro", MB_OK | MB_ICONERROR);
                    break;
                }
                int y2,m2,d2,hh2,mm2;
                if (!parseDateDDMMYYYY(ed, y2, m2, d2) || !parseTimeHHMM(et, hh2, mm2)) {
                    MessageBoxA(hwndMain, "Data ou hora de fim invalida. Use DD-MM-YYYY e HH:MM", "Erro", MB_OK | MB_ICONERROR);
                    break;
                }

                // convert to controller format YYYY-MM-DD HH:MM
                std::ostringstream sstart, send;
                sstart << std::setw(4) << std::setfill('0') << y1 << "-" << pad2(m1) << "-" << pad2(d1) << " " << pad2(hh1) << ":" << pad2(mm1);
                send << std::setw(4) << std::setfill('0') << y2 << "-" << pad2(m2) << "-" << pad2(d2) << " " << pad2(hh2) << ":" << pad2(mm2);

                std::vector<std::string> vt;
                if (!tags.empty()) {
                    std::istringstream ss(tags);
                    std::string tk;
                    while (std::getline(ss, tk, ',')) {
                        while (!tk.empty() && isspace((unsigned char)tk.front())) tk.erase(tk.begin());
                        while (!tk.empty() && isspace((unsigned char)tk.back())) tk.pop_back();
                        if (!tk.empty()) vt.push_back(tk);
                    }
                }

                c->createEvent(title, sstart.str(), send.str(), vt);
                refreshList();
                // clear input fields for the next entry
                SetWindowTextA(hwndTitle, "");
                SetWindowTextA(hwndStartDate, "");
                SetWindowTextA(hwndStartTime, "");
                SetWindowTextA(hwndEndDate, "");
                SetWindowTextA(hwndEndTime, "");
                SetWindowTextA(hwndTags, "");
                SetFocus(hwndTitle);
            } else if (id == ID_REFRESH) {
                refreshList();
            } else if (id == ID_LIST_BY_MONTH) {
                auto c = controller.lock(); if (!c) break;
                auto items = c->getEventStringsByMonth();
                SendMessageA(hwndList, LB_RESETCONTENT, 0, 0);
                for (auto &it : items) SendMessageA(hwndList, LB_ADDSTRING, 0, (LPARAM)it.c_str());
                // count only event lines (they start with "  - ") to report number of eventos
                int eventCount = 0;
                for (auto &it : items) {
                    if (it.rfind("  - ", 0) == 0) ++eventCount;
                }
                std::ostringstream s; s << eventCount << " eventos";
                SetWindowTextA(hwndStatus, s.str().c_str());
            } else if (id == ID_SAVE) {
                char filename[MAX_PATH] = "agenda.json";
                OPENFILENAMEA ofn = {0};
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFile = filename;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_OVERWRITEPROMPT;
                ofn.lpstrFilter = "JSON Files\0*.json\0All\0*.*\0";
                if (GetSaveFileNameA(&ofn)) {
                    auto c = controller.lock(); if (c) c->save(filename);
                }
            } else if (id == ID_LOAD) {
                char filename[MAX_PATH] = "agenda.json";
                OPENFILENAMEA ofn = {0};
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFile = filename;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
                ofn.lpstrFilter = "JSON Files\0*.json\0All\0*.*\0";
                if (GetOpenFileNameA(&ofn)) {
                    auto c = controller.lock(); if (c) c->load(filename);
                    refreshList();
                }
            } else if (id == ID_EXIT) {
                PostMessageA(hwnd, WM_CLOSE, 0, 0);
            }
            return 0;
        }
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, msg, wp, lp);
}

void GUIView::run() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = GUIView::StaticWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WIN_CLASS;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassExA(&wc);

    hwndMain = CreateWindowExA(0, WIN_CLASS, "Agenda POO - GUI",
                               WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 850, 600,
                               NULL, NULL, hInstance, this);
    if (!hwndMain) {
        DWORD err = GetLastError();
        char buf[256];
        std::snprintf(buf, sizeof(buf), "Falha ao criar janela principal. GetLastError=%lu", (unsigned long)err);
        MessageBoxA(NULL, buf, "Erro", MB_OK | MB_ICONERROR);
        return;
    }
    ShowWindow(hwndMain, SW_SHOW);
    // diagnostic: confirm window shown (temporary)
    MessageBoxA(hwndMain, "Agenda aberta (diagnóstico)", "Info", MB_OK | MB_ICONINFORMATION);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}
