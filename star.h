#include <stack>

class star {

    typedef std::vector<std::shared_ptr<channel>> channels_type;
   
    levels_type _levels;
    channels_type _channels;
    size_t _current_level;
    currents_type _current_left;
    currents_type _current_right;
    
    void _clear_channels();
    void _copy_channels();
        
public:
    //star();
    star(const std::shared_ptr<switcher>&);
   
    void add_level(const std::shared_ptr<level>&, size_t);
    size_t is_level_present(const std::shared_ptr<level>&);
    size_t remove_level(size_t);
    
    bool is_switcher_present(const std::weak_ptr<switcher>&);
    const U& get_level_description();
        
    bool is_channel_present(const std::weak_ptr<switcher>&);
    const V/*&*/ get_channel_value();
    
    bool is_newstar_present();
    size_t get_newstar();
    
    void get_back();
    inline size_t current_level() const {return _current_level;}    
    
    bool is_available();
    inline size_t size() const {return _levels.size();}

    void clone (const star&);
    inline void go(worm& to_update) {_levels[0] -> go(to_update);} 
    
    std::shared_ptr<channel> initial_channel();
};


