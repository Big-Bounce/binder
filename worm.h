class worm {
    friend binder;
    
    W _switchers_values;
    X _levels_values;
    std::weak_ptr<channel> _current_channel;
    std::weak_ptr<channel> _initial_channel;
    channel_id _id;
    channel_id _initial_id;
    bool _am_i_in_channel;    
    bool _am_i_done;
    
protected:  
   
    const W& switchers_values() const {return _switchers_values;}
    const X& levels_values() const {return _levels_values;}    
        
    inline channel_id get_channel_id() const {return _id;}
    inline channel_id get_initial_channel_id() const {return _initial_id;}
    
    V get_channel_value();
    V get_initial_channel_value();
    
    void set_channel_value(const V&);
    void set_initial_channel_value(const V&);
    
    inline bool am_i_in_channel() const {return _am_i_in_channel;}        
    inline bool am_i_done() const {return _am_i_done;}        
    
public: 
    worm(): _am_i_in_channel(false) {}
    virtual void process() = 0;
    virtual ~worm() {}
};
