# 4353 Software Design Week 02 Videos Part 3 Effective Encapsulation Part II

invariant of an object is a property, condition, or constraint that never changes is you would always expect to be true
- ex. age of a person should never be lower than they currently are

encapsulation helps preserve the invariant and helps transition objects from one valid state to another

Encapsulation works better when hiding implementation details as well as data
- ex. dont expose that the data is in a linkedlist instead only expose it as a list 

Separate the interface from the implementation

- You can modify the class much more easily 
- As a user you can use it without worrying about the details of the implementation

To properly encapsulate data and implementation you must manage concurrency (avoiding race conditions)

## Safe iteration

When returning data return an unmodifiable copy of the data so that iterators are not affected by concurrent changes