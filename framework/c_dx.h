//
// Created by ruppet on 2/4/2020.
//

#ifndef uc-sflying-loader-c-dx-h
#define uc-sflying-loader-c-dx-h


extern LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

class c_dx {
public:
    bool setup();
    bool render();

    void push_resize(ImVec2 size) { should_resize = true; next_size = size; }
    void resize(ImVec2 size);

    c_ui* ui = nullptr;
    c_ui_window* info_window = nullptr;
    c_ui_window* auth_window = nullptr;
    c_ui_window* main_window = nullptr;

private:
    IDirect3D9Ex* directx = nullptr;
    IDirect3DDevice9Ex* device = nullptr;

    bool should_resize = false;
    ImVec2 next_size;
};


#endif //uc-sflying-loader-c-dx-h
