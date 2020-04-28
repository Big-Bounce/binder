class switcher {

    struct child_type {
        std::weak_ptr<switcher> _child;
        unsigned _counter;
        child_type();
        child_type(const std::weak_ptr<switcher> &);    
    };

    typedef std::list<child_type> children_type;
    
    T _value;
    binder& _mybinder;
    children_type _children;
    
public:

    switcher(binder&, const T& value = T());    

    void add_child(const std::weak_ptr<switcher> &);
    void remove_child(const std::weak_ptr<switcher> &);
    void go(worm&);
    inline const T& value() const {return _value;}
    
};



