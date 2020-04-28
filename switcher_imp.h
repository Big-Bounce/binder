#include <algorithm>
#include <cassert>

template<typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::switcher::child_type::child_type() {    
}

template<typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::switcher::child_type::child_type(const std::weak_ptr<switcher>& child): 
    _child(child), _counter(1) {    
}

template<typename T, typename U, typename V, typename W, typename X>
binder<T,U,V,W,X>::switcher::switcher (binder& mybinder, const T& value):
    _value(value), _mybinder(mybinder){    
}

template<typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::switcher::add_child(const std::weak_ptr<switcher> & newchild){
    
    assert(!newchild.expired());
    auto _lambda = [&newchild] (const child_type& arg) {
            assert(!arg._child.expired());
            return arg._child.lock() == newchild.lock();
        };        
    typename children_type::iterator iter = std::find_if(_children.begin(), _children.end(), _lambda);
    if (iter == _children.end()) //hasn't had this child yet
        _children.emplace_back(newchild);
    else                         //has already had this child
        ++ iter -> _counter;
}

template<typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::switcher::remove_child(const std::weak_ptr<switcher> & oldchild){

    assert(!oldchild.expired());
    auto _lambda = [&oldchild] (const child_type& arg) {
            assert(!arg._child.expired());
            return arg._child.lock() == oldchild.lock();
        };
    typename children_type::iterator iter = std::find_if(_children.begin(), _children.end(), _lambda);
    assert(iter != _children.end() && iter -> _counter);
    if (iter -> _counter == 1) //child will be no more present
        _children.erase(iter); 
    else
        -- iter -> _counter;  //child will be still present
}    
/*
template<typename T, typename U, typename V, typename W, typename X>
void binder<T,U,V,W,X>::switcher::go(worm& to_update) {
    
    std::shared_ptr<binder> mybinder(_mybinder);
    
    auto go_deeper = [&mybinder, &to_update] (child_type& child) {
        if (mybinder -> check_switcher(child._child, to_update)) {
            std::shared_ptr<switcher> (child._child) -> go(to_update);
            mybinder -> get_back(to_update);
        }
            
    };
    std::for_each(_children.begin(), _children.end(), go_deeper);
    
}
*/    
    
