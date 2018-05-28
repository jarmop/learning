// define the Person Class
function Person() {}

Person.prototype.walk = function(){
  console.log ('I am walking!');
};
Person.prototype.sayHello = function(){
  console.log ('hello');
};

// define the Student class
function Student() {
  // Call the parent constructor
  Person.call(this);
}

// inherit Person
Student.prototype = Object.create(Person.prototype);

// correct the constructor pointer because it points to Person
Student.prototype.constructor = Student;

// replace the sayHello method
Student.prototype.sayHello = function(){
  console.log('hi, I am a student');
};

// add sayGoodBye method
Student.prototype.sayGoodBye = function(){
  console.log('goodBye');
};

let student1 = new Student();
student1.sayHello();
student1.walk();
student1.sayGoodBye();

// check inheritance
console.log(student1 instanceof Person); // true
console.log(student1 instanceof Student); // true