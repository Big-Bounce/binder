class level {
         
    U _value;
    switchers_type _switchers;
    
public:
    
    //level();
    level(const U&, const switchers_type&);

    void connect(const std::shared_ptr<level>&);
    void disconnect(const std::shared_ptr<level>&);
    
    bool has_switcher(const std::weak_ptr<switcher>&);
    size_t find_switcher(const std::weak_ptr<switcher>&);    
    
    inline size_t size() const {return _switchers.size();}
    inline const U& get_value() const {return _value;}
    inline const switchers_type& get_switchers() const {return _switchers;}
    
    void go(worm&);
};



