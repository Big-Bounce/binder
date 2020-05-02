template <typename T, typename U, typename V, typename W, typename X>
V binder<T,U,V,W,X>::worm::get_channel_value() {
    if (_current_channel.expired())
        throw channel_does_not_exist();        
    return _current_channel.lock() -> get_value();
}

template <typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::worm::set_channel_value(const T& value) {
    if (_current_channel.expired())
        throw channel_does_not_exist();        
    _current_channel.lock() -> set_value(value);
}
