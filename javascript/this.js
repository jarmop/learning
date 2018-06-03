const A = function() {
  this.a = 1;

  this.callbackWithRegularFunction = function() {
    console.log(this); // A {...}
    setTimeout(function() {
      console.log(this); // Timeout {...}
    }, 100);
  };

  this.callbackWithArrowFunction = function() {
    console.log(this); // A {...}
    setTimeout(() => {
      console.log(this); // A {...}
    }, 200);
  };

  console.log(this); // A {...}
  // console.log(arguments);
  // console.log(super);
  // console.log(new.target);
};

// A();
console.log(this); // {}
let a = new A(2, 5);
a.callbackWithRegularFunction();
a.callbackWithArrowFunction();

/**
 * In addition to not having it's own "this", the arrow function also doesn't
 * have it's own "arguments", "super" or "new.target"
 */