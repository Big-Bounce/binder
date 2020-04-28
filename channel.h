class channel {
    std::weak_ptr<switcher> _edge;
    V _value;
    size_t _newstar;        
public:
    //channel();
    channel(const std::weak_ptr<switcher>&, const V&, size_t);
    
    inline V value() const {return _value;}
    inline size_t newstar() const {return _newstar;}
    inline std::weak_ptr<switcher> edge() {return _edge;}
    
    inline void set_newstar(size_t newstar) {_newstar = newstar;}
    inline void set_value(const V& value) {_value = value;}
    
};
