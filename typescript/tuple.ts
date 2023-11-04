// (string | number)[]
const t1 = [1, '2']

t1[2]

// readonly [1, '2']
const t2 = [1, '2'] as const

// @ts-expect-error
t2[2]

// @ts-expect-error
t2[1].toFixed()

t2[0].toFixed()

// @ts-expect-error
t2[0].charAt

t2[1].charAt

// @ts-expect-error
t2[0] === 2

t2[0] === 1

const t3: [number, string] = [1, '2']

// @ts-expect-error
t3[2]

// @ts-expect-error
const t4: [number, string] = ['1', 2]
