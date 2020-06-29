//
// Created by ruppet on 04.01.2020.
//

#ifndef uc-sflying-cm-_NETCHANNEL_H
#define uc-sflying-cm-_NETCHANNEL_H


class c_buffer {
public:
    void *data; //0x000C
    int32_t data_bytes; //0x0010
    int32_t data_bits; //0x0014
    int32_t cur_bit; //0x0018
    bool is_overflow; //0x001C
    bool assert_on_overflow; //0x001D
    char *debug_name; //0x001E
};

class c_netchannel;

class c_inetmessage
{
public:
    virtual	                ~c_inetmessage() {};
    virtual void	        set_net_channel(c_netchannel * netchan) = 0;
    virtual void	        set_reliable( bool state ) = 0;
    virtual bool	        process( void ) = 0;
    virtual	bool	        read_from_buffer( c_buffer &buffer ) = 0;
    virtual	bool	        write_to_buffer( c_buffer &buffer ) = 0;
    virtual bool	        is_reliable( void ) const = 0;
    virtual int				get_type( void ) const = 0;
    virtual int				get_group( void ) const = 0;
    virtual const char		*get_name( void ) const = 0;
    virtual c_netchannel    *get_net_channel( void ) const = 0;
    virtual const char		*to_string( void ) const = 0;
};

class c_netchannelinfo
{
public:
    enum {
        GENERIC = 0,
        LOCALPLAYER,
        OTHERPLAYERS,
        ENTITIES,
        SOUNDS,
        EVENTS,
        USERMESSAGES,
        ENTMESSAGES,
        VOICE,
        STRINGTABLE,
        MOVE,
        STRINGCMD,
        SIGNON,
        TOTAL,
    };

    virtual const char  *get_name( void ) const = 0;
    virtual const char  *get_address( void ) const = 0;
    virtual float		get_time( void ) const = 0;
    virtual float		get_time_connected( void ) const = 0;
    virtual int			get_buffer_size( void ) const = 0;
    virtual int			get_data_rate( void ) const = 0;

    virtual bool		is_loopback( void ) const = 0;
    virtual bool		is_timing_out( void ) const = 0;
    virtual bool		is_playback( void ) const = 0;

    virtual float		get_latency( int flow ) const = 0;
    virtual float		get_avg_latency( int flow ) const = 0;
    virtual float		get_avg_loss( int flow ) const = 0;
    virtual float		get_avg_choke( int flow ) const = 0;
    virtual float		get_avg_data( int flow ) const = 0;
    virtual float		get_avg_packets( int flow ) const = 0;
    virtual int			get_total_data( int flow ) const = 0;
    virtual int			get_sequence_nr( int flow ) const = 0;
    virtual bool		is_valid_packet( int flow, int frame_number ) const = 0;
    virtual float		get_packet_time( int flow, int frame_number ) const = 0;
    virtual int			get_packet_bytes( int flow, int frame_number, int group ) const = 0;
    virtual bool		get_stream_progress( int flow, int *received, int *total ) const = 0;
    virtual float		get_time_since_last_recv( void ) const = 0;
    virtual	float		get_cmd_interp_amount( int flow, int frame_number ) const = 0;
    virtual void		get_packet_resp_activity( int flow, int frame_number, int *pnLatencyMsecs, int *pnChoke ) const = 0;
    virtual void		get_remote_framerate( float *pflFrameTime, float *pflFrameTimeStdDeviation ) const = 0;

    virtual float		get_timeout_seconds() const = 0;
};

class c_netmessage : public c_inetmessage
{
public:
    c_netmessage() { m_is_reliable = true;
        m_net_channel = NULL; }

    virtual         ~c_netmessage() {};
    virtual int		get_group() const { return c_netchannelinfo::GENERIC; }
    c_netchannel	*get_net_channel() const { return m_net_channel; }
    virtual void	set_reliable( bool state) { m_is_reliable = state;};
    virtual bool	is_reliable() const { return m_is_reliable; };
    virtual void    set_net_channel(c_netchannel * netchan) { m_net_channel = netchan; }
    virtual bool	process() { Assert( 0 ); return false; };

    bool			m_is_reliable;
    c_netchannel	*m_net_channel;
};

class c_msg_move : public c_netmessage
{
public:
		bool			read_from_buffer( c_buffer &buffer );
		bool			write_to_buffer( c_buffer &buffer );
		const char		*to_string() const;
		int				get_type() const { return 9; }
		const char		*get_name() const { return "clc_Move"; }

	void *m_pMessageHandler;
	bool process() { return true; }
    int	get_group() const { return c_netchannelinfo::MOVE; }
    c_msg_move() { m_is_reliable = false; }

public:
    int				m_nBackupCommands;
    int				m_nNewCommands;
    int				m_nLength;
    c_buffer		m_DataIn;
    c_buffer		m_DataOut;
};

class c_netchannel : public c_netchannelinfo
{
public:
    virtual	        ~c_netchannel( void ) {};

    virtual void	set_data_rate(float rate) = 0;
    virtual bool	register_message(void *msg) = 0;
    virtual bool	start_streaming( unsigned int challengeNr ) = 0;
    virtual void	reset_streaming( void ) = 0;
    virtual void	set_timeout(float seconds) = 0;
    virtual void	set_demo_recorder(void *recorder) = 0;
    virtual void	set_challenge_nr(unsigned int chnr) = 0;

    virtual void	reset( void ) = 0;
    virtual void	clear( void ) = 0;
    virtual void	shutdown(const char *reason) = 0;

    virtual void	process_playback( void ) = 0;
    virtual bool	process_stream( void ) = 0;
    virtual void	process_packet( struct netpacket_s* packet, bool bHasHeader ) = 0;

    virtual bool	set_net_msg(void* msg, bool bForceReliable = false, bool bVoice = false ) = 0;
    virtual bool	send_data(void* msg, bool bReliable = true) = 0;
    virtual bool	send_file(const char *filename, unsigned int transferID) = 0;
    virtual void	deny_file(const char *filename, unsigned int transferID) = 0;
    virtual void	unused(const char *filename, unsigned int transferID) = 0;
    virtual void	set_choked( void ) = 0; // 45
    virtual int		send_datagram(void*) = 0; // 46 <- correct :D
    virtual bool	transmit(bool onlyReliable = false) = 0;

    virtual const void*	get_remote_address( void ) const = 0;
    virtual void *get_msg_handler( void ) const = 0;
    virtual int				get_drop_number( void ) const = 0;
    virtual int				get_socket( void ) const = 0;
    virtual unsigned int	get_challenge_nr( void ) const = 0;
    virtual void			get_sequence_data( int &nOutSequenceNr, int &nInSequenceNr, int &nOutSequenceNrAck ) = 0;
    virtual void			set_sequence_data( int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck ) = 0;

    virtual void	update_message_stats( int msggroup, int bits) = 0;
    virtual bool	can_packet( void ) const = 0;
    virtual bool	is_overflowed( void ) const = 0;
    virtual bool	is_timed_out( void ) const  = 0;
    virtual bool	has_pending_reliable_data( void ) = 0;

    virtual void	set_file_trans_mode(bool bBackgroundMode) = 0;
    virtual void	set_compression_mode( bool bUseCompression ) = 0;
    virtual unsigned int request_file(const char *filename) = 0;
    virtual float	get_time_since_last_recv( void ) const = 0;	// get time since last received packet in seconds

    virtual void	set_max_buffer_size(bool bReliable, int nBytes, bool bVoice = false ) = 0;

    virtual bool	is_null() const = 0;
    virtual int		get_num_bits_written( bool bReliable ) = 0;
    virtual void	set_interp_amount( float flInterpolationAmount ) = 0;
    virtual void	set_remote_framerate( float flFrameTime, float flFrameTimeStdDeviation ) = 0;

    // Max # of payload bytes before we must split/fragment the packet
    virtual void	set_max_routable_pl_size( int nSplitSize ) = 0;
    virtual int		get_max_routable_pl_size() = 0;

    virtual int		get_proto_version() = 0;

    char pad_0x0000[0x18]; //0x0000
    int out_seq_nr; //0x0018
    int in_sec_nr; //0x001C
    int out_sec_nr_ack; //0x0020
    int out_rel_state; //0x0024
    int in_rel_state; //0x0028
    int choked_packets; //0x002C
    char pad_0030[220]; //0x0030
    float last_received; //0x010C
    double connect_time; //0x0110
    int32_t rate; //0x0118
    char pad_011C[4]; //0x011C
    double clear_time; //0x0120
};

#endif //uc-sflying-cm-_NETCHANNEL_H
