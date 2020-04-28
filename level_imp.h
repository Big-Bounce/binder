#include <algorithm>
#include <cassert>
/*
template<typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::level::level() {    
}
*/
template<typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::level::level(const U& description, const switchers_type& switchers):
    _description(description), _switchers(switchers) {
}

template<typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::level::connect(const std::shared_ptr<level>& newlevel) {
    
    newlevel.use_count();
    for (typename switchers_type::iterator i = _switchers.begin(); i != _switchers.end(); ++i)
    for (typename switchers_type::iterator j = newlevel->_switchers.begin(); j != newlevel->_switchers.end(); ++j)
        (*i) -> add_child(*j);

}

template<typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::level::disconnect(const std::shared_ptr<level>& oldlevel) {
    
    for (typename switchers_type::iterator i = _switchers.begin(); i != _switchers.end(); ++i)
    for (typename switchers_type::iterator j = oldlevel->_switchers.begin(); j != oldlevel->_switchers.end(); ++j)
            (*i) -> remove_child(*j);

}

template<typename T, typename U, typename V, typename W, typename X>
bool binder<T,U,V,W,X>::level::has_switcher(const std::weak_ptr<switcher>& to_find) {
    assert(!to_find.expired());
    return std::find(_switchers.begin(),_switchers.end(),std::shared_ptr<switcher> (to_find)) != _switchers.end();
}    

template<typename T, typename U, typename V, typename W, typename X>
size_t binder<T,U,V,W,X>::level::find_switcher(const std::weak_ptr<switcher>& to_find) {

    size_t position = 0;
    assert(!to_find.expired());
    while (_switchers[position] != std::shared_ptr<switcher> (to_find)) {
        ++ position;
        assert(position < _switchers.size());
    }    
    return position;
}    

template<typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::level::go(worm& to_update) {
    assert(_switchers.size() == 1);
    _switchers[0] -> go(to_update);
}

