#include <iostream>

template <typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::binder(const std::string& name): _name(name), _available(true) {

    std::shared_ptr<switcher> initial_switcher(std::make_shared<switcher> (*this));   
    //   _stars.emplace_back(new star(initial_switcher));
 //   _current_star.push(0);       
    
}
/*
template <typename T, typename U, typename V, typename W, typename X>
bool binder<T,U,V,W,X>::check_switcher(std::weak_ptr<switcher>& to_check, worm& to_update) {
    
    
    if (_stars[_current_star.top()] -> is_switcher_present(to_check)) {
        to_update.push_back_switchers_stack (to_check.lock() -> value());
        to_update.push_back_levels_stack (_stars[_current_star.top()] -> get_level_description());
                  
        if (_stars[_current_star.top()] -> is_channel_present(to_check)) 
            if (_stars[_current_star.top()] -> is_newstar_present())
                _current_star.push(_stars[_current_star.top()] -> get_newstar());
            else {
                to_update._channel_value = _stars[_current_star.top()] -> get_channel_value();
                //to_update._id._object = set_value(to_check);
                to_update._updated_channel_value = true;                       
            }
        
        return true;
    }
    return false;
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::get_back(worm& to_update) {
    
    
    _stars[_current_star.top()] -> get_back();
    if (! _stars[_current_star.top()] -> current_level())
        _current_star.pop();
    to_update.pop_back_switchers_stack();
    to_update.pop_back_levels_stack();
    to_update._updated_channel_value = false;
}
*/
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
}
/*
template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::add_level(level_id lid, channel_id cid, size_t pos) {
    
    if (!_available)
        throw traversal_is_in_operation();
    if (!pos)
        throw cannot_add_level_on_this_position();
    if (lid._object.expired())
        throw level_does_not_exist();
    _lid_assert(lid);
    if (cid._object.expired())
        throw channel_does_not_exist();        
    
    std::shared_ptr<channel> object(cid._object);    
    
    if (!object -> newstar()) {
        if (pos != 1)
            throw cannot_add_level_on_this_position();
        size_t position = _empty_position(_stars);
        if (position == _stars.size())
            _stars.emplace_back(new star(std::shared_ptr<switcher> (object -> edge())));
        else
            _stars[position] = std::unique_ptr<star> (new star(std::shared_ptr<switcher> (object -> edge())));
        object -> set_newstar(position);
        _stars[position] -> add_level(_free_levels[lid._level_number], pos);
    }

    else {        
        assert(object -> newstar() < _stars.size() && _stars[object -> newstar()]);
        if (!_stars[object -> newstar()] -> is_available())
            throw cannot_add_level_on_this_channel();
        if (pos > _stars[object -> newstar()] -> size())
            throw cannot_add_level_on_this_position();
        _stars[object -> newstar()] -> add_level(_free_levels[lid._level_number], pos);
    }
            
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::remove_level(level_id lid, channel_id cid) {
    
    if (!_available)
        throw traversal_is_in_operation();    
    if (lid._object.expired())
        throw level_does_not_exist();
    _lid_assert(lid);
    if (cid._object.expired())
        throw channel_does_not_exist();        
    
    std::shared_ptr<channel> object(cid._object);    
    
    if (!object -> newstar())
        throw cannot_remove_level_from_this_channel();    
    assert(object -> newstar() < _stars.size() && _stars[object -> newstar()]);
    if (!_stars[object -> newstar()] -> available())
        throw cannot_remove_level_from_this_channel();    
    size_t position = _stars[object -> newstar()] -> is_level_present(object);
    if (position == _stars[object -> newstar()] -> size())
        throw level_does_not_exist_in_this_channel();
    
    size_t newsize = _stars[object -> newstar()] -> remove_level(position);
    assert(newsize);
    if (newsize == 1)
        _stars[object -> newstar()].reset();
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::set_channel_value(channel_id id, const V& value) {
    if (!_available)
        throw traversal_is_in_operation();
    if (id._object.expired())
        throw channel_does_not_exist();        
    std::shared_ptr<channel> (id._object) -> set_value(value);
}
*/
template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::_lid_assert(const typename binder<T,U,V,W,X>::level_id& lid) {
    assert(lid._level_number < _free_levels.size());
    lid._object.lock()  == _free_levels[lid._level_number];
}
/*
template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::go(worm& to_update) {
    _available = false;
    _stars[0] -> go(to_update);
    _available = true;
}

template <typename T, typename U, typename V, typename W, typename X>
typename binder<T,U,V,W,X>::channel_id binder<T,U,V,W,X>::get_initial_channel() {
    channel_id id;
    id._object = _stars[0] -> initial_channel();    
    return id;
}
*/
