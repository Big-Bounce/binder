#ifndef BINDER_GUARD
#define BINDER_GUARD

#include <vector>
#include <list>
#include <memory>
#include <string>
#include <stack>
#include "exceptions.h"

template <typename T, typename U, typename V, typename W = std::vector<T>, typename X = std::vector<U>> 
//T - switcher value type
//U - level value type
//V - channel value type
//W - worm's container with T - values
//X - worm's container with U - values

class binder {

public:
    class channel_id;
    class level_id;
    class worm;
    
private:    
           
    #include "switcher.h"    
    typedef std::vector<std::shared_ptr<switcher>> switchers_type;

    #include "channel.h"
    
    #include "level.h"
    typedef std::vector<std::shared_ptr<level>> levels_type;    
/*    
    typedef std::stack<size_t> currents_type;
    #include "star.h"    
    typedef std::vector<std::unique_ptr<star>> stars_type;    
*/    
    std::string _name;
    //stars_type _stars;
    //currents_type _current_star;
    levels_type _free_levels;
    bool _available;
    
    template <typename Y> size_t _empty_position(const Y&);    
    void _lid_assert(const level_id&);
    
public:    
    #include "id.h"
    #include "worm.h"    
    
    binder(const std::string& = std::string());

    //bool check_switcher(std::weak_ptr<switcher>&, worm&);
    //void get_back(worm&);
    
    template<typename Y> level_id create_level(const U&, const Y&);
    void erase_level(const level_id&);
    //void add_level(level_id, channel_id, size_t pos = 1);
    //void remove_level(level_id, channel_id);
    //void set_channel_value(channel_id, const V&);
    //void go(worm&);
    //channel_id get_initial_channel();

};

#include "switcher_imp.h"
#include "channel_imp.h"
#include "level_imp.h"
//#include "star_imp.h"
#include "binder_imp.h"

#endif
