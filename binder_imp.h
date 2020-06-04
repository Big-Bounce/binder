#include <iostream>

template <typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::binder(const std::string& name): _name(name), _ready(true) {

    std::shared_ptr<switcher> initial_switcher(std::make_shared<switcher> (*this));   
    _stars.emplace_back(new star(initial_switcher));
    _current_star.push(0);
}

template <typename T, typename U, typename V, typename W, typename X>
bool binder<T,U,V,W,X>::check_switcher(std::weak_ptr<switcher>& to_check, worm& to_update) {
    
    assert(_current_star.size()); //_current_star has to always have at least one element 
    assert(!_ready); //binder has to be blocked while traversal        
    assert(_current_star.top() < _stars.size() && _stars[_current_star.top()]); //current star is correct
    
    //check if star change is necessery
    if (_stars[_current_star.top()] -> is_channel_present())
        if (_stars[_current_star.top()] -> is_newstar_present())
            _current_star.push(_stars[_current_star.top()] -> get_newstar());
    size_t top = _current_star.top();
    
    //check if switcher is present on current star's next level
    if (_stars[top] -> is_switcher_present(to_check)) { //if it is then ...
        _stars[top] -> prepare_to_go(to_check); // ... let the star know ...
        to_update._switchers_values.push_back (to_check.lock() -> get_value()); //... update worm's data ... 
        to_update._levels_values.push_back(_stars[top] -> get_level_value(_stars[top] -> get_current_level()));        
        to_update._current_channel.reset(); //... and clear worm's channel - related data
        to_update._id._object.reset();
        to_update._am_i_in_channel = false;
        to_update._am_i_done = false;
        
        //check if channel is present
        if (_stars[top] -> is_channel_present()) { //if it is, update worm's channel-related data
            to_update._current_channel =  _stars[top] -> get_channel();
            to_update._id._object = _stars[top] -> get_channel();
            to_update._am_i_in_channel = true;            
            if (!_stars[top] -> is_newstar_present()) //check if star change will be necessery
                to_update._am_i_done = true;
        }
                        
        return true;
            
    }
    return false;   
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::get_back(worm& to_update) {
    
    assert(_current_star.size()); //_current_star has to always have at least one element 
    
    size_t top = _current_star.top();
    assert(top < _stars.size() && _stars[top]);
    _stars[top] -> get_back();
    
    if (!_stars[top] -> get_current_level()) {        
        _current_star.pop();
        assert(_current_star.size()); //_current_star has to always have at least one element
    }    

    assert(to_update._switchers_values.size());
    to_update._switchers_values.pop_back();  
    assert(to_update._levels_values.size());
    to_update._levels_values.pop_back();
}

template <typename T, typename U, typename V, typename W, typename X> 
template <typename Y>
size_t binder<T,U,V,W,X>::_empty_position(const Y& container) {
    size_t i = 0;
    while (i < container.size()) {
        if (!container[i])
            return i;
        ++i;
    }
    return i;        
}

template <typename T, typename U, typename V, typename W, typename X>
template <typename Y>
typename binder<T,U,V,W,X>::level_id binder<T,U,V,W,X>::create_level(const U& value, const Y& data) {
    
    switchers_type initial_data;
    initial_data.reserve(data.size());
    
    auto _lambda = [this, &initial_data] (const T& val) {
        initial_data.emplace_back(std::make_shared<switcher> (*this, val));
    };
    std::for_each(data.begin(), data.end(), _lambda);
        
    size_t i = _empty_position(_free_levels);
    if (i == _free_levels.size())
        _free_levels.emplace_back(std::make_shared<level> (value, initial_data));
    else
        _free_levels[i] = std::shared_ptr<level> (std::make_shared<level> (value, initial_data));

    level_id id;
    id._level_number = i;
    id._object = _free_levels[i];
    return id;
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::erase_level(const level_id& id) {
    if (id._object.expired())
        throw level_does_not_exist();
    _lid_assert(id);
    if (id._object.use_count() > 1)
        throw level_is_beeing_used();    
    _free_levels[id._level_number].reset();
    if (id._level_number == _free_levels.size() - 1)
        _cut_tail(_free_levels);
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::add_level(level_id& lid, channel_id& cid, size_t pos) {
    
    if (!_ready)
        throw traversal_is_in_operation();
    if (lid._object.expired())
        throw level_does_not_exist();
    _lid_assert(lid);
    if (cid._object.expired())
        throw channel_does_not_exist();        
    
    std::shared_ptr<channel> initial(cid._object);    
    
    if (!initial -> get_newstar()) { // making new star 
        if (pos != 1) //newly created star has just one level
            throw cannot_add_level_on_this_position();
        size_t position = _empty_position(_stars);
        if (position == _stars.size())
            _stars.emplace_back(new star(initial->get_edge().lock()));
        else
            _stars[position] = std::unique_ptr<star> (new star(initial->get_edge().lock()));
        initial -> set_newstar(position);
        _stars[position] -> add_level(_free_levels[lid._level_number], pos);
    }

    else { // adding to existing star
        size_t newstar = initial -> get_newstar();
        assert(newstar < _stars.size() && _stars[newstar]);
        if (!_stars[newstar] -> is_available())
            throw cannot_add_level_on_this_channel();
        if (!pos || pos > _stars[newstar] -> size())
            throw cannot_add_level_on_this_position();
        _stars[newstar] -> add_level(_free_levels[lid._level_number], pos);
    }
            
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::remove_level(level_id& lid, channel_id& cid) {
    
    if (!_ready)
        throw traversal_is_in_operation();    
    if (lid._object.expired())
        throw level_does_not_exist();
    _lid_assert(lid);
    if (cid._object.expired())
        throw channel_does_not_exist();        
    
    std::shared_ptr<channel> initial(cid._object);    
 
    if (!initial -> get_newstar()) //channel has to switch to some star
        throw cannot_remove_level_from_this_channel();    
    
    size_t newstar = initial -> get_newstar();
    assert(newstar < _stars.size() && _stars[newstar]);
    if (!_stars[newstar] -> is_available()) //star has to be available
        throw cannot_remove_level_from_this_channel();    
    
    std::shared_ptr<level> to_remove(lid._object);    
    
    bool present = _stars[newstar] -> is_level_present(to_remove);
    if (!present) //star has to contain the level
        throw level_does_not_exist_in_this_channel();
    
    size_t position = _stars[newstar] -> get_level_position(to_remove);    
    size_t newsize = _stars[newstar] -> remove_level(position);
    assert(newsize);
    if (newsize == 1) { //erase star if doesn't exist anymore, clear channel's newstar
        _stars[newstar].reset();
        initial -> set_newstar(0);
        if (newstar ==  _stars.size() - 1)
            _cut_tail(_stars);
    }

}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::set_channel_value(channel_id& id, const V& value) {
    if (id._object.expired())
        throw channel_does_not_exist();        
    id._object.lock() -> set_value(value);
}

template <typename T, typename U, typename V, typename W, typename X>
V binder<T,U,V,W,X>::get_channel_value(channel_id& id) {
    if (id._object.expired())
        throw channel_does_not_exist();        
    return id._object.lock() -> get_value();
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::_lid_assert(const typename binder<T,U,V,W,X>::level_id& lid) {
    assert(lid._level_number < _free_levels.size());
    lid._object.lock()  == _free_levels[lid._level_number];
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::go(worm& to_update) {
    _ready = false;
    _stars[0] -> go(to_update);
    _ready = true;
}

template <typename T, typename U, typename V, typename W, typename X>
typename binder<T,U,V,W,X>::channel_id binder<T,U,V,W,X>::get_initial_channel() {
    channel_id id;
    id._object = _stars[0] -> get_channel();    
    return id;
}

template <typename T, typename U, typename V, typename W, typename X>
template <typename Y>
void binder<T,U,V,W,X>::_cut_tail(Y& container) {
    size_t i = container.size();
    while (container[--i])
        container.pop_back();
}

