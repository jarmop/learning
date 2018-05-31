const A = function() {
  this.a = 1;

  this.callbackWithRegularFunction = function() {
    setTimeout(function() {
      console.log(this);
    }, 100);
  };

  this.callbackWithArrowFunction = function() {
    setTimeout(() => {
      console.log(this);
    }, 100);
  };

  console.log(this);
  // console.log(arguments);
  // console.log(super);
  // console.log(new.target);
};

// A();
let a = new A(2, 5);
a.callbackWithRegularFunction();
a.callbackWithArrowFunction();

/**
 * In addition to not having it's own "this", the arrow function also doesn't
 * have it's own "arguments", "super" or "new.target"
 */