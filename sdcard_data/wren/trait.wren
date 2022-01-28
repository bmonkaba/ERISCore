/* Module "trait.wren" */
 
/* Cloneable is an abstract class which enables child classes to automatically be
   recognized as 'cloneable' by overriding the 'clone' method.
*/
class Cloneable {
    clone() { this } /* to be overridden by child class */
}
 
/* CloneableSeq is an abstract class which enables child classes to automatically be
   recognized as both Sequences and 'cloneable' by overriding the 'clone' method.
*/
class CloneableSeq is Sequence {
    clone() { this } /* to be overridden by child class */ 
}
 
/*
    Comparable is an abstract class which enables child classes to automatically
    inherit the comparison operators by just overriding the 'compare' method.
    Comparable itself inherits from Cloneable though if one does not wish to override
    the 'clone' method, it will just return the current object by default.
*/
class Comparable is Cloneable {
    compare(other) {
        // This should be overridden in child classes to return -1, 0 or +1
        // depending on whether this < other, this == other or this > other.
    }
 
    < (other) { compare(other) <  0 }
    > (other) { compare(other) >  0 }    
    <=(other) { compare(other) <= 0 }
    >=(other) { compare(other) >= 0 }
    ==(other) { compare(other) == 0 }
    !=(other) { compare(other) != 0 }
}
 
/* Stepped wraps a Sequence so it can be iterated by steps other than 1. */
class Stepped is Sequence {
    // Constructs a new stepped sequence.
    construct new(seq, step) {
        if (!(seq is Sequence)) Fiber.abort("First argument must be a sequence.")
        _seq = seq
        _step = (step < 1) ? 1 : step // minimum step of 1
    }
 
    // Ensures a range is ascending before passing it to the constructor.
    // It it isn't, returns an empty range. Useful when bounds are variable.
    static ascend(range, step) {
        if (!(range is Range)) Fiber.abort("First argument must be a range.")
        return (range.from <= range.to) ? new(range, step) : 0...0
    }
 
    // Ensures a range is descending before passing it to the constructor.
    // It it isn't, returns an empty range. Useful when bounds are variable.
    static descend(range, step) {
        if (!(range is Range)) Fiber.abort("First argument must be a range.")
        return (range.from >= range.to) ? new(range, step) : 0...0
    }
 
    // Convenience versions of the above methods which call them with a step of 1.
    static ascend(range)  { ascend(range,  1) }
    static descend(range) { descend(range, 1) }
 
    // Iterator protocol methods.
    iterate(iterator) {
        if (!iterator) {
            return _seq.iterate(iterator)
        } else {
            var count = _step
            while (count > 0 && iterator) {
               iterator = _seq.iterate(iterator)
               count = count - 1
            }
            return iterator
        }
    }
 
    iteratorValue(iterator) { _seq.iteratorValue(iterator) }
}
 
/* 
    Reversed wraps a Sequence (other than a range) so it can be iterated in reverse
    and by steps other than 1.
*/
class Reversed is Sequence {
    // Constructs a new reversed sequence.
    construct new(seq, step) {
        if (!(seq is Sequence) || seq is Range) {
            Fiber.abort("First argument must be a sequence other than a range.")
        }
        _seq = seq
        _step = (step < 1) ? 1 : step // minimum step of 1
    }
 
    // Convenience method which calls the constructor with a step of 1.
    static new(seq) { Reversed.new(seq, 1) }
 
    // Iterator protocol methods.
    iterate(iterator) {
        var it = _seq.iterate(iterator)
        if (it == null || it == 0) {
            it = _seq.count - 1
        } else if (it == false) {
            it = _seq.count - 1 - _step
        } else { 
            it = it - 1 - _step
        }       
        return (it >= 0) ? it : false
    }
 
    iteratorValue(iterator) { _seq.iteratorValue(iterator) }
}
 
/* SeqEntry represents an (index, value) pair for use with the Indexed class. */
class SeqEntry {
    // Constructs a new SeqEntry object.
    construct new(index, value) {
        _index = index
        _value = value
    }
 
    // Properties.
    index { _index }
    value { _value }
 
    // Returns the current instance's string representation.
    toString { "%(_index):%(_value)" }
}
 
/*
    Indexed wraps a Sequence (other than a range) so its elements can be iterated over 
    together with their zero-based indices. 
*/
class Indexed is Sequence {
    // Constructs a new indexed sequence with a step of 'step' and optionally reversed.
    construct new(seq, step, reversed) {
        if (seq is Range) Fiber.abort("First argument cannot be a range.")
        if (!(reversed is Bool)) Fiber.abort("Third argument must be true or false.")
        _seq = !reversed ? Stepped.new(seq, step) : Reversed.new(seq, step)
    }
 
    // Constructs a new indexed sequence with a step of 'step' and 'reversed' set to false.
    static new(seq, step) { new(seq, step, false) }
 
    // Constructs a new indexed sequence with a step of 1.
    construct new(seq) {
        if (!(seq is Sequence) || seq is Range) {
            Fiber.abort("Argument must be a sequence other than a range.")
        }
        _seq = seq
    }
 
    // Iterator protocol methods.
    iterate(iterator) { _seq.iterate(iterator) }
 
    iteratorValue(iterator) {
        return SeqEntry.new(iterator, _seq.iteratorValue(iterator))
    }
}
 
/*  ByRef wraps a value to enable it to be passed by reference to a method or function. */
class ByRef {
    // Constructs a new ByRef object.
    construct new(value) {
        _value = value
    }
 
    // Properties to get and set the value of the current instance.
    value     { _value }
    value=(v) { _value = v }
 
    // Returns the string representation of the current instance's value.
    toString { _value.toString }
}
 
/*
    ByKey wraps a reference type object to enable it to be used as a Map key. It does this
    by using a counter to create a unique integral key for each such object and maintaining
    an internal map which enables the object to be quickly retrieved from its key.
*/
class ByKey {
    // Retrieves a ByKey object from its key. Returns null if the key is not present.
    static [key] { __map ? __map[key] : null }
 
    // Retrieves a ByKey object's key from the object it wraps. 
    // If the same object has been wrapped more than once, returns the first key found which may
    // not be the lowest. Returns 0 if the object is unwrapped.
    static findKey(obj) {
        for (me in __map) {
            if (me.value.obj == obj) return me.key
        }
        return 0
    }
 
    // Returns the number of objects currently in the internal map.
    static mapCount { __tally ? __map.count : 0 }
 
    // Returns the total number of keys created to date.
    static keyCount { __tally ? __tally : 0 }
 
    // Constructs a new ByKey object.
    construct new(obj) {
        _obj = obj
        if (!__tally) {
            __map = {}
            __tally = 1
        } else {
            __tally = __tally + 1
        }
        _key = __tally
        __map[_key] = this
    }
 
    // Properties
    obj { _obj }  // returns the current instance
    key { _key }  // returns the current instance's key
 
    // Removes the current instance from the internal map and sets it to null.
    unkey() {
        __map.remove(_key)
        _obj = null
    }
 
    // Returns the string representation of the current instance.
    toString { _obj.toString }
}
