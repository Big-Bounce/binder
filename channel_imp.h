/*
template <typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::channel::channel(){   
}
*/
template <typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::channel::channel(const std::weak_ptr<switcher>& edge, const V& value, size_t newstar):
    _edge(edge), _value(value), _newstar(0) {   
}
