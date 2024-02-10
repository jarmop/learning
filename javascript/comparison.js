function compareArray() {
  const arr = new Array(3).fill([1, 2])
  const arr2 = [
    [1, 2],
    [1, 2],
    [1, 2],
  ]

  const newArr = arr.map((v, i) => (i === 1 ? v : [1, 2, 3]))

  console.log(newArr.map((v, i) => v == arr[i]))
}

function compareObject() {
  const foo = { a: 1 }
  const bar = { a: 1 }
  const result = Object.is(foo, bar)

  console.log(result)
}

compareObject()
