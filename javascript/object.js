/**
 * Object literal
 * {}
 *
 * Object instance
 * new A()
 *
 * Object constructor
 * const A = function() {}
 */

let objectLiteral = {
  foo() {
    console.log('A shorthand for a function in an object literal');
  },
  bar: {a: 1}
};

objectLiteral.foo();
console.log(objectLiteral.bar);