class worm {
    friend binder;
    
    W _switchers_values;
    X _levels_values;
    std::weak_ptr<channel> _current_channel;
    channel_id _id;
    bool _am_i_in_channel;    
    
protected:  
   
    const W& switchers_values() const {return _switchers_values;}
    const X& levels_values() const {return _levels_values;}    
        
    inline channel_id get_channel_id() const {return _id;}
    V get_channel_value();
    void set_channel_value(const T&);
    
    inline bool am_i_in_channel() const {return _am_i_in_channel;}        
    
public: 
    worm(): _am_i_in_channel(false) {}
    virtual void process() = 0;
    virtual ~worm() {}
};
