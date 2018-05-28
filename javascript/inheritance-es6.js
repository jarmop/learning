class Person {
  sayHello() {
    console.log('hello');
  }

  walk() {
    console.log('I am walking!');
  }
}

class Student extends Person {
  sayGoodBye() {
    console.log('goodBye');
  }

  sayHello() {
    console.log('hi, I am a student');
  }
}

let student1 = new Student();
student1.sayHello();
student1.walk();
student1.sayGoodBye();

// check inheritance
console.log(student1 instanceof Person); // true 
console.log(student1 instanceof Student); // true