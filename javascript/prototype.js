console.log(Object.prototype);
console.log(Function.prototype);

let literalObject = {};
console.log(Object.getPrototypeOf(literalObject));

// let literalFunction = function(){};
function literalFunction(){}
console.log(Object.getPrototypeOf(literalFunction));


console.log(Object.getPrototypeOf(new function() {}));
console.log(Object.getPrototypeOf(new literalFunction()));

let obj = {
  f: function() {
    console.log(this);
  }
};

console.log(obj);
obj.f();



function a() {
  console.log(typeof this)
}
console.log(a);
a();


let Class = function() {
  // console.log(this);
  this.f = function() {
    console.log(this);
  }
};
Class.prototype.f2 = function() {
  console.log(this);
  console.log(Object.getPrototypeOf(this));
};
// console.log(Class());

let instance = new Class();
instance.f();
instance.f2();

console.log(instance);

// let f = function() {
//   this.a = 1;
//   this.c = function() {
//     return this.a + 1;
//   }
// };
// f.prototype.b = 2;
// f.prototype.d = function() {
//   return this.b + 1;
// };
// let o = new f();
// o.d = () => 15;
//
// console.log(o);
// console.log(f.prototype);
// console.log(Object.getPrototypeOf(o).d());
// console.log(Object.getPrototypeOf(f));
//
// console.log(o.c());
// console.log(o.d());
// o.e = function() {
//   return this.a + this.b + 1;
// };
//
// console.log(Object.getPrototypeOf(Object.prototype)); // null
//
// class C {
//   constructor() {
//     this.a = 1;
//     this.b = 2;
//   }
//
//   c() {
//     return 3;
//   }
// }
//
// c = new C();
//
// console.log(Object.getPrototypeOf(c));
// console.log(C.prototype);
// console.log(c.c());
//
//
//
//
