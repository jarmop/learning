interface WTF {
  (foo: string): string
}

function generics2(foo: string) {
  return 'wrj'
}

const foo: WTF = generics2

function fooBar(...args: unknown[]) {
  console.log(args)
}

export function generics() {
  fooBar(1, 2, 5)
}

export function promisify<T>(requestFunc: RequestFunc<T>) {
  return () => {
    const foo = new Promise<T>((resolve) => {
      requestFunc((response) => {
        if (response.status === 'success') {
          resolve(response.data)
        }
      })
    })

    return foo
  }
}