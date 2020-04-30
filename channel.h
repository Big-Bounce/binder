class channel {
    std::weak_ptr<switcher> _edge;
    V _value;
    size_t _newstar;        
public:

    channel(const std::weak_ptr<switcher>&, const V& = V());
    
    inline const V& get_value() const {return _value;}
    inline size_t get_newstar() const {return _newstar;}
    inline const std::weak_ptr<switcher>& get_edge() {return _edge;}
    
    inline void set_newstar(size_t newstar) {_newstar = newstar;}
    inline void set_value(const V& value) {_value = value;}
    
};
