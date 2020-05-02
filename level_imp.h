#include <algorithm>
#include <cassert>

template<typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::level::level(const U& value, const switchers_type& switchers):
    _value(value), _switchers(switchers) {
}

template<typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::level::connect(const std::shared_ptr<level>& newlevel) {

    typedef typename switchers_type::iterator iter_type;
    auto _lambda = [&newlevel] (const std::shared_ptr<switcher>& arg) {
        for (iter_type iter = newlevel->_switchers.begin(); iter != newlevel->_switchers.end(); ++iter)
            arg -> add_child(*iter);
    };
    std::for_each(_switchers.begin(), _switchers.end(), _lambda); 

}

template<typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::level::disconnect(const std::shared_ptr<level>& oldlevel) {
    
    typedef typename switchers_type::iterator iter_type;
    auto _lambda = [&oldlevel] (const std::shared_ptr<switcher>& arg) {
        for (iter_type iter = oldlevel->_switchers.begin(); iter != oldlevel->_switchers.end(); ++iter)
            arg -> remove_child(*iter);
    };
    std::for_each(_switchers.begin(), _switchers.end(), _lambda); 
}

template<typename T, typename U, typename V, typename W, typename X>
bool binder<T,U,V,W,X>::level::has_switcher(const std::weak_ptr<switcher>& to_check) {
    assert(!to_check.expired());
    return std::find(_switchers.begin(), _switchers.end(), to_check.lock()) != _switchers.end();
}    

template<typename T, typename U, typename V, typename W, typename X>
size_t binder<T,U,V,W,X>::level::find_switcher(const std::weak_ptr<switcher>& to_find) {

    assert(!to_find.expired());
    assert(has_switcher(to_find));
    size_t position = 0;
    while (_switchers[position] != to_find.lock())
        ++ position;      
    return position;
}    

template<typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::level::go(worm& to_update) {
    assert(_switchers.size() == 1);
    _switchers[0] -> go(to_update);
}

