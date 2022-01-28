/* Module "lsystem.wren" */
 
/* 
   Rule represents a production rule in an L-system i.e. predecessor -> successor mapping.
   Rule objects are immutable.
*/
class Rule {
    // Constructs a new Rule object from a predecessor and successor
    construct new(pred, succ) {
        if (!((pred is String) && pred.count == 1)) {
            Fiber.abort("Predecessor must be a single character string.")
        }
        if (!((succ is String) && succ.count > 0)) {
            Fiber.abort("Successor must be a non-empty string.")
        }
        _pred = pred
        _succ = succ
    }
 
    // Self-evident getter properties.
    pred { _pred }
    succ { _succ }
 
    // Returns a string representation of the current instance.
    toString { "{%(_pred), %(_succ)}" }
}
 
/* 
    LSystem represents a Lindenmayer L-system with deterministic rules.
    The only mutable field is 'angle'.
*/
class LSystem {
    // Performs an operation on each symbol in the symbols string.
    static execute(symbols, operations) {
        if (!((symbols is String) && symbols.count > 1)) {
            Fiber.abort("Symbols must be a non-empty string.")
        }
        if (!((operations is Map) && operations.count > 1)) {
            Fiber.abort("Operations must be a non-empty map of single character strings to functions.")
        }
        for (me in operations) {
            if (!((me.key is String) && me.key.count == 1)) {
                Fiber.abort("Operation keys must be single character strings.")
            }
            if (!((me.value is Fn) && me.value.arity == 0)) {
                Fiber.abort("Operation values must be parameterless functions.")
            }
        }
        for (c in symbols) {
            var op = operations[c]
            if (op) op.call()
        }
    }
 
    // Constructs a new LSystem object. Throws an error if axioms include undeclared symbols 
    // or rule predecessors are not variables.
    construct new(variables, constants, axiom, rules, angle) {
        if (!(variables is List) || !(constants is List) || !(rules is List)) {
            Fiber.abort("Variables/constants/rules must all be lists.")
        }
        var symbols = variables + constants
        for (symbol in symbols) {
            if (!((symbol is String) && symbol.count == 1)) {
                Fiber.abort("Variables/constants must all be single character strings.")
            }
        }
        if (!((axiom is String) && axiom.count > 1)) {
            Fiber.abort("Axiom must be a non-empty string.")
        }
        for (c in axiom) {
            if (!symbols.contains(c)) Fiber.abort("Axiom contains an undeclared symbol '%(c)'")
        }
        for (rule in rules) {
            if (!variables.contains(rule.pred)) {
                Fiber.abort("Rule predecessor '%(rule.pred)' is not a declared variable.")
            }
            for (c in rule.succ) {
                if (!symbols.contains(c)) Fiber.abort("'%(rule.succ)' contains an undeclared symbol '%(c)'")
            }
        }
        if (!(angle is Num)) Fiber.abort("Angle must a number of radians.")
        _variables = variables.toList
        _constants = constants.toList
        _axiom = axiom
        _rules = rules.toList
        _angle = angle
    }
 
    // Convenience method to construct an LSystem object with a zero angle.
    static new(variables, constants, axiom, rules) {
        return new(variables, constants, axiom, rules, 0)
    }
 
    // Getter and setter properties for 'angle' field
    angle     { _angle }
    angle=(a) {
        if (!(a is Num)) Fiber.abort("Angle must be a number of radians.")
        _angle = a
    }
 
    // Getter properties for other fields
    variables { _variables.toList }
    constants { _constants.toList }
    axiom     { _axiom }
    rules     { _rules.toList }
 
    // Private helper method which iterates the L-system just once starting from a given axiom
    // and returns the result.
    iterateOnce_(axiom) {
        var result = ""
        for (c in axiom) {
            if (_constants.contains(c)) {
                result = result + c
                continue
            }
            for (rule in _rules) {
                if (rule.pred == c) {
                    result = result + rule.succ
                    break
                }
            }
        }
        return result
    }
 
    // Iterates the L-system 'n' times, starting from the current instance's axiom
    // and returns the result.
    iterate(n) {
        var result = _axiom
        for (i in 0...n) result = iterateOnce_(result)
        return result
    }
 
    // Returns a string representation of the current instance.
    toString {
        return "
Variables = %(_variables)
Constants = %(_constants)
Axiom     = %(_axiom)
Rules     = %(_rules)
Angle     = %(_angle)
"
    }
}
