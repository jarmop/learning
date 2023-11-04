{
  // type string
  let str1 = 'hello'

  // type "hello"
  let str2 = 'hello' as const

  // type "hello"
  const str3 = 'hello'

  // type { str: string }
  const obj1 = {
    str: 'hello',
  }

  // type { str: "hello" }
  const obj2 = {
    str: 'hello',
  } as const
}

