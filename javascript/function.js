function a() {
  function b() {

  }
}

if (true) {
  function c() {

  }
}

console.log(typeof a !== 'undefined'); // true
console.log(typeof b !== 'undefined'); // false
console.log(typeof c !== 'undefined'); // true

/**
 * So default function declaration is var. These are the same:
 * function a() {}
 * var a = function() {}
 */