// Labeled loop can be broken from anywhere within the loop
let c = 0;
loop1:
for (let i = 0; i < 10; i++) {
  c++;
  for (let j = 0; j < 10; j++) {
    c++;
    if (c === 50) {
      break loop1;
    }
  }
}

console.log(c);