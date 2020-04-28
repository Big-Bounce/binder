class worm {
    friend binder;
    
    W _switchers_stack;
    X _levels_stack;
    V _channel_value;
    channel_id _id;
    bool _updated_channel_value;

    virtual void push_back_switchers_stack(const T& value) {_switchers_stack.push_back(value);}
    virtual void push_back_levels_stack(const U& value) {_levels_stack.push_back(value);}
    virtual void pop_back_switchers_stack() {_switchers_stack.pop_back();}
    virtual void pop_back_levels_stack() {_levels_stack.pop_back();}
     
protected:  
    const W& switchers_stack() const {return _switchers_stack;}
    const X& levels_stack() const {return _levels_stack;}    
    const V& channel_value() const {return _channel_value;}
    inline channel_id id() const {return _id;}
    inline bool updated_channel_value() const {return _updated_channel_value;}        
    
public: 
    virtual void process() = 0;
    virtual ~worm() {};
};
