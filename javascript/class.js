class A {
  constructor() {
    this.a = 1;
    this.arrow = () => {
      console.log(this);
    };
    console.log(this);
  }

  method() {
    console.log(this);
  }

  callbackWithRegularMethod() {
    setTimeout(this.method, 100);
  }

  callbackWithArrowMethod() {
    setTimeout(this.arrow, 100);
  }
}

// Class constructor cannot be invoked without 'new'
// console.log(A()); // TypeError...

let a = new A();

// a.method();
// a.callbackWithRegularMethod();
// a.callbackWithArrowMethod();

console.log(A.name);

const B = class {
  get foo() {
    return 'bar';
  }
};

let b = new B();
console.log(b.foo);
console.log(B.prototype);

