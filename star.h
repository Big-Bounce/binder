#include <stack>

class star {

    typedef std::vector<std::shared_ptr<channel>> channels_type;
   
    levels_type _levels;
    channels_type _channels;
    size_t _current_level;
    currents_type _current_left;
    currents_type _current_right;
    
    void _clear_channels();
    void _preserve_channels();
        
public:

    star(const std::shared_ptr<switcher>&);

    void add_level(const std::shared_ptr<level>&, size_t, bool = true);
    size_t remove_level(size_t, bool = true);
    
    bool is_level_present(const std::shared_ptr<level>&);
    size_t get_level_position(const std::shared_ptr<level>&);
        
    bool is_switcher_present(const std::weak_ptr<switcher>&);
    const U& get_level_value();
       
    bool is_channel_present(const std::weak_ptr<switcher>&);
    const V& get_channel_value();
    
    bool is_newstar_present();
    size_t get_newstar();
    
    //void get_back();
    //inline void go(worm& to_update) {_levels[0] -> go(to_update);}       
    
    bool is_available();
    inline size_t size() const {return _levels.size();}
    inline size_t get_current_level() const {return _current_level;}    
    std::shared_ptr<channel>& get_initial_channel();
};


