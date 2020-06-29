//
// Created by ruppet on 10.01.2020.
//

#ifndef uc-sflying-cm-_INPUT_H
#define uc-sflying-cm-_INPUT_H

class c_verifiedcmd {
public:
    c_usercmd cmd;
    unsigned long crc;
};

class c_input {
public:
    void*               pvftable;
    bool                is_ir_track_available;          //0x04
    bool                is_mouse_initiazted;          //0x05
    bool                is_mouse_active;               //0x06
    bool                is_joystick_initialized;      //0x07
    char                pad_0x08[0x2C];               //0x08
    void*               keys;                      //0x34
    char                pad_0x38[0x64];               //0x38
    int                    pad_0x41;
    int                    pad_0x42;
    bool                is_camera_intercepting_mouse;   //0x9C
    bool                is_camera_in_tp;       //0x9D
    bool                is_camera_moving_with_mouse;     //0x9E
    c_vector                camera_offset;            //0xA0
    bool                camera_distance_move;        //0xAC
    int                 camera_oldx;                //0xB0
    int                 camera_oldy;                //0xB4
    int                 camera_x;                   //0xB8
    int                 camera_y;                   //0xBC
    bool                is_ortho_camera;       //0xC0
    c_angle              prev_view_angles;      //0xC4
    c_angle              prev_tilted_angles;  //0xD0
    float               last_forward_move;          //0xDC
    int                 clear_input_state;           //0xE0
    char                pad_0xE4[0x8];                //0xE4
    c_usercmd*           commands;                  //0xEC
    c_verifiedcmd*    verified_commands;          //0xF0

    virtual	void		init_all( void ) = 0;
    virtual void		shutdown_all( void ) = 0;
    virtual int			get_button_bits( int ) = 0;
    virtual void		create_move ( int sequence_number, float input_sample_frametime, bool active ) = 0;
    virtual void		extra_mouse_sample( float frametime, bool active ) = 0;
    virtual bool		write_usercmd_delta_to_buffer( c_buffer *buf, int from, int to, bool isnewcommand ) = 0;
    virtual void		encode_usercmd_delta_to_buffer( c_buffer& buf, int slot ) = 0;
    virtual void		decode_usercmd_delta_to_buffer( c_buffer& buf, int slot ) = 0;
    virtual c_usercmd*  get_user_cmd(int nSlot, int sequence_number) = 0;
};

#endif //uc-sflying-cm-_INPUT_H
