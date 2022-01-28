/* Module "check.wren" */
 
/*
   Check is a class containing static methods to check that:
   (1) a particular value is of a given type or types and, where possible, within 
       a given range of values; or
   (2) a more general condition is met. 
   If it isn't, the fiber is aborted with an appropriate error message. 
*/
class Check {
    /* Private helper methods. */
    static abort_(name, value, desc) {
        if (name == "" || name == null) name = "Value"
        Fiber.abort("'%(name)' must be %(desc), have '%(value)'.")
    }
 
    static listType_(name, value, elementType, firstOnly) {
        var etype = elementType.toString
        var isInt = false
        if (elementType == "Int") {
            etype = "Num"
            isInt = true
        }
        var ok
        var index = 0
        if (value.count == 0) {
            ok = true
        } else if (firstOnly) {
            ok = value[0].type.toString == etype && (isInt ? value[0].isInteger : true)
        } else {
            ok = true
            for (e in value) {
                if (e.type.toString != etype || (isInt && !e.isInteger)) {
                    ok = false
                    break
                }
                index = index + 1
            }
        }
        if (!ok) abort_(name, value[index], "a List<%(elementType)> but, at index %(index)")
    }
 
    static mapType_(name, value, keyType, valType, firstOnly) {
        listType_(name + ".keys", value.keys.toList, keyType, firstOnly)
        listType_(name + ".values", value.values.toList, valType, firstOnly)
    }
 
    /* Methods to check if the value is a Num. */
    static num(name, value) {
        if (!(value is Num)) abort_(name, value, "a number")
    }
 
    static num(name, value, min) {
        if (!((value is Num) && value >= min)) abort_(name, value, "a number >= %(min)")
    }
 
    static num(name, value, min, max) {
        num("max", max, min)
        if (!((value is Num) && value >= min && value <= max)) {
            abort_(name, value, "a number between %(min) and %(max)")
        }
    }
 
    static nonZeroNum(name, value) {
        if (!((value is Num) && value != 0)) {
            abort_(name, value, "a non-zero number")
        }
    }
 
    static nonNegNum(name, value) { num(name, value, 0) }
 
    static numOpt(name, value, options) {
        if (!((value is Num) && options.any { |o| value == o })) {
            abort_(name, value, "a number within %(options)")
        }
    }
 
    /* Methods to check if the value is an an integer Num. */
    static int(name, value) {
        if (!((value is Num) && value.isInteger)) abort_(name, value, "an integer")
    }
 
    static int(name, value, min) {
        if (!((value is Num) && value.isInteger && value >= min)) {
            abort_(name, value, "an integer >= %(min)")
        }
    }
 
    static nonZeroInt(name, value) {
        if (!((value is Num) && value.isInteger && value != 0)) {
            abort_(name, value, "a non-zero integer")
        }
    }
 
    static int(name, value, min, max) {
        int("max", max, min)
        if (!((value is Num) && value.isInteger && value >= min && value <= max)) {
            abort_(name, value, "an integer between %(min) and %(max)")
        }
    }
 
    static posInt(name, value)        { int(name, value, 1) }
    static nonNegInt(name, value)     { int(name, value, 0) }
    static safeInt(name, value)       { int(name, value, Num.minSafeInteger, Num.maxSafeInteger) }
    static posSafeInt(name, value)    { int(name, value, 1, Num.maxSafeInteger) }
    static nonNegSafeInt(name, value) { int(name, value, 0, Num.maxSafeInteger) }
 
    static nonZeroSafeInt(name, value) {
        if (!(safeInt(value) && value != 0)) abort_(name, value, "a non-zero 'safe' integer")
    }
 
    static intOpt(name, value, options) {
        if (!((value is Num) && value.isInteger && options.any { |o| value == o })) {
            abort_(name, value, "an integer within %(options)")
        }
    }
 
    /* Methods to check if the value is a string or a single character. */
    static str(name, value) {
        if (!(value is String)) abort_(name, value, "a string")
    }
 
    static str(name, value, minLen) {
        if (!((value is String) && value.count >= minLen)) {
            abort_(name, value, "a string of minimum length %(minLen)")
        }
    }
 
    static str(name, value, minLen, maxLen) {
        int("maximum length", maxLen, minLen)
        if (!((value is String) && value.count >= minLen && value.count <= maxLen)) {
            if (minLen != maxLen) {
                abort_(name, value, "a string betwen %(minLen) and %(maxLen) in length")
            } else if (minLen == 1) {
                abort_(name, value, "a character")
            } else {  
                abort_(name, value, "a string of length %(minLen)")
            }
        }
    }
 
    static strOpt(name, value, options) {
        if (!((value is String) && options.any { |o| value == o })) {
            abort_(name, value, "a string within %(options)")
        }
    }
 
    static char(name, value) { str(name, value, 1, 1) }
 
    static char(name, value, min, max) {
         int("max", max, min)
         char(name, value)
         if (value.codePoints[0] < min.codePoints[0] || value.codePoints[0] > max.codePoints[0]) {
            abort_(name, value, "a character between '%(min)' and '%(max)'")
         }
    }
 
    static charOpt(name, value, optionStr) {
        char(name, value)
        if (!optionStr.contains(value)) abort_(name, value, "a character within '%(optionStr)'")
    }
 
    /* Method to check if a value is a valid Wren identifier. */
    static ident(name, value) {
        str(name, value, 1)
        var allChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
        var digits = "0123456789"
        return value.all { |c| allChars.indexOf(c) >= 0 } && digits.indexOf(value[0]) == -1
    } 
 
    /* Methods to check if the value is a list or a typed list. */
    static list(name, value) {
        if (!(value is List)) abort_(name, value, "a list")
    }
 
    static list(name, value, minLen) {
        if (!((value is List) && value.count >= minLen)) {
            abort_(name, value, "a list of minimum length %(minLen)")
        }
    }
 
    static list(name, value, minLen, maxLen) {
        int("maximum length", maxLen, minLen)
        if (!((value is List) && value.count >= minLen && value.count <= maxLen)) {
            if (minLen != maxLen) {
                abort_(name, value, "a list betwen %(minLen) and %(maxLen) in length")
            } else {
                abort_(name, value, "a list of length %(minLen)")
            }
        }
    }
 
    static typedList(name, value, elementType, firstOnly) {
        list(name, value)
        listType_(name, value, elementType, firstOnly)
    }
 
    static typedList(name, value, elementType, firstOnly, minLen) {
        list(name, value, minLen)
        listType_(name, value, elementType, firstOnly)
    }
 
    static typedList(name, value, elementType, firstOnly, minLen, maxLen) {
        int("maximum length", maxLen, minLen)
        list(name, value, minLen, maxLen)
        listType_(name, value, elementType, firstOnly)
    }
 
    /* Methods to check if the value is a map or a typed map. */
    static map(name, value) {
        if (!(value is Map)) abort_(name, value, "a map")
    }
 
    static map(name, value, minLen) {
        if (!((value is Map) && value.count >= minLen)) {
            abort_(name, value, "a map of minimum length %(minLen)")
        }
    }
 
    static map(name, value, minLen, maxLen) {
        int("maximum length", maxLen, minLen)
        if (!((value is Map) && value.count >= minLen && value.count <= maxLen)) {
            if (minLen != maxLen) {
                abort_(name, value, "a map betwen %(minLen) and %(maxLen) in length")
            } else {
                abort_(name, value, "a map of length %(minLen)")
            }
        }
    }
 
    static typedMap(name, value, keyType, valType, firstOnly) {
        map(name, value)
        mapType_(name, value, keyType, valType, firstOnly)
    }
 
    static typedMap(name, value, keyType, valType, firstOnly, minLen) {
        map(name, value, minLen)
        mapType_(name, value, keyType, valType, firstOnly)
    }
 
    static typedMap(name, value, keyType, valType, firstOnly, minLen, maxLen) {
        int("maximum length", maxLen, minLen)
        map(name, value, minLen, maxLen)
        mapType_(name, value, keyType, valType, firstOnly)
    }
 
    /* Methods to check if a value is of some other type. */  
    static bool(name, value) {
        if (!(value is Bool)) abort_(name, value, "a boolean")
    }
 
    static range(name, value) {
        if (!(value is Range)) abort_(name, value, "a range")
    }
 
    static func(name, value) {
        if (!(value is Fn)) abort_(name, value, "a function")
    }
 
    static func(name, value, arity) {
        func(name, value)
        if (value.arity != arity) abort_(name, value.arity, "a function with arity %(arity)")
    }
 
    static seq(name, value) {
        if (!(value is Sequence)) abort_(name, value, "a sequence")
    }
 
    static cls(name, value) {
        if (!(value is Class)) abort_(name, value, "a class")
    }
 
    static fiber(name, value) {
        if (!(value is Fiber)) abort_(name, value, "a fiber")
    }
 
    static type(name, value, type) {
        if (value.type.toString != type.toString) abort_(name, value, "an object of type %(type)")
    }
 
    static type(name, value, type, message) {
        if (value.type.toString != type.toString) abort_(name, value, message)
    }    
 
    static typeOpt(name, value, options) {
        if (!options.any { |o| value.type.toString == o.toString }) {
            abort_(name, value, "an object whose type is within %(options)")
        }
    }
 
    /* Methods to check if a condition is met. */
    static ok(condition, message) {
        if (!condition) Fiber.abort(message)
    }
 
    static ok(condition) { ok(condition, "Condition is not met.") }
}
