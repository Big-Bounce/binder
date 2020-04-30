#include <cassert>
#include <algorithm>

template <typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::star::star(const std::shared_ptr<switcher>& initial_switcher): _current_level(0), 
    _channels(1, std::shared_ptr<channel> (std::make_shared<channel> (initial_switcher))) {
    
    switchers_type initial;
    initial.emplace_back(initial_switcher);
    _levels.emplace_back(std::make_shared<level> (U(), initial));
    _current_left.push(0);
    _current_right.push(0);
}
    
template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::star::add_level(const std::shared_ptr<level>& newlevel, size_t position, bool preserve){

    assert(is_available());
    assert(position && position <= _levels.size());    
    _levels.insert(_levels.begin() + position, newlevel);    

    _levels[position-1] -> connect(_levels[position]);    
    if (position + 1 < _levels.size()) { //level added on non-last position
        _levels[position] -> connect(_levels[position+1]);
        _levels[position-1] -> disconnect(_levels[position+1]);        
    } else //otherwise preserving impossible
        preserve = false; 
        
    if (preserve)
        _preserve_channels;
    else
        _clear_channels;
    
}

template <typename T, typename U, typename V, typename W, typename X>
size_t binder<T,U,V,W,X>::star::remove_level(size_t position, bool preserve) {
    
    assert(is_available());
    assert(position && position < _levels.size());    
    
    if (position + 1 < _levels.size()) { // level removed from non-last position
        _levels[position-1] -> connect(_levels[position+1]);    
        _levels[position] -> disconnect(_levels[position+1]);
    } else // otherwise preserving impossible
        preserve = false;
    
    _levels[position-1] -> disconnect(_levels[position]);            
    _levels.erase(_levels.begin() + position);

    if (preserve)
        _preserve_channels;
    else
        _clear_channels;
    return _levels.size();
}

template <typename T, typename U, typename V, typename W, typename X>
bool binder<T,U,V,W,X>::star::is_switcher_present(const std::weak_ptr<switcher>& to_check) {
    if (_current_level+1 < _levels.size() && _levels[_current_level+1] -> has_switcher(to_check)) {
        ++ _current_level;
        return true;
    }
    return false;
}

template <typename T, typename U, typename V, typename W, typename X>
const U& binder<T,U,V,W,X>::star::get_level_value() {
    assert(_current_level < _levels.size());
    return _levels[_current_level] -> value();
}

template <typename T, typename U, typename V, typename W, typename X>
bool binder<T,U,V,W,X>::star::is_channel_present(const std::weak_ptr<switcher>& to_check) {
    
    size_t left = _current_left.top();
    size_t right = _current_right.top();            
    
    assert(left <= right);
    assert(right < _levels.size());
        
    size_t position = _levels[_current_level] -> find_switcher(to_check);
    _current_left.push(left + position/_levels.size()*_channels.size());    
    _current_right.push(right + (position+1)/_levels.size()*_channels.size() - 1);
    
    assert(_current_left.top() <= _current_right.top());
    assert(_current_left.top() < _levels.size());   
    
    return _current_left.top() == _current_right.top();
}

template <typename T, typename U, typename V, typename W, typename X>
const V& binder<T,U,V,W,X>::star::get_channel_value() {
    assert(_current_left.top() == _current_right.top());
    return _channels[_current_left.top()] -> value();    
}

template <typename T, typename U, typename V, typename W, typename X>
bool binder<T,U,V,W,X>::star::is_newstar_present() {
    assert(_current_left.top() == _current_right.top());
    return _channels[_current_left.top()] -> newstar();    
}

template <typename T, typename U, typename V, typename W, typename X>
size_t binder<T,U,V,W,X>::star::get_newstar() {
    assert(_channels[_current_left.top()] -> newstar());
    return _channels[_current_left.top()] -> newstar();    
}
/*
template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::star::get_back() {
    _current_left.pop();
    _current_right.pop();
    -- _current_level;
}
*/
template <typename T, typename U, typename V, typename W, typename X>
bool binder<T,U,V,W,X>::star::is_available() {
    auto my_lambda = [] (std::shared_ptr<channel>& to_check) {
        return to_check -> newstar();
    };
    return ! std::any_of(_channels.begin(), _channels.end(), my_lambda);
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::star::_clear_channels() {
    
    assert(is_available());    
    size_t newsize = 1;
    for (size_t i = 0; i < _levels.size(); ++i)
        newsize *= _levels[i] -> size();
    _channels.clear();
    _channels.reserve(newsize);
    
    const switchers_type edges = _levels[_levels.size() - 1] -> get_switchers();
    channels_type input;
    input.reserve(edges.size());
    for (size_t i = 0; i < edges.size(); ++i)
        input.emplace_back(std::make_shared<channel> (edges[i], V()));
        
    typename channels_type::iterator iter = _channels.begin();    
    while (_channels.size() < newsize) {
        _channels.insert(iter, input.cbegin(), input.cend());
        iter += input.size();
    }
    
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::star::_preserve_channels() {
    
    assert(is_available());
    
    size_t newsize = 1;
    for (size_t i = 1; i < _levels.size(); ++i)
        newsize *= _levels[i] -> size();
    if (newsize <= _channels.size()) {
        _channels.resize(newsize);
        return;
    }

    _channels.reserve(newsize);    
    size_t oldsize = _channels.size();
    while (_channels.size() < newsize) 
        _channels.insert(_channels.end(), _channels.begin(), _channels.begin() + oldsize);   
    
} 

template <typename T, typename U, typename V, typename W, typename X>
bool binder<T,U,V,W,X>::star::is_level_present(const std::shared_ptr<level>& to_check) {
    return ! std::find(_levels.begin(), _levels.end(), to_check) == _levels.end();
}

template <typename T, typename U, typename V, typename W, typename X>
size_t binder<T,U,V,W,X>::star::get_level_position(const std::shared_ptr<level>& to_find) {
    assert(is_level_present(to_find));
    size_t result = 0;
    while (_levels[result] != to_find)
        ++ result;
    return result;
}

template <typename T, typename U, typename V, typename W, typename X>
typename std::shared_ptr<typename binder<T,U,V,W,X>::channel>&
binder<T,U,V,W,X>::star::get_initial_channel() {
    assert(_channels.size() == 1);
    return _channels[0];
}
