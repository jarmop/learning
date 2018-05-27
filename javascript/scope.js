var globalVar = null;
let globalLet = null;
const globalConst = null;

if (true) {
  var blockVar = null;
  let blockLet = null;
  const blockConst = null;
}

function f() {
  var functionVar = null;
  let functionLet = null;
  const functionConst = null;
}

console.log(typeof globalVar !== 'undefined'); // true
console.log(typeof globalLet !== 'undefined'); // true
console.log(typeof globalConst !== 'undefined'); // true

console.log(typeof blockVar !== 'undefined'); // true
console.log(typeof blockLet !== 'undefined'); // false
console.log(typeof blockConst !== 'undefined'); // false

console.log(typeof functionVar !== 'undefined'); // false
console.log(typeof functionLet !== 'undefined'); // false
console.log(typeof functionConst !== 'undefined'); // false