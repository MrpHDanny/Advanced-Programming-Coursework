#pragma once

// Demonstrate template
template <class T>
class SmartPtr {
    T* ptr; 
public:
    // Constructor
    explicit SmartPtr(T* p = nullptr) { ptr = p; }

    // Destructor
    ~SmartPtr() { delete (ptr); }

    // Overloading dereferncing operator
    T& operator*() { return *ptr; }

    // Overloading arrow operator so that
    // members of T can be accessed
    // like a pointer (useful if T represents
    // a class or struct or union type)
    T* operator->() { return ptr; }
};