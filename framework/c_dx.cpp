//
// Created by ruppet on 2/4/2020.
//

#include "../loader.h"
#include "c_dx.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
    if (!core->is_setup)
        return DefWindowProcA(hwnd, msg, w, l);

    static auto getwindowrect = api(GetWindowRect);
    static auto getcursorpos = api(GetCursorPos);

    static auto can_drag = [hwnd]() -> bool {
        RECT rect;
        if (!getwindowrect(hwnd, &rect))
            return false;

        POINT cur;
        getcursorpos(&cur);

        auto y = cur.y - rect.top;
        auto x = cur.x - rect.left;
        return y <= 20 && x <= (rect.right - rect.left) - 20;
    };

    ImGui_ImplWin32_WndProcHandler(hwnd, msg, w, l);

    if (msg == WM_CLOSE || msg == WM_DESTROY)
        exit(0);
    if (msg == WM_NCHITTEST && can_drag())
        return HTCAPTION;

    return DefWindowProcA(hwnd, msg, w, l);
}

bool c_dx::render() {
    if (!device || !directx) return false;

    device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);
    device->BeginScene();

    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX9_NewFrame();
    ImGui::NewFrame();

    if (ui)
        ui->draw();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    device->EndScene();
    device->Present(0, 0, 0, 0);

    if (should_resize) {
        resize(next_size);
        should_resize = false;
    }

    return true;
}

bool c_dx::setup() {
    Direct3DCreate9Ex(D3D_SDK_VERSION, &directx);
    if (!directx) {
        core->last_error = str("Unexpected error 20. Please contact support");
        return false;
    }

    D3DPRESENT_PARAMETERS pp{};
    pp.Windowed = true;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = core->window;

    directx->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, core->window, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &pp, 0, &device);

    if (!device) {
        core->last_error = str("Unexpected error 21. Please contact support");
        return false;
    }

    ImGui::CreateContext();
    ImGui_ImplWin32_Init(core->window);
    ImGui_ImplDX9_Init(device);

    ui = new c_ui;
    ui->setup();

    info_window = new c_ui_window;
    info_window->setup(str("uc-sflying"));
    info_window->set_pos(ImVec2(0, 0));
    info_window->set_size(ImVec2(300, 200));
    info_window->set_active(true);
    {
        static auto spinner = new c_ui_spinner;
        spinner->setup(str("Loading spinner"), device);
        spinner->set_pos(ImVec2(138.f, 88.f));

        static auto status = new c_ui_label;
        status->setup(str("Status"));
        status->set_text(str("Connecting..."));
        status->set_pos(ImVec2(7.f, 195.f - status->get_size().y));

        info_window->add(spinner);
        info_window->add(status);
    }

    auth_window = new c_ui_window;
    auth_window->setup(str("uc-sflying"));
    auth_window->set_pos(ImVec2(0, 0));
    auth_window->set_size(ImVec2(300, 200));
    {
        static std::string un;
        static std::string pw;

        static auto username = new c_ui_textbox;
        username->setup(str("Username"), &un, 32);
        username->set_pos(ImVec2(55, 50));

        static auto password = new c_ui_textbox;
        password->setup(str("Password"), &pw, 64);
        password->set_pos(ImVec2(55, 92));
        password->set_password(true);

        static auto signin = new c_ui_button;
        signin->setup(str("Sign in"));
        signin->set_pos(ImVec2(102, 140));
        signin->set_size(ImVec2(100.f, 25.f));
        signin->set_callback([&](c_ui_element*) {
            if (!un.empty() && !pw.empty()) {
                std::thread([&]() {
                    dx->info_window->set_active(true);
                    dx->auth_window->set_active(false);

                    ((c_ui_label *) dx->info_window->find(str("Status")))->set_text(str("Logging in..."));

                    utils->log(str("API: authorizing"));
                    if (!api->authenticate(un, pw))
                    {
                        if (core->last_error == str("Invalid")) {
                            dx->info_window->set_active(false);
                            dx->auth_window->set_active(true);

                            ui->add_message(str("Login failed"), str("Username or password is incorrect"));
                        } else
                            core->crash(core->last_error.c_str());
                    } else {
                        for (const auto& sub : api->subscriptions)
                            ((c_ui_products*)dx->main_window->find(str("Products")))->add(sub);

                        ((c_ui_label*)dx->main_window->find(str("Welcome")))->set_text(str("Welcome ") + un);
                        ((c_ui_products*)dx->main_window->find(str("Products")))->call_callback();

                        dx->push_resize(ImVec2(460, 300));
                        dx->info_window->set_active(false);
                        dx->main_window->set_active(true);
                    }

                    utils->log(str("API: authorized"));
                }).detach();
            }
        });

        auth_window->add(username);
        auth_window->add(password);
        auth_window->add(signin);
    }

    main_window = new c_ui_window;
    main_window->setup(str("uc-sflying"));
    main_window->set_pos(ImVec2(0, 0));
    main_window->set_size(ImVec2(460, 300));
    {
        static int selected_prod = 0;
        static std::unordered_map<int, bool> autorun_state;

        for (auto i = 0; i < 32; i++)
            autorun_state[i] = api(GetPrivateProfileIntA)(str("al"), std::to_string(i).c_str(), 0, str("c:/uc-sflying/ldr/_"));

        static auto spinner = new c_ui_spinner;
        spinner->setup(str("Spinner"), device);
        spinner->set_pos(ImVec2(218.f, 138.f));
        spinner->set_hidden(true);

        static auto subs = new c_ui_products;
        subs->setup(str("Products"), &selected_prod, device);
        subs->set_pos(ImVec2(20, 40));
        subs->set_size(ImVec2(240.f, 240.f));

        static auto welcome = new c_ui_label;
        welcome->setup(str("Welcome"));
        welcome->set_text(str("Welcome panzerfaust"));
        welcome->set_bold(true);
        welcome->set_pos(ImVec2(270, 40));

        static auto status = new c_ui_label;
        status->setup(str("Status"));
        status->set_text(str("Anti-cheat status: Undetected"));
        status->set_pos(ImVec2(270, 60));

        static auto expires = new c_ui_label;
        expires->setup(str("Expires"));
        expires->set_text(str("Expires: Never"));
        expires->set_pos(ImVec2(270, 78));

        static auto autorun = new c_ui_checkbox;
        autorun->setup(str("Auto-run game"), &autorun_state[selected_prod]);
        autorun->set_pos(ImVec2(270, 206));
        autorun->set_callback([&](c_ui_element*) {
            api(WritePrivateProfileStringA)(str("al"), std::to_string(api->subscriptions.at(selected_prod).product_id).c_str(),
                    std::to_string(autorun->get_value()).c_str(), str("c:/uc-sflying/ldr/_"));
        });

        static auto refresh = new c_ui_button;
        refresh->setup(str("Refresh"));
        refresh->set_pos(ImVec2(270, 228));
        refresh->set_size(ImVec2(170, 25));

        static auto load = new c_ui_button;
        load->setup(str("Load"));
        load->set_pos(ImVec2(270, 255));
        load->set_size(ImVec2(170, 25));
        load->set_callback([&](c_ui_element*) {
            spinner->set_hidden(false);
            subs->set_hidden(true);
            welcome->set_hidden(true);
            status->set_hidden(true);
            expires->set_hidden(true);
            autorun->set_hidden(true);
            load->set_hidden(true);
            refresh->set_hidden(true);

            std::thread([&]() {
                if (!api->load(api->subscriptions.at(selected_prod).product_id, selected_prod)) {
                    ui->add_message(str("Load failed"), core->last_error);

                    spinner->set_hidden(true);
                    subs->set_hidden(false);
                    welcome->set_hidden(false);
                    status->set_hidden(false);
                    expires->set_hidden(false);
                    autorun->set_hidden(false);
                    load->set_hidden(false);
                    refresh->set_hidden(false);
                } else
                    exit(0);
            }).detach();
        });

        refresh->set_callback([&](c_ui_element*) {
            spinner->set_hidden(false);
            subs->set_hidden(true);
            welcome->set_hidden(true);
            status->set_hidden(true);
            expires->set_hidden(true);
            autorun->set_hidden(true);
            load->set_hidden(true);
            refresh->set_hidden(true);

            std::thread([&]() {
                if (!api->refresh()) {
                    dx->push_resize(ImVec2(300, 200));
                    dx->main_window->set_active(false);
                    dx->auth_window->set_active(true);

                    subs->remove_all();
                } else {
                    subs->remove_all();

                    for (const auto& sub : api->subscriptions)
                        ((c_ui_products*)dx->main_window->find(str("Products")))->add(sub);
                    ((c_ui_products*)dx->main_window->find(str("Products")))->call_callback();
                }

                spinner->set_hidden(true);
                subs->set_hidden(false);
                welcome->set_hidden(false);
                status->set_hidden(false);
                expires->set_hidden(false);
                autorun->set_hidden(false);
                load->set_hidden(false);
                refresh->set_hidden(false);
            }).detach();
        });

        subs->set_callback([&](c_ui_element*) {
            auto s = api->subscriptions.at(selected_prod);

            if (s.status == 0 || s.status == 3)
                status->set_text(str("Anti-cheat status: unknown"));
            if (s.status == 1)
                status->set_text(str("Anti-cheat status: undetected"));
            if (s.status == 2)
                status->set_text(str("Anti-cheat status: detected"));

            expires->set_text(str("Expires: ") + s.expire_date);
            autorun->set_active(s.steamid != "" && s.status != 0 && s.last_update != str("N/A"));
            autorun->set_var(&autorun_state[s.product_id]);

            load->set_active(s.status != 0 && s.last_update != str("N/A"));
        });

        main_window->add(spinner);
        main_window->add(subs);
        main_window->add(welcome);
        main_window->add(status);
        main_window->add(expires);
        main_window->add(autorun);
        main_window->add(refresh);
        main_window->add(load);
    }

    ui->add(info_window);
    ui->add(auth_window);
    ui->add(main_window);

    api(SetLayeredWindowAttributes)(core->window, 0, 255, LWA_ALPHA);
    api(ShowWindow)(core->window, SW_SHOW);
    api(UpdateWindow)(core->window);

    api(SetFocus)(core->window);
    api(SetForegroundWindow)(core->window);

    return true;
}

void c_dx::resize(ImVec2 size) {
    RECT cur{};
    api(GetWindowRect)(core->window, &cur);
    api(SetWindowPos)(core->window, HWND_TOPMOST, cur.left - (size.x - (cur.right - cur.left)) / 2, cur.top - (size.y - (cur.bottom - cur.top)) / 2,
            size.x, size.y, 0);

    D3DPRESENT_PARAMETERS pp{};
    pp.Windowed = true;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = core->window;
    pp.BackBufferWidth = size.x;
    pp.BackBufferHeight = size.y;

    ImGui_ImplDX9_InvalidateDeviceObjects();
    device->Reset(&pp);
    ImGui_ImplDX9_CreateDeviceObjects();
}
