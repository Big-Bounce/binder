
struct binder_exceptions {    
    virtual const char* what() const {return ":(";}
    virtual ~binder_exceptions() {};
};

struct level_does_not_exist: public binder_exceptions {
    const char* what() const {return "Level does not exist :(";}
};

struct level_is_beeing_used: public binder_exceptions {
    const char* what() const {return "Level is beeing used :(";}
};

struct channel_does_not_exist: public binder_exceptions {
    const char* what() const {return "Channel does not exist :(";}
};

struct cannot_add_level_on_this_position: public binder_exceptions {
    const char* what() const {return "Cannot add level on this position :(";}
};

struct cannot_add_level_on_this_channel: public binder_exceptions {
    const char* what() const {return "Cannot add level on this channel :(";}
};

struct level_does_not_exist_in_this_channel: public binder_exceptions {
    const char* what() const {return "Level does not exist in this channel :(";}
};

struct cannot_remove_level_from_this_channel: public binder_exceptions {
    const char* what() const {return "Cannot remove level from this channel :(";}
};

struct traversal_is_in_operation: public binder_exceptions {
    const char* what() const {return "Traversal is in operation :(";}
};
