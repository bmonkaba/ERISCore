/* Module "array.wren" */
 
import "meta" for Meta
import "./check" for Check
 
/*
   Array represents a List whose size cannot be changed after it has been constructed
   but whose elements can be changed. If an array is created from a list, the
   list is shallow-copied, not cloned.
*/
class Array is Sequence {
    // Constructs a new array from a List or other Sequence.
    construct from(a) {
        Check.seq("Argument", a)
        _a = a.toList // create a list or shallow copy if the argument is already a list.
    }
 
    // Constructs a new array from a List or other Sequence by fitting it to a given size
    // truncating if it's too big or filling out with a given value if it's too small.
    construct fit(size, a, v) {
        Check.nonNegInt("Size", size)
        Check.seq("Second argument", a)
        a = a.toList
        if (a.count == size) {
            _a = a
        } else if (a.count > size) {
            _a = a[0...size]
        } else {
            _a = a
            for (i in a.count...size) _a.add(v)
        }
    }
 
    // Convenience version of 'fit' which uses a default value of null.
    static fit(size, a) { fit(size, a, null) }
 
    // Constructs a new array of a given size and sets all elements to the same value 'v'.
    construct new(size, v) {
        Check.nonNegInt("Size", size)
        _a = List.filled(size, v)
    }
 
    // Convenience version of 'new' which sets all elements to null.
    static new(size) { new(size, null) }
 
    // Property
    count { _a.count }  // returns the number of elements in the array
 
    // Creates a shallow copy of the current instance.
    copy() { Array.from(_a) }
 
    // Resets all elements of the array to 'v'.
    reset(v) {
        for (i in 0..._a.count) _a[i] = v
    }
 
    // Gets the element at 'index.' If index is negative, it counts backwards
    // from the end of the array where -1 is the last element.
    // If index is a range it creates a new array from the appropriate elements.
    [index] { (index is Range) ? Array.from(_a[index]) : _a[index] }
 
    // Sets the element at 'index'. Negative indices are treated as in the getter.
    [index]=(v) { _a[index] = v }
 
    // Returns the index of 'value' in the current instance or -1 if 'value' is not found.
    indexOf(value) { _a.indexOf(value) }
 
    // Returns the index of the last occurrence of 'value' in the current instance
    // or -1 if 'value' is not found.
    lastIndexOf(value) {
        if (_a.count == 0) return 0
        for (i in _a.count-1..0) {
            if (_a[i] == value) return i
        }
        return -1
    }
 
    // Replaces all occurrences of 'old' by 'new' in the current instance
    // and returns ['old', 'new'].
    replace(old, new) {
        for (i in 0..._a.count) {
            if (_a[i] == old) _a[i] = new
        }
        return [old, new]
    }
 
    // Sorts the elements of the array in place and both overloads work in exactly
    // the same manner as the corresponding methods in the List class.
    sort()         { _a.sort() }
    sort(comparer) { _a.sort(comparer) }
 
    // Swaps the elements at index1 and index2 within the array.
    swap(index1, index2) { _a.swap(index1, index2) }
 
    // Applies a function to each element of the array.
    apply(fn) {
        Check.func("fn", fn, 1)
        for (i in 0..._a.count) _a[i] = fn.call(_a[i])
    }
 
    // Iterator protocol methods.
    iterate(iterator) { _a.iterate(iterator) }
    iteratorValue(iterator) { _a.iteratorValue(iterator) }
 
    // Returns the string representation of the underlying list.
    toString { _a.toString }
}
 
/*
    ArrayType creates a named class which inherits from Array and always has the same
    size and default values. The named class has four constructors:
    1. new(v)    - sets all elements to 'v'
    2. new()     - sets all elements to the default value
    3. fit(a, v) - fits the sequence 'a' to 'size' filling out with 'v' if too short
    4. fit(a)    - as (3) but fills out with the default value if too short 
    and four instance methods of its own:
    5. default   - returns the default value
    6. toArray   - converts the current instance to an Array
    7. copy()    - creates a shallow copy of the current instance
                 - overriding the copy() method inherited from Array
    8. reset()   - resets all elements to the default value.
*/
class ArrayType {
    // Creates a class for the ArrayType (with an underscore after the name), with a
    // given size and default value for its elements, and returns a reference to it.
    static create(name, size, default) {
        Check.ident("Name", name)
        Check.nonNegInt("Size", size)
        name = name +  "_"
        var s = "class %(name) is Array {\n"
        s = s + "    construct new(v) {\n"
        s = s + "        super(%(size), v)\n"
        s = s + "    }\n"
        s = s + "    construct new()  {\n"
        s = s + "        super(%(size), %(default))\n"
        s = s + "    }\n"
        s = s + "    construct fit(a, v) {\n"
        s = s + "        super(%(size), a, v)\n"
        s = s + "    }\n"
        s = s + "    construct fit(a) {\n"
        s = s + "        super(%(size), a, %(default))\n"
        s = s + "    }\n"
        s = s + "    default { %(default) }\n"
        s = s + "    toArray() { Array.from(this) }\n"
        s = s + "    copy() {\n"
        s = s + "        var d = %(name).new()\n"
        s = s + "        for (i in 0...%(size)) d[i] = this[i]\n"
        s = s + "        return d\n"
        s = s + "    }\n"
        s = s + "    reset() { reset(%(default)) }\n}\n"
        s = s + "return %(name)"
        return Meta.compile(s).call()
    }
 
    // Convenience version of 'create' which always uses a default value of null.
    static create(name, size ) { create(name, size, null) }
}
 
/*
   BitArray represents a List<Bool> whose size cannot be changed after it has been constructed
   but whose elements can be changed. It uses only 1/32nd as much memory as a 'normal' List<Bool>
   but is around 4 times slower to index. Also, unlike List<Bool>, BitArray is not a Sequence.
*/
class BitArray {
    // Constructs a new BitArray of a given size and sets all elements to the same value 'v'.
    // 'size' is rounded to the higher multiple of 32 where necessary.
    construct new(size, v) {
        Check.posInt("size", size)
        Check.bool("value", v)
        _len = (size / 32).ceil
        _a = List.filled(_len, v ? 4294967295 : 0)
    }
 
    // Convenience version of 'new' which sets all elements to false.
    static new(size) { new(size, false) }
 
    // Returns the number of elements in the BitArray.
    count { 32 * _len }
 
    // Creates a copy of the current instance.
    copy() { 
        var c = BitArray.new(count, false)
        for (i in 0...count) c[i] = this[i]
        return c
    }
 
    // Resets all elements of the BitArray to 'v'.
    reset(v) {
        Check.bool("value", v)
        var value = v ? 4294967295 : 0
        for (i in 0..._len) _a[i] = value
    }
 
    // Gets the element at 'index'. If index is negative, it counts backwards
    // from the end of the array where -1 is the last element.
    // To maximize access speed, this method doesn't validate the index.
    // Use the 'get' method instead if you need to do that.
    [index] {
        if (index < 0) index = count + index
        var ix = (index/32).floor
        var bit = index%32
        return ((_a[ix] >> bit) & 1) == 1
    }
 
    // Sets the element at 'index'. Negative indices are treated as in the getter.
    // To maximize access speed, this method doesn't validate the index nor the new value.
    // Use the 'set' method instead if you need to do that.
    [index]=(v) {
        if (index < 0) index = count + index
        var ix = (index/32).floor
        var bit = index%32
        _a[ix] = v ?  _a[ix] | (1 << bit) : _a[ix] & ~(1 << bit)
    }
 
    // As [index] method but validates the index. 
    get(index) {
         Check.int("index", index, -count, count-1)
         return this[index]
    }
 
    // As [index]=(v) method but validates the index and the new value.
    set(index, v) {
         Check.int("index", index, -count, count-1)
         Check.bool("value", v)
         this[index] = v
    }
 
    // Returns a List<Bool> using the normal 8 bytes for each element.
    toList {
        var bools = List.filled(count, false)
        for (i in 0...count) bools[i] = this[i]
        return bools
    }
 
    // Returns an Array<Bool> using the normal 8 bytes for each element.
    toArray {
        var bools = Array.new(count, false)
        for (i in 0...count) bools[i] = this[i]
        return bools
    }
 
    // Returns a bit string representation of this BitArray.
    toString {
        var bytes = List.filled(count, 0)
        for (i in 0...count) if (this[i]) bytes[i] = 1
        return bytes.join()
    }
}
